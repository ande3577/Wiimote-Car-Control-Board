/*
 * sensor_term.c
 *
 *  Created on: Sep 20, 2011
 *      Author: desertfx5
 */

#include "modules/sensor.h"
#include "utility/error.h"
#include "../common/platform.h"
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
