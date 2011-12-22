/*
 * nvm.h
 *
 *  Created on: Mar 7, 2011
 *      Author: desertfx5
 */

#ifndef NVM_H_
#define NVM_H_

#include <stdint.h>
#include "motor.h"

extern motor_parameters_t motor_parameters;

void nvm_init();
int8_t nvm_save();
int8_t get_nvm_error_flag();

#endif /* NVM_H_ */
