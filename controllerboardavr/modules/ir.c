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
#include "ir.h"
#include "timestamp.h"
#include "config.h"
#include "comm.h"

bool ir_led = false;

/*!
 \brief Setup DDR to make infrared enable lines outputs.
 */
void ir_init(void)
{
	IR_LED_INIT();

	set_ir_led(false);
}

int8_t set_ir_led(bool on)
{
	if (on)
		debug_P(PSTR("IR_LED ON @ %lu\n"), get_current_time());
	else
		debug_P(PSTR("IR_LED OFF @ %lu\n"), get_current_time());

	//STK500 LEDS are inverted
#if !_DEV_BOARD
	SET_IR_LED(on);
#else
	SET_IR_LED(!on);
#endif

	ir_led = on;
	return 0;
}

bool get_ir_led()
{
	return ir_led;
}

void ir_shutdown(void)
{
	set_ir_led(false);
}

