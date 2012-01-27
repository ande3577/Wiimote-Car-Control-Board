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

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdint.h>
#include "config.h"
#include "error.h"

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
