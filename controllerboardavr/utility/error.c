/*
 * error.c
 *
 *  Created on: Sep 8, 2010
 *      Author: dsanderson
 */
#include <stdint.h>
#include <stdio.h>
#include "error.h"
#include "timestamp.h"

int8_t last_error_code = ERR_NONE;
int32_t last_error_timestamp = 0;

int8_t post_error(int8_t error_code)
{
	if (ERR_NONE != error_code)
	{
		last_error_code = error_code;
		last_error_timestamp = get_current_time();
	}
	return error_code;
}

int8_t get_last_error(int32_t *timestamp )
{
	if (NULL != timestamp)
	{
		*timestamp = last_error_timestamp;
	}
	return last_error_code;
}



void clear_last_error()
{
	last_error_code = ERR_NONE;
	last_error_timestamp = 0;
}
