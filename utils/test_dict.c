#include "../tests/check.h"
#include "dict.c"

static void test_DictCreate(void) {
  Dict *dict = DictCreate();
  free(dict->buffer);
  free(dict);
}

static void test_DictDestroy(void) {
  Dict *dict = DictCreate();
  DictDestroy(dict);
}

static void test_DictLength(void) {
  Dict dict = {
      .length = 3,
  };
  check(DictLength(&dict) == 3);
}

static void test_DictSet(void) {
  Dict *dict = DictCreate();
  DictSet(dict, "foo", "bar", NULL);
  DictSet(dict, "baz", strdup("qux"), free);
  DictDestroy(dict);
}

static void test_DictHasKey(void) {
  Dict *dict = DictCreate();
  DictSet(dict, "foo", NULL, NULL);
  DictSet(dict, "baz", NULL, NULL);
  check(DictHasKey(dict, "foo"));
  check(!DictHasKey(dict, "bar"));
  check(DictHasKey(dict, "baz"));
  DictDestroy(dict);
}

static void test_DictGetKeys(void) {
  Dict *dict = DictCreate();
  DictSet(dict, "foo", NULL, NULL);
  DictSet(dict, "bar", NULL, NULL);
  DictSet(dict, "baz", NULL, NULL);
  List *keys = DictGetKeys(dict);
  DictDestroy(dict);

  check(ListLength(keys) == 3);
  check(strcmp(ListGet(keys, 0), "foo") == 0);
  check(strcmp(ListGet(keys, 1), "bar") == 0);
  check(strcmp(ListGet(keys, 2), "baz") == 0);
  ListDestroy(keys);
}

static void test_DictGet(void) {
  Dict *dict = DictCreate();
  DictSet(dict, "foo", "bogus", NULL);
  DictSet(dict, "bar", strdup("doofus"), free);

  check(strcmp(DictGet(dict, "foo"), "bogus") == 0);
  check(strcmp(DictGet(dict, "bar"), "doofus") == 0);

  DictDestroy(dict);
}

static void test_DictRemove(void) {
  Dict *dict = DictCreate();
  DictSet(dict, "foo", "one", NULL);
  DictSet(dict, "bar", "two", NULL);
  DictSet(dict, "baz", strdup("three"), free);

  check(DictHasKey(dict, "foo"));
  check(strcmp(DictRemove(dict, "foo"), "one") == 0);
  check(!DictHasKey(dict, "foo"));

  check(DictHasKey(dict, "bar"));
  check(strcmp(DictRemove(dict, "bar"), "two") == 0);
  check(!DictHasKey(dict, "bar"));

  check(DictHasKey(dict, "baz"));
  char *val = (char *)DictRemove(dict, "baz");
  check(strcmp(val, "three") == 0);
  free(val);
  check(!DictHasKey(dict, "baz"));

  DictDestroy(dict);
}

CHECK_BEGIN
CHECK_ADD("DictCreate", test_DictCreate)
CHECK_ADD("DictDestroy", test_DictDestroy)
CHECK_ADD("DictLength", test_DictLength)
CHECK_ADD("DictSet", test_DictSet)
CHECK_ADD("DictHasKey", test_DictHasKey)
CHECK_ADD("DictGetKeys", test_DictGetKeys)
CHECK_ADD("DictGet", test_DictGet)
CHECK_ADD("DictRemove", test_DictRemove)
CHECK_END
