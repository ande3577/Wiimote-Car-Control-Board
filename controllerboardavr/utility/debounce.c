/*
 * debounce.c
 *
 *  Created on: Dec 7, 2010
 *      Author: desertfx5
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
