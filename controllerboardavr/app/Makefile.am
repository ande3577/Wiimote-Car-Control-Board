bin_PROGRAMS = controllerboardavr.elf
controllerboardavr_elf_SOURCES = main.c

controllerboardavr_elf_LDADD = ../utility/libutility.la ../hal_avr/libhal.la ../modules/libmodules.la 
controllerboardavr_elf_LDADD += ../terminal/libterminal.la

controllerboardavr.map : bin_PROGRAMS
	avr-nm  --format=posix controllerboardavr.elf > controllerboardavr.map

bin_HEXFILE : bin_PROGRAMS
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature controllerboardavr.elf controllerboardavr.hex

bin_SIZE : bin_PROGRAMS
	avr-size --format=avr --mcu=atmega644 controllerboardavr.elf