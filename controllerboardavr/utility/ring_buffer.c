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

#include <stdint.h>
#include "error.h"
#include "ring_buffer.h"

static int16_t increment_head(volatile buffer_index_t *head,
		volatile buffer_index_t *tail, buffer_index_t buffer_size)
{
	buffer_index_t temp_head = *head;
	++temp_head;
	if (temp_head >= buffer_size)
		temp_head = 0;

	if (temp_head == *tail)
		return ERR_BUFFER_FULL;

	*head = temp_head;
	return ERR_NONE;
}

static int16_t increment_tail(volatile buffer_index_t *head,
		volatile buffer_index_t *tail, buffer_index_t buffer_size)
{
	buffer_index_t temp_tail = *tail;
	if (temp_tail == *head)
		return ERR_BUFFER_EMPTY;

	++temp_tail;
	if (temp_tail >= buffer_size)
		temp_tail = 0;

	*tail = temp_tail;
	return ERR_NONE;
}

int16_t ring_put(buffer_data_t value, volatile buffer_data_t *buffer,
		buffer_index_t buffer_size, volatile buffer_index_t *head,
		volatile buffer_index_t *tail)
{
	int8_t ret_val;
	buffer_index_t temp_head = *head;

	ret_val = increment_head((volatile buffer_index_t *) &temp_head, tail,
			buffer_size);
	if (ERR_NONE != ret_val)
		return ret_val;

	buffer[*head] = value;
	*head = temp_head;

	return ERR_NONE;
}

int16_t ring_get(buffer_data_t *value, volatile buffer_data_t *buffer,
		buffer_index_t buffer_size, volatile buffer_index_t *head,
		volatile buffer_index_t *tail)
{
	int16_t ret_val;
	buffer_data_t temp_value;

	temp_value = buffer[*tail];
	ret_val = increment_tail(head, tail, buffer_size);
	if (ERR_NONE != ret_val)
		return ret_val;

	*value = temp_value;
	return ERR_NONE;
}

int16_t ring_empty(buffer_index_t head, buffer_index_t tail)
{
	return (head == tail);
}

int16_t ring_get_string(buffer_data_t *dest, buffer_index_t dest_size,
		volatile buffer_data_t *buffer, buffer_index_t buffer_size,
		volatile buffer_index_t *head, volatile buffer_index_t *tail)
{
	buffer_index_t count = 0;
	int16_t ret_val;
	buffer_data_t temp_value = '\0';
	do
	{
		ret_val = ring_get(&temp_value, buffer, buffer_size, head, tail);
		if (ERR_NONE != ret_val)
			break;
		dest[count++] = temp_value;
	} while ((count < dest_size) && (temp_value != '\0'));

	if (temp_value != '\0')
		return 0;
	else
		return count;
}

int16_t ring_put_string(volatile buffer_data_t *src, buffer_index_t src_size,
		volatile buffer_data_t *buffer, buffer_index_t buffer_size,
		volatile buffer_index_t *head, volatile buffer_index_t *tail)
{
	buffer_data_t temp_value = '\0';
	buffer_index_t count = 0;
	int16_t ret_val;
	while (count < src_size)
	{
		temp_value = src[count++];
		ret_val = ring_put(temp_value, buffer, buffer_size, head, tail);
		if (ERR_NONE != ret_val)
			break;
		if ('\0' == temp_value)
			break;
	};

	if (temp_value != '\0')
		return 0;
	else
		return count;
}

int16_t ring_get_all(buffer_data_t *dest, buffer_index_t dest_size,
		volatile buffer_data_t *buffer, buffer_index_t buffer_size,
		volatile buffer_index_t *head, volatile buffer_index_t *tail)
{
	buffer_index_t count = 0;
	int16_t ret_val;
	buffer_data_t temp_value;
	do
	{
		ret_val = ring_get(&temp_value, buffer, buffer_size, head, tail);
		if (ERR_NONE != ret_val)
			break;
		dest[count++] = temp_value;
	} while ((count < dest_size));

	return count;
}

void ring_flush(volatile buffer_data_t *buffer, volatile buffer_index_t *head,
		volatile buffer_index_t *tail)
{
	*head = 0;
	*tail = 0;
}
