#include <stdint.h>
#include <stddef.h>
#include "utility/error.h"
#include "utility/timestamp.h"
#include "motor.h"
#include "sensor.h"
#include "hal_avr/comm.h"

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
{ 0, DEFAULT_DIRECTION_NULL_VALUE };

uint16_t direction_null_value;

typedef enum
{
	MOTOR_STATE_IDLE,
	MOTOR_STATE_BRAKING,
	MOTOR_STATE_BRAKE_COAST,
	MOTOR_STATE_BRAKE_READ_BACK_EMF,
	MOTOR_STATE_COASTING,
	MOTOR_STATE_ACTIVE
} MotorState_t;

typedef enum
{
	MOTOR_DIRECTION_UNKNOWN,
	MOTOR_DIRECTION_FORWARD,
	MOTOR_DIRECTION_REVERSE,
	MOTOR_DIRECTION_STOPPED
} MotorDirection_t;

uint32_t motor_timestamp = 0;
uint32_t motor_overcurrent_timestamp = 0;
uint32_t motor_brake_timestamp = 0;
uint32_t motor_coast_timestamp = 0;

uint8_t motor_read_back_emf_count = 0;

uint8_t motor_stopped_count = 0;
uint8_t motor_forward_count = 0;
uint8_t motor_reverse_count = 0;

MotorState_t motor_state;
MotorDirection_t motor_direction;

int16_t motor_timeout;

nvm_t nvm_data;

/// \todo what to do to shutdown direction motor?  should leave it as it's current value or return to null?
static int8_t shutdown_motor(int8_t motor_channel)
{
	motor_levels.speed_channel_level = 0;
	set_speed_motor_pwm_level(0);
	return 0;
}

int8_t shutdown_all_motors(void)
{
	return shutdown_motor(MOTOR_SPEED_CHANNEL);
}

#if _DEBUG
static void print_motor_direction(MotorDirection_t direction)
{
	switch (direction)
	{
	case MOTOR_DIRECTION_FORWARD:
		debug_P(PSTR("FWD"));
		break;
	case MOTOR_DIRECTION_REVERSE:
		debug_P(PSTR("REV"));
		break;
	case MOTOR_DIRECTION_STOPPED:
		debug_P(PSTR("STOP"));
		break;
	case MOTOR_DIRECTION_UNKNOWN:
		debug_P(PSTR("???"));
		break;
	}
}
#endif

static void set_motor_direction(MotorDirection_t direction)
{
	motor_direction = direction;
	motor_stopped_count = 0;
	motor_forward_count = 0;
	motor_reverse_count = 0;
#if _DEBUG
	debug_P(PSTR("MOT DIR SET "));
	print_motor_direction(direction);
	debug_P(PSTR(" @ %lu\n"), get_current_time());
#endif
}

static void start_braking(void)
{
	motor_brake_timestamp = get_current_time();
	set_speed_motor_pwm_level(MOTOR_LEVEL_BRAKE);
	debug_P(PSTR("Start Braking @ %lu\n"), get_current_time());
	motor_state = MOTOR_STATE_BRAKING;
}

static void start_brake_coasting(void)
{
	motor_coast_timestamp = get_current_time();
	set_speed_motor_pwm_level(0);
	debug_P(PSTR("Start Brake Coasting @ %lu\n"), get_current_time());
	motor_state = MOTOR_STATE_BRAKE_COAST;
}

int8_t set_motor_levels(int16_t motor_speed_level,
		uint16_t motor_direction_level)
{
	motor_levels.speed_channel_level = motor_speed_level;
	motor_levels.direction_channel_level = motor_direction_level;
	motor_timestamp = get_current_time();

	if (motor_speed_level == MOTOR_LEVEL_BRAKE)
	{
		start_braking();
	}
	else if (motor_speed_level > 0) // if want to go forward
	{
		// if already going forward or stopped
		if ((motor_direction == MOTOR_DIRECTION_FORWARD) || (motor_direction
				== MOTOR_DIRECTION_STOPPED))
		{
			set_speed_motor_pwm_level(motor_speed_level);
			motor_state = MOTOR_STATE_ACTIVE;
			set_motor_direction(MOTOR_DIRECTION_FORWARD);
		}
		else // if trying to change directions
		{
			start_brake_coasting();
			post_error(ERR_MOTOR_FAULT);
		}
	}
	else if (motor_speed_level < 0) // if want to go backward
	{
		if ((motor_direction == MOTOR_DIRECTION_REVERSE) || (motor_direction
				== MOTOR_DIRECTION_STOPPED))
		{
			set_speed_motor_pwm_level(motor_speed_level);
			motor_state = MOTOR_STATE_ACTIVE;
			set_motor_direction(MOTOR_DIRECTION_REVERSE);
		}
		else // if trying to changed directions
		{
			start_brake_coasting();
			post_error(ERR_MOTOR_FAULT);
		}
	}
	else // if stopping, but not changing directions, then just coast
	{
		set_speed_motor_pwm_level(motor_speed_level);
		motor_state = MOTOR_STATE_COASTING;
		motor_coast_timestamp = get_current_time();
	}

	return ERR_NONE;
}

void get_motor_levels(int16_t *speed_motor_level,
		uint16_t *direction_motor_level)
{
	*speed_motor_level = motor_levels.speed_channel_level;
	*direction_motor_level = motor_levels.direction_channel_level;
}

/*!
 @brief Setup micro IO to driver motor controls.
 */

void motor_init(void)
{

	motor_state = MOTOR_STATE_IDLE;
	set_sensor_waiting_for_measurement(true);
	set_motor_direction(MOTOR_DIRECTION_UNKNOWN);
	if (get_nvm_error_flag() == ERR_NONE)
	{
		direction_null_value = nvm_data.direction_null_value;
		motor_timeout = nvm_data.motor_timeout;
	}
	else
	{
		direction_null_value = DEFAULT_DIRECTION_NULL_VALUE;
		motor_timeout = DEFAULT_MOTOR_TIMEOUT;
	}

	set_direction_motor_pwm_level(direction_null_value);
	pwm_init();
}

int8_t set_motor_timeout(int16_t timeout)
{
	motor_timeout = timeout;
	return 0;
}

int16_t get_motor_timeout()
{
	return motor_timeout;
}

static void handle_back_emf_measurement()
{
	uint16_t emf_fwd = get_sensor_adc_counts(SENSOR_MOTOR_EMF_FWD);
	uint16_t emf_rev = get_sensor_adc_counts(SENSOR_MOTOR_EMF_REV);
#if _DEBUG
	MotorDirection_t last_motor_direction = motor_direction;
#endif

	if (emf_fwd < EMF_THRESHOLD)
	{
		if (motor_forward_count < EMF_DEMOD_COUNT)
			motor_forward_count++;
	}
	else
	{
		motor_forward_count = 0;
		if (motor_direction == MOTOR_DIRECTION_REVERSE)
		{
#if _DEBUG
			if (last_motor_direction != motor_direction)
				debug_P(PSTR("MOTOR READ ??? @ %lu\n"), get_current_time());
#endif
			motor_direction = MOTOR_DIRECTION_UNKNOWN;
		}
	}

	if (emf_rev < EMF_THRESHOLD)
	{
		if (motor_reverse_count < EMF_DEMOD_COUNT)
			motor_reverse_count++;
	}
	else
	{
		motor_reverse_count = 0;
		if (motor_direction == MOTOR_DIRECTION_FORWARD)
		{
#if _DEBUG
			if (last_motor_direction != motor_direction)
				debug_P(PSTR("MOTOR READ ??? @ %lu\n"), get_current_time());
#endif
			motor_direction = MOTOR_DIRECTION_UNKNOWN;
		}
	}

	if (motor_forward_count == EMF_DEMOD_COUNT)
	{
		if (motor_reverse_count == EMF_DEMOD_COUNT)
		{
			motor_direction = MOTOR_DIRECTION_STOPPED;
#if _DEBUG
			if (last_motor_direction != motor_direction)
				debug_P(PSTR("MOTOR READ STOP @ %lu\n"), get_current_time());
#endif
		}
		else
		{
			motor_direction = MOTOR_DIRECTION_REVERSE;
#if _DEBUG
			if (last_motor_direction != motor_direction)
				debug_P(PSTR("MOTOR READ REVERSE @ %lu\n"), get_current_time());
#endif
		}
	}
	else
	{
		if (motor_reverse_count == EMF_DEMOD_COUNT)
		{
			motor_direction = MOTOR_DIRECTION_FORWARD;
#if _DEBUG
			if (last_motor_direction != motor_direction)
				debug_P(PSTR("MOTOR READ FWD @ %lu\n"), get_current_time());
#endif
		}
	}

}

void motor_run_handler(uint32_t current_time)
{
	if (get_nvm_error_flag() != ERR_NONE)
		return;

	switch (motor_state)
	{
	case MOTOR_STATE_IDLE:
		if (!get_sensor_waiting_for_measurement())
		{
			handle_back_emf_measurement();
			set_sensor_waiting_for_measurement(true);

			if (motor_direction == MOTOR_DIRECTION_STOPPED)
			{
				if (get_last_error(NULL) == ERR_MOTOR_FAULT)
					clear_last_error();
			}
			else
			{
				post_error(ERR_MOTOR_FAULT);
			}
		}
		break;
	case MOTOR_STATE_BRAKING:
		if (check_for_timeout(current_time, motor_brake_timestamp,
				MOTOR_BRAKE_TIME))
		{
			start_brake_coasting();
		}
		break;
	case MOTOR_STATE_BRAKE_COAST:
		if (check_for_timeout(current_time, motor_coast_timestamp,
				MOTOR_COAST_TIME))
		{
			debug_P(PSTR("Stop brake coasting @ %lu\n"), current_time);
			set_sensor_waiting_for_measurement(true);
			motor_state = MOTOR_STATE_BRAKE_READ_BACK_EMF;
			motor_read_back_emf_count = 0;
		}
		break;
	case MOTOR_STATE_BRAKE_READ_BACK_EMF:
		if (!get_sensor_waiting_for_measurement())
		{
			handle_back_emf_measurement();

			// if we have receive a brake command we just keep braking
			if (motor_levels.speed_channel_level == MOTOR_LEVEL_BRAKE)
			{
				if (motor_direction == MOTOR_DIRECTION_STOPPED)
				{
					if (get_last_error(NULL) == ERR_MOTOR_FAULT)
						clear_last_error();
				}
				else
				{
					post_error(ERR_MOTOR_FAULT);
				}
			}
			else if (motor_levels.speed_channel_level > 0)
			{
				if ((motor_direction == MOTOR_DIRECTION_STOPPED)
						|| (motor_direction == MOTOR_DIRECTION_FORWARD))
				{
					motor_state = MOTOR_STATE_ACTIVE;
					set_motor_direction(MOTOR_DIRECTION_FORWARD);
				}
			}
			else if (motor_levels.speed_channel_level < 0)
			{
				if ((motor_direction == MOTOR_DIRECTION_STOPPED)
						|| (motor_direction == MOTOR_DIRECTION_REVERSE))
				{
					motor_state = MOTOR_STATE_ACTIVE;
					set_motor_direction(MOTOR_DIRECTION_REVERSE);
				}
			}
			else if (motor_direction == MOTOR_DIRECTION_STOPPED)
			{
				motor_state = MOTOR_STATE_IDLE;
				set_motor_direction(MOTOR_DIRECTION_STOPPED);
			}

			// motor fault is done
			if (motor_state != MOTOR_STATE_BRAKE_READ_BACK_EMF)
			{
				if (get_last_error(NULL) == ERR_MOTOR_FAULT)
					clear_last_error();
			}
			else if (++motor_read_back_emf_count == BACK_EMF_READ_COUNT)
			{
				start_braking();
			}
			else
			{
				set_sensor_waiting_for_measurement(true);
			}
		}
		break;
	case MOTOR_STATE_COASTING:
		if (check_for_timeout(current_time, motor_coast_timestamp,
				MOTOR_COAST_TIME))
		{
			set_sensor_waiting_for_measurement(true);
			motor_state = MOTOR_STATE_IDLE;
		}
		break;
	case MOTOR_STATE_ACTIVE:
		if (check_for_timeout(current_time, motor_timestamp, motor_timeout))
		{
			motor_levels.speed_channel_level = 0;
			start_braking();
			debug_P(PSTR("MOTOR Timeout @ %lu\n"), get_current_time());
			post_error(ERR_MOTOR_TIMEOUT);
		}
		else
		{
			set_speed_motor_pwm_level(motor_levels.speed_channel_level);
			set_direction_motor_pwm_level(motor_levels.direction_channel_level);
		}
		break;
	}
}

uint16_t get_direction_null_value()
{
	return direction_null_value;
}

void set_direction_null_value(uint16_t null_value)
{
	direction_null_value = null_value;
}

void motor_shutdown(void)
{
	shutdown_all_motors();
}

