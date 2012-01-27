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

#ifndef STATUS_LED_H_
#define STATUS_LED_H_

#include "led.h"

void status_led_init(void);

int8_t set_status_led(LedFlashState_t led_state, int16_t flash_rate);
int8_t get_status_led(LedFlashState_t *led_state, int16_t *flash_rate);

void status_led_timer_handler(uint32_t current_time);
void status_led_shutdown(void);

#endif /* STATUS_LED_H_ */
