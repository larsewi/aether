#ifndef _AETHER_LOGGER_H
#define _AETHER_LOGGER_H

#include <stdbool.h>

enum LoggerLogLevel {
  LOGGER_MESSAGE_TYPE_DEBUG,
  LOGGER_MESSAGE_TYPE_WARNING,
  LOGGER_MESSAGE_TYPE_ERROR,
  LOGGER_MESSAGE_TYPE_CRITICAL,
};

/**
 * @brief Log a debug message using a format string and arguments.
 * @note Debug messages are printed to stdout if and only if debug messaging
 *       is enabled through LoggerSetDebug(). These are messages are intended
 *       to aid developers in debugging the software.
 */
#define LOG_DEBUG(...)                                                         \
  LoggerLogMessage(LOGGER_MESSAGE_TYPE_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Log a warning message using a format string and arguments.
 * @note These messages are intended to warn the user against common mistakes.
 */
#define LOG_WARNING(...)                                                       \
  LoggerLogMessage(LOGGER_MESSAGE_TYPE_WARNING, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Log a debug message using a format string and arguments.
 * @note These messages are intended to inform about recoverable errors.
 */
#define LOG_ERROR(...)                                                         \
  LoggerLogMessage(LOGGER_MESSAGE_TYPE_ERROR, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Log a debug message using a format string and arguments.
 * @note These are messages intended to inform about irrecoverable errors.
 * @warning Any call to LOG_CRITICAL(...) will print the message and abort the
 *          program.
 */
#define LOG_CRITICAL(...)                                                      \
  LoggerLogMessage(LOGGER_MESSAGE_TYPE_CRITICAL, __FILE__, __LINE__,           \
                   __VA_ARGS__)

/**
 * @brief Enable debug log messages.
 * @param enable Enable debug log messages if true, disable if false.
 * @note Debugging is disabled by default.
 */
void LoggerSetDebug(bool enable);

/**
 * @brief Format log message.
 * @param level Type of message. One of; LOG_DEBUG, LOG_WARNING, LOG_ERROR or
 *              LOG_CRITICAL
 * @param file Name of file from where this function is called.
 * @param line Line number in file from where this function is called.
 * @param format Format string.
 * @param ... Arguments to be converted.
 * @note The parameters file and line are required when level is LOG_DEBUG or
 *       LOG_CRITICAL, and will be ignored for any other value of level. If
 *       level is LOG_CRITICAL, the logger will log the message and then abort
 *       the program.
 * @warning Don't use this function directly, use macros LOG_DEBUG(...),
 *          LOG_WARNING(...), LOG_ERROR(...), LOG_CRITICAL(...) instead.
 */
void LoggerLogMessage(enum LoggerLogLevel level, const char *file,
                      const int line, const char *format, ...);

#endif // _AETHER_LOGGER_H
