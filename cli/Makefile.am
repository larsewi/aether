AM_CFLAGS = -Wall -Wextra -Wconversion -Wformat

bin_PROGRAMS = aether

aether_LDADD = $(top_builddir)/utils/libutils.la \
    $(top_builddir)/parser/libparser.la \
    $(top_builddir)/interpreter/libinterpreter.la

aether_SOURCES = main.c
