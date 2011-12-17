/*
 * ir.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
 */

#ifndef IR_H_
#define IR_H_

#include <stdint.h>
#include <stdbool.h>

void ir_init(void);
int8_t set_ir_led(bool on);
bool get_ir_led();
void ir_shutdown(void);

#endif /* IR_H_ */
