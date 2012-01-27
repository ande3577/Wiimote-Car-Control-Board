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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "hw.h"
#include "error.h"
#include "comm.h"
#include "timestamp.h"
#include "sleep.h"
#include "config.h"

#if _DEBUG
char temp_comm_buffer[TX_BUFFER_SIZE] =
{ 0 };
#endif

volatile buffer_data_t rx_buffer[RX_BUFFER_SIZE] =
{ 0 };
volatile buffer_index_t rx_head = 0;
volatile buffer_index_t rx_tail = 0;

volatile buffer_data_t tx_buffer[TX_BUFFER_SIZE] =
{ 0 };
volatile buffer_index_t tx_head = 0;
volatile buffer_index_t tx_tail = 0;

volatile bool tx_flag = false;

#define MYUBRR (F_CPU/16/BAUD-1)

#define ENABLE_TRANSMIT_ISR() (UCSR0B |= (1 << TXCIE0))
#define DISABLE_TRANSMIT_ISR() (UCSR0B &= ~(1 << TXCIE0))
#define TRANSMIT_READY() (UCSR0A & (1 << UDRE0))
#define TRANSMIT_BYTE(x) (UDR0 = x)
#define COMM_ERROR() ((UCSR0A & (1 << FE0)) || (UCSR0A & (1 << UPE0)))
#define BYTE_RECEIVED() 	(UCSR0A & (1 << RXC0))
#define RECEIVE_BYTE()	(UDR0)

/// Flag tracking whether password has been received
#if _SKIP_PASSWORD
volatile bool password_received = true;
#else
volatile bool password_received = false;
#endif

volatile uint32_t frame_start_time;

/**
 * External dependence needed by printf implementation. Write a character to standard out.
 *
 * @param c Specifies the character to be written.
 * @return Returns the character written. No error conditions are managed.
 */
int putChar(int ch);
int getChar(void);

void comm_flush_rx(void)
{
	ring_flush(rx_buffer, &rx_head, &rx_tail);
}

void comm_init(void)
{
	UBRR0H = (MYUBRR >> 8);
	UBRR0L = (MYUBRR);

	comm_enable_receive_transmit();

	debug_P(PSTR("\n\n"));
	debug_P(PSTR("Startup\n"));
}

int getChar(void)
{
	int ret_val;
	uint8_t control_reg = UCSR0B;
	UCSR0B &= ~(1 << RXCIE0);
	while (!BYTE_RECEIVED())
		;
	if (COMM_ERROR())
	{
		RECEIVE_BYTE(); // dummy read to reset
		ret_val = -1;
	}
	else
	{
		ret_val = RECEIVE_BYTE();
	}
	UCSR0B = control_reg;
	return ret_val;
}

int putChar(int ch)
{
#if !_FAST_DEBUG
	enter_critical_section();
	if (!tx_flag)
	{
		//		tx_flag = true;
		while (!TRANSMIT_READY())
			;
		TRANSMIT_BYTE(ch);
		//		ENABLE_TRANSMIT_ISR();
	}
	else
	{
		ring_put(ch, tx_buffer, TX_BUFFER_SIZE, &tx_head, &tx_tail);
	}
	exit_critical_section();
#else
	TRANSMIT_BYTE(ch);
#endif
	return ch;
}

size_t __write(int handle, const unsigned char *buf, size_t bufSize)
{
	size_t nChars = 0;

	/* Check for the command to flush all handles */
	if (handle == -1)
	{
		return 0;
	}
	/* Check for stdout and stderr
	 (only necessary if FILE descriptors are enabled.) */
	if (handle != 1 && handle != 2)
	{
		return 0;
	}
	for (/* Empty */; bufSize > 0; --bufSize)
	{
		putChar(*buf);
		++buf;
		++nChars;
	}
	return nChars;
}

ISR( USART_TX_vect)
{
	char c;
	int16_t ret_val = ring_get(&c, tx_buffer, TX_BUFFER_SIZE, &tx_head,
			&tx_tail);
	if (ERR_NONE == ret_val)
	{
		TRANSMIT_BYTE(c);
	}
	else
	{
		DISABLE_TRANSMIT_ISR();
		tx_flag = false;
		wake();
	}
}
#if _DEBUG
uint16_t debug_P(const char *fmt, ...)
{
	int8_t ret_val = 0;
	int8_t i;
	va_list args;
	va_start(args, fmt);
	ret_val = vsprintf_P(temp_comm_buffer, fmt, args);
	if (0 <= ret_val)
	{
		for (i = 0; i < ret_val; i++)
		{
			putChar(temp_comm_buffer[i]);
		}
	}
	va_end(args);
	return ret_val;
}
#endif

int send_char(char c)
{
	int ret_val = putChar(c);

	return ret_val;
}

ISR( USART_RX_vect)
{
	int16_t ret_val = 0;
	char c;

	if (!COMM_ERROR())
	{
		c = RECEIVE_BYTE();

		ret_val = ring_put(c, rx_buffer, RX_BUFFER_SIZE, &rx_head, &rx_tail);
		if (ERR_NONE != ret_val)
		{
			debug_P(PSTR("Error adding to ring\n"));
			comm_flush_rx();
		}
		else
		{
			if (comm_wake(c))
				wake();
		}
	}
	else
	{
		RECEIVE_BYTE(); // dummy read to clear register
	}
}

int16_t send_buffer(uint8_t *bfr, uint16_t size)
{
	uint16_t i;
	for (i = 0; i < size; i++)
		putChar(*bfr++);
	return i;
}

void comm_shutdown(void)
{
	while (tx_flag)
		;
}

void comm_disable_receive()
{
	UCSR0B &= ~((1 << RXEN0) | (1 << RXCIE0));
}

void comm_enable_receive_transmit()
{
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}

