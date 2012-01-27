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
