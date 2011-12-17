/*
 * ring_buffer.h
 *
 *  Created on: Sep 9, 2010
 *      Author: dsanderson
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>
#include "error.h"
#include "config.h"

int16_t ring_put(buffer_data_t value, volatile buffer_data_t *buffer,
		buffer_index_t buffer_size, volatile buffer_index_t *head, volatile buffer_index_t *tail);
int16_t ring_get(buffer_data_t *value, volatile buffer_data_t *buffer,
		buffer_index_t buffer_size, volatile buffer_index_t *head, volatile buffer_index_t *tail);
int16_t ring_empty(buffer_index_t head, buffer_index_t tail);
int16_t ring_get_string(buffer_data_t *dest, buffer_index_t dest_size,
		volatile buffer_data_t *buffer, buffer_index_t buffer_size,
		volatile buffer_index_t *head, volatile buffer_index_t *tail);
int16_t ring_put_string(volatile buffer_data_t *src, buffer_index_t src_size,
		volatile buffer_data_t *buffer, buffer_index_t buffer_size,
		volatile buffer_index_t *head, volatile buffer_index_t *tail);
int16_t ring_get_all(buffer_data_t *dest, buffer_index_t dest_size,
		volatile buffer_data_t *buffer, buffer_index_t buffer_size,
		volatile buffer_index_t *head, volatile buffer_index_t *tail);
void ring_flush(volatile buffer_data_t *buffer, volatile buffer_index_t *head,
		volatile buffer_index_t *tail);

#endif /* RING_BUFFER_H_ */
