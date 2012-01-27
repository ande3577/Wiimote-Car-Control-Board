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

#include <stdint.h>
#include <avr/interrupt.h>

uint8_t critical_section_count = 0;

void enter_critical_section(void)
{
	critical_section_count++;
	cli();
	/* disable interrupts */
}

void exit_critical_section(void)
{
	if (critical_section_count)
		--critical_section_count;
	if (!critical_section_count)
		sei();
	/* enable interrupts */
}

void jump_to_bootloader()
{
	enter_critical_section();

#if !_FAST_DEBUG
	MCUSR = 0;
#if defined __AVR_ATmega644__
	asm("JMP 0xF800");
#elif defined __AVR_ATmega168__
	asm("JMP 0x3800");
#elif defined __AVR_ATmega328__ || defined __AVR_ATmega328P__
	asm("JMP 0x7800");
#else
#error "Unsupported AVR"
#endif // AVR type
#endif
}

