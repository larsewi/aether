#ifndef _AETHER_INTERPRETER_H
#define _AETHER_INTERPRETER_H

#include <stdbool.h>

typedef enum {
  // Statements
  SYMBOL_TYPE_STATEMENT = 0,
  SYMBOL_TYPE_ASSIGNMENT,
  SYMBOL_TYPE_VARIABLE,
  SYMBOL_TYPE_DECLARATION,
  SYMBOL_TYPE_REFERENCE,
  SYMBOL_TYPE_MUTABLE,
  SYMBOL_TYPE_DATATYPE,
  // Expressions
  SYMBOL_TYPE_EXPRESSION,
  SYMBOL_TYPE_OR,
  SYMBOL_TYPE_CONDITION,
  SYMBOL_TYPE_AND,
  SYMBOL_TYPE_COMPARISON,
  SYMBOL_TYPE_LESS_THAN,
  SYMBOL_TYPE_GREATER_THAN,
  SYMBOL_TYPE_EQUAL,
  SYMBOL_TYPE_LESS_EQUAL,
  SYMBOL_TYPE_GREATER_EQUAL,
  SYMBOL_TYPE_NOT_EQUAL,
  SYMBOL_TYPE_TERM,
  SYMBOL_TYPE_ADD,
  SYMBOL_TYPE_SUBTRACT,
  SYMBOL_TYPE_FACTOR,
  SYMBOL_TYPE_MULTIPLY,
  SYMBOL_TYPE_DIVIDE,
  SYMBOL_TYPE_MODULO,
  SYMBOL_TYPE_UNARY,
  SYMBOL_TYPE_MINUS,
  SYMBOL_TYPE_NEGATE,
  SYMBOL_TYPE_PRIMARY,
  SYMBOL_TYPE_FNCALL,
  SYMBOL_TYPE_ARGUMENTS,
  SYMBOL_TYPE_SUBSCRIPTION,
  SYMBOL_TYPE_SLICE,
  // Atoms
  SYMBOL_TYPE_ATOM,
  SYMBOL_TYPE_IDENTIFIER,
  SYMBOL_TYPE_INTEGER_LITERAL,
  SYMBOL_TYPE_FLOAT_LITERAL,
  SYMBOL_TYPE_STRING_LITERAL,
  SYMBOL_TYPE_BOOLEAN_LITERAL,
  SYMBOL_TYPE_NONE_LITERAL,
  SYMBOL_TYPE_DICT,
  SYMBOL_TYPE_ENTRIES,
  SYMBOL_TYPE_LIST,
  SYMBOL_TYPE_ELEMENTS,
  SYMBOL_TYPE_INNER_EXPRESSION,
} SymbolType;

typedef struct {
  SymbolType type;
  struct first {
    int line;
    int column;
  } first;
  struct last {
    int line;
    int column;
  } last;
  void *data;
} Symbol;

typedef struct {

} Context;

Symbol *SymbolIntegerLiteral(Context *context, int first_line, int first_column,
                        int last_line, int last_column, const char *token);

Symbol *SymbolOr(Context *context, int first_line, int first_column,
                        int last_line, int last_column, const char *token);

Symbol *SymbolAnd(Context *context, int first_line, int first_column,
                        int last_line, int last_column, const char *token);

Symbol *SymbolEqual(Context *context, int first_line, int first_column,
                        int last_line, int last_column, const char *token);

Symbol *SymbolLessEqual(Context *context, int first_line, int first_column,
                        int last_line, int last_column, const char *token);

Symbol *SymbolGreaterEqual(Context *context, int first_line, int first_column,
                           int last_line, int last_column, const char *token);

Symbol *SymbolNotEqual(Context *context, int first_line, int first_column,
                       int last_line, int last_column, const char *token);

Symbol *SymbolIdentifier(Context *context, int first_line, int first_column,
                         int last_line, int last_column, const char *token);

#endif // _AETHER_INTERPRETER_H
