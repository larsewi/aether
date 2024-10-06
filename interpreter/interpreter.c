#include "interpreter.h"
#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../utils/alloc.h"
#include "../utils/logger.h"
#include "../utils/string_lib.h"

Symbol *SymbolIntegerLiteral(Context *const context, const int first_line,
                           const int first_column, const int last_line,
                           const int last_column, const char *const token) {
  Symbol *const symbol = xmalloc(sizeof(Symbol));
  symbol->first.line = first_line;
  symbol->first.column = first_column;
  symbol->last.line = last_line;
  symbol->last.column = last_column;
  symbol->type = SYMBOL_TYPE_OR;
  symbol->data = xmalloc(sizeof(unsigned long long));
  int ret = sscanf(token, "%llu", symbol->data);
  if (ret != 1) {
    LOG_CRITICAL("sscanf(3): Failed to scan float from string '%s': %s", token, ret, strerror(errno));
  }
  return symbol;
}

Symbol *SymbolOr(Context *const context, const int first_line,
                           const int first_column, const int last_line,
                           const int last_column, const char *const token) {
  Symbol *const symbol = xmalloc(sizeof(Symbol));
  symbol->first.line = first_line;
  symbol->first.column = first_column;
  symbol->last.line = last_line;
  symbol->last.column = last_column;
  symbol->type = SYMBOL_TYPE_OR;
  symbol->data = NULL;
  return symbol;
}

Symbol *SymbolAnd(Context *const context, const int first_line,
                           const int first_column, const int last_line,
                           const int last_column, const char *const token) {
  Symbol *const symbol = xmalloc(sizeof(Symbol));
  symbol->first.line = first_line;
  symbol->first.column = first_column;
  symbol->last.line = last_line;
  symbol->last.column = last_column;
  symbol->type = SYMBOL_TYPE_AND;
  symbol->data = NULL;
  return symbol;
}

Symbol *SymbolEqual(Context *const context, const int first_line,
                           const int first_column, const int last_line,
                           const int last_column, const char *const token) {
  Symbol *const symbol = xmalloc(sizeof(Symbol));
  symbol->first.line = first_line;
  symbol->first.column = first_column;
  symbol->last.line = last_line;
  symbol->last.column = last_column;
  symbol->type = SYMBOL_TYPE_EQUAL;
  symbol->data = NULL;
  return symbol;
}

Symbol *SymbolLessEqual(Context *const context, const int first_line,
                           const int first_column, const int last_line,
                           const int last_column, const char *const token) {
  Symbol *const symbol = xmalloc(sizeof(Symbol));
  symbol->first.line = first_line;
  symbol->first.column = first_column;
  symbol->last.line = last_line;
  symbol->last.column = last_column;
  symbol->type = SYMBOL_TYPE_LESS_EQUAL;
  symbol->data = NULL;
  return symbol;
}

Symbol *SymbolGreaterEqual(Context *const context, const int first_line,
                           const int first_column, const int last_line,
                           const int last_column, const char *const token) {
  Symbol *const symbol = xmalloc(sizeof(Symbol));
  symbol->first.line = first_line;
  symbol->first.column = first_column;
  symbol->last.line = last_line;
  symbol->last.column = last_column;
  symbol->type = SYMBOL_TYPE_GREATER_EQUAL;
  symbol->data = NULL;
  return symbol;
}

Symbol *SymbolNotEqual(Context *const context, const int first_line,
                       const int first_column, const int last_line,
                       const int last_column, const char *const token) {
  Symbol *const symbol = xmalloc(sizeof(Symbol));
  symbol->first.line = first_line;
  symbol->first.column = first_column;
  symbol->last.line = last_line;
  symbol->last.column = last_column;
  symbol->type = SYMBOL_TYPE_NOT_EQUAL;
  symbol->data = NULL;
  return symbol;
}

Symbol *SymbolIdentifier(Context *const context, const int first_line,
                         const int first_column, const int last_line,
                         const int last_column, const char *const token) {
  Symbol *const symbol = xmalloc(sizeof(Symbol));
  symbol->first.line = first_line;
  symbol->first.column = first_column;
  symbol->last.line = last_line;
  symbol->last.column = last_column;
  symbol->type = SYMBOL_TYPE_IDENTIFIER;
  symbol->data = StringDuplicate(token);
  return symbol;
}
