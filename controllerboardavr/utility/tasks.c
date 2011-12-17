/*
 * tasks.c
 *
 *  Created on: Sep 8, 2010
 *      Author: dsanderson
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

