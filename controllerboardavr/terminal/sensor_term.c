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

#include "sensor.h"
#include "error.h"
#include "config.h"
#include "command.h"

#include <stdio.h>
#include <string.h>

int8_t parse_get_sensor_adc_counts()
{
	uint8_t i;

	send_response_P(PSTR(":OK"));
	for (i = 0; i < NUMBER_OF_SENSOR_CHANNELS; i++)
	{
		send_response_P(PSTR(" %d"), get_sensor_adc_counts(i));
	}
	send_response_P(PSTR("\n"));

	return 0;
}

int8_t parse_sensor_command(char *command, char *bfr, uint16_t buffer_size)
{
	if (0 == strcmp_P(command, PSTR("GSV")))
	{
		return parse_get_sensor_adc_counts(); //
	}
	else
		return ERR_CMD;
}
