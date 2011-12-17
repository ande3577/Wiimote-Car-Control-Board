/*
 * status_led.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
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
