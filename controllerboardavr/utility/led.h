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

#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum StatusLedFlashState_t
{
	STATUS_LED_OFF, //
	STATUS_LED_ON, //
	STATUS_LED_FLASH, //
	STATUS_LED_UNITIALIZED = -1,
} LedFlashState_t;

typedef struct LedState_t
{
	bool is_on; //
	bool current_pin_state;
	LedFlashState_t flash_state; //
	LedFlashState_t last_state; //
	uint16_t flash_rate; //
	uint16_t last_flash_rate; //
	uint32_t flash_timestamp; //
} LedState_t;

int8_t set_led_state(LedState_t *led_state,
		LedFlashState_t flash_state, int16_t flash_rate);
void handle_led(LedState_t *led_state, uint32_t current_time);
void led_init(LedState_t *led_state);

#endif /* LED_H_ */
