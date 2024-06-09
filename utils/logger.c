#include "logger.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static bool LOGGER_LOG_DEBUG = false;

void LoggerSetDebug(const bool enable) { LOGGER_LOG_DEBUG = enable; }

void LoggerLogMessage(enum LoggerLogLevel level, const char *file,
                      const int line, const char *format, ...) {
  assert(format != NULL);

  va_list ap;
  va_start(ap, format);

  char message[4096];
  int size = vsnprintf(message, sizeof(message), format, ap);
  if (size < 0 || (size_t)size >= sizeof(message)) {
    LOG_WARNING("The following log message is trucated: Too long (%d >= %d)",
                size, sizeof(message));
  }

  va_end(ap);

  switch (level) {
  case LOGGER_MESSAGE_TYPE_DEBUG:
    if (LOGGER_LOG_DEBUG) {
      fprintf(stdout, "DEBUG [%s:%d]: %s\n", file, line, message);
    }
    break;
  case LOGGER_MESSAGE_TYPE_WARNING:
    fprintf(stdout, "WARNING: %s\n", message);
    break;
  case LOGGER_MESSAGE_TYPE_ERROR:
    fprintf(stderr, "ERROR: %s\n", message);
    break;
  case LOGGER_MESSAGE_TYPE_CRITICAL:
    fprintf(stderr, "CRITICAL [%s:%d]: %s\n", file, line, message);
    abort(); // It is not safe to proceed
  }
}
