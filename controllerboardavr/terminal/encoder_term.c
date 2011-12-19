/*
 * encoder_term.c
 *
 *  Created on: Sep 20, 2011
 *      Author: desertfx5
 */

#include "command.h"
#include "modules/encoder.h"
#include "../common/platform.h"

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
