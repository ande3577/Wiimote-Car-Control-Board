#include <stdint.h>
#include <stddef.h>
#include "error.h"
#include "timestamp.h"
#include "motor.h"
#include "encoder.h"
#include "comm.h"
#include "algorithms.h"

#if _FAST_DEBUG

// time motor has to be stopped before changing directions
#define MOTOR_BRAKE_TIME COMPUTE_TICKS(20)

#define MOTOR_COAST_TIME COMPUTE_TICKS(5)

#define BACK_EMF_READ_COUNT 4

#define EMF_DEMOD_COUNT 2

#else
// time motor has to be stopped before changing directions
#define MOTOR_BRAKE_TIME COMPUTE_TICKS(200)

#define MOTOR_COAST_TIME COMPUTE_TICKS(10)

#define BACK_EMF_READ_COUNT 25

#define EMF_DEMOD_COUNT 10
#endif

#define EMF_THRESHOLD 50

///@brief Low level hardware drivers for motor control circuit.
motor_level_t motor_levels =
{ DEFAULT_SPEED_NULL_VALUE, DEFAULT_DIRECTION_NULL_VALUE };

typedef enum
{
	MOTOR_STATE_IDLE,
	MOTOR_STATE_BRAKING,
	MOTOR_STATE_BRAKE_COAST,
	MOTOR_STATE_BRAKE_READ_BACK_EMF,
	MOTOR_STATE_COASTING,
	MOTOR_STATE_ACTIVE
} MotorState_t;

uint32_t motor_timestamp = 0;
uint32_t motor_overcurrent_timestamp = 0;
uint32_t motor_brake_timestamp = 0;
uint32_t motor_coast_timestamp = 0;

uint8_t motor_read_back_emf_count = 0;

uint8_t motor_stopped_count = 0;
uint8_t motor_forward_count = 0;
uint8_t motor_reverse_count = 0;

MotorState_t motor_state;

motor_parameters_t motor_parameters;

/// \todo what to do to shutdown direction motor?  should leave it as it's current value or return to null?
int8_t shutdown_all_motors(void)
{
	motor_levels.speed_channel_level = DEFAULT_SPEED_NULL_VALUE;
	set_speed_motor_pwm_level(DEFAULT_SPEED_NULL_VALUE);
	return ERR_NONE;
}

int8_t set_motor_levels(uint16_t motor_speed_level,
		uint16_t motor_direction_level)
{
	if (get_nvm_error_flag() != ERR_NONE)
		return ERR_NVM_CORRUPT;

	motor_speed_level =
			COERCE(motor_speed_level, motor_parameters.speed_motor.min_value, motor_parameters.speed_motor.max_value);
	motor_direction_level =
			COERCE(motor_direction_level, motor_parameters.direction_motor.min_value, motor_parameters.direction_motor.max_value);

	motor_levels.speed_channel_level = motor_speed_level;
	motor_levels.direction_channel_level = motor_direction_level;
	motor_timestamp = get_current_time();

	encoder_direction_t motor_direction = get_encoder_direction();

	if (motor_speed_level > motor_parameters.speed_motor.null_value) // if want to go forward
	{
		// if already going forward or stopped
		if ((motor_direction == ENCODER_DIRECTION_FORWARD)
				|| (motor_direction == ENCODER_DIRECTION_STOPPED))
		{
			set_speed_motor_pwm_level(motor_speed_level);
		}
		else
		{
			shutdown_all_motors();
			// if trying to change directions
			post_error(ERR_MOTOR_FAULT);
		}
	}
	else if (motor_speed_level < motor_parameters.speed_motor.null_value) // if want to go backward
	{
		if ((motor_direction == ENCODER_DIRECTION_REVERSE)
				|| (motor_direction == ENCODER_DIRECTION_STOPPED))
		{
			set_speed_motor_pwm_level(motor_speed_level);
		}
		else
		{
			shutdown_all_motors();
			// if trying to changed directions
			post_error(ERR_MOTOR_FAULT);
		}
	}
	else // if stopping, but not changing directions, then just coast
	{
		set_speed_motor_pwm_level(motor_speed_level);
	}

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

	motor_state = MOTOR_STATE_IDLE;

	set_direction_motor_pwm_level(motor_parameters.direction_motor.null_value);
	set_speed_motor_pwm_level(motor_parameters.speed_motor.null_value);
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
