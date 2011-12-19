/*
 * error_list.h
 *
 *  Created on: Dec 19, 2011
 *      Author: dsanderson
 */

#ifndef ERROR_LIST_H_
#define ERROR_LIST_H_

//Error codes
typedef enum ErrorID_t
{
	ERR_NONE = 0, //
	ERR_MOTOR_TIMEOUT = -1, //
	ERR_PARAM = -2, //
	ERR_CMD = -3, //
	ERR_EXEC = -4, //
	ERR_BUFFER_FULL = -5, //
	ERR_BUFFER_EMPTY = -6, //
	ERR_FRAME = -7,
	ERR_UNKN = -8,
	ERR_NVM_CORRUPT = -64,
	ERR_MOTOR_FAULT = -65
} ErrorID_t;


#endif /* ERROR_LIST_H_ */
