/*
 * timer.c
 *
 *  Created on: Sep 14, 2010
 *      Author: dsanderson
 */

#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "hw.h"
#include "timer.h"
#include "../common/platform.h"
#include "sleep.h"
#include "utility/timestamp.h"
#include "utility/tasks.h"

#define PRESCALAR 64
#define TIMER_COUNT ((((uint64_t) TIMER_TICK_MS*F_CPU+500)/1000+PRESCALAR/2)/PRESCALAR - 1)

void timer_init(void)
{
	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << CS22);
	OCR2A = TIMER_COUNT;
	TIMSK2 |= (1 << OCIE2A);
}

ISR(TIMER2_COMPA_vect)
{
	timer_isr();
	wake();
}



