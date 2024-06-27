#ifndef _AETHER_PARSER_SATE
#define _AETHER_PARSER_SATE

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/list.h"

/****************************************************************************/

typedef struct {
  int line;
  int column;
} ParserState;

typedef enum {
  SYMBOL_TYPE_EXPR,
  SYMBOL_TYPE_LP1,
  SYMBOL_TYPE_OR,
  SYMBOL_TYPE_LP2,
  SYMBOL_TYPE_AND,
  SYMBOL_TYPE_LP3,
  SYMBOL_TYPE_NOT,
  SYMBOL_TYPE_COMP,
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
  SYMBOL_TYPE_PRIMARY,
  SYMBOL_TYPE_FNCALL,
  SYMBOL_TYPE_ARGUMENTS,
  SYMBOL_TYPE_SUBSCRIPTION,
  SYMBOL_TYPE_SLICE,
  SYMBOL_TYPE_ATOM,
  SYMBOL_TYPE_IDENTIFIER,
  SYMBOL_TYPE_INTEGER_LITERAL,
  SYMBOL_TYPE_FLOAT_LITERAL,
  SYMBOL_TYPE_STRING_LITERAL,
  SYMBOL_TYPE_BOOLEAN_LITERAL,
  SYMBOL_TYPE_NONE_LITERAL,
  SYMBOL_TYPE_INNER_EXPR,
} SymbolType;

typedef struct Expr Expr;
typedef struct LP1 LP1;
typedef struct Or Or;
typedef struct LP2 LP2;
typedef struct And And;
typedef struct LP3 LP3;
typedef struct Not Not;
typedef struct Comp Comp;
typedef struct LessThan LessThan;
typedef struct GreaterThan GreaterThan;
typedef struct Equal Equal;
typedef struct LessEqual LessEqual;
typedef struct GreaterEqual GreaterEqual;
typedef struct NotEqual NotEqual;
typedef struct Term Term;
typedef struct Add Add;
typedef struct Subtract Subtract;
typedef struct Factor Factor;
typedef struct Multiply Multiply;
typedef struct Divide Divide;
typedef struct Modulo Modulo;
typedef struct Unary Unary;
typedef struct Minus Minus;
typedef struct Primary Primary;
typedef struct Fncall Fncall;
typedef struct Arguments Arguments;
typedef struct Subscription Subscription;
typedef struct Slice Slice;
typedef struct Atom Atom;
typedef struct Identifier Identifier;
typedef struct IntegerLiteral IntegerLiteral;
typedef struct FloatLiteral FloatLiteral;
typedef struct StringLiteral StringLiteral;
typedef struct BooleanLiteral BooleanLiteral;
typedef struct NoneLiteral NoneLiteral;

/****************************************************************************/

struct Expr {
  SymbolType type;
  void *value;
};

/****************************************************************************/

struct LP1 {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct Or {
  SymbolType type;
  LP1 *lp1;
  LP2 *lp2;
};

/****************************************************************************/

struct LP2 {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct And {
  SymbolType type;
  LP2 *lp2;
  LP3 *lp3;
};

/****************************************************************************/

struct LP3 {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct Not {
  SymbolType type;
  LP3 *lp3;
};

/****************************************************************************/

struct Comp {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct LessThan {
  SymbolType type;
  Comp *comp;
  Term *term;
};

/****************************************************************************/

struct GreaterThan {
  SymbolType type;
  Comp *comp;
  Term *term;
};

/****************************************************************************/

struct Equal {
  SymbolType type;
  Comp *comp;
  Term *term;
};

/****************************************************************************/

struct LessEqual {
  SymbolType type;
  Comp *comp;
  Term *term;
};

/****************************************************************************/

struct GreaterEqual {
  SymbolType type;
  Comp *comp;
  Term *term;
};

/****************************************************************************/

struct NotEqual {
  SymbolType type;
  Comp *comp;
  Term *term;
};

/****************************************************************************/

struct Term {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct Add {
  SymbolType type;
  Term *term;
  Factor *factor;
};

/****************************************************************************/

struct Subtract {
  SymbolType type;
  Term *term;
  Factor *factor;
};

/****************************************************************************/

struct Factor {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct Multiply {
  SymbolType type;
  Factor *factor;
  Unary *unary;
};

/****************************************************************************/

struct Divide {
  SymbolType type;
  Factor *factor;
  Unary *unary;
};

/****************************************************************************/

struct Modulo {
  SymbolType type;
  Factor *factor;
  Unary *unary;
};

/****************************************************************************/

struct Unary {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct Minus {
  SymbolType type;
  Unary *unary;
};

/****************************************************************************/

struct Primary {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct Fncall {
  SymbolType type;
  Primary *primary;
  Arguments *arguments;
};

/****************************************************************************/

struct Arguments {
  SymbolType type;
  struct Arguments *arguments;
  Expr *expr;
};

/****************************************************************************/

struct Subscription {
  SymbolType type;
  Primary *primary;
  Expr *expr;
};

/****************************************************************************/

struct Slice {
  SymbolType type;
  Primary *primary;
  Expr *left_expr;
  Expr *right_expr;
};

/****************************************************************************/

struct Atom {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct Identifier {
  SymbolType type;
  int line;
  int column;
  char *value;
};

/****************************************************************************/

struct IntegerLiteral {
  SymbolType type;
  int line;
  int column;
  unsigned long long value;
};

/****************************************************************************/

struct FloatLiteral {
  SymbolType type;
  int line;
  int column;
  double value;
};

/****************************************************************************/

struct StringLiteral {
  SymbolType type;
  int line;
  int column;
  char *value;
};

/****************************************************************************/

struct BooleanLiteral {
  SymbolType type;
  int line;
  int column;
  bool value;
};

/****************************************************************************/

struct NoneLiteral {
  SymbolType type;
  int line;
  int column;
};

/****************************************************************************/

extern ParserState PARSER_STATE;

#endif // _AETHER_PARSER_SATE
