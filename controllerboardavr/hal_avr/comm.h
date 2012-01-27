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

#ifndef COMM_H_
#define COMM_H_

#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "ring_buffer.h"
#include "config.h"


extern volatile buffer_data_t rx_buffer[];
extern volatile buffer_index_t rx_head;
extern volatile buffer_index_t rx_tail;

extern volatile buffer_data_t tx_buffer[];
extern volatile buffer_index_t tx_head;
extern volatile buffer_index_t tx_tail;

extern volatile bool tx_flag;
extern volatile bool password_received;

void comm_init(void);
#if _DEBUG
uint16_t debug_P(const char *fmt, ...);
#else
#define debug_P(...)
#endif
int8_t char_received(char c);
void tx_ready(void);
int send_char(char c);
int16_t send_buffer(uint8_t *bfr, uint16_t size);
void comm_shutdown(void);

void comm_disable_receive();
void comm_enable_receive_transmit();
void comm_flush_rx(void);

/**
 * \br Protocol must provide function indicating whether to wake on comm
 * \retval true to wake, false to return to sleep
 */
extern bool comm_wake(uint8_t rx_char);

#endif /* COMM_H_ */
