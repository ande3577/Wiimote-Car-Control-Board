/*
 * motor_term.c
 *
 *  Created on: Sep 20, 2011
 *      Author: desertfx5
 */

#include "motor.h"
#include "nvm.h"
#include "error.h"
#include "config.h"
#include "command.h"
#include "timestamp.h"

#include <stdio.h>
#include <string.h>

static int8_t parse_set_motor_command(char *bfr, int8_t bfr_length)
{
	int8_t ret_val = 0;
	motor_level_t levels;
	char speed_channel_string[16] = "";

	if (get_nvm_error_flag() != ERR_NONE)
	{
		send_response_P(PSTR(":ERR NVM_CORR\n"));
		ret_val = ERR_NVM_CORRUPT;
	}

	if (NUMBER_OF_MOTOR_CHANNELS
			!= sscanf_P(bfr, PSTR("SML %s %u"), speed_channel_string,
					&levels.direction_channel_level))
	{
		ret_val = ERR_PARAM;
	}
	else if ((levels.direction_channel_level > MAX_ABSOLUTE_DIRECTION)
			|| (levels.direction_channel_level < MIN_ABSOLUTE_DIRECTION))
	{
		ret_val = ERR_PARAM;
	}

	if (ERR_NONE == ret_val)
	{
		if (0 == strcmp_P(speed_channel_string, PSTR("BRK")))
			levels.speed_channel_level = MOTOR_LEVEL_BRAKE;
		else if (1
				!= sscanf_P(speed_channel_string, PSTR("%d"),
						&levels.speed_channel_level))
			ret_val = ERR_PARAM;
		else if ((levels.speed_channel_level > MAX_ABSOLUTE_MOTOR_LEVEL)
				|| (levels.speed_channel_level < MIN_ABSOLUTE_MOTOR_LEVEL))
			ret_val = ERR_PARAM;
	}

	if (ERR_NONE == ret_val)
	{
		set_motor_levels(levels.speed_channel_level,
				levels.direction_channel_level);
	}

	if (ERR_NONE != ret_val)
	{
		shutdown_all_motors();
		if (ERR_PARAM == ret_val)
			send_response_P(PSTR(":ERR PARAM\n"));
		else if (ERR_EXEC == ret_val)
			send_response_P(PSTR(":ERR EXEC\n"));
	}
	else
		send_response_P(PSTR(":OK\n"));
	return ret_val;
}

static int8_t parse_get_motor_command()
{
	int16_t speed_value;
	uint16_t direction_value;

	get_motor_levels(&speed_value, &direction_value);

	if (speed_value == MOTOR_LEVEL_BRAKE)
		send_response_P(PSTR(":OK BRK %u\n"), direction_value);
	else
		send_response_P(PSTR(":OK %d %u\n"), speed_value, direction_value);
	return 0;
}

static int8_t parse_set_motor_timeout(char *bfr, int8_t bfr_length)
{
	int16_t timeout;
	if (1 != sscanf_P(bfr, PSTR("SMT %d"), &timeout))
	{
		send_response_P(PSTR(":ERR PARAM\n"));
		return ERR_PARAM;
	}

	timeout = COMPUTE_TICKS(timeout);

	if (timeout == 0)
	{
		send_response_P(PSTR(":ERR PARAM\n"));
		return ERR_PARAM;
	}
	else
	{
		set_motor_timeout(timeout);
		send_response_P(PSTR(":OK\n"));
		return ERR_NONE;
	}
}

static int8_t parse_get_motor_timeout()
{
	int16_t motor_timeout = get_motor_timeout();

	if (-1 == motor_timeout)
		send_response_P(PSTR(":OK %d\n"), motor_timeout);
	else
		send_response_P(PSTR(":OK %d\n"), (int16_t) COMPUTE_MS(motor_timeout));
	return 0;
}

static int8_t parse_set_direction_null_value(char *bfr, int8_t bfr_length)
{
	uint16_t direction_null_value;
	if (1 != sscanf_P(bfr, PSTR("SNV %u"), &direction_null_value))
	{
		send_response_P(PSTR(":ERR PARAM\n"));
		return ERR_PARAM;
	}

	set_direction_null_value(direction_null_value);
	send_response_P(PSTR(":OK\n"));
	return ERR_NONE;
}

static int8_t parse_get_direction_null_value()
{
	uint16_t direction_null_value = get_direction_null_value();

	send_response_P(PSTR(":OK %u\n"), direction_null_value);
	return ERR_NONE;
}

static int8_t parse_save_nvm_params()
{
	if (ERR_NONE == nvm_save())
	{
		send_response_P(PSTR(":OK\n"));
		return ERR_NONE;
	}
	else
	{
		send_response_P(PSTR(":ERR EXEC\n"));
		return ERR_EXEC;
	}
}

int8_t parse_motor_command(char *command, char *bfr, uint16_t buffer_size)
{
	if (0 == strcmp_P(command, PSTR("SML")))
	{
		return parse_set_motor_command(bfr, buffer_size); //
	}
	else if (0 == strcmp_P(command, PSTR("GML")))
	{
		return parse_get_motor_command(); //
	}
	else if (0 == strcmp_P(command, PSTR("SMT")))
	{
		return parse_set_motor_timeout(bfr, buffer_size); //
	}
	else if (0 == strcmp_P(command, PSTR("GMT")))
	{
		return parse_get_motor_timeout(); //
	}
	else if (0 == strcmp_P(command, PSTR("SNV")))
	{
		return parse_set_direction_null_value(bfr, buffer_size);
	}
	else if (0 == strcmp_P(command, PSTR("GNV")))
	{
		return parse_get_direction_null_value();
	}
	else if (0 == strcmp_P(command, PSTR("SAV")))
	{
		return parse_save_nvm_params();
	}
	else
		return ERR_CMD;
}

