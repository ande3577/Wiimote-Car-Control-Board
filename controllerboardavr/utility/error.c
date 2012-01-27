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
#include <stdio.h>
#include "error.h"
#include "timestamp.h"

int8_t last_error_code = ERR_NONE;
int32_t last_error_timestamp = 0;

int8_t post_error(int8_t error_code)
{
	if (ERR_NONE != error_code)
	{
		last_error_code = error_code;
		last_error_timestamp = get_current_time();
	}
	return error_code;
}

int8_t get_last_error(int32_t *timestamp )
{
	if (NULL != timestamp)
	{
		*timestamp = last_error_timestamp;
	}
	return last_error_code;
}



void clear_last_error()
{
	last_error_code = ERR_NONE;
	last_error_timestamp = 0;
}
