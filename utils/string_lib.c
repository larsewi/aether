#include "string_lib.h"

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <logger.h>

bool StringEqual(const char *const str_1, const char *const str_2) {
  assert(str_1 != NULL);
  assert(str_2 != NULL);
  return strcmp(str_1, str_2) == 0;
}

char *StringFormat(const char *const format, ...) {
  assert(format != NULL);

  va_list ap;
  va_start(ap, format);
  const int length = vsnprintf(NULL, 0, format, ap);
  assert(length >= 0);
  va_end(ap);

  char *const str = (char *)malloc((size_t)length + 1);
  if (str == NULL) {
    LOG_CRITICAL("malloc(3): Failed to allocate memory: %s", strerror(errno));
  }

  va_start(ap, format);
  const int ret = vsnprintf(str, (size_t)length + 1, format, ap);
  va_end(ap);
  if (ret != length) {
    LOG_CRITICAL("vsnprintf(3): Unexpected return value (%d != %d)", ret,
                 length);
  }

  return str;
}

char *StringDuplicate(const char *const str) {
  assert(str != NULL);

  char *duplicate = strdup(str);
  if (duplicate == NULL) {
    LOG_CRITICAL("strdup(3): Failed to duplicate string: %s", strerror(errno));
  }

  return duplicate;
}

char *StringDuplicateN(const char *const str, const size_t num) {
  assert(str != NULL);

  char *duplicate = strndup(str, num);
  if (duplicate == NULL) {
    LOG_CRITICAL("strdup(3): Failed to duplicate string: %s", strerror(errno));
  }

  return duplicate;
}
