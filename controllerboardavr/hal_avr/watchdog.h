/*
 * watchdog.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

void watchdog_init(void);
void watchdog_reset(uint32_t timestamp);

#endif /* WATCHDOG_H_ */
