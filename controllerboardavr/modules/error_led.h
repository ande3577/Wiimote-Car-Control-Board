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

#ifndef ERROR_LED_H_
#define ERROR_LED_H_

#include "led.h"

#define SOLID_ERROR_LED_CODE -64

void error_led_init(void);
void error_led_timer_handler(uint32_t current_time);

int8_t set_error_led(LedFlashState_t led_flash_state, int16_t flash_rate);
int8_t get_error_led(LedFlashState_t *led_flash_state, int16_t *flash_rate);

void error_led_shutdown(void);

#endif /* ERROR_LED_H_ */
