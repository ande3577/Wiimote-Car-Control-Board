/*
 * led.c
 *
 *  Created on: Apr 29, 2009
 *      Author: ande3577
 */

#include "led.h"
#include "timestamp.h"

int8_t set_led_state(LedState_t *led_state,
		LedFlashState_t flash_state, int16_t flash_rate)
{
	led_state->flash_state = flash_state;
	led_state->flash_rate = flash_rate;
	return 0;
}

static void toggle_led_pin_state(LedState_t *led_state)
{
	led_state->is_on ^= 1; // toggle the pin state
}

void handle_led(LedState_t *led_state, uint32_t current_time)
{
	if ((led_state->flash_state != led_state->last_state) || //
			(led_state->flash_rate != led_state->last_flash_rate))
	{
		led_state->is_on = (led_state->flash_state != STATUS_LED_OFF);
		led_state->last_state = led_state->flash_state;
		led_state->last_flash_rate = led_state->flash_rate;

		led_state->flash_timestamp = current_time;
	}
	else if (STATUS_LED_FLASH == led_state->flash_state)
	{
		if (check_for_timeout(current_time, led_state->flash_timestamp,
				led_state->flash_rate))
		{
			led_state->flash_timestamp = current_time;
			toggle_led_pin_state(led_state);
		}
	}
}

void led_init(LedState_t *led_state)
{
	led_state->is_on = false;
	led_state->current_pin_state = false;
	led_state->flash_state = STATUS_LED_OFF;
	led_state->last_state = STATUS_LED_UNITIALIZED;
	led_state->last_flash_rate = 0;
	led_state->flash_timestamp = 0;
}
