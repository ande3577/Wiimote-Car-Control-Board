/*
 * timestamp.c
 *
 *  Created on: Dec 6, 2010
 *      Author: desertfx5
 */

#include "timestamp.h"

volatile int32_t timestamp;

int32_t get_current_time(void)
{
	return timestamp;
}

void reset_time(void)
{
	timestamp = 0;
}

void increment_timestamp(void)
{
	timestamp++;
}

bool check_for_timeout(uint32_t current_time, uint32_t timestamp,
		uint32_t timeout)
{
	if (0 > timeout)
		return false;
	else
		return (current_time - (uint32_t) timestamp > timeout);
}

