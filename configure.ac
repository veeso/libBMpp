#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libBMPP], [0.1.0], [https://github.com/ChristianVisintin/libBMPP])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_SRCDIR([include/bmp.hpp])
AC_CONFIG_HEADERS([config.h])
AC_LANG(C++)
AC_REQUIRE_CPP

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AM_PROG_AR

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])


# Checks for library functions.

AC_OUTPUT(Makefile src/Makefile)