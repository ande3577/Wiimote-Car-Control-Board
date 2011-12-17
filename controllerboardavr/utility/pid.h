/*
 * pid.h
 *
 *  Created on: Dec 6, 2010
 *      Author: dsanderson
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
