/*
 * status_led.c
 *
 *  Created on: Sep 20, 2011
 *      Author: desertfx5
 */

#include "status_led.h"
#include "error.h"
#include "config.h"
#include "command.h"
#include "timestamp.h"

#include <stdio.h>
#include <string.h>

static int8_t parse_set_status_led(char *bfr, int8_t bfr_length)
{
	char flash_string[16];
	int16_t flash_rate;
	LedFlashState_t flash_state;

	uint8_t param_count = sscanf_P(bfr, PSTR("SSL %s %d"), flash_string,
			&flash_rate);

	if (1 > param_count)
	{
		send_response_P(PSTR(":ERR PARAM\n"));
		return ERR_PARAM;
	}

	if (0 == strcmp(flash_string, "ON"))
	{
		flash_state = STATUS_LED_ON;
		if (1 == param_count)
			flash_rate = 0;
	}
	else if (0 == strcmp(flash_string, "OFF"))
	{
		flash_state = STATUS_LED_OFF;
		if (1 == param_count)
			flash_rate = 0;
	}
	else if (0 == strcmp(flash_string, "FLASH"))
	{
		if (2 != param_count)
		{
			send_response_P(PSTR(":ERR PARAM\n"));
			return ERR_PARAM;
		}

		flash_state = STATUS_LED_FLASH;
	}
	else
	{
		send_response_P(PSTR(":ERR PARAM\n"));
		return ERR_PARAM;
	}

	if (0 == set_status_led(flash_state, COMPUTE_TICKS(flash_rate)))
	{
		send_response_P(PSTR(":OK\n"));
		return 0;
	}
	else
	{
		send_response_P(PSTR(":ERR EXEC\n"));
		return ERR_EXEC;
	}
}

static int8_t parse_get_status_led()
{
	LedFlashState_t led_state;
	int16_t flash_rate;
	char led_state_string[16];

	get_status_led(&led_state, &flash_rate);

	if (STATUS_LED_ON == led_state)
	{
		strcpy_P(led_state_string, PSTR("ON"));
	}
	else if (STATUS_LED_OFF == led_state)
	{
		strcpy_P(led_state_string, PSTR("OFF"));
	}
	else if (STATUS_LED_FLASH == led_state)
	{
		strcpy_P(led_state_string, PSTR("FLASH"));
	}

	send_response_P(PSTR(":OK %s %d\n"), led_state_string,
			COMPUTE_MS(flash_rate));

	return 0;
}

int8_t parse_status_led_command(char *command, char *bfr, uint16_t buffer_size)
{
	if (0 == strcmp_P(command, PSTR("SSL")))
	{
		return parse_set_status_led(bfr, buffer_size); //
	}
	else if (0 == strcmp_P(command, PSTR("GSL")))
	{
		return parse_get_status_led(); //
	}
	else
		return ERR_CMD;
}
