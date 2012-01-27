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
