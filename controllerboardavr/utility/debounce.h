/*
 * debounce.h
 *
 *  Created on: Dec 7, 2010
 *      Author: desertfx5
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
