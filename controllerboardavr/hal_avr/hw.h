/*
 * hw.h
 *
 *  Created on: Sep 7, 2010
 *      Author: dsanderson
 */

#ifndef HW_H_
#define HW_H_

#include <stdbool.h>
#include <stdint.h>

#include <avr/wdt.h>

void jump_to_bootloader();

void enter_critical_section(void);
void exit_critical_section(void);

#endif /* HW_H_ */
