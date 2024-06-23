#include "config.h"

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parser/parser.h"
#include "../parser/parser_state.h"
#include "../utils/logger.h"

extern FILE *yyin;
ParserState PARSER_STATE = {.line = 1, .col = 1};

static const struct option long_options[] = {
    {"filename", required_argument, 0, 'f'},
    {"debug", no_argument, 0, 'd'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0},
};

static const char *const descriptions[] = {
    "input file",
    "enable debug logging",
    "print help message",
};

static void PrintHelp(void) {
  printf("%s\n\n", PACKAGE_STRING);

  size_t longest = 0;
  for (int i = 0; long_options[i].val != 0; i++) {
    const size_t length = strlen(long_options[i].name);
    if (length > longest) {
      longest = length;
    }
  }

  char format[512];
  int ret = snprintf(format, sizeof(format), "  --%%-%zus %%s\n", longest);
  assert(ret >= 0 && (size_t)ret < sizeof(format));

  printf("Options:\n");
  for (int i = 0; long_options[i].val != 0; i++) {
    printf(format, long_options[i].name, descriptions[i]);
  }

  printf("\nReport bugs to: <%s>\n", PACKAGE_BUGREPORT);
  printf("%s home page: <%s>\n", PACKAGE_NAME, PACKAGE_URL);
}

int main(int argc, char *argv[]) {
  const char *filename = NULL;

  int c;
  while ((c = getopt_long(argc, argv, "f:dh", long_options, NULL)) != -1) {
    switch (c) {
    case 'f':
      filename = optarg;
      break;

    case 'd':
      LoggerSetDebug(true);
      break;

    case 'h':
      PrintHelp();
      return 0;

    default:
      LOG_ERROR("Bad argument ...");
      PrintHelp();
      return 1;
    }
  }

  yyin = fopen(filename, "r");
  if (yyin == NULL) {
    LOG_ERROR("Failed to open file '%s': %s", filename, strerror(errno));
    return EXIT_FAILURE;
  }

  while (!feof(yyin)) {
    yyparse();

    if (ferror(yyin)) {
      LOG_ERROR("Failed to parse file '%s': %s", strerror(errno));
      fclose(yyin);
      return EXIT_FAILURE;
    }
  }

  fclose(yyin);

  return EXIT_SUCCESS;
}
