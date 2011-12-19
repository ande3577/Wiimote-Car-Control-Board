#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([Controller Board AVR], [0.0.10], [https://github.com/ande3577/Wiimote-Car-Control-Board])
AC_CONFIG_HEADERS([config.h])


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(-Wall)

case $target in
	avr*)
		;;
	*)
		AC_MSG_ERROR([invalid target $target])
		;;
esac

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])

# Checks for libraries.
AC_CHECK_LIB([printf_min], [printf], , AC_MSG_ERROR([cannot find printf-min]))
AC_CHECK_LIB([scanf_min], [scanf], , AC_MSG_ERROR([cannot find printf-min]))

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.

# Setup build variables
CFLAGS="-Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=14745600UL" 
DFLAGS="-Wl,--gc-sections -Wl,--relax -Wl,-u,vfprintf -lprintf_min -Wl,-u,vfscanf -lscanf_min --mmcu=atmega328p"

AC_CONFIG_FILES([Makefile
                 app/Makefile
                 hal_avr/Makefile
                 modules/Makefile
                 terminal/Makefile
                 utility/Makefile])
AC_OUTPUT
