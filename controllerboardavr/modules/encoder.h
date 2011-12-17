/*
 * encoder.h
 *
 *  Created on: Sep 20, 2011
 *      Author: dsanderson
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
