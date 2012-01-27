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
