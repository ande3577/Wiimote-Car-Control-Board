/*
 * nvm.c
 *
 *  Created on: Mar 7, 2011
 *      Author: desertfx5
 */

#include <avr/eeprom.h>
#include <util/crc16.h>
#include "nvm.h"
#include "hw.h"
#include "error.h"
#include "comm.h"
#include "timestamp.h"
#include "motor.h"
#include "config.h"

#if !_FAST_DEBUG
EEMEM motor_parameters_t ee_nvm_data =
{
	{	DEFAULT_SPEED_NULL_VALUE, MIN_ABSOLUTE_SPEED, MAX_ABSOLUTE_SPEED},
	{	DEFAULT_DIRECTION_NULL_VALUE, MIN_ABSOLUTE_DIRECTION,
		MAX_ABSOLUTE_DIRECTION}, DEFAULT_MOTOR_TIMEOUT};
EEMEM uint16_t ee_nvm_crc = 0;
#else
motor_parameters_t ee_nvm_data =
{
{ DEFAULT_SPEED_NULL_VALUE, MIN_ABSOLUTE_SPEED, MAX_ABSOLUTE_SPEED },
		{ DEFAULT_DIRECTION_NULL_VALUE, MIN_ABSOLUTE_DIRECTION,
				MAX_ABSOLUTE_DIRECTION }, DEFAULT_MOTOR_TIMEOUT };
#endif

int8_t nvm_error_flag = ERR_NVM_CORRUPT;

void nvm_init()
{
#if !_FAST_DEBUG
	uint16_t i;
	uint16_t crc = 0;
	int8_t ret_val = ERR_NONE;
	enter_critical_section();
	for(i = 0; i < sizeof(motor_parameters_t); i++)
	{
		*((uint8_t *)&motor_parameters + i) = eeprom_read_byte(((uint8_t *)&ee_nvm_data) + i);
		crc = _crc16_update(crc, *((uint8_t *)&motor_parameters + i));
	}

	uint16_t stored_crc = eeprom_read_word(&ee_nvm_crc);

	if (crc == stored_crc)
	{
		debug_P(PSTR("NVM Valid @ %lu\n"), get_current_time());
		nvm_error_flag = ERR_NONE;
	}
	else if (stored_crc == 0)
	{
		nvm_error_flag = nvm_save();
		if (nvm_error_flag == ERR_NONE)
		debug_P(PSTR("NVM Initialized @ %lu\n"), get_current_time());
		else
		debug_P(PSTR("NVM Invalid @ %lu\n"), get_current_time());

		ret_val = post_error(nvm_error_flag);
	}
	else
	{
		debug_P(PSTR("NVM Corrupt @ %lu\n"), get_current_time());
		nvm_error_flag = ERR_NVM_CORRUPT;
		ret_val = post_error(ERR_NVM_CORRUPT);
	}

	exit_critical_section();

#else
	debug_P(PSTR("NVM Valid @ %lu\n"), get_current_time());
	motor_parameters = ee_nvm_data;
	nvm_error_flag = ERR_NONE;
#endif
}

int8_t nvm_save()
{
#if !_FAST_DEBUG
	uint16_t i;
	uint16_t crc = 0;
	uint8_t byte;
	int8_t ret_val = ERR_NONE;
	enter_critical_section();
	for (i = 0; i < sizeof(motor_parameters_t);i++)
	{
		byte = *((uint8_t *)&motor_parameters + i);
		crc = _crc16_update(crc, byte);
		eeprom_write_byte(((uint8_t *)&ee_nvm_data) + i, byte);
		if (byte != eeprom_read_byte(((uint8_t *)&ee_nvm_data) + i))
		ret_val = ERR_NVM_CORRUPT;
	}

	eeprom_write_word(&ee_nvm_crc, crc);

	if (crc != eeprom_read_word(&ee_nvm_crc))
	ret_val = ERR_NVM_CORRUPT;

	if (ERR_NONE == ret_val)
	{
		if (get_last_error(NULL) == ERR_NVM_CORRUPT)
		clear_last_error();

		debug_P(PSTR("NVM Saved @ %lu\n"), get_current_time());
	}
	else
	{
		debug_P(PSTR("NVM Save failed @ %lu\n"), get_current_time());
	}
	nvm_error_flag = ret_val;
	exit_critical_section();
	return post_error(ret_val);
#else
	if (get_last_error(NULL) == ERR_NVM_CORRUPT)
		clear_last_error();

	debug_P(PSTR("NVM Saved @ %lu\n"), get_current_time());
	nvm_error_flag = ERR_NONE;
	return ERR_NONE;
#endif
}

int8_t get_nvm_error_flag()
{
	return nvm_error_flag;
}

