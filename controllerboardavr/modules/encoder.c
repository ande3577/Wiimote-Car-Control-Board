/*
 * encoder.c
 *
 *  Created on: Mar 21, 2011
 *      Author: desertfx5
 */

#include <util/atomic.h>

#include "timestamp.h"
#include "encoder.h"
#include "config.h"
#include "hw.h"
#include "comm.h"

volatile uint8_t encoder_channel[2] =
{ 0, 0 };
volatile uint16_t next_encoder_forward_count = 0;
volatile uint16_t next_encoder_reverse_count = 0;

uint16_t encoder_forward_count = 0;
uint16_t encoder_reverse_count = 0;

int32_t encoder_count = 0;
uint32_t encoder_speed_timestamp;
int32_t encoder_speed = 0;

#if _DEBUG
encoder_direction_t encoder_direction;
#endif

int32_t get_encoder_count(void)
{
	return encoder_count;
}

int32_t get_encoder_speed(void)
{
	return encoder_speed;
}

uint16_t get_encoder_forward_count()
{
	return encoder_forward_count;
}

uint16_t get_encoder_reverse_count()
{
	return encoder_reverse_count;
}

void encoder_init(void)
{
	encoder_channel[0] = READ_ENCODER_CHANNEL1();
	encoder_channel[1] = READ_ENCODER_CHANNEL2();
	ENABLE_ENCODER_CHANNEL1_ISR();
	ENABLE_ENCODER_CHANNEL2_ISR();
	encoder_speed_timestamp = get_current_time();
}

#if _DEBUG
static void print_encoder_direction(encoder_direction_t direction)
{
	switch (direction)
	{
	case ENCODER_DIRECTION_FORWARD:
		debug_P(PSTR("FWD"));
		break;
	case ENCODER_DIRECTION_REVERSE:
		debug_P(PSTR("REV"));
		break;
	case ENCODER_DIRECTION_STOPPED:
		debug_P(PSTR("STOP"));
		break;
	case ENCODER_DIRECTION_UNKNOWN:
		debug_P(PSTR("???"));
		break;
	}
}
#endif

void encoder_timer_handler(uint32_t current_time)
{
	if (check_for_timeout(current_time, encoder_speed_timestamp,
			ENCODER_SPEED_TIME))
	{
		ATOMIC_BLOCK(NONATOMIC_RESTORESTATE)
		{
			encoder_forward_count = next_encoder_forward_count;
			next_encoder_forward_count = 0;
			encoder_reverse_count = next_encoder_reverse_count;
			next_encoder_reverse_count = 0;
		}
		encoder_speed = encoder_forward_count - encoder_reverse_count;
		encoder_count += encoder_speed;

#if _DEBUG
		encoder_direction_t new_encoder_direction = get_encoder_direction();
		if (new_encoder_direction != encoder_direction)
			print_encoder_direction(new_encoder_direction);

		encoder_direction = new_encoder_direction;
#endif
	}
}

void encoder_channel_1_handler(void)
{
	uint8_t temp_channel1 = READ_ENCODER_CHANNEL1();
	uint8_t temp_channel2 = READ_ENCODER_CHANNEL2();

	// if channel 1 has changed
	if (temp_channel1 != encoder_channel[0])
	{
		if (((temp_channel1 != 0) && (temp_channel2 != 0))
				|| ((temp_channel1 == 0) && (temp_channel2 == 0))) // if channel 1 == channel 2
		{
			encoder_forward_count++; // going forward
		}
		else
		{
			encoder_reverse_count--; // going reverse
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
		if (((temp_channel2 != 0) && (temp_channel1 != 0))
				|| ((temp_channel2 == 0) && (temp_channel1 == 0))) // if channel 1 == channel 2
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

encoder_direction_t get_encoder_direction()
{
	uint16_t next_forward_count_temp, next_reverse_count_temp;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		next_forward_count_temp = next_encoder_forward_count;
		next_reverse_count_temp = next_encoder_reverse_count;
	}

	// define as moving in direction if any encoder state in the next 1+ cycles
	bool forward = (encoder_forward_count || next_forward_count_temp);
	bool reverse = (encoder_reverse_count || next_reverse_count_temp);

	if (forward)
	{
		if (reverse)
			return ENCODER_DIRECTION_UNKNOWN;
		else
			return ENCODER_DIRECTION_FORWARD;
	}
	else if (reverse)
	{
		return ENCODER_DIRECTION_REVERSE;
	}
	else
	{
		return ENCODER_DIRECTION_STOPPED;
	}
}

