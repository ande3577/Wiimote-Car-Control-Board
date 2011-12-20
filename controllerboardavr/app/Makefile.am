bin_PROGRAMS = controllerboardavr.elf
controllerboardavr_elf_SOURCES = main.c

controllerboardavr_elf_LDADD = ../utility/libutility.la ../hal_avr/libhal.la ../modules/libmodules.la 
controllerboardavr_elf_LDADD += ../terminal/libterminal.la

bin_MAP = $(bin_PROGRAMS:.elf=.map)
bin_HEXFILE = $(bin_PROGRAMS:.elf=.hex)
bin_EEPFILE = $(bin_PROGRAMS:.elf=.eep)

all install install-exec: $(bin_PROGRAMS)
	avr-nm  --format=posix $(bin_PROGRAMS) >  $(bin_MAP)
	avr-size --format=avr --mcu=atmega328p  $(bin_PROGRAMS)
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $(bin_PROGRAMS) $(bin_HEXFILE)
	avr-objcopy -O ihex -R .text -R .fuse -R .lock -R .signature $(bin_PROGRAMS) $(bin_EEPFILE)

clean-local:
	rm -f  $(bin_MAP)
	rm -f $(bin_HEXFILE)
	rm -f $(bin_EEPFILE)