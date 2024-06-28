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

typedef struct SymbolExpr SymbolExpr;
typedef struct SymbolLP1 SymbolLP1;
typedef struct SymbolOr SymbolOr;
typedef struct SymbolLP2 SymbolLP2;
typedef struct SymbolAnd SymbolAnd;
typedef struct SymbolLP3 SymbolLP3;
typedef struct SymbolNot SymbolNot;
typedef struct SymbolComp SymbolComp;
typedef struct SymbolLessThan SymbolLessThan;
typedef struct SymbolGreaterThan SymbolGreaterThan;
typedef struct SymbolEqual SymbolEqual;
typedef struct SymbolLessEqual SymbolLessEqual;
typedef struct SymbolGreaterEqual SymbolGreaterEqual;
typedef struct SymbolNotEqual SymbolNotEqual;
typedef struct SymbolTerm SymbolTerm;
typedef struct SymbolAdd SymbolAdd;
typedef struct SymbolSubtract SymbolSubtract;
typedef struct SymbolFactor SymbolFactor;
typedef struct SymbolMultiply SymbolMultiply;
typedef struct SymbolDivide SymbolDivide;
typedef struct SymbolModulo SymbolModulo;
typedef struct SymbolUnary SymbolUnary;
typedef struct SymbolMinus SymbolMinus;
typedef struct SymbolPrimary SymbolPrimary;
typedef struct SymbolFncall SymbolFncall;
typedef struct SymbolArguments SymbolArguments;
typedef struct SymbolSubscription SymbolSubscription;
typedef struct SymbolSlice SymbolSlice;
typedef struct SymbolAtom SymbolAtom;
typedef struct SymbolIdentifier SymbolIdentifier;
typedef struct SymbolIntegerLiteral SymbolIntegerLiteral;
typedef struct SymbolFloatLiteral SymbolFloatLiteral;
typedef struct SymbolStringLiteral SymbolStringLiteral;
typedef struct SymbolBooleanLiteral SymbolBooleanLiteral;
typedef struct SymbolNoneLiteral SymbolNoneLiteral;

/****************************************************************************/

struct SymbolExpr {
  SymbolType type;
  void *value;
};

/****************************************************************************/

struct SymbolLP1 {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolOr {
  SymbolType type;
  SymbolLP1 *lp1;
  SymbolLP2 *lp2;
};

/****************************************************************************/

struct SymbolLP2 {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolAnd {
  SymbolType type;
  SymbolLP2 *lp2;
  SymbolLP3 *lp3;
};

/****************************************************************************/

struct SymbolLP3 {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolNot {
  SymbolType type;
  SymbolLP3 *lp3;
};

/****************************************************************************/

struct SymbolComp {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolLessThan {
  SymbolType type;
  SymbolComp *comp;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolGreaterThan {
  SymbolType type;
  SymbolComp *comp;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolEqual {
  SymbolType type;
  SymbolComp *comp;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolLessEqual {
  SymbolType type;
  SymbolComp *comp;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolGreaterEqual {
  SymbolType type;
  SymbolComp *comp;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolNotEqual {
  SymbolType type;
  SymbolComp *comp;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolTerm {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolAdd {
  SymbolType type;
  SymbolTerm *term;
  SymbolFactor *factor;
};

/****************************************************************************/

struct SymbolSubtract {
  SymbolType type;
  SymbolTerm *term;
  SymbolFactor *factor;
};

/****************************************************************************/

struct SymbolFactor {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolMultiply {
  SymbolType type;
  SymbolFactor *factor;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolDivide {
  SymbolType type;
  SymbolFactor *factor;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolModulo {
  SymbolType type;
  SymbolFactor *factor;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolUnary {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolMinus {
  SymbolType type;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolPrimary {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolFncall {
  SymbolType type;
  SymbolPrimary *primary;
  SymbolArguments *arguments;
};

/****************************************************************************/

struct SymbolArguments {
  SymbolType type;
  struct SymbolArguments *arguments;
  SymbolExpr *expr;
};

/****************************************************************************/

struct SymbolSubscription {
  SymbolType type;
  SymbolPrimary *primary;
  SymbolExpr *expr;
};

/****************************************************************************/

struct SymbolSlice {
  SymbolType type;
  SymbolPrimary *primary;
  SymbolExpr *left_expr;
  SymbolExpr *right_expr;
};

/****************************************************************************/

struct SymbolAtom {
  SymbolType type;
  void *symbol;
};

/****************************************************************************/

struct SymbolIdentifier {
  SymbolType type;
  int line;
  int column;
  char *value;
};

/****************************************************************************/

struct SymbolIntegerLiteral {
  SymbolType type;
  int line;
  int column;
  unsigned long long value;
};

/****************************************************************************/

struct SymbolFloatLiteral {
  SymbolType type;
  int line;
  int column;
  double value;
};

/****************************************************************************/

struct SymbolStringLiteral {
  SymbolType type;
  int line;
  int column;
  char *value;
};

/****************************************************************************/

struct SymbolBooleanLiteral {
  SymbolType type;
  int line;
  int column;
  bool value;
};

/****************************************************************************/

struct SymbolNoneLiteral {
  SymbolType type;
  int line;
  int column;
};

/****************************************************************************/

extern ParserState PARSER_STATE;

#endif // _AETHER_PARSER_SATE
