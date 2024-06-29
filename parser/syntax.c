#include "syntax.h"
#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../utils/alloc.h"
#include "../utils/string_lib.h"
#include "parser.h"

#define P PARSER_STATE
#define PRINT_START(symbol, print_tree, indent)                                \
  if (print_tree) {                                                            \
    printf("%*s<%s>\n", indent, "", StrRepr(symbol->type));                    \
  }
#define PRINT_END(symbol, print_tree, indent)                                  \
  if (print_tree) {                                                            \
    printf("%*s</%s>\n", indent, "", StrRepr(symbol->type));                   \
  }

extern FILE *yyin;
ParserState PARSER_STATE = {0};

static void WalkSymbolExpr(const SymbolExpr *expr, bool print_tree, int indent);
static void WalkSymbolCond(const SymbolCond *cond, bool print_tree, int indent);
static void WalkSymbolComp(const SymbolComp *comp, bool print_tree, int indent);
static void WalkSymbolTerm(const SymbolTerm *term, bool print_tree, int indent);
static void WalkSymbolFactor(const SymbolFactor *factor, bool print_tree,
                             int indent);
static void WalkSymbolUnary(const SymbolUnary *unary, bool print_tree,
                            int indent);
static void WalkSymbolPrimary(const SymbolPrimary *primary, bool print_tree,
                              int indent);

static const char *StrRepr(SymbolType type) {
  static const char str_repr[][64] = {
      "expr",
      "or",
      "cond",
      "and",
      "comp",
      "less_than",
      "greater_than",
      "equal",
      "less_equal",
      "greater_equal",
      "not_equal",
      "term",
      "add",
      "subtract",
      "factor",
      "multiply",
      "divide",
      "modulo",
      "unary",
      "minus",
      "negate",
      "primary",
      "fncall",
      "arguments",
      "subscription",
      "slice",
      "atom",
      "IDENTIFIER",
      "INTEGER_LITERAL",
      "FLOAT_LITERAL",
      "STRING_LITERAL",
      "BOOLEAN_LITERAL",
      "NONE_LITERAL",
      "inner_expr",
  };
  if (type < 0 || type >= sizeof(str_repr) / sizeof(str_repr[0])) {
    return "undefined";
  }
  return str_repr[type];
}

/****************************************************************************/

static void WalkSymbolIdentifier(const SymbolIdentifier *const identifier,
                                 const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*sLn  %d\n", indent, "", identifier->line);
    printf("%*sCol %d\n", indent, "", identifier->column);
    printf("%*sVal %s\n", indent, "", identifier->value);
  }
}

/****************************************************************************/

static void
WalkSymbolIntegerLiteral(const SymbolIntegerLiteral *const integer_literal,
                         const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*sLn  %d\n", indent, "", integer_literal->line);
    printf("%*sCol %d\n", indent, "", integer_literal->column);
    printf("%*sVal %llu\n", indent, "", integer_literal->value);
  }
}

/****************************************************************************/

static void
WalkSymbolFloatLiteral(const SymbolFloatLiteral *const float_literal,
                       const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*sLn  %d\n", indent, "", float_literal->line);
    printf("%*sCol %d\n", indent, "", float_literal->column);
    printf("%*sVal %lf\n", indent, "", float_literal->value);
  }
}

/****************************************************************************/

static void
WalkSymbolStringLiteral(const SymbolStringLiteral *const string_literal,
                        const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*sLn  %d\n", indent, "", string_literal->line);
    printf("%*sCol %d\n", indent, "", string_literal->column);
    printf("%*sVal \"%s\"\n", indent, "", string_literal->value);
  }
}

/****************************************************************************/

static void
WalkSymbolBooleanLiteral(const SymbolBooleanLiteral *const boolean_literal,
                         const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*sLn  %d\n", indent, "", boolean_literal->line);
    printf("%*sCol %d\n", indent, "", boolean_literal->column);
    printf("%*sVal %s\n", indent, "",
           (boolean_literal->value) ? "true" : "false");
  }
}

/****************************************************************************/

static void WalkSymbolNoneLiteral(const SymbolNoneLiteral *const none_literal,
                                  const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*sLn  %d\n", indent, "", none_literal->line);
    printf("%*sCol %d\n", indent, "", none_literal->column);
    printf("%*sVal none", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolAtom(const SymbolAtom *const atom, const bool print_tree,
                           const int indent) {
  Symbol *symbol = (Symbol *)atom->symbol;
  switch (symbol->type) {
  case SYMBOL_TYPE_IDENTIFIER:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolIdentifier(atom->symbol, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_INTEGER_LITERAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolIntegerLiteral(atom->symbol, print_tree,
                             indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_FLOAT_LITERAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolFloatLiteral(atom->symbol, print_tree,
                           indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_STRING_LITERAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolStringLiteral(atom->symbol, print_tree,
                            indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_BOOLEAN_LITERAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolBooleanLiteral(atom->symbol, print_tree,
                             indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_NONE_LITERAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolNoneLiteral(atom->symbol, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_EXPR:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolExpr(atom->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(symbol->type));
  }
}

/****************************************************************************/

static void WalkSymbolArguments(const SymbolArguments *const arguments,
                                const bool print_tree, const int indent) {
  if (arguments->arguments != NULL) {
    if (arguments->arguments->type != SYMBOL_TYPE_ARGUMENTS) {
      LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
    }
    PRINT_START(arguments->arguments, print_tree, indent);
    WalkSymbolArguments(arguments->arguments, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(arguments->arguments, print_tree, indent);
  }

  if (arguments->expr->type != SYMBOL_TYPE_EXPR) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(arguments->expr, print_tree, indent);
  WalkSymbolExpr(arguments->expr, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(arguments->expr, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolFncall(const SymbolFncall *const fncall,
                             const bool print_tree, const int indent) {
  if (fncall->primary->type != SYMBOL_TYPE_PRIMARY) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(fncall->primary, print_tree, indent);
  WalkSymbolPrimary(fncall->primary, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(fncall->primary, print_tree, indent);

  if (fncall->arguments->type != SYMBOL_TYPE_ARGUMENTS) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(fncall->arguments, print_tree, indent);
  WalkSymbolArguments(fncall->arguments, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(fncall->arguments, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolSubscription(const SymbolSubscription *const subscription,
                                   const bool print_tree, const int indent) {
  if (subscription->primary->type != SYMBOL_TYPE_PRIMARY) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(subscription->primary, print_tree, indent);
  WalkSymbolPrimary(subscription->primary, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(subscription->primary, print_tree, indent);

  if (subscription->expr->type != SYMBOL_TYPE_EXPR) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(subscription->expr, print_tree, indent);
  WalkSymbolExpr(subscription->expr, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(subscription->expr, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolSlice(const SymbolSlice *const slice,
                            const bool print_tree, const int indent) {
  if (slice->primary->type != SYMBOL_TYPE_PRIMARY) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(slice->primary, print_tree, indent);
  WalkSymbolPrimary(slice->primary, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(slice->primary, print_tree, indent);

  if (slice->left_expr != NULL) {
    if (slice->left_expr->type != SYMBOL_TYPE_EXPR) {
      LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
    }
    PRINT_START(slice->left_expr, print_tree, indent);
    WalkSymbolExpr(slice->left_expr, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(slice->left_expr, print_tree, indent);
  }

  if (slice->right_expr != NULL) {
    if (slice->right_expr->type != SYMBOL_TYPE_EXPR) {
      LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
    }
    PRINT_START(slice->right_expr, print_tree, indent);
    WalkSymbolExpr(slice->right_expr, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(slice->right_expr, print_tree, indent);
  }
}

/****************************************************************************/

static void WalkSymbolPrimary(const SymbolPrimary *const primary,
                              const bool print_tree, const int indent) {
  Symbol *symbol = (Symbol *)primary->symbol;
  switch (symbol->type) {
  case SYMBOL_TYPE_ATOM:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolAtom(primary->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_FNCALL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolFncall(primary->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_SUBSCRIPTION:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolSubscription(primary->symbol, print_tree,
                           indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_SLICE:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolSlice(primary->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(symbol->type));
  }
}

/****************************************************************************/

static void WalkSymbolMinus(const SymbolMinus *const minus,
                            const bool print_tree, const int indent) {
  if (minus->unary->type != SYMBOL_TYPE_UNARY) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(minus->unary, print_tree, indent);
  WalkSymbolUnary(minus->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(minus->unary, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolNegate(const SymbolNegate *const negate,
                             const bool print_tree, const int indent) {
  if (negate->unary->type != SYMBOL_TYPE_UNARY) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(negate->unary, print_tree, indent);
  WalkSymbolUnary(negate->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(negate->unary, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolUnary(const SymbolUnary *const unary,
                            const bool print_tree, const int indent) {
  Symbol *symbol = (Symbol *)unary->symbol;
  switch (symbol->type) {
  case SYMBOL_TYPE_PRIMARY:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolPrimary(unary->symbol, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_MINUS:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolMinus(unary->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_NEGATE:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolNegate(unary->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(symbol->type));
  }
}

/****************************************************************************/

static void WalkSymbolMultiply(const SymbolMultiply *const multiply,
                               const bool print_tree, const int indent) {
  if (multiply->factor->type != SYMBOL_TYPE_FACTOR) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(multiply->factor, print_tree, indent);
  WalkSymbolFactor(multiply->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(multiply->factor, print_tree, indent);

  if (multiply->unary->type != SYMBOL_TYPE_UNARY) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(multiply->unary, print_tree, indent);
  WalkSymbolUnary(multiply->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(multiply->unary, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolDivide(const SymbolDivide *const divide,
                             const bool print_tree, const int indent) {
  if (divide->factor->type != SYMBOL_TYPE_FACTOR) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(divide->factor, print_tree, indent);
  WalkSymbolFactor(divide->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(divide->factor, print_tree, indent);

  if (divide->unary->type != SYMBOL_TYPE_UNARY) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(divide->unary, print_tree, indent);
  WalkSymbolUnary(divide->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(divide->unary, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolModulo(const SymbolModulo *const modulo,
                             const bool print_tree, const int indent) {
  if (modulo->factor->type != SYMBOL_TYPE_FACTOR) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(modulo->factor, print_tree, indent);
  WalkSymbolFactor(modulo->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(modulo->factor, print_tree, indent);

  if (modulo->unary->type != SYMBOL_TYPE_UNARY) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(modulo->unary, print_tree, indent);
  WalkSymbolUnary(modulo->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(modulo->unary, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolFactor(const SymbolFactor *const factor,
                             const bool print_tree, const int indent) {
  Symbol *symbol = (Symbol *)factor->symbol;
  switch (symbol->type) {
  case SYMBOL_TYPE_UNARY:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolUnary(factor->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_MULTIPLY:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolMultiply(factor->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_DIVIDE:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolDivide(factor->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_MODULO:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolModulo(factor->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(symbol->type));
  }
}

/****************************************************************************/

static void WalkSymbolAdd(const SymbolAdd *const add, const bool print_tree,
                          const int indent) {
  if (add->term->type != SYMBOL_TYPE_TERM) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(add->term, print_tree, indent);
  WalkSymbolTerm(add->term, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(add->term, print_tree, indent);

  if (add->factor->type != SYMBOL_TYPE_FACTOR) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(add->factor, print_tree, indent);
  WalkSymbolFactor(add->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(add->factor, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolSubtract(const SymbolSubtract *const subtract,
                               const bool print_tree, const int indent) {
  if (subtract->term->type != SYMBOL_TYPE_TERM) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(subtract->term, print_tree, indent);
  WalkSymbolTerm(subtract->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(subtract->term, print_tree, indent);

  if (subtract->factor->type != SYMBOL_TYPE_FACTOR) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(subtract->factor, print_tree, indent);
  WalkSymbolFactor(subtract->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(subtract->factor, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolTerm(const SymbolTerm *const term, const bool print_tree,
                           const int indent) {
  Symbol *symbol = (Symbol *)term->symbol;
  switch (symbol->type) {
  case SYMBOL_TYPE_FACTOR:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolFactor(term->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_ADD:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolAdd(term->symbol, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_SUBTRACT:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolSubtract(term->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(symbol->type));
  }
}

/****************************************************************************/

static void WalkSymbolLessThan(const SymbolLessThan *const less_than,
                               const bool print_tree, const int indent) {
  if (less_than->comp->type != SYMBOL_TYPE_COMP) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(less_than->comp, print_tree, indent);
  WalkSymbolComp(less_than->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(less_than->comp, print_tree, indent);

  if (less_than->term->type != SYMBOL_TYPE_TERM) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(less_than->term, print_tree, indent);
  WalkSymbolTerm(less_than->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(less_than->term, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolGreaterThan(const SymbolGreaterThan *const greater_than,
                                  const bool print_tree, const int indent) {
  if (greater_than->comp->type != SYMBOL_TYPE_COMP) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(greater_than->comp, print_tree, indent);
  WalkSymbolComp(greater_than->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(greater_than->comp, print_tree, indent);

  if (greater_than->term->type != SYMBOL_TYPE_TERM) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(greater_than->term, print_tree, indent);
  WalkSymbolTerm(greater_than->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(greater_than->term, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolEqual(const SymbolEqual *const equal, const int indent,
                            const bool print_tree) {
  if (equal->comp->type != SYMBOL_TYPE_COMP) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(equal->comp, print_tree, indent);
  WalkSymbolComp(equal->comp, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(equal->comp, print_tree, indent);

  if (equal->term->type != SYMBOL_TYPE_TERM) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(equal->term, print_tree, indent);
  WalkSymbolTerm(equal->term, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(equal->term, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolLessEqual(const SymbolLessEqual *const less_equal,
                                const bool print_tree, const int indent) {
  if (less_equal->comp->type != SYMBOL_TYPE_COMP) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(less_equal->comp, print_tree, indent);
  WalkSymbolComp(less_equal->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(less_equal->comp, print_tree, indent);

  if (less_equal->term->type != SYMBOL_TYPE_TERM) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(less_equal->term, print_tree, indent);
  WalkSymbolTerm(less_equal->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(less_equal->term, print_tree, indent);
}

/****************************************************************************/

static void
WalkSymbolGreaterEqual(const SymbolGreaterEqual *const greater_equal,
                       const bool print_tree, const int indent) {
  if (greater_equal->comp->type != SYMBOL_TYPE_COMP) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(greater_equal->comp, print_tree, indent);
  WalkSymbolComp(greater_equal->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(greater_equal->comp, print_tree, indent);

  if (greater_equal->term->type != SYMBOL_TYPE_TERM) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(greater_equal->term, print_tree, indent);
  WalkSymbolTerm(greater_equal->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(greater_equal->term, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolNotEqual(const SymbolNotEqual *const not_equal,
                               const bool print_tree, const int indent) {
  if (not_equal->comp->type != SYMBOL_TYPE_COMP) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(not_equal->comp, print_tree, indent);
  WalkSymbolComp(not_equal->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(not_equal->comp, print_tree, indent);

  if (not_equal->term->type != SYMBOL_TYPE_TERM) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(not_equal->term, print_tree, indent);
  WalkSymbolTerm(not_equal->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(not_equal->term, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolComp(const SymbolComp *const comp, const bool print_tree,
                           const int indent) {
  Symbol *symbol = (Symbol *)comp->symbol;
  switch (symbol->type) {
  case SYMBOL_TYPE_TERM:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolTerm(comp->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_LESS_THAN:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolLessThan(comp->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_GREATER_THAN:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolGreaterThan(comp->symbol, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_EQUAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolEqual(comp->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_LESS_EQUAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolLessEqual(comp->symbol, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_GREATER_EQUAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolGreaterEqual(comp->symbol, print_tree,
                           indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_NOT_EQUAL:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolNotEqual(comp->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(symbol->type));
  }
}

/****************************************************************************/

static void WalkSymbolAnd(const SymbolAnd *const and, const bool print_tree,
                          const int indent) {
  if (and->cond->type != SYMBOL_TYPE_COND) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(and->cond, print_tree, indent);
  WalkSymbolCond(and->cond, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(and->cond, print_tree, indent);

  if (and->comp->type != SYMBOL_TYPE_COMP) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(and->comp, print_tree, indent);
  WalkSymbolComp(and->comp, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(and->comp, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolCond(const SymbolCond *const cond, const bool print_tree,
                           const int indent) {
  Symbol *symbol = (Symbol *)cond->symbol;
  switch (symbol->type) {
  case SYMBOL_TYPE_COMP:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolComp(cond->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_AND:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolAnd(cond->symbol, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(symbol->type));
  }
}

/****************************************************************************/

static void WalkSymbolOr(const SymbolOr *const or, const bool print_tree,
                         const int indent) {
  if (or->expr->type != SYMBOL_TYPE_EXPR) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(or->expr, print_tree, indent);
  WalkSymbolExpr(or->expr, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(or->expr, print_tree, indent);

  if (or->cond->type != SYMBOL_TYPE_COND) {
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
  }
  PRINT_START(or->cond, print_tree, indent);
  WalkSymbolCond(or->cond, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
  PRINT_END(or->cond, print_tree, indent);
}

/****************************************************************************/

static void WalkSymbolExpr(const SymbolExpr *const expr, const bool print_tree,
                           const int indent) {
  Symbol *symbol = (Symbol *)expr->symbol;
  switch (symbol->type) {
  case SYMBOL_TYPE_COND:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolCond(expr->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  case SYMBOL_TYPE_OR:
    PRINT_START(symbol, print_tree, indent);
    WalkSymbolOr(expr->symbol, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(symbol, print_tree, indent);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol %s", StrRepr(symbol->type));
  }
}

/****************************************************************************/

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
  return true;
}

/****************************************************************************/

void WalkSyntaxTree(const bool print_tree) {
  const int indent = 0;
  if (P.expr != NULL) {
    if (P.expr->type != SYMBOL_TYPE_EXPR) {
      LOG_CRITICAL("Unexpected symbol %s", StrRepr(P.expr->type));
    }
    PRINT_START(P.expr, print_tree, indent);
    WalkSymbolExpr(P.expr, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
    PRINT_END(P.expr, print_tree, indent);
  }
}
