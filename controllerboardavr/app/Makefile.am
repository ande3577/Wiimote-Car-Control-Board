bin_PROGRAMS = controllerboardavr.elf
controllerboardavr_elf_SOURCES = main.c

controllerboardavr_elf_LDADD = ../utility/libutility.la ../hal_avr/libhal.la ../modules/libmodules.la 
controllerboardavr_elf_LDADD += ../terminal/libterminal.la


all install install-exec: $(bin_PROGRAMS)
	avr-nm  --format=posix $(bin_PROGRAMS) > controllerboardavr.map
	avr-size --format=avr --mcu=atmega328p  $(bin_PROGRAMS)
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $(bin_PROGRAMS) controllerboardavr.hex
	avr-objcopy -O ihex -R .text -R .fuse -R .lock -R .signature $(bin_PROGRAMS) controllerboardavr.eep

clean-local:
	rm -f controllerboardavr.map
	rm -f controllerboardavr.hex
	rm -f controllerboardavr.eep