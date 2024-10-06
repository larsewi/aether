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

#define CHECK_BEGIN                                                            \
  int main(int argc, char *argv[]) {                                           \
    Test t[] = {

#define CHECK_ADD(name, func) {name, func},

#define CHECK_END                                                              \
  {                                                                            \
    NULL, NULL                                                                 \
  }                                                                            \
  }                                                                            \
  ;                                                                            \
  for (size_t i = 0; argc >= 2 && t[i].name != NULL; i++) {                    \
    if (strcmp(t[i].name, argv[1]) == 0) {                                     \
      t[i].func();                                                             \
      return EXIT_SUCCESS;                                                     \
    }                                                                          \
  }                                                                            \
  return EXIT_FAILURE;                                                         \
  }

#define CHECK_RUN(...)                                                         \
  int main(int argc, char *argv[]) {                                           \
    Test t[] = ##__VA_ARGS__;                                                  \
    for (size_t i = 0; argc >= 2 && t[i].name != NULL; i++) {                  \
      if (strcmp(t[i].name, argv[1]) == 0) {                                   \
        t[i].func();                                                           \
        return EXIT_SUCCESS;                                                   \
      }                                                                        \
    }                                                                          \
    return EXIT_FAILURE;                                                       \
  }
