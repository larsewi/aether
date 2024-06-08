AM_CFLAGS = -Wall -Wextra -Wconversion -Wformat
BUILT_SOURCES = parser.h
AM_YFLAGS = -d

lib_LTLIBRARIES = libparser.la

libparser_la_LIBADD = $(top_builddir)/utils/libutils.la

libparser_la_SOURCES = lexer.l parser.y

