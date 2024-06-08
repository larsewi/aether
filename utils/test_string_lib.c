#include "string_lib.c"

int main(void) {
    if (StringEqual("foo", "bar")) {
        return 1;
    }

    if (StringEqual("bar", "foo")) {
        return 1;
    }

    if (!StringEqual("foo", "foo")) {
        return 1;
    }

    if (!StringEqual("bar", "bar")) {
        return 1;
    }

    return 0;
}
