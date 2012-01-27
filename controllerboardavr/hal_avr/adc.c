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

#include "sensor.h"
#include "config.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

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
