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


#include "ir.h"
#include "error.h"
#include "config.h"
#include "command.h"

#include <stdio.h>
#include <string.h>

static int8_t parse_set_ir_led(char *bfr, int8_t bfr_length)
{
	bool on = false;
	char param[16];

	if (1 != sscanf_P(bfr, PSTR("SIL %s"), param))
	{
		send_response_P(PSTR(":ERR PARAM\n"));
		return ERR_PARAM;
	}

	if (0 == strcmp(param, "ON"))
	{
		on = true;
	}
	else if (0 == strcmp(param, "OFF"))
	{
		on = false;
	}
	else
	{
		send_response_P(PSTR(":ERR PARAM\n"));
		return ERR_PARAM;
	}

	if (0 == set_ir_led(on))
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

static int8_t parse_get_ir_led()
{
	bool on = get_ir_led();
	char led_state_string[16];

	if (on)
	{
		strcpy_P(led_state_string, PSTR("ON"));
	}
	else
	{
		strcpy_P(led_state_string, PSTR("OFF"));
	}

	send_response_P(PSTR(":OK %s\n"), led_state_string);

	return 0;
}

int8_t parse_ir_command(char *command, char *bfr, uint16_t buffer_size)
{
	if (0 == strcmp_P(command, PSTR("SIL")))
	{
		return parse_set_ir_led(bfr, buffer_size); //
	}
	else if (0 == strcmp_P(command, PSTR("GIL")))
	{
		return parse_get_ir_led(); //
	}
	else
		return ERR_CMD;
}
