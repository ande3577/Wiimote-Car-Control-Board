#include <stdint.h>
#include <stddef.h>
#include "error.h"
#include "timestamp.h"
#include "motor.h"
#include "encoder.h"
#include "comm.h"
#include "algorithms.h"

///@brief Low level hardware drivers for motor control circuit.
motor_level_t motor_levels =
{ DEFAULT_SPEED_NULL_VALUE, DEFAULT_DIRECTION_NULL_VALUE };

uint32_t motor_timestamp = 0;
motor_parameters_t motor_parameters;

/// \todo what to do to shutdown direction motor?  should leave it as it's current value or return to null?
int8_t shutdown_all_motors(void)
{
	motor_levels.speed_channel_level = DEFAULT_SPEED_NULL_VALUE;
	set_speed_motor_pwm_level(DEFAULT_SPEED_NULL_VALUE);
	motor_levels.direction_channel_level = DEFAULT_DIRECTION_NULL_VALUE;
	set_direction_motor_pwm_level(DEFAULT_DIRECTION_NULL_VALUE);
	return ERR_NONE;
}

int8_t set_motor_levels(int16_t motor_speed_level,
		uint16_t motor_direction_level)
{
	set_speed_motor_pwm_level(motor_speed_level);
	set_direction_motor_pwm_level(motor_direction_level);

	return ERR_NONE;
}

const motor_level_t *get_motor_levels()
{
	return &motor_levels;
}

/*!
 @brief Setup micro IO to driver motor controls.
 */
void motor_init(void)
{

	shutdown_all_motors();
	pwm_init();
}

void set_motor_timeout(int16_t timeout)
{
	motor_parameters.motor_timeout = timeout;
}

int16_t get_motor_timeout()
{
	return motor_parameters.motor_timeout;
}

uint16_t get_direction_null_value()
{
	return motor_parameters.direction_motor.null_value;
}

void set_direction_null_value(uint16_t null_value)
{
	motor_parameters.direction_motor.null_value = null_value;
}

void motor_shutdown(void)
{
	shutdown_all_motors();
}

uint16_t get_direction_min_value()
{
	return motor_parameters.direction_motor.min_value;
}
void set_direction_min_value(uint16_t min_value)
{
	motor_parameters.direction_motor.min_value = min_value;
}
uint16_t get_direction_max_value()
{
	return motor_parameters.direction_motor.max_value;
}
void set_direction_max_value(uint16_t max_value)
{
	motor_parameters.direction_motor.max_value = max_value;
}

uint16_t get_speed_null_value()
{
	return motor_parameters.speed_motor.null_value;
}
void set_speed_null_value(uint16_t null_value)
{
	motor_parameters.speed_motor.null_value = null_value;
}
uint16_t get_speed_min_value()
{
	return motor_parameters.speed_motor.min_value;
}
void set_speed_min_value(uint16_t min_value)
{
	motor_parameters.speed_motor.min_value = min_value;
}
uint16_t get_speed_max_value()
{
	return motor_parameters.speed_motor.max_value;
}
void set_speed_max_value(uint16_t max_value)
{
	motor_parameters.speed_motor.max_value = max_value;
}

void motor_run_handler(uint32_t current_time)
{
	if (get_nvm_error_flag() != ERR_NONE)
		return;

	if ((motor_levels.speed_channel_level
			!= motor_parameters.speed_motor.null_value)
			&& (check_for_timeout(current_time, motor_timestamp,
					motor_parameters.motor_timeout)))
	{
		motor_levels.speed_channel_level =
				motor_parameters.speed_motor.null_value;
		debug_P(PSTR("MOTOR Timeout @ %lu\n"), get_current_time());
		post_error(ERR_MOTOR_TIMEOUT);
	}
}
