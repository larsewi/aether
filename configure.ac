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

AC_DEFINE([DEFAULT_BUFFER_CAPACITY], 1024,
          [Default initial buffer capacity allocated by aether])
AC_DEFINE([DEFAULT_LIST_CAPACITY], 256,
          [Default initial list capacity allocated by aether])
AC_DEFINE([DEFAULT_DICT_CAPACITY], 256,
          [Default initial dictionary capacity allocated by aether])
AC_DEFINE([DEFAULT_DICT_MAX_LOAD_FACTOR], 0.75f,
          [Default dictionary max load factor used by aether (i.e., when do we expand the internal buffer)])
AC_DEFINE([DEFAULT_DICT_MIN_LOAD_FACTOR], 0.5f,
          [Default dictionary min load factor used by aether (i.e., when do we remove invalidated entries)])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADER_STDBOOL
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([fcntl.h inttypes.h libintl.h malloc.h stdint.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strerror])

AC_CONFIG_TESTDIR([tests])
AC_CONFIG_FILES([Makefile
                 cli/Makefile
                 parser/Makefile
                 interpreter/Makefile
                 utils/Makefile
                 tests/Makefile])
AC_OUTPUT
