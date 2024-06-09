#include "buffer.c"

#include <stdlib.h>

#include "../tests/check.h"

static void test_BufferCreate(void) {
  Buffer *const buf = BufferCreate();
  check(buf != NULL);
  check(buf->capacity == BUFFER_SIZE);
  check(buf->length == 0);
  check(strcmp(buf->buffer, "") == 0);
  free(buf->buffer);
  free(buf);
}

static void test_BufferData(void) {
  Buffer buf = {
      .buffer = "foo",
  };
  check(strcmp(BufferData(&buf), "foo") == 0);
}

static void test_BufferLength(void) {
  Buffer buf = {
      .length = 3,
  };
  check(BufferLength(&buf) == 3);
}

static void test_BufferAppend(void) {
  Buffer buf = {0};
  const char *const str = "foo";
  for (size_t i = 0; i < strlen(str); i++) {
    BufferAppend(&buf, str[i]);
  }
  check(strcmp(buf.buffer, str) == 0);
  check(buf.length == 3);
  free(buf.buffer);
}

static void test_BufferPrint(void) {
  Buffer buf = {0};
  const char *const str = "foo";
  BufferPrint(&buf, str);
  check(strcmp(buf.buffer, str) == 0);
  check(buf.length == 3);
  free(buf.buffer);
}

static void test_BufferPrintFormat(void) {
  Buffer buf = {0};
  const char *const str = "foo";
  BufferPrintFormat(&buf, "%s", str);
  check(strcmp(buf.buffer, str) == 0);
  check(buf.length == 3);
  free(buf.buffer);
}

static void test_BufferToString(void) {
  Buffer *buf = BufferCreate();
  char *str = BufferToString(buf);
  check(strcmp(str, "") == 0);
  free(str);
}

static void test_BufferFromString(void) {
  const char *const str = "foo";
  Buffer *buf = BufferFromString(str);
  check(strcmp(buf->buffer, str) == 0);
  check(buf->length == 3);
  free(buf->buffer);
  free(buf);
}

static void test_BufferReadFile(void) {
  char filename[] = "testfile_XXXXXX";
  check(mkstemp(filename));

  const char *const str = "foo";
  const size_t len = strlen(str);

  const int fd = open(filename, O_WRONLY | O_TRUNC);
  check(fd != -1);
  size_t tot = 0;
  while (tot < len) {
    ssize_t num = write(fd, str + tot, len - tot);
    check(num != -1);
    tot += (size_t)num;
  }
  close(fd);

  Buffer buf = {0};
  check(BufferReadFile(&buf, filename));
  check(strcmp(buf.buffer, str) == 0);
  check(buf.length == 3);
  unlink(filename);
}

static void test_BufferDestroy(void) {
  Buffer *buf = BufferCreate();
  BufferDestroy(buf);
}

int main(int argc, char *argv[]) {
  Test tests[] = {
      {"BufferCreate", test_BufferCreate},
      {"BufferData", test_BufferData},
      {"BufferLength", test_BufferLength},
      {"BufferAppend", test_BufferAppend},
      {"BufferPrint", test_BufferPrint},
      {"BufferPrintFormat", test_BufferPrintFormat},
      {"BufferToString", test_BufferToString},
      {"BufferFromString", test_BufferFromString},
      {"BufferReadFile", test_BufferReadFile},
      {"BufferDestroy", test_BufferDestroy},
      {NULL, NULL},
  };

  for (size_t i = 0; argc >= 2 && tests[i].name != NULL; i++) {
    if (strcmp(tests[i].name, argv[1]) == 0) {
      tests[i].func();
      return EXIT_SUCCESS;
    }
  }

  return EXIT_FAILURE;
}
