/*
 * sensor.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum SensorChannel_t
{
	SENSOR_MOTOR_EMF_FWD, //
	SENSOR_MOTOR_EMF_REV, //
	SENSOR_1, //
} SensorChannel_t;

void sensor_init(void);

int16_t get_sensor_adc_counts(uint8_t channel);

void sensor_run_handler(uint32_t current_time);
void sensor_timer_handler(uint32_t current_time);

bool get_sensor_waiting_for_measurement();
void set_sensor_waiting_for_measurement(bool value);

void sensor_adc_complete();

/// port hal must provide function to initialze adc
extern void adc_init();
/// port hal must provide function to start adc conversion on given channel
extern void adc_start_conversion(uint8_t channel);

#endif /* SENSOR_H_ */
