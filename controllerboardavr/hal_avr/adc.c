/*
 * adc.c
 *
 *  Created on: Sep 21, 2011
 *      Author: dsanderson
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "../common/platform.h"
#include "modules/sensor.h"

#if _DIP_PACKAGE
const uint8_t PROGMEM sensor_adc_channel[NUMBER_OF_SENSOR_CHANNELS] =
SENSOR_CHANNELS;
#else
const uint8_t PROGMEM sensor_adc_channel[NUMBER_OF_SENSOR_CHANNELS] =
SENSOR_CHANNELS;
#endif

ISR( ADC_vect)
{
	sensor_adc_complete();
}

void adc_start_conversion(uint8_t channel)
{
	ADMUX = pgm_read_byte(&sensor_adc_channel[channel]);
	ADCSRA |= (1 << ADSC);
}

/*!
 @brief Setup ADC to read distance sensors
 */
void adc_init(void)
{
	ADCSRA = ((1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1
			<< ADPS0));
	ADMUX = pgm_read_byte(&sensor_adc_channel[0]);
}
