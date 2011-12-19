/*
 * encoder_isr.c
 *
 *  Created on: Sep 21, 2011
 *      Author: dsanderson
 */

#include <stdint.h>
#include <avr/interrupt.h>

#include "modules/encoder.h"

ISR(INT1_vect)
{
	encoder_channel_1_handler();
}

ISR(INT0_vect)
{
	encoder_channel_2_handler();
}
