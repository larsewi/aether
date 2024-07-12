#ifndef _AETHER_SYNTAX_H
#define _AETHER_SYNTAX_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/****************************************************************************/

typedef struct ParserState ParserState;
typedef struct Symbol Symbol;

// Statements
typedef struct SymbolStatement SymbolStatement;
typedef struct SymbolAssignment SymbolAssignment;
typedef struct SymbolVariable SymbolVariable;
typedef struct SymbolDecl SymbolDecl;
typedef struct SymbolReference SymbolReference;
typedef struct SymbolMutable SymbolMutable;
typedef struct SymbolDatatype SymbolDatatype;

// Expressions
typedef struct SymbolExpression SymbolExpression;
typedef struct SymbolOr SymbolOr;
typedef struct SymbolCondition SymbolCondition;
typedef struct SymbolAnd SymbolAnd;
typedef struct SymbolComparrison SymbolComparrison;
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
typedef struct SymbolNegate SymbolNegate;
typedef struct SymbolPrimary SymbolPrimary;
typedef struct SymbolFncall SymbolFncall;
typedef struct SymbolDict SymbolDict;
typedef struct SymbolPairs SymbolPairs;
typedef struct SymbolList SymbolList;
typedef struct SymbolElements SymbolElements;
typedef struct SymbolArguments SymbolArguments;
typedef struct SymbolSubscription SymbolSubscription;
typedef struct SymbolSlice SymbolSlice;
typedef struct SymbolAtom SymbolAtom;

// Terminals
typedef struct SymbolIdentifier SymbolIdentifier;
typedef struct SymbolIntegerLiteral SymbolIntegerLiteral;
typedef struct SymbolFloatLiteral SymbolFloatLiteral;
typedef struct SymbolStringLiteral SymbolStringLiteral;
typedef struct SymbolBooleanLiteral SymbolBooleanLiteral;
typedef struct SymbolNoneLiteral SymbolNoneLiteral;
typedef struct SymbolMutableSpecifier SymbolMutableSpecifier;

/****************************************************************************/

typedef enum {
  SYMBOL_TYPE_STATEMENT = 0,
  SYMBOL_TYPE_ASSIGNMENT,
  SYMBOL_TYPE_VARIABLE,
  SYMBOL_TYPE_DECL,
  SYMBOL_TYPE_REFERENCE,
  SYMBOL_TYPE_MUTABLE,
  SYMBOL_TYPE_DATATYPE,
  SYMBOL_TYPE_EXPRESSION,
  SYMBOL_TYPE_OR,
  SYMBOL_TYPE_CONDITION,
  SYMBOL_TYPE_AND,
  SYMBOL_TYPE_COMPARRISON,
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
  SYMBOL_TYPE_ATOM,
  SYMBOL_TYPE_IDENTIFIER,
  SYMBOL_TYPE_INTEGER_LITERAL,
  SYMBOL_TYPE_FLOAT_LITERAL,
  SYMBOL_TYPE_STRING_LITERAL,
  SYMBOL_TYPE_BOOLEAN_LITERAL,
  SYMBOL_TYPE_NONE_LITERAL,
  SYMBOL_TYPE_DICT,
  SYMBOL_TYPE_PAIRS,
  SYMBOL_TYPE_LIST,
  SYMBOL_TYPE_ELEMENTS,
  SYMBOL_TYPE_INNER_EXPRESSION,
  SYMBOL_TYPE_MUTABLE_SPECIFIER,
} SymbolType;

/****************************************************************************/

struct Symbol {
  SymbolType type;
};

/****************************************************************************/

struct ParserState {
  const char *filename;
  int line;
  int column;
  SymbolStatement *statement;
};

/****************************************************************************/

struct SymbolStatement {
  SymbolType type;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolAssignment {
  SymbolType type;
  Symbol *symbol;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolVariable {
  SymbolType type;
  Symbol *symbol;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolDecl {
  SymbolType type;
  Symbol *symbol;
  SymbolIdentifier *identifier;
};

/****************************************************************************/

struct SymbolReference {
  SymbolType type;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolMutable {
  SymbolType type;
  SymbolDatatype *datatype;
};

/****************************************************************************/

struct SymbolDatatype {
  SymbolType type;
  SymbolIdentifier *identifier;
};

/****************************************************************************/

struct SymbolExpression {
  SymbolType type;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolOr {
  SymbolType type;
  SymbolExpression *expression;
  SymbolCondition *condition;
};

/****************************************************************************/

struct SymbolCondition {
  SymbolType type;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolAnd {
  SymbolType type;
  SymbolCondition *condition;
  SymbolComparrison *comparrison;
};

/****************************************************************************/

struct SymbolComparrison {
  SymbolType type;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolLessThan {
  SymbolType type;
  SymbolComparrison *comparrison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolGreaterThan {
  SymbolType type;
  SymbolComparrison *comparrison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolEqual {
  SymbolType type;
  SymbolComparrison *comparrison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolLessEqual {
  SymbolType type;
  SymbolComparrison *comparrison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolGreaterEqual {
  SymbolType type;
  SymbolComparrison *comparrison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolNotEqual {
  SymbolType type;
  SymbolComparrison *comparrison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolTerm {
  SymbolType type;
  Symbol *symbol;
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
  Symbol *symbol;
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
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolMinus {
  SymbolType type;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolNegate {
  SymbolType type;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolPrimary {
  SymbolType type;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolFncall {
  SymbolType type;
  SymbolPrimary *primary;
  SymbolArguments *arguments;
};

/****************************************************************************/

struct SymbolDict {
  SymbolType type;
  SymbolPairs *pairs;
};

/****************************************************************************/

struct SymbolPairs {
  SymbolType type;
  SymbolPairs *pairs;
  SymbolStringLiteral *string_literal;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolList {
  SymbolType type;
  SymbolElements *elements;
};

/****************************************************************************/

struct SymbolElements {
  SymbolType type;
  SymbolElements *elements;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolArguments {
  SymbolType type;
  struct SymbolArguments *arguments;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolSubscription {
  SymbolType type;
  SymbolPrimary *primary;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolSlice {
  SymbolType type;
  SymbolPrimary *primary;
  SymbolExpression *left_expression;
  SymbolExpression *right_expression;
};

/****************************************************************************/

struct SymbolAtom {
  SymbolType type;
  Symbol *symbol;
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

struct SymbolMutableSpecifier {
  SymbolType type;
  int line;
  int column;
};

#endif // _AETHER_SYNTAX_H
