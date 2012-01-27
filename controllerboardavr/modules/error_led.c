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

#include <stdint.h>
#include <stdbool.h>

#include "error.h"
#include "error_led.h"
#include "timestamp.h"
#include "config.h"
#include "comm.h"

LedState_t error_led_state;

#define ERROR_LED_TIMEOUT 5000

int8_t set_error_led(LedFlashState_t led_flash_state, int16_t flash_rate)
{
	set_led_state(&error_led_state, led_flash_state, flash_rate);
	return 0;
}

int8_t get_error_led(LedFlashState_t *led_flash_state, int16_t *flash_rate)
{
	*led_flash_state = error_led_state.flash_state;
	*flash_rate = error_led_state.flash_rate;
	return 0;
}

static void set_error_led_pin(bool is_on)
{
	if (is_on)
		debug_P(PSTR("ERROR LED ON @ %lu\n"), get_current_time());
	else
		debug_P(PSTR("ERROR LED OFF @ %lu\n"), get_current_time());

	error_led_state.current_pin_state = is_on;
	SET_ERROR_LED(is_on);
}

void error_led_init(void)
{
	led_init(&error_led_state);
	set_error_led_pin(error_led_state.is_on);
	ERROR_LED_INIT();
}

void error_led_timer_handler(uint32_t current_time)
{
	int32_t timestamp = 0;
	bool pin_state = false;
	int8_t error_code;

	handle_led(&error_led_state, current_time);

	error_code = get_last_error(&timestamp);

	if ((error_code <= SOLID_ERROR_LED_CODE)
			|| ((error_code < 0) && (!check_for_timeout(current_time,
					timestamp, ERROR_LED_TIMEOUT))) || (current_time
			< LED_STARTUP_TIME))
	{
		pin_state = true; // override the led handling
	}
	else
	{
		pin_state = error_led_state.is_on;
	}

	if (current_time < LED_STARTUP_TIME)
	{
		if (!error_led_state.current_pin_state)
		{
			error_led_state.current_pin_state = true;
			set_error_led_pin(true);
		}
	}
	else if (error_led_state.current_pin_state != pin_state)
	{
		error_led_state.current_pin_state = pin_state;
		set_error_led_pin(pin_state);
	}
}

void error_led_shutdown(void)
{
	set_error_led(STATUS_LED_OFF, 0);
	set_error_led_pin(false);
}
