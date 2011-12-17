/*
 * comm.h
 *
 *  Created on: Sep 7, 2010
 *      Author: dsanderson
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
