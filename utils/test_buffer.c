#include "buffer.c"

#include <stdlib.h>

#include "../tests/check.h"

static void test_BufferCreate(void) {
  Buffer *const buf = BufferCreate();
  check(buf != NULL);
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
  BufferDestroy(NULL);
}

CHECK_BEGIN
CHECK_ADD("BufferCreate", test_BufferCreate)
CHECK_ADD("BufferData", test_BufferData)
CHECK_ADD("BufferLength", test_BufferLength)
CHECK_ADD("BufferAppend", test_BufferAppend)
CHECK_ADD("BufferPrint", test_BufferPrint)
CHECK_ADD("BufferPrintFormat", test_BufferPrintFormat)
CHECK_ADD("BufferToString", test_BufferToString)
CHECK_ADD("BufferFromString", test_BufferFromString)
CHECK_ADD("BufferReadFile", test_BufferReadFile)
CHECK_ADD("BufferDestroy", test_BufferDestroy)
CHECK_END
