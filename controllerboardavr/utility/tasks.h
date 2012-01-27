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
