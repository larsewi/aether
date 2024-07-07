#include "config.h"

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../interpreter/interpreter.h"
#include "../parser/parser.h"
#include "../parser/syntax.h"
#include "../utils/logger.h"

extern ParserState PARSER_STATE;
extern bool ParseFile(const char *filename);

static const struct option LONG_OPTIONS[] = {
    {"syntax", no_argument, NULL, 's'},
    {"debug", no_argument, NULL, 'd'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
};

static const char *const DESCRIPTIONS[] = {
    "print syntax tree",
    "enable debug logging",
    "print help message",
};

static void PrintHelp(void) {
  printf("%s\n\n", PACKAGE_STRING);

  printf("Usage: %s [OPTIONS] SOURCE\n\n", PACKAGE_NAME);

  size_t longest = 0;
  for (int i = 0; LONG_OPTIONS[i].val != 0; i++) {
    const size_t length = strlen(LONG_OPTIONS[i].name);
    if (length > longest) {
      longest = length;
    }
  }

  char format[64];
  int ret = snprintf(format, sizeof(format), "  --%%-%zus    %%s\n", longest);
  assert(ret >= 0 && (size_t)ret < sizeof(format));

  printf("OPTIONS:\n");
  for (int i = 0; LONG_OPTIONS[i].val != 0; i++) {
    printf(format, LONG_OPTIONS[i].name, DESCRIPTIONS[i]);
  }

  printf("\nReport bugs to: <%s>\n", PACKAGE_BUGREPORT);
  printf("%s home page: <%s>\n", PACKAGE_NAME, PACKAGE_URL);
}

int main(int argc, char *argv[]) {
  bool print_syntax_tree = false;

  int c;
  while ((c = getopt_long(argc, argv, "sdht", LONG_OPTIONS, NULL)) != -1) {
    switch (c) {
    case 's':
      print_syntax_tree = true;
      break;

    case 'd':
      LoggerSetDebug(true);
      break;

    case 'h':
      PrintHelp();
      return EXIT_SUCCESS;

    case '?':
      // Error already printed by getopt_long(3)
      return EXIT_FAILURE;

    default:
      LOG_CRITICAL("Unhandled option '%c'", c);
    }
  }

  if (optind >= argc) {
    LOG_ERROR("Missing argument SOURCE ...");
    return EXIT_FAILURE;
  }
  const char *filename = argv[optind++];

  if (!ParseFile(filename)) {
    return EXIT_FAILURE;
  }

  WalkSyntaxTree(PARSER_STATE.expr, print_syntax_tree);

  return EXIT_SUCCESS;
}
