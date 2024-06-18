#ifndef _AETHER_STRING_LIB_H
#define _AETHER_STRING_LIB_H

#include <stdbool.h>

/**
 * @brief Check if two strings are equal.
 * @param str_1 First string to check.
 * @param str_2 Second string to check.
 * @return True if strings are equal, otherwise false.
 */
bool StringEqual(const char *str_1, const char *str_2);

/**
 * @brief Produce string according to format.
 * @param format Format string.
 * @param ... arguments to be converted.
 * @return
 */
char *StringFormat(const char *format, ...);

/**
 * @brief Duplicate a string.
 * @param str The string.
 * @return The duplicate.
 * @note Caller takes ownership of returned value.
 */
char *StringDuplicate(const char *const str);

#endif // _AETHER_STRING_LIB_H
