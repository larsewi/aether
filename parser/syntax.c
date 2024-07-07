#include "syntax.h"
#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../utils/alloc.h"
#include "../utils/string_lib.h"
#include "parser.h"

#define P PARSER_STATE

extern FILE *yyin;
ParserState PARSER_STATE = {0};

extern void yylex_destroy();

bool ParseFile(const char *const filename) {
  P.filename = filename;
  P.line = 1;
  P.column = 1;

  LOG_DEBUG("Parsing file '%s'", filename);

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
  yylex_destroy();
  return true;
}
