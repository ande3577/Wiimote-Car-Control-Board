/*
 * tasks.h
 *
 *  Created on: Sep 8, 2010
 *      Author: dsanderson
 */

#ifndef TASKS_H_
#define TASKS_H_

#include <stdbool.h>
#include <stdint.h>
#include "config.h"

typedef void (*InitHandler_t)(void);
typedef void (*RunHandler_t)(uint32_t CurrentTime);
typedef void (*TimerHandler_t)(uint32_t CurrentTime);
typedef void (*ShutdownHandler_t)(void);

void run_task(void);
void timer_isr();
void shutdown(void);

/// application must provide list of initialization handlers (null terminated)
extern const InitHandler_t PROGMEM InitHandlerList[];
/**
 * \br application must provide list of timer handlers (null terminated)
 *
 * These functions will be called every tick of timer, but will be called from
 * background thread.  If background thread takes longer than one timer tick,
 * a timer handler function could be missed.
 */
extern const TimerHandler_t PROGMEM TimerHandlerList[];
/**
 * \br application must provide list of run handlers (null terminated)
 * These functions are continually called from background loop.
 */
extern const RunHandler_t PROGMEM RunHandlerList[];
/**
 * \br application must provide list of shutdown handlers (null terminated)
 * They should configure the sensor into a safe state.
 */
extern const ShutdownHandler_t PROGMEM ShutdownHandlerList[];
/// hal layer must provide a method to disable interrupts
extern void enter_critical_section(void);
/// hal layer must provide a method to reenable interrupts
extern void exit_critical_section(void);

#endif /* TASKS_H_ */
