#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([bash_pattern], [0.1], [leo.singer@ligo.org])
AC_CONFIG_SRCDIR([bash_pattern.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([silent-rules foreign])
LT_INIT

# Enable silent build rules if this version of Automake supports them
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.

AX_CREATE_PKGCONFIG_INFO
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
