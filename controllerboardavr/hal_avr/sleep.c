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
