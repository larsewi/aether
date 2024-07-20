#ifndef _AETHER_SYNTAX_H
#define _AETHER_SYNTAX_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/****************************************************************************/

typedef struct ParserState ParserState;

// Statements
typedef struct SymbolStatement SymbolStatement;
typedef struct SymbolAssignment SymbolAssignment;
typedef struct SymbolDeclaration SymbolDeclaration;
typedef struct SymbolReference SymbolReference;
typedef struct SymbolMutable SymbolMutable;
typedef struct SymbolDatatype SymbolDatatype;

// Expressions
typedef struct SymbolExpression SymbolExpression;
typedef struct SymbolOr SymbolOr;
typedef struct SymbolCondition SymbolCondition;
typedef struct SymbolAnd SymbolAnd;
typedef struct SymbolComparison SymbolComparison;
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
typedef struct SymbolEntries SymbolEntries;
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

/****************************************************************************/

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

struct ParserState {
  const char *filename;
  int line;
  int column;
  SymbolStatement *statement;
};

typedef struct {
  int line;
  int column;
} SymbolLocation;

/****************************************************************************/

typedef struct {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
} Symbol;

/****************************************************************************/

struct SymbolStatement {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolAssignment {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolDeclaration {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
  SymbolIdentifier *identifier;
};

/****************************************************************************/

struct SymbolReference {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolMutable {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolDatatype *datatype;
};

/****************************************************************************/

struct SymbolDatatype {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolIdentifier *identifier;
};

/****************************************************************************/

struct SymbolExpression {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolOr {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolExpression *expression;
  SymbolCondition *condition;
};

/****************************************************************************/

struct SymbolCondition {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolAnd {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolCondition *condition;
  SymbolComparison *comparison;
};

/****************************************************************************/

struct SymbolComparison {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolLessThan {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolComparison *comparison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolGreaterThan {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolComparison *comparison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolEqual {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolComparison *comparison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolLessEqual {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolComparison *comparison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolGreaterEqual {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolComparison *comparison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolNotEqual {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolComparison *comparison;
  SymbolTerm *term;
};

/****************************************************************************/

struct SymbolTerm {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolAdd {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolTerm *term;
  SymbolFactor *factor;
};

/****************************************************************************/

struct SymbolSubtract {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolTerm *term;
  SymbolFactor *factor;
};

/****************************************************************************/

struct SymbolFactor {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolMultiply {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolFactor *factor;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolDivide {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolFactor *factor;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolModulo {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolFactor *factor;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolUnary {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolMinus {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolNegate {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolUnary *unary;
};

/****************************************************************************/

struct SymbolPrimary {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolFncall {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolPrimary *primary;
  SymbolArguments *arguments;
};

/****************************************************************************/

struct SymbolDict {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolEntries *entries;
};

/****************************************************************************/

struct SymbolEntries {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolEntries *entries;
  SymbolStringLiteral *string_literal;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolList {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolElements *elements;
};

/****************************************************************************/

struct SymbolElements {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolElements *elements;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolArguments {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolArguments *arguments;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolSubscription {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolPrimary *primary;
  SymbolExpression *expression;
};

/****************************************************************************/

struct SymbolSlice {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  SymbolPrimary *primary;
  SymbolExpression *left_expression;
  SymbolExpression *right_expression;
};

/****************************************************************************/

struct SymbolAtom {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  Symbol *symbol;
};

/****************************************************************************/

struct SymbolIdentifier {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  char *value;
};

/****************************************************************************/

struct SymbolIntegerLiteral {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  unsigned long long value;
};

/****************************************************************************/

struct SymbolFloatLiteral {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  double value;
};

/****************************************************************************/

struct SymbolStringLiteral {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  char *value;
};

/****************************************************************************/

struct SymbolBooleanLiteral {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
  bool value;
};

/****************************************************************************/

struct SymbolNoneLiteral {
  SymbolType type;
  SymbolLocation first;
  SymbolLocation last;
};

#endif // _AETHER_SYNTAX_H
