/*
 * timestamp.h
 *
 *  Created on: Dec 6, 2010
 *      Author: desertfx5
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdint.h>
#include <stdbool.h>

#define TIMER_TICK_MS (1 << TIMER_TICK_SHIFT)

#define COMPUTE_TICKS(x) ((x < TIMER_TICK_MS) ? 0 : (x >> TIMER_TICK_SHIFT)  - 1)
#define COMPUTE_MS(x) ((x << TIMER_TICK_SHIFT) + 1)

int32_t get_current_time(void);
void reset_time(void);
void increment_timestamp(void);
bool check_for_timeout(uint32_t current_time, uint32_t timestamp,
		uint32_t timeout);

#endif /* TIMESTAMP_H_ */
