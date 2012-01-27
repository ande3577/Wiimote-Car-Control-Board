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

#ifndef PID_H_
#define PID_H_

#include <stdint.h>

#ifndef KP_ENABLE
#define KP_ENABLE 1
#endif

#ifndef KD_ENABLE
#define KD_ENABLE 0
#endif

#ifndef KI_ENABLE
#define KI_ENABLE 0
#endif

#ifndef CONTROL_SCALING
#define CONTROL_SCALING 256
#endif

#ifndef control_multiple_t
#define control_multiple_t int16_t
#endif

#ifndef control_error_t
#define control_error_t int16_t
#endif

#ifndef control_t
#define control_t int16_t
#endif

#ifndef control_integral_t
#define control_integral_t int32_t
#endif

#ifndef control_temp_t
#define control_temp_t int32_t
#endif

typedef struct pidobject
{
#if KP_ENABLE
	control_multiple_t kp;
#endif

#if KI_ENABLE
	control_multiple_t ki;
	control_integral_t error_accumulator;
	control_integral_t max_accumulation;
#endif

#if KD_ENABLE
	control_multiple_t kd;
	control_error_t last_error;
#endif
	control_t max_control;

} pidobject_t;

void pid_init(pidobject_t *pid, control_multiple_t kp, control_multiple_t ki,
		control_multiple_t kd, control_t max_control,
		control_integral_t max_accumulation);
control_t pid_execute(pidobject_t *pid, control_error_t error);

#endif /* PID_H_ */
