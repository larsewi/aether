#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char *const name;
  void (*func)(void);
} Test;

void _assert_failed(const char *const file, const int line,
                    const char *const expr) {
  fprintf(stderr, "Assertion '%s' failed in %s:%d\n", expr, file, line);
  exit(22);
}

#define check(expr)                                                            \
  if (!(expr))                                                                 \
  _assert_failed(__FILE__, __LINE__, #expr)
