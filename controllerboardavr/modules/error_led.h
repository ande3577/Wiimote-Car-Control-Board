/*
 * error_led.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
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
