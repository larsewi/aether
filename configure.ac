#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([aether],
        [0.0.1],
        [https://github.com/larsewi/aether/issues],
        [aether],
        [https://github.com/larsewi/aether])
AC_CONFIG_SRCDIR([utils/string_lib.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_YACC
AC_PROG_LEX(noyywrap)
AM_PROG_AR

LT_INIT

AC_DEFINE([BUFFER_SIZE], 1024,
          [Default initial buffer size allocated by aether])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADER_STDBOOL

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_TESTDIR([tests])
AC_CONFIG_FILES([Makefile
                 cli/Makefile
                 parser/Makefile
                 interpreter/Makefile
                 utils/Makefile
                 tests/Makefile])
AC_OUTPUT
