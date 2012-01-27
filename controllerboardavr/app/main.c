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

#include "tasks.h"
#include "comm.h"
#include "hw.h"
#include "encoder.h"
#include "error_led.h"
#include "ir.h"
#include "command.h"
#include "program_info.h"
#include "nvm.h"
#include "motor.h"
#include "sensor.h"
#include "status_led.h"
#include "timer.h"
#include "watchdog.h"
#include "sleep.h"

#include "encoder_term.h"
#include "error_led_term.h"
#include "ir_term.h"
#include "motor_term.h"
#include "sensor_term.h"
#include "status_led_term.h"
#include "config.h"

#include <stdbool.h>


const InitHandler_t PROGMEM InitHandlerList[] =
{
		timer_init,
		comm_init,
		nvm_init,
		ir_init,
		sensor_init,
		status_led_init,
		motor_init,
		error_led_init,
		encoder_init,
		watchdog_init,
		NULL
};

const TimerHandler_t PROGMEM TimerHandlerList[] =
{
		status_led_timer_handler,
		sensor_timer_handler,
		error_led_timer_handler,
		comm_timer_handler,
		encoder_timer_handler,
		NULL
};

const RunHandler_t PROGMEM RunHandlerList[] =
{
		motor_run_handler,
		comm_run_handler,
		sensor_run_handler,
		watchdog_reset,
		sleep,
		NULL
};

const ShutdownHandler_t PROGMEM ShutdownHandlerList[] =
{
		motor_shutdown,
		ir_shutdown,
		status_led_shutdown,
		error_led_shutdown,
		comm_shutdown,
		jump_to_bootloader,
		NULL
};

const CommParser_t PROGMEM CommParserList[] =
{
		parse_motor_command,
		parse_sensor_command,
		parse_encoder_command,
		parse_status_led_command,
		parse_error_led_command,
		parse_ir_command,
		NULL
};

const CommErrorHandler_t PROGMEM CommErrorHandlerList[] =
{
		motor_shutdown,
		NULL
};

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
	run_task();
	return 0;
}
