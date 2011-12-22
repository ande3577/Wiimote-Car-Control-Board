/*
 * encoder.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
 */

#ifndef ENCODER_H_
#define ENCODER_H_

typedef enum
{
	ENCODER_DIRECTION_UNKNOWN,
	ENCODER_DIRECTION_FORWARD,
	ENCODER_DIRECTION_REVERSE,
	ENCODER_DIRECTION_STOPPED
} encoder_direction_t;

int32_t get_encoder_count(void);
int32_t get_encoder_speed(void);

uint16_t get_encoder_forward_count();
uint16_t get_encoder_reverse_count();

void encoder_init(void);
void encoder_timer_handler(uint32_t current_time);

void encoder_channel_1_handler(void);
void encoder_channel_2_handler(void);

encoder_direction_t get_encoder_direction();

#endif /* ENCODER_H_ */
