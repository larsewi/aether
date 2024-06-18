#include "../tests/check.h"
#include "list.c"

#include <string.h>

static void test_ListCreate(void) {
  List *list = ListCreate();
  check(list != NULL);
  check(list->buffer != NULL);
  check(list->length == 0);
  free(list->buffer);
  free(list);
}

static void test_ListDestroy(void) {
  List *list = ListCreate();
  ListDestroy(list);
}

static void test_ListLength(void) {
  List list = {
      .length = 3,
  };
  check(ListLength(&list) == 3);
}

static void test_ListAppend(void) {
  List *list = ListCreate();

  ListAppend(list, "foo", NULL);
  ListAppend(list, strdup("bar"), free);
  check(list->length == 2);

  check(list->buffer[0]->destroy == NULL);
  check(strcmp(list->buffer[0]->value, "foo") == 0);

  check(list->buffer[1]->destroy == free);
  check(strcmp(list->buffer[1]->value, "bar") == 0);

  ListDestroy(list);
}

static void test_ListGet(void) {
  List *list = ListCreate();
  ListAppend(list, "foo", NULL);
  ListAppend(list, "bar", NULL);
  ListAppend(list, "baz", NULL);
  check(strcmp(ListGet(list, 0), "foo") == 0);
  check(strcmp(ListGet(list, 1), "bar") == 0);
  check(strcmp(ListGet(list, 2), "baz") == 0);
  ListDestroy(list);
}

static void test_ListSet(void) {
  List *list = ListCreate();
  ListAppend(list, "one", NULL);
  ListAppend(list, strdup("two"), free);
  ListAppend(list, "three", NULL);

  ListSet(list, 0, "four", NULL);
  ListSet(list, 2, strdup("six"), free);

  check(strcmp((char *)ListGet(list, 0), "four") == 0);
  check(strcmp((char *)ListGet(list, 1), "two") == 0);
  check(strcmp((char *)ListGet(list, 2), "six") == 0);

  ListDestroy(list);
}

static void test_ListRemove(void) {
  List *list = ListCreate();
  ListAppend(list, "one", NULL);
  ListAppend(list, "two", NULL);
  ListAppend(list, "three", NULL);
  ListAppend(list, strdup("four"), free);
  ListAppend(list, strdup("five"), free);

  check(list->length = 5);
  check(strcmp(ListRemove(list, 2), "three") == 0);
  check(list->length = 4);
  check(strcmp(ListRemove(list, 0), "one") == 0);
  check(list->length = 3);
  char *const val = (char *)ListRemove(list, 2);
  check(list->length = 2);
  check(strcmp(val, "five") == 0);
  free(val);
  check(strcmp(ListGet(list, 0), "two") == 0);
  check(strcmp(ListGet(list, 1), "four") == 0);
  ListDestroy(list);
}

static void test_ListInsert(void) {
  List *list = ListCreate();

  ListAppend(list, "two", NULL);
  ListAppend(list, strdup("four"), free);

  ListInsert(list, 0, "one", NULL);
  ListInsert(list, 2, strdup("three"), free);
  ListInsert(list, 4, "five", NULL);

  check(strcmp((char *)ListGet(list, 0), "one") == 0);
  check(strcmp((char *)ListGet(list, 1), "two") == 0);
  check(strcmp((char *)ListGet(list, 2), "three") == 0);
  check(strcmp((char *)ListGet(list, 3), "four") == 0);
  check(strcmp((char *)ListGet(list, 4), "five") == 0);

  ListDestroy(list);
}

CHECK_BEGIN
CHECK_ADD("ListCreate", test_ListCreate)
CHECK_ADD("ListDestroy", test_ListDestroy)
CHECK_ADD("ListLength", test_ListLength)
CHECK_ADD("ListAppend", test_ListAppend)
CHECK_ADD("ListGet", test_ListGet)
CHECK_ADD("ListSet", test_ListSet)
CHECK_ADD("ListRemove", test_ListRemove)
CHECK_ADD("ListInsert", test_ListInsert)
CHECK_END