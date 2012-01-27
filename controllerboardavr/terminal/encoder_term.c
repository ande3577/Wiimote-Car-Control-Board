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

#include "command.h"
#include "encoder.h"
#include "config.h"

#include <string.h>

static int8_t parse_get_encoder_speed()
{
	int32_t encoder_speed = get_encoder_speed();

	send_response_P(PSTR(":OK %d\n"), encoder_speed);
	return 0;
}

static int8_t parse_get_encoder_counts()
{
	int32_t encoder_counts = get_encoder_count();

	send_response_P(PSTR(":OK %d\n"), encoder_counts);
	return 0;
}

int8_t parse_encoder_command(char *command, char *bfr, uint16_t buffer_size)
{
	if (0 == strcmp_P(command, PSTR("GES")))
	{
		return parse_get_encoder_speed(); //
	}
	else if (0 == strcmp_P(command, PSTR("GEC")))
	{
		return parse_get_encoder_counts(); //
	}
	else
		return ERR_CMD;
}
