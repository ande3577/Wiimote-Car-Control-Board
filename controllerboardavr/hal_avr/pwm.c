/*
 * pwm.c
 *
 *  Created on: Jan 12, 2011
 *      Author: desertfx5
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "motor.h"
#include "hw.h"
#include "comm.h"
#include "error.h"
#include "timestamp.h"
#include "config.h"

#define MOTOR_SPEED_FORWARD_THRESHOLD (MAX_ABSOLUTE_SPEED / 2)
#define MOTOR_SPEED_REVERSE_THRESHOLD (0 - MOTOR_SPEED_FORWARD_THRESHOLD)

#define MOTOR_DIRECTION_LEFT_THRESHOLD ((DEFAULT_DIRECTION_NULL_VALUE + MIN_ABSOLUTE_DIRECTION) / 2)
#define MOTOR_DIRECTION_RIGHT_THRESHOLD ((MAX_ABSOLUTE_DIRECTION + DEFAULT_DIRECTION_NULL_VALUE) / 2)

void set_speed_motor_pwm_level(int16_t value)
{
	if (value > MOTOR_SPEED_FORWARD_THRESHOLD)
	{
		DISABLE_REVERSE_PIN();
		ENABLE_FORWARD_PIN();
	}
	else if (value < MOTOR_SPEED_REVERSE_THRESHOLD)
	{
		DISABLE_FORWARD_PIN();
		ENABLE_REVERSE_PIN();
	}
	else
	{
		DISABLE_FORWARD_PIN();
		DISABLE_REVERSE_PIN();
	}
}

void set_direction_motor_pwm_level(uint16_t value)
{
	if (value > MOTOR_DIRECTION_RIGHT_THRESHOLD)
	{
		DISABLE_LEFT_PIN();
		ENABLE_RIGHT_PIN();
	}
	else if (value < MOTOR_DIRECTION_LEFT_THRESHOLD)
	{
		DISABLE_RIGHT_PIN();
		ENABLE_LEFT_PIN();
	}
	else
	{
		DISABLE_LEFT_PIN();
		DISABLE_RIGHT_PIN();
	}

}

void pwm_init(void)
{
	DISABLE_FORWARD_PIN();
	DISABLE_REVERSE_PIN();
	SPEED_SET_PIN_DDR();

	DISABLE_LEFT_PIN();
	DISABLE_RIGHT_PIN();
	DIRECTION_SET_PIN_DDR();
	DIRECTION_SET_PIN_DDR();
}

