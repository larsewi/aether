#include "syntax.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../utils/alloc.h"
#include "../utils/string_lib.h"
#include "parser.h"

extern FILE *yyin;
ParserState PARSER_STATE = {0};

bool ParseFile(const char *const filename) {
  PARSER_STATE.filename = StringDuplicate(filename);
  PARSER_STATE.line = 1;
  PARSER_STATE.column = 1;

  yyin = fopen(filename, "r");
  if (yyin == NULL) {
    LOG_ERROR("Failed to open file '%s': %s", filename, strerror(errno));
    return false;
  }

  while (!feof(yyin)) {
    yyparse();

    if (ferror(yyin)) {
      LOG_ERROR("Failed to parse file '%s': %s", strerror(errno));
      fclose(yyin);
      return false;
    }
  }

  fclose(yyin);
  return true;
}

void PrintSyntaxTree(void) { printf("Hello World\n"); }
