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

static void WalkSymbolExpr(SymbolExpr *expr, bool print_tree, int indent);
static void WalkSymbolCond(SymbolCond *cond, bool print_tree, int indent);
static void WalkSymbolComp(SymbolComp *comp, bool print_tree, int indent);
static void WalkSymbolTerm(SymbolTerm *term, bool print_tree, int indent);
static void WalkSymbolFactor(SymbolFactor *factor, bool print_tree, int indent);
static void WalkSymbolUnary(SymbolUnary *unary, bool print_tree, int indent);
static void WalkSymbolPrimary(SymbolPrimary *primary, bool print_tree,
                              int indent);

/****************************************************************************/

static void WalkSymbolIdentifier(SymbolIdentifier *const identifier,
                                 const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<IDENTIFIER ln=\"%d\" col=\"%d\">\n", indent, "",
           identifier->line, identifier->column);
    printf("%*s  %s\n", indent, "", identifier->value);
  }

  free(identifier->value);
  free(identifier);

  if (print_tree) {
    printf("%*s</IDENTIFIER>\n", indent, "");
  }
}

/****************************************************************************/

static void
WalkSymbolIntegerLiteral(SymbolIntegerLiteral *const integer_literal,
                         const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<INTEGER_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           integer_literal->line, integer_literal->column);
    printf("%*s  %lld\n", indent, "", integer_literal->value);
  }

  free(integer_literal);

  if (print_tree) {
    printf("%*s</INTEGER_LITERAL>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolFloatLiteral(SymbolFloatLiteral *const float_literal,
                                   const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<FLOAT_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           float_literal->line, float_literal->column);
    printf("%*s  %lf\n", indent, "", float_literal->value);
  }

  free(float_literal);

  if (print_tree) {
    printf("%*s</FLOAT_LITERAL>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolStringLiteral(SymbolStringLiteral *const string_literal,
                                    const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<STRING_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           string_literal->line, string_literal->column);
    printf("%*s  \"%s\"\n", indent, "", string_literal->value);
  }

  free(string_literal->value);
  free(string_literal);

  if (print_tree) {
    printf("%*s</STRING_LITERAL>\n", indent, "");
  }
}

/****************************************************************************/

static void
WalkSymbolBooleanLiteral(SymbolBooleanLiteral *const boolean_literal,
                         const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<BOOLEAN_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           boolean_literal->line, boolean_literal->column);
    printf("%*s  %s\n", indent, "", boolean_literal->value ? "true" : "false");
  }

  free(boolean_literal);

  if (print_tree) {
    printf("%*s</BOOLEAN_LITERAL>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolNoneLiteral(SymbolNoneLiteral *const none_literal,
                                  const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<NONE_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           none_literal->line, none_literal->column);
  }

  free(none_literal);

  if (print_tree) {
    printf("%*s</NONE_LITERAL>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolAtom(SymbolAtom *const atom, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<atom>\n", indent, "");
  }

  switch (atom->symbol->type) {
  case SYMBOL_TYPE_IDENTIFIER:
    WalkSymbolIdentifier((SymbolIdentifier *)atom->symbol, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_INTEGER_LITERAL:
    WalkSymbolIntegerLiteral((SymbolIntegerLiteral *)atom->symbol, print_tree,
                             indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_FLOAT_LITERAL:
    WalkSymbolFloatLiteral((SymbolFloatLiteral *)atom->symbol, print_tree,
                           indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_STRING_LITERAL:
    WalkSymbolStringLiteral((SymbolStringLiteral *)atom->symbol, print_tree,
                            indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_BOOLEAN_LITERAL:
    WalkSymbolBooleanLiteral((SymbolBooleanLiteral *)atom->symbol, print_tree,
                             indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_NONE_LITERAL:
    WalkSymbolNoneLiteral((SymbolNoneLiteral *)atom->symbol, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_EXPR:
    WalkSymbolExpr((SymbolExpr *)atom->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", atom->symbol->type);
  }

  free(atom);

  if (print_tree) {
    printf("%*s</atom>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolArguments(SymbolArguments *const arguments,
                                const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<arguments>\n", indent, "");
  }

  if (arguments->arguments != NULL) {
    WalkSymbolArguments(arguments->arguments, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  WalkSymbolExpr(arguments->expr, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(arguments);

  if (print_tree) {
    printf("%*s</arguments>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolFncall(SymbolFncall *const fncall, const bool print_tree,
                             const int indent) {
  if (print_tree) {
    printf("%*s<fncall>\n", indent, "");
  }

  WalkSymbolPrimary(fncall->primary, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);

  if (fncall->arguments != NULL) {
    WalkSymbolArguments(fncall->arguments, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  free(fncall);

  if (print_tree) {
    printf("%*s</fncall>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolSubscription(SymbolSubscription *const subscription,
                                   const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<subscription>\n", indent, "");
  }

  WalkSymbolPrimary(subscription->primary, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolExpr(subscription->expr, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(subscription);

  if (print_tree) {
    printf("%*s</subscription>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolSlice(SymbolSlice *const slice, const bool print_tree,
                            const int indent) {
  if (print_tree) {
    printf("%*s<slice left_expr=\"%s\" right_expr=\"%s\">\n", indent, "",
           (slice->left_expr != NULL) ? "true" : "false",
           (slice->right_expr != NULL) ? "true" : "false");
  }

  WalkSymbolPrimary(slice->primary, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);

  if (slice->left_expr != NULL) {
    WalkSymbolExpr(slice->left_expr, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  if (slice->right_expr != NULL) {
    WalkSymbolExpr(slice->right_expr, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  free(slice);

  if (print_tree) {
    printf("%*s</slice>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolPrimary(SymbolPrimary *const primary,
                              const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<primary>\n", indent, "");
  }

  switch (primary->symbol->type) {
  case SYMBOL_TYPE_ATOM:
    WalkSymbolAtom((SymbolAtom *)primary->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_FNCALL:
    WalkSymbolFncall((SymbolFncall *)primary->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_SUBSCRIPTION:
    WalkSymbolSubscription((SymbolSubscription *)primary->symbol, print_tree,
                           indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_SLICE:
    WalkSymbolSlice((SymbolSlice *)primary->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", primary->symbol->type);
  }

  free(primary);

  if (print_tree) {
    printf("%*s</primary>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolMinus(SymbolMinus *const minus, const bool print_tree,
                            const int indent) {
  if (print_tree) {
    printf("%*s<minus>\n", indent, "");
  }

  WalkSymbolUnary(minus->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(minus);

  if (print_tree) {
    printf("%*s</minus>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolNegate(SymbolNegate *const negate, const bool print_tree,
                             const int indent) {
  if (print_tree) {
    printf("%*s<negate>\n", indent, "");
  }

  WalkSymbolUnary(negate->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(negate);

  if (print_tree) {
    printf("%*s</negate>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolUnary(SymbolUnary *const unary, const bool print_tree,
                            const int indent) {
  if (print_tree) {
    printf("%*s<unary>\n", indent, "");
  }

  switch (unary->symbol->type) {
  case SYMBOL_TYPE_PRIMARY:
    WalkSymbolPrimary((SymbolPrimary *)unary->symbol, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_MINUS:
    WalkSymbolMinus((SymbolMinus *)unary->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_NEGATE:
    WalkSymbolNegate((SymbolNegate *)unary->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", unary->symbol->type);
  }

  free(unary);

  if (print_tree) {
    printf("%*s</unary>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolMultiply(SymbolMultiply *const multiply,
                               const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<multiply>\n", indent, "");
  }

  WalkSymbolFactor(multiply->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolUnary(multiply->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(multiply);

  if (print_tree) {
    printf("%*s</multiply>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolDivide(SymbolDivide *const divide, const bool print_tree,
                             const int indent) {
  if (print_tree) {
    printf("%*s<divide>\n", indent, "");
  }

  WalkSymbolFactor(divide->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolUnary(divide->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(divide);

  if (print_tree) {
    printf("%*s</divide>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolModulo(SymbolModulo *const modulo, const bool print_tree,
                             const int indent) {
  if (print_tree) {
    printf("%*s<modulo>\n", indent, "");
  }

  WalkSymbolFactor(modulo->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolUnary(modulo->unary, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(modulo);

  if (print_tree) {
    printf("%*s</modulo>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolFactor(SymbolFactor *const factor, const bool print_tree,
                             const int indent) {
  if (print_tree) {
    printf("%*s<factor>\n", indent, "");
  }

  switch (factor->symbol->type) {
  case SYMBOL_TYPE_UNARY:
    WalkSymbolUnary((SymbolUnary *)factor->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_MULTIPLY:
    WalkSymbolMultiply((SymbolMultiply *)factor->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_DIVIDE:
    WalkSymbolDivide((SymbolDivide *)factor->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_MODULO:
    WalkSymbolModulo((SymbolModulo *)factor->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", factor->symbol->type);
  }

  free(factor);

  if (print_tree) {
    printf("%*s</factor>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolAdd(SymbolAdd *const add, const bool print_tree,
                          const int indent) {
  if (print_tree) {
    printf("%*s<add>\n", indent, "");
  }

  WalkSymbolTerm(add->term, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolFactor(add->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(add);

  if (print_tree) {
    printf("%*s</add>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolSubtract(SymbolSubtract *const subtract,
                               const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<subtract>\n", indent, "");
  }

  WalkSymbolTerm(subtract->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolFactor(subtract->factor, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);

  if (print_tree) {
    printf("%*s</subtract>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolTerm(SymbolTerm *const term, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<term>\n", indent, "");
  }

  switch (term->symbol->type) {
  case SYMBOL_TYPE_FACTOR:
    WalkSymbolFactor((SymbolFactor *)term->symbol, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_ADD:
    WalkSymbolAdd((SymbolAdd *)term->symbol, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_SUBTRACT:
    WalkSymbolSubtract((SymbolSubtract *)term->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", term->symbol->type);
  }

  free(term);

  if (print_tree) {
    printf("%*s</term>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolLessThan(SymbolLessThan *const less_than,
                               const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<less_than>\n", indent, "");
  }

  WalkSymbolComp(less_than->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolTerm(less_than->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(less_than);

  if (print_tree) {
    printf("%*s</less_than>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolGreaterThan(SymbolGreaterThan *const greater_than,
                                  const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<greater_than>\n", indent, "");
  }

  WalkSymbolComp(greater_than->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolTerm(greater_than->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(greater_than);

  if (print_tree) {
    printf("%*s</greater_than>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolEqual(SymbolEqual *const equal, const int indent,
                            const bool print_tree) {
  if (print_tree) {
    printf("%*s<equal>\n", indent, "");
  }

  WalkSymbolComp(equal->comp, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolTerm(equal->term, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(equal);

  if (print_tree) {
    printf("%*s</equal>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolLessEqual(SymbolLessEqual *const less_equal,
                                const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<less_equal>\n", indent, "");
  }

  WalkSymbolComp(less_equal->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolTerm(less_equal->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(less_equal);

  if (print_tree) {
    printf("%*s</less_equal>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolGreaterEqual(SymbolGreaterEqual *const greater_equal,
                                   const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<greater_equal>\n", indent, "");
  }

  WalkSymbolComp(greater_equal->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolTerm(greater_equal->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(greater_equal);

  if (print_tree) {
    printf("%*s</greater_equal>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolNotEqual(SymbolNotEqual *const not_equal,
                               const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<not_equal>\n", indent, "");
  }

  WalkSymbolComp(not_equal->comp, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolTerm(not_equal->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(not_equal);

  if (print_tree) {
    printf("%*s</not_equal>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolComp(SymbolComp *const comp, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<comp>\n", indent, "");
  }

  switch (comp->symbol->type) {
  case SYMBOL_TYPE_TERM:
    WalkSymbolTerm((SymbolTerm *)comp->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_LESS_THAN:
    WalkSymbolLessThan((SymbolLessThan *)comp->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_GREATER_THAN:
    WalkSymbolGreaterThan((SymbolGreaterThan *)comp->symbol, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_EQUAL:
    WalkSymbolEqual((SymbolEqual *)comp->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_LESS_EQUAL:
    WalkSymbolLessEqual((SymbolLessEqual *)comp->symbol, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_GREATER_EQUAL:
    WalkSymbolGreaterEqual((SymbolGreaterEqual *)comp->symbol, print_tree,
                           indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_NOT_EQUAL:
    WalkSymbolNotEqual((SymbolNotEqual *)comp->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", comp->symbol->type);
  }

  free(comp);

  if (print_tree) {
    printf("%*s</comp>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolAnd(SymbolAnd *const and, const bool print_tree,
                          const int indent) {
  if (print_tree) {
    printf("%*s<and>\n", indent, "");
  }

  WalkSymbolCond(and->cond, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolComp(and->comp, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(and);

  if (print_tree) {
    printf("%*s</and>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolCond(SymbolCond *const cond, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<cond>\n", indent, "");
  }

  switch (cond->symbol->type) {
  case SYMBOL_TYPE_COMP:
    WalkSymbolComp((SymbolComp *)cond->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_AND:
    WalkSymbolAnd((SymbolAnd *)cond->symbol, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", cond->symbol->type);
  }

  free(cond);

  if (print_tree) {
    printf("%*s</cond>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolOr(SymbolOr *const or, const bool print_tree,
                         const int indent) {
  if (print_tree) {
    printf("%*s</or>\n", indent, "");
  }

  WalkSymbolExpr(or->expr, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolCond(or->cond, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(or);

  if (print_tree) {
    printf("%*s</or>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolExpr(SymbolExpr *const expr, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<expr>\n", indent, "");
  }

  switch (expr->symbol->type) {
  case SYMBOL_TYPE_COND:
    WalkSymbolCond((SymbolCond *)expr->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_OR:
    WalkSymbolOr((SymbolOr *)expr->symbol, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", expr->symbol->type);
  }

  free(expr);

  if (print_tree) {
    printf("%*s</expr>\n", indent, "");
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
  yylex_destroy();
  return true;
}

/****************************************************************************/

void WalkSyntaxTree(const bool print_tree) {
  if (P.expr != NULL) {
    WalkSymbolExpr(P.expr, print_tree, 0);
  }
}
