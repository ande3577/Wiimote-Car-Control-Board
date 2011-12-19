/*
 * command.h
 *
 *  Created on: Sep 9, 2010
 *      Author: dsanderson
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdint.h>
#include "../common/platform.h"
#include "utility/error.h"

#define COMMAND_PARSE_SIZE RX_BUFFER_SIZE

#ifndef RX_FRAME_TIMEOUT
#define RX_FRAME_TIMEOUT 100
#endif

typedef int8_t (*CommParser_t)(char *, char *, uint16_t);
typedef void (*CommErrorHandler_t)(void);

uint16_t send_response_P(const char *fmt, ...);

void comm_run_handler();
void comm_timer_handler(uint32_t current_time);
int8_t send_program_info(char separator);

/// application must provide an array of command handlers, null terminated
extern const CommParser_t PROGMEM CommParserList[];
/**
 * \br application must provide an array of invalid command error handlers, null terminated
 * Only gets called for invalid command, commands themselves must handle parameter errors
 */
extern const CommErrorHandler_t PROGMEM CommErrorHandlerList[];

#endif /* COMMAND_H_ */
