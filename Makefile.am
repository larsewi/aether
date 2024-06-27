ACLOCAL_AMFLAGS = -I m4
SUBDIRS = utils parser interpreter cli . tests

format:
	clang-format -i --verbose **/*.{c,h}

super-clean:
	git clean -fxd
