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

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include "config.h"

#define NUMBER_OF_MOTOR_CHANNELS 2

typedef struct
{
	uint16_t direction_null_value;
	int16_t motor_timeout;
} nvm_t;


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

extern nvm_t nvm_data;

int8_t set_motor_levels(int16_t motor_speed_level, uint16_t motor_direction_level);
void get_motor_levels(int16_t *speed_motor_level, uint16_t *direction_motor_level);
int8_t set_motor_timeout(int16_t timeout);
int16_t get_motor_timeout();
int8_t handle_motor_timeout(void);
int8_t shutdown_all_motors();
void motor_init(void);
void motor_run_handler(uint32_t current_time);
uint16_t get_direction_null_value();
void set_direction_null_value(uint16_t null_value);
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
