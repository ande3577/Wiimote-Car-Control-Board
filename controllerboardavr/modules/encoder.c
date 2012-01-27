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

#include "timestamp.h"
#include "encoder.h"
#include "config.h"
#include "hw.h"

volatile int32_t encoder_count = 0;
volatile uint8_t encoder_channel[2] = {0, 0};
uint32_t encoder_speed_timestamp;
int32_t encoder_speed = 0;

int32_t get_encoder_count(void)
{
	int32_t ret_val;
	enter_critical_section();
	ret_val = encoder_count;
	exit_critical_section();
	return encoder_count;
}

int32_t get_encoder_speed(void)
{
	return encoder_speed;
}

void encoder_init(void)
{
	encoder_channel[0] = READ_ENCODER_CHANNEL1();
	encoder_channel[1] = READ_ENCODER_CHANNEL2();
	ENABLE_ENCODER_CHANNEL1_ISR();
	ENABLE_ENCODER_CHANNEL2_ISR();
	encoder_speed_timestamp = get_current_time();
}

void encoder_timer_handler(uint32_t current_time)
{
	if (check_for_timeout(current_time, encoder_speed_timestamp, ENCODER_SPEED_TIME))
	{
		encoder_speed = get_encoder_count() - encoder_speed;
	}
}

void encoder_channel_1_handler(void)
{
	uint8_t temp_channel1 = READ_ENCODER_CHANNEL1();
	uint8_t temp_channel2 = READ_ENCODER_CHANNEL2();

	// if channel 1 has changed
	if (temp_channel1 != encoder_channel[0])
	{
		if (((temp_channel1 != 0) && (temp_channel2 != 0)) ||
				((temp_channel1 == 0) && (temp_channel2 == 0))) // if channel 1 == channel 2
		{
			encoder_count++; // going forward
		}
		else
		{
			encoder_count--; // going reverse
		}
	}
	encoder_channel[0] = temp_channel1;
}

void encoder_channel_2_handler(void)
{
	uint8_t temp_channel1 = READ_ENCODER_CHANNEL1();
	uint8_t temp_channel2 = READ_ENCODER_CHANNEL2();

	// if channel 2 has changed
	if (temp_channel2 != encoder_channel[1])
	{
		if (((temp_channel2 != 0) && (temp_channel1 != 0)) ||
				((temp_channel2 == 0) && (temp_channel1 == 0))) // if channel 1 == channel 2
		{
			encoder_count--; // going backward
		}
		else
		{
			encoder_count++; // going forward
		}
	}
	encoder_channel[1] = temp_channel2;
}



