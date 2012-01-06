/*
 * motor.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include "config.h"

#define NUMBER_OF_MOTOR_CHANNELS 2

typedef struct
{
	int16_t null_value;
	int16_t min_value;
	int16_t max_value;
} motor_channel_paramter_t;

typedef struct
{
	motor_channel_paramter_t speed_motor;
	motor_channel_paramter_t direction_motor;
	int16_t motor_timeout;
} motor_parameters_t;

typedef enum MotorChannel_t
{
	MOTOR_SPEED_CHANNEL, //
	MOTOR_DIRECTION_CHANNEL,
} MotorChannel_t;

typedef struct
{
	int16_t speed_channel_level;
	uint16_t direction_channel_level;
} motor_level_t;

extern motor_parameters_t motor_parameters;

int8_t set_motor_levels(int16_t motor_speed_level,
		uint16_t motor_direction_level);
const motor_level_t *get_motor_levels();
void set_motor_timeout(int16_t timeout);
int16_t get_motor_timeout();
int8_t handle_motor_timeout(void);
int8_t shutdown_all_motors();
void motor_init(void);
void motor_run_handler(uint32_t current_time);

uint16_t get_direction_null_value();
void set_direction_null_value(uint16_t null_value);
uint16_t get_direction_min_value();
void set_direction_min_value(uint16_t min_value);
uint16_t get_direction_max_value();
void set_direction_max_value(uint16_t min_value);

uint16_t get_speed_null_value();
void set_speed_null_value(uint16_t null_value);
uint16_t get_speed_min_value();
void set_speed_min_value(uint16_t min_value);
uint16_t get_speed_max_value();
void set_speed_max_value(uint16_t min_value);

void motor_shutdown(void);

/// port hal layer must provide function to initialize pwm
extern void pwm_init(void);
/// port hal layer must provide function to set speed pwm
extern void set_speed_motor_pwm_level(int16_t value);
/// port hal must provide function to set motor pwm
extern void set_direction_motor_pwm_level(uint16_t value);
/// port hal must provide function to determine whether nvm is corrupt
extern int8_t get_nvm_error_flag();

#endif /* MOTOR_H_ */
