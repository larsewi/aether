#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "string_lib.c"

bool TestStringEqual(void) {
  if (StringEqual("foo", "bar")) {
    return false;
  }

  if (StringEqual("bar", "foo")) {
    return false;
  }

  if (!StringEqual("foo", "foo")) {
    return false;
  }

  if (!StringEqual("bar", "bar")) {
    return false;
  }

  return true;
}

bool TestStringFormat(void) {
  char *str = StringFormat("%s %s\n", "Hello", "World");
  bool success = StringEqual(str, "Hello World\n");
  free(str);
  return success;
}

int main(int argc, char *argv[]) {
  if ((argc <= 2) &&
      ((strcmp(argv[1], "StringEqual") == 0 && TestStringEqual()) ||
       (strcmp(argv[1], "StringFormat") == 0 && TestStringFormat()))) {
    return 0;
  }
}
