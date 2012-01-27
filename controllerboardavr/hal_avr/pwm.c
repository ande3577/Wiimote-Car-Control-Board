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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "motor.h"
#include "hw.h"
#include "comm.h"
#include "error.h"
#include "timestamp.h"
#include "config.h"

volatile motor_level_t current_motor_levels =
{ 0, DEFAULT_DIRECTION_NULL_VALUE };

void set_speed_motor_pwm_level(int16_t value)
{
    current_motor_levels.speed_channel_level = value;
    if (MOTOR_LEVEL_BRAKE == value)
    {
        DISABLE_FWD();
        DISABLE_REV();

        ENABLE_SPEED_PWM_PIN();
        DISABLE_SPEED_PWM();
    }
    else if (0 == value)
    {
        DISABLE_FWD();
        DISABLE_REV();

        DISABLE_SPEED_PWM_PIN();
        DISABLE_SPEED_PWM();

        OCR0A = 0;
        OCR0B = 0;
    }
    else if (0 < value) // forward
    {
        DISABLE_REV();
        ENABLE_FWD();
        if (MAX_ABSOLUTE_MOTOR_LEVEL == value)
        {
            ENABLE_SPEED_PWM_PIN();
            DISABLE_SPEED_PWM();
            OCR0A = 0;
        }
        else
        {
#if _DEV_BOARD
        	OCR0A = MAX_ABSOLUTE_MOTOR_LEVEL - value;
#else
            OCR0A = value;
#endif
            ENABLE_SPEED_PWM();
        }

    }
    else // reverse
    {
        DISABLE_FWD();
        ENABLE_REV();

        if (MIN_ABSOLUTE_MOTOR_LEVEL == value)
        {
            ENABLE_SPEED_PWM_PIN();
            DISABLE_SPEED_PWM();
            OCR0A = 0;
        }
        else
        {
            ENABLE_SPEED_PWM();
#if _DEV_BOARD
            OCR0A = MAX_ABSOLUTE_DIRECTION + value;
#else
            OCR0A = -value;
#endif
        }
    }
}

void set_direction_motor_pwm_level(uint16_t value)
{
    current_motor_levels.direction_channel_level = value;
    if (value >= MAX_ABSOLUTE_DIRECTION)
    {
        ENABLE_DIR_PWM_PIN();
        DISABLE_DIR_PWM();
        OCR1B = 0;
    }
    else if (value == 0)
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
    TCCR0B = 0;
    SPEED_SET_PIN_DDR();
    DISABLE_FWD();
    DISABLE_REV();
    DISABLE_SPEED_PWM_PIN();
    TCNT0 = 0;
    TCCR0A = (1 << WGM00);
    OCR0A = 0;
    current_motor_levels.speed_channel_level = 0;
    TCCR0B = (1 << CS01) | (1 << CS00);

    TCCR1B = 0;
    DIRECTION_SET_PIN_DDR();
    DISABLE_DIR_PWM_PIN();
    TCNT1 = 0;
    TCCR1A = (1 << WGM11) | (1<< WGM10);
    OCR1A = MAX_ABSOLUTE_DIRECTION;
    OCR1B = 0;
    current_motor_levels.direction_channel_level = get_direction_null_value();
    TCCR1B = (1<<WGM12) | (1 << CS10);
}





