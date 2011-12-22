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

void set_speed_motor_pwm_level(uint16_t value)
{
	if (value >= MAX_ABSOLUTE_SPEED)
	{
		ENABLE_SPEED_PWM_PIN();
		DISABLE_SPEED_PWM();
		OCR1A = 0;
	}
	else if (value <= MIN_ABSOLUTE_SPEED)
	{
		DISABLE_SPEED_PWM_PIN();
		DISABLE_SPEED_PWM();
		OCR1A = 0;
	}
	else
	{
#if _DEV_BOARD
		OCR1A = MAX_ABSOLUTE_DIRECTION - value;
#else
		OCR1A = value;
#endif
		ENABLE_SPEED_PWM();
	}
}

void set_direction_motor_pwm_level(uint16_t value)
{
	if (value >= MAX_ABSOLUTE_DIRECTION)
	{
		ENABLE_DIR_PWM_PIN();
		DISABLE_DIR_PWM();
		OCR1B = 0;
	}
	else if (value <= MIN_ABSOLUTE_DIRECTION)
	{
		DISABLE_DIR_PWM_PIN();
		DISABLE_DIR_PWM();
		OCR1B = 0;
	}
	else
	{
#if _DEV_BOARD
		OCR1B = MAX_ABSOLUTE_DIRECTION - value;
#else
		OCR1B = value;
#endif
		ENABLE_DIR_PWM();
	}
}

void pwm_init(void)
{
	TCCR1B = 0;
	SPEED_SET_PIN_DDR();
	DISABLE_SPEED_PWM_PIN();
	OCR1A = 0;

	DIRECTION_SET_PIN_DDR();
	DISABLE_DIR_PWM_PIN();
	TCNT1 = 0;
	OCR1B = 0;

	TCCR1A = (1 << WGM11) | (1 << WGM10);
	TCCR1B = (1 << WGM12) | (1 << CS10);
}

