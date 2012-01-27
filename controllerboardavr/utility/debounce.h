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

#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include <stdint.h>
#include <stdbool.h>

#if !_FAST_DEBUG
#define DEBOUNCE_CLICK_TIME 20
#define DEBOUNCE_RELEASE_TIME 20
#else
#define DEBOUNCE_CLICK_TIME 1
#define DEBOUNCE_RELEASE_TIME 1
#endif

typedef enum debounce_state_t
{
	DEBOUNCE_STATE_RELEASED, //
	DEBOUNCE_STATE_PRESSED
} debounce_state_t;

typedef struct debounce_t
{
	debounce_state_t state;
	uint16_t hold_count;
} debounce_t;

void debounce_init(debounce_t *debounce);
debounce_state_t debounce_execute(debounce_t *debounce, bool pressed);

#endif /* DEBOUNCE_H_ */
