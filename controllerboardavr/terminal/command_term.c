/**
 * @author David S Anderson
 *
 *
 * Copyright (C) 2011 David S Anderson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "hw.h"
#include "command.h"
#include "error.h"
#include "comm.h"
#include "program_info.h"
#include "tasks.h"
#include "timestamp.h"
#include "ring_buffer.h"
#include "timer.h"
#include "motor.h"

uint16_t rx_count = 0;
uint32_t rx_timestamp;

bool program_info_sent = false;
bool sending_response = false;

uint16_t send_response_P(const char *fmt, ...)
{
	uint16_t ret_val = 0;
	uint8_t i;
	va_list args;
	va_start(args, fmt);

	// disable receive since we're about to stomp on the rx buffer
	comm_disable_receive();

	ret_val = vsprintf_P((char *) rx_buffer, fmt, args);
	if (0 <= ret_val)
	{
		enter_critical_section();
		for (i = 0; i < ret_val; i++)
		{
			send_char(rx_buffer[i]);
		}
		exit_critical_section();
	}
	va_end(args);
	sending_response = true;
	return ret_val;
}

int8_t send_program_info(char separator)
{
	send_response_P(PSTR("%s%c"), PROGRAM_NAME, separator);
	send_response_P(PSTR("v: %s%c"), PROGRAM_VERSION, separator);
	send_response_P(PSTR("Built: %s "), __DATE__);
	send_response_P(PSTR(" %s%c"), __TIME__, separator);
	return ERR_NONE;
}

static int8_t get_command(char *bfr, char *command, int8_t bfr_length,
		int8_t command_length)
{
	return sscanf_P(bfr, PSTR("%s"), command);
}

static int8_t parse_get_program_info(void)
{
	int8_t ret_val;
	send_response_P(PSTR(":OK "));
	ret_val = send_program_info(';');
	send_response_P(PSTR("\n"));
	return ret_val;
}

static int8_t parse_get_timestamp(void)
{
	send_response_P(PSTR(":OK %lu\n"), get_current_time());
	return 0;
}

static int8_t parse_get_last_error(void)
{
	int32_t timestamp;
	int8_t error_code = get_last_error(&timestamp);

	send_response_P(PSTR(":OK "));

	switch (error_code)
	{
	case ERR_NONE:
		send_response_P(PSTR("NO_ERR "));
		break;
	case ERR_MOTOR_TIMEOUT:
		send_response_P(PSTR("MTO "));
		clear_last_error();
		break;
	case ERR_PARAM:
		send_response_P(PSTR("PARAM "));
		clear_last_error();
		break;
	case ERR_CMD:
		send_response_P(PSTR("CMD "));
		clear_last_error();
		break;
	case ERR_EXEC:
		send_response_P(PSTR("EXEC "));
		clear_last_error();
		break;
	case ERR_BUFFER_EMPTY:
		send_response_P(PSTR("BFR_EMPTY "));
		clear_last_error();
		break;
	case ERR_BUFFER_FULL:
		send_response_P(PSTR("BFR_FULL "));
		clear_last_error();
		break;
	case ERR_NVM_CORRUPT:
		send_response_P(PSTR("NVM_CORR "));
		break;
	case ERR_MOTOR_FAULT:
		send_response_P(PSTR("ERR_FAULT "));
		break;
	default:
		send_response_P(PSTR("??? "));
		break;
	}

	send_response_P(PSTR("%lu "), timestamp);

	send_response_P(PSTR("\n"));
	return 0;
}

static int8_t parse_initialize_command(char *bfr, int8_t bfr_length)
{
	char password[33];

	if (1 != sscanf_P(bfr, PSTR("ICB %s"), &password))
	{
		if (password_received)
			send_response_P(PSTR(":ERR PARAM"));

		return ERR_PARAM;
	}

	if (0 != strcmp_P(password, PSTR("WIFIBOT123")))
	{
		if (password_received)
			send_response_P(PSTR(":ERR EXEC"));

		return ERR_EXEC;
	}

	if (!password_received)
		send_response_P(PSTR("%s"), bfr);

	password_received = true;
	send_response_P(PSTR(":OK\n"));
	return ERR_NONE;
}

static int8_t parse_command(char *bfr, uint16_t bfr_length)
{
	if (strlen(bfr) > COMMAND_PARSE_SIZE)
	{
		send_response_P(PSTR(":ERR ???"));
		return ERR_UNKN;
	}

	int8_t ret_val = ERR_CMD;
	char command[COMMAND_PARSE_SIZE];
	get_command(bfr, command, COMMAND_PARSE_SIZE, COMMAND_PARSE_SIZE);

	if (0 == strcmp_P(command, PSTR("ICB")))
	{
		ret_val = parse_initialize_command(bfr, COMMAND_PARSE_SIZE);
	}
	else if (password_received)
	{
		uint8_t i = 0;
		CommParser_t CommParser;
		do
		{
			CommParser = (CommParser_t) pgm_read_word(&CommParserList[i++]);
			if (CommParser != NULL)
			{
				ret_val = CommParser(command, bfr, COMMAND_PARSE_SIZE);
			}
			else
				break;
		} while (ret_val == ERR_CMD);

		if (ret_val == ERR_CMD)
		{
			if (0 == strcmp_P(command, PSTR("TIM")))
			{
				ret_val = parse_get_timestamp();
			}
			else if (0 == strcmp_P(command, PSTR("GLE")))
			{
				ret_val = parse_get_last_error();
			}
			else if (0 == strcmp_P(command, PSTR("PGM")))
			{
				ret_val = parse_get_program_info();
			}
			else if (0 == strcmp_P(command, PSTR("SDN")))
			{
				send_response_P(PSTR(":OK\n"));
				debug_P(PSTR("Exiting...\n"));
				shutdown();
			}
			else
			{
				send_response_P(PSTR(":ERR CMD\n"));
				uint8_t i = 0;
				CommErrorHandler_t CommErrorHandler;
				do
				{
					CommErrorHandler = (CommErrorHandler_t) pgm_read_word(
							&CommErrorHandlerList[i++]);
					if (CommErrorHandler != NULL)
					{
						CommErrorHandler();
					}
					else
						break;
				} while (1);
				ret_val = ERR_CMD;
			}
		}
	}
	return post_error(ret_val);
}

static void receive_byte(uint8_t byte)
{
	if ((password_received) && (byte != '\0'))
		send_char(byte);
	if ((byte >= 'a') && (byte <= 'z')) // if lowercase letter
	{
		byte -= 'a' - 'A'; // discard case
	}

	if (rx_count < RX_BUFFER_SIZE)
	{
		rx_buffer[rx_count] = byte;
		++rx_count;
	}
	else
	{
		rx_count = 0;
		post_error(ERR_BUFFER_FULL);
		if (password_received)
			send_response_P(PSTR("ERR: Buffer Full\n"));
	}
}

void comm_run_handler(uint32_t current_time)
{
#if _DEBUG
	if (!program_info_sent)
	{
		program_info_sent = true;
		debug_P(PSTR("\n"));
		send_program_info('\n');
	}
#endif

	// if we've started sending our response
	if (sending_response)
	{
		// if we've finished sending response, reenable the receive
		if (!tx_flag)
		{
			comm_enable_receive_transmit();
			comm_flush_rx();
			sending_response = false;
		}
	}

	buffer_data_t rx_char;
	//handle all received bytes
	int8_t error_flag;
	do
	{
		enter_critical_section();
		error_flag = ring_get(&rx_char, rx_buffer, RX_BUFFER_SIZE, &rx_head,
				&rx_tail);
		exit_critical_section();
		if (ERR_NONE == error_flag)
		{
			if (rx_count == 0)
			{
				if ((rx_char != '\n') && (rx_char != '\r'))
				{
					rx_timestamp = current_time;
					receive_byte(rx_char);
				}
			}
			else if ((rx_char == '\n') || (rx_char == '\r')) // if recenved endline
			{
				receive_byte('\0');
				rx_count = 0;
				parse_command((char *) rx_buffer, RX_BUFFER_SIZE); // full command received
			}
			else
			{
				receive_byte(rx_char);
			}
		}
	} while (error_flag == ERR_NONE);
}

void comm_timer_handler(uint32_t current_time)
{
	if (rx_count)
	{
		if (check_for_timeout(current_time, rx_timestamp, RX_FRAME_TIMEOUT))
		{
			rx_count = 0;
			debug_P(PSTR("COM Frame Timeout\n"));
			if (password_received)
			{
				post_error(ERR_FRAME);
				send_response_P(PSTR("ERR: FRAME\n"));
			}
		}
	}
}

bool comm_wake(uint8_t rx_char)
{
	return ((rx_char == '\n') || (rx_char == '\r'));
}

