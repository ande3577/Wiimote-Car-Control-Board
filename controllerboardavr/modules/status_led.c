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
#include "led.h"
#include "config.h"
#include "comm.h"

LedState_t status_led_state;

int8_t set_status_led(LedFlashState_t led_flash_state, int16_t flash_rate)
{
	set_led_state(&status_led_state, led_flash_state, flash_rate);
	return 0;
}

int8_t get_status_led(LedFlashState_t *led_flash_state, int16_t *flash_rate)
{
	*led_flash_state = status_led_state.flash_state;
	*flash_rate = status_led_state.flash_rate;
	return 0;
}

static void set_status_led_pin(bool is_on)
{
	if (is_on)
		debug_P(PSTR("STATUS LED ON @ %lu\n"), get_current_time());
	else
		debug_P(PSTR("STATUS LED OFF @ %lu\n"), get_current_time());

	status_led_state.current_pin_state = is_on;

	SET_STATUS_LED(is_on);
}

void status_led_timer_handler(uint32_t current_time)
{
	handle_led(&status_led_state, current_time);
	if (current_time < LED_STARTUP_TIME)
	{
		if (!status_led_state.current_pin_state)
		{
			status_led_state.current_pin_state = true;
			set_status_led_pin(true);
		}
	}
	else if (status_led_state.current_pin_state != status_led_state.is_on)
	{
		status_led_state.current_pin_state = status_led_state.is_on;
		set_status_led_pin(status_led_state.is_on);
	}

}

void status_led_init(void)
{
	led_init(&status_led_state);
	set_status_led_pin(true);
	INIT_STATUS_LED();
}

void status_led_shutdown(void)
{
	set_status_led(STATUS_LED_OFF, 0);
	set_status_led_pin(false);
}
