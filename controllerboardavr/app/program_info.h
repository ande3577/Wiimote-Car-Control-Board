/*
 * program_info.h
 *
 *  Created on: Sep 7, 2010
 *      Author: dsanderson
 *
 *
 *  Revision History:
 *  	0.0.1 - initial communications test (PC Based)
 *  	0.0.2 - ported to STM32 - support for dev board or BT Car (latter untested)
 *      0.0.3 - works on actual hardware, returns ERR_EXEC if channel blocked
 *      0.0.4 - ported to avr8
 *      0.0.5 - added support for jump to bootloader
 *      	  - reworked key mapping
 *      	  - added pushbutton support
 *      	  - ported to atmega644
 *      	  - added lcd support (hd44780 based)
 *      0.0.6 - ported to atmega328
 *      	  - added motor state machine
 *      	  - added encoder
 *      0.0.7 - added primitive scheduler
 *      	  - ring buffer index changed to uint16_t, and typedef'd
 *     	0.0.8 - added sleep mode
 *     	0.0.9 - removed redundant comm buffers from terminal
 *     		  - ported to avr-eclipse plugin instead of manual makefile
 */

#ifndef PROGRAM_INFO_H_
#define PROGRAM_INFO_H_

#define PROGRAM_NAME "ControllerBoard"
#define PROGRAM_VERSION "0.0.9"

#endif /* PROGRAM_INFO_H_ */