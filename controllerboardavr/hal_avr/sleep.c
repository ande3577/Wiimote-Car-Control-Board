/*
 * sleep.c
 *
 *  Created on: Sep 20, 2011
 *      Author: desertfx5
 */

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include "config.h"

volatile bool woken = false;

void wake(void)
{
	woken = true;
}

void sleep(uint32_t timestamp)
{
	set_sleep_mode(SELECTED_SLEEP_MODE);
	cli();
	while (!woken)
	{
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
	woken = false;
	sei();
}
