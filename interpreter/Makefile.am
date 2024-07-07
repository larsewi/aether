AM_CFLAGS = -Wall -Wextra -Wconversion -Wformat

lib_LTLIBRARIES = libinterpreter.la

libinterpreter_la_LIBADD = $(top_builddir)/utils/libutils.la \
    $(top_builddir)/parser/libparser.la

libinterpreter_la_SOURCES = interpreter.h interpreter.c
