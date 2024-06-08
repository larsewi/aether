ACLOCAL_AMFLAGS = -I m4
SUBDIRS = utils parser interpreter cli . tests

format:
	clang-format -i utils/*.{c,h} parser/*.{c,h} interpreter/*.{c,h} cli/*.{c,h} tests/*{c.h}

super-clean:
	git clean -fxd
