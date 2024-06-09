AM_CFLAGS = -Wall -Wextra -Wconversion -Wformat

lib_LTLIBRARIES = libutils.la
libutils_la_SOURCES = \
    string_lib.h string_lib.c \
    logger.h logger.c \
    buffer.h buffer.c

check_PROGRAMS = test_string_lib test_buffer

test_string_lib_LDADD = libutils.la
test_string_lib_SOURCES = test_string_lib.c

test_buffer_LDADD = libutils.la
test_buffer_SOURCES = test_buffer.c
