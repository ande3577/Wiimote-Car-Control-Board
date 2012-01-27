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

#include <stdbool.h>
#include <string.h>

#include "tasks.h"
#include "timestamp.h"
#include "error.h"

bool shutdown_flag = false;
volatile bool timer_flag = false;

static void handle_timer_tick(uint32_t current_time)
{
	if (!timer_flag)
		return;

	timer_flag = false;

	uint8_t i = 0;
	TimerHandler_t TimerHandler;
	do
	{
		TimerHandler = (TimerHandler_t) pgm_read_word(&TimerHandlerList[i++]);
		if (TimerHandler != NULL)
			TimerHandler(current_time);
		else
			break;
	} while (1);
}

void timer_isr()
{
	increment_timestamp();
	timer_flag = true;
}

/**
 * @br Perform background tasks when woken
 */
void run_task(void)
{
	uint8_t i = 0;
	InitHandler_t InitHandler;
	enter_critical_section();
	do
	{
		InitHandler = (InitHandler_t) pgm_read_word(&InitHandlerList[i++]);
		if (InitHandler != NULL)
			InitHandler();
		else
			break;
	} while (1);
	exit_critical_section();

	uint32_t current_time;
	reset_time();

	while (!shutdown_flag)
	{
		i = 0;
		current_time = get_current_time();
		handle_timer_tick(current_time);
		RunHandler_t RunHandler;
		do
		{
			RunHandler = (RunHandler_t) pgm_read_word(&RunHandlerList[i++]);
			if (RunHandler != NULL)
				RunHandler(current_time);
			else
				break;
		} while (1);
	}

	i = 0;
	ShutdownHandler_t ShutdownHandler;
	do
	{
		ShutdownHandler = (ShutdownHandler_t) pgm_read_word(&ShutdownHandlerList[i]);
		if (ShutdownHandler != NULL)
		{
			ShutdownHandler();
			++i;
		}
	} while (1);
}

void shutdown(void)
{
	shutdown_flag = true;
}

