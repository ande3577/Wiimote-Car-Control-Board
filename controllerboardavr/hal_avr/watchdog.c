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

