#include "string_lib.h"

#include <assert.h>
#include <string.h>

bool StringEqual(const char *const str_1, const char *const str_2) {
    assert(str_1 != NULL);
    assert(str_2 != NULL);
    return strcmp(str_1, str_2) == 0;
}
