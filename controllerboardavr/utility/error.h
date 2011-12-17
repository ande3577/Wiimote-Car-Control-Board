/*
 * error.h
 *
 *  Created on: Sep 8, 2010
 *      Author: dsanderson
 */

#ifndef ERROR_H_
#define ERROR_H_
#include <stdint.h>
#include "config.h"

int8_t post_error(int8_t error_code);
int8_t get_last_error(int32_t *timestamp);
void clear_last_error();

#endif /* ERROR_H_ */
