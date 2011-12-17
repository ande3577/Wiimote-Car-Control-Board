/*
 * pid.c
 *
 *  Created on: Dec 6, 2010
 *      Author: dsanderson
 */

#include "pid.h"

void pid_init(pidobject_t *pid, control_multiple_t kp, control_multiple_t ki, control_multiple_t kd,
		control_t max_control, control_integral_t max_accumulation)
{
	pid->max_control = max_control;

#if KP_ENABLE
	pid->kp = kp;
#endif

#if KD_ENABLE
	pid->kd = kd;
	pid->last_error = 0;
#endif

#if KI_ENABLE
	pid->ki = ki;
	pid->max_accumulation = max_accumulation;
	pid->error_accumulator = 0;
#endif
}

control_t pid_execute(pidobject_t *pid, control_error_t error)
{
	control_temp_t control = 0;

#if KP_ENABLE
	control += (control_temp_t) error * pid->kp;
#endif

#if KI_ENABLE
	pid->error_accumulator += error;
	if (pid->error_accumulator > pid->max_accumulation)
		pid->error_accumulator = pid->max_accumulation;
	else if (pid->error_accumulator < -pid->max_accumulation)
		pid->error_accumulator = -pid->max_accumulation;

	control += (control_temp_t) pid->error_accumulator * pid->ki;
#endif

#if KD_ENABLE
	control += (control_temp_t) (error - pid->last_error) * pid->kd;
	pid->last_error = error;
#endif

	if (control > 0)
	{
		control /= CONTROL_SCALING;
		control++;
	}
	else if (control < 0)
	{
		control /= CONTROL_SCALING;
		control--;
	}

	if (control > pid->max_control)
		control = pid->max_control;
	else if (control < -pid->max_control)
		control = -pid->max_control;


	return control;
}
