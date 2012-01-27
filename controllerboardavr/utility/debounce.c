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

#include "debounce.h"

void debounce_init(debounce_t *debounce)
{
	debounce->state = false;
	debounce->hold_count = 0;
}

debounce_state_t debounce_execute(debounce_t *debounce, bool pressed)
{
	if (DEBOUNCE_STATE_PRESSED == debounce->state) // if state is pressed
	{
		if (!pressed) // if released it
		{
			if (++debounce->hold_count >= DEBOUNCE_RELEASE_TIME)
			{
				debounce->state = DEBOUNCE_STATE_RELEASED;
				debounce->hold_count = 0;
			}
		}
	}
	else // if currently released
	{
		if (pressed) // if starting to press
		{
			if (++debounce->hold_count >= DEBOUNCE_CLICK_TIME)
			{
				debounce->state = DEBOUNCE_STATE_PRESSED;
				debounce->hold_count = 0;
			}
		}
	}
	return debounce->state;
}
