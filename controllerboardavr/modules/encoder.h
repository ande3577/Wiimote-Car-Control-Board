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

#ifndef ENCODER_H_
#define ENCODER_H_

int32_t get_encoder_count(void);
int32_t get_encoder_speed(void);
void encoder_init(void);
void encoder_timer_handler(uint32_t current_time);

void encoder_channel_1_handler(void);
void encoder_channel_2_handler(void);

#endif /* ENCODER_H_ */
