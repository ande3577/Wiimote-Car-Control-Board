#include "utility/tasks.h"
#include "hal_avr/comm.h"
#include "hal_avr/hw.h"
#include "modules/encoder.h"
#include "modules/error_led.h"
#include "modules/ir.h"
#include "terminal/command.h"
#include "hal_avr/nvm.h"
#include "modules/motor.h"
#include "modules/sensor.h"
#include "modules/status_led.h"
#include "hal_avr/timer.h"
#include "hal_avr/watchdog.h"
#include "hal_avr/sleep.h"

#include "terminal/encoder_term.h"
#include "terminal/error_led_term.h"
#include "terminal/ir_term.h"
#include "terminal/motor_term.h"
#include "terminal/sensor_term.h"
#include "terminal/status_led_term.h"
#include "../common/platform.h"

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
