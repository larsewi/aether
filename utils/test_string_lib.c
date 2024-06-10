#include "../tests/check.h"
#include "string_lib.c"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void test_StringEqual(void) {
  check(!StringEqual("foo", "bar"));
  check(!StringEqual("bar", "foo"));
  check(StringEqual("foo", "foo"));
  check(StringEqual("bar", "bar"));
}

static void test_StringFormat(void) {
  char *str = StringFormat("%s %s\n", "Hello", "World");
  check(strcmp(str, "Hello World\n") == 0);
  free(str);
}

CHECK_BEGIN
CHECK_ADD("StringEqual", test_StringEqual)
CHECK_ADD("StringFormat", test_StringFormat)
CHECK_END
