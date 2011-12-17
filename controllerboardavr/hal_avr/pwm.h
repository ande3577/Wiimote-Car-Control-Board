/*
 * pwm.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
 */

#ifndef PWM_H_
#define PWM_H_

void pwm_init(void);

void set_speed_motor_pwm_level(int16_t value);
void set_direction_motor_pwm_level(uint16_t value);

#endif /* PWM_H_ */
