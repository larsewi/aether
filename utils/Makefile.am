AM_CFLAGS = -Wall -Wextra -Wconversion -Wformat

lib_LTLIBRARIES = libutils.la
libutils_la_SOURCES = \
    string_lib.h string_lib.c \
    logger.h logger.c \
    buffer.h buffer.c \
    list.h list.c \
    dict.h dict.c

check_PROGRAMS = \
    test_logger \
    test_string_lib \
    test_buffer \
    test_list \
    test_dict

test_logger_LDADD = libutils.la
test_logger_SOURCES = test_logger.c

test_string_lib_LDADD = libutils.la
test_string_lib_SOURCES = test_string_lib.c

test_buffer_LDADD = libutils.la
test_buffer_SOURCES = test_buffer.c

test_list_LDADD = libutils.la
test_list_SOURCES = test_list.c

test_dict_LDADD = libutils.la
test_dict_SOURCES = test_dict.c
