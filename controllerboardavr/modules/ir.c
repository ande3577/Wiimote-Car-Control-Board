/*!
 \file
 \brief Hardware routines for enabling/disabling infrareed LEDS.
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

