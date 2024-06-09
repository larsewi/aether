#include "../tests/check.h"
#include "logger.h"

static void test_LOG_DEBUG(void) {
  LOG_DEBUG("%s", "foo");
  LoggerSetDebug(true);
  LOG_DEBUG("%s", "bar");
}

static void test_LOG_WARNING(void) { LOG_WARNING("%s", "foo"); }

static void test_LOG_ERROR(void) { LOG_ERROR("%s", "foo"); }

static void test_LOG_CRITICAL(void) { LOG_CRITICAL("%s", "foo"); }

int main(int argc, char *argv[]) {
  Test tests[] = {
      {"LOG_DEBUG", test_LOG_DEBUG},
      {"LOG_WARNING", test_LOG_WARNING},
      {"LOG_ERROR", test_LOG_ERROR},
      {"LOG_CRITICAL", test_LOG_CRITICAL},
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