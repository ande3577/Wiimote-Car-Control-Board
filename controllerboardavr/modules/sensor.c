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

#include "timestamp.h"
#include "sensor.h"
#include "config.h"
#include "sleep.h"
#include "hw.h"

volatile uint8_t current_sensor_channel;
#if NUMBER_OF_SENSOR_CHANNELS != SENSOR_REQUIRED_CHANNELS
volatile uint8_t round_robin_channel;
#endif
volatile bool adc_conversion_in_progress;
volatile bool sensor_measurement_ready;

bool sensor_waiting_for_measurement;

uint32_t sensor_timestamp;
uint32_t sensor_stream_timestamp;

int16_t sensor_adc_counts[NUMBER_OF_SENSOR_CHANNELS];

volatile int16_t temp_sensor_adc_counts[NUMBER_OF_SENSOR_CHANNELS];

/*!
 @brief Setup ADC to read distance sensors
 */
void sensor_init(void)
{
	adc_init();

	current_sensor_channel = 0;
	adc_conversion_in_progress = false;
	sensor_measurement_ready = false;
	sensor_timestamp = 0;
#if NUMBER_OF_SENSOR_CHANNELS != SENSOR_REQUIRED_CHANNELS
	round_robin_channel = SENSOR_REQUIRED_CHANNELS;
#endif

	uint8_t i;
	for (i = 0; i < NUMBER_OF_SENSOR_CHANNELS; i++)
	{
		sensor_adc_counts[i] = 0;
		temp_sensor_adc_counts[i] = 0;
	}
}

int16_t get_sensor_adc_counts(uint8_t channel)
{
	return sensor_adc_counts[channel];
}

void sensor_run_handler(uint32_t current_time)
{
	if (!sensor_measurement_ready)
		return;

	sensor_measurement_ready = false;

	uint8_t i;

	enter_critical_section();
	for (i = 0; i < NUMBER_OF_SENSOR_CHANNELS; i++)
	{
		sensor_adc_counts[i] = temp_sensor_adc_counts[i];
	}
	exit_critical_section();
	sensor_waiting_for_measurement = false;

#if _DEBUG >= 2
	debug_P(PSTR("SENSOR:"));
	for (i = 0; i < NUMBER_OF_SENSOR_CHANNELS; i++)
	{
		debug_P(PSTR(" %d"), sensor_adc_counts[i]);
	}
	debug_P(PSTR("\n"));
#endif
}

/*!
 @brief Start a conversion.
 */
void sensor_timer_handler(uint32_t current_time)
{

	if (check_for_timeout(current_time, sensor_timestamp, SENSOR_UPDATE_TIME))
	{
		if (!adc_conversion_in_progress)
		{
			sensor_timestamp = current_time;
			current_sensor_channel = 0;
			adc_conversion_in_progress = true;
			adc_start_conversion(current_sensor_channel);
		}
	}
}

bool get_sensor_waiting_for_measurement()
{
	return sensor_waiting_for_measurement;
}
void set_sensor_waiting_for_measurement(bool value)
{
	sensor_waiting_for_measurement = value;
}

void sensor_adc_complete()
{
	// on STK500, pushbuttons are inverted
#if _DEV_BOARD
	temp_sensor_adc_counts[current_sensor_channel] = MAX_SENSOR_ADC - ADC;
#else
	temp_sensor_adc_counts[current_sensor_channel] = ADC;
#endif

#if NUMBER_OF_SENSOR_CHANNELS == SENSOR_REQUIRED_CHANNELS
	if (SENSOR_CHANNEL_MAX == current_sensor_channel)
	{
		current_sensor_channel = 0;
		adc_conversion_in_progress = false;
		sensor_measurement_ready = true;
	}
#else
	if (current_sensor_channel == round_robin_channel) // if just finished optional channel
	{
		current_sensor_channel = 0;
		adc_conversion_in_progress = false;
		if (++round_robin_channel == NUMBER_OF_SENSOR_CHANNELS)
			round_robin_channel = SENSOR_REQUIRED_CHANNELS;
		sensor_measurement_ready = true;
		wake();
	}
	else if (current_sensor_channel == SENSOR_REQUIRED_MAX) // if just finished last required
	{
		current_sensor_channel = round_robin_channel;
		adc_start_conversion(current_sensor_channel);
	}
#endif
	else
	{
		current_sensor_channel++;
		adc_start_conversion(current_sensor_channel);
	}
}

