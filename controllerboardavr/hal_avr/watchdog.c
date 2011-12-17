/*
 * watchdog.c
 *
 *  Created on: Dec 6, 2010
 *      Author: desertfx5
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

void watchdog_init(void)
{
	wdt_reset();
	wdt_disable();
#if (!_DEBUG && !_FAST_DEBUG)
	WDTCSR |= (1<<WDE);
	wdt_enable(WDTO_250MS);
#endif
}

void watchdog_reset(uint32_t timestamp)
{
	wdt_reset();
}

