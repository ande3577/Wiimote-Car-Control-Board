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

#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "hw.h"
#include "tasks.h"
#include "timer.h"
#include "config.h"
#include "sleep.h"
#include "timestamp.h"

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



