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
