#include "../tests/check.h"
#include "logger.c"

static void test_LOG_DEBUG(void) {
  LOG_DEBUG("%s", "foo");
  LoggerSetDebug(true);
  LOG_DEBUG("%s", "bar");
}

static void test_LOG_WARNING(void) { LOG_WARNING("%s", "foo"); }

static void test_LOG_ERROR(void) { LOG_ERROR("%s", "foo"); }

static void test_LOG_CRITICAL(void) { LOG_CRITICAL("%s", "foo"); }

CHECK_BEGIN
CHECK_ADD("LOG_DEBUG", test_LOG_DEBUG)
CHECK_ADD("LOG_WARNING", test_LOG_WARNING)
CHECK_ADD("LOG_ERROR", test_LOG_ERROR)
CHECK_ADD("LOG_CRITICAL", test_LOG_CRITICAL)
CHECK_END
