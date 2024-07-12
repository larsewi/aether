#include "interpreter.h"
#include "config.h"

#include <stdio.h>

#include "../utils/logger.h"

static void WalkSymbolExpression(SymbolExpression *expression, bool print_tree,
                                 int indent);
static void WalkSymbolCondition(SymbolCondition *condition, bool print_tree,
                                int indent);
static void WalkSymbolComparrison(SymbolComparrison *comparrison,
                                  bool print_tree, int indent);
static void WalkSymbolTerm(SymbolTerm *term, bool print_tree, int indent);
static void WalkSymbolFactor(SymbolFactor *factor, bool print_tree, int indent);
static void WalkSymbolUnary(SymbolUnary *unary, bool print_tree, int indent);
static void WalkSymbolPrimary(SymbolPrimary *primary, bool print_tree,
                              int indent);
static void WalkSymbolPairs(SymbolPairs *pairs, bool print_tree, int indent);
static void WalkSymbolElements(SymbolElements *elements, bool print_tree,
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

static void WalkSymbolDict(SymbolDict *const dict, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<dict>\n", indent, "");
  }

  WalkSymbolPairs(dict->pairs, print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(dict);

  if (print_tree) {
    printf("%*s<dict>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolPairs(SymbolPairs *const pairs, const bool print_tree,
                            const int indent) {
  if (print_tree) {
    printf("%*s<pairs>\n", indent, "");
  }

  if (pairs->pairs != NULL) {
    WalkSymbolPairs(pairs->pairs, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  WalkSymbolStringLiteral(pairs->string_literal, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolExpression(pairs->expression, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(pairs);

  if (print_tree) {
    printf("%*s<pairs>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolList(SymbolList *const list, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<list>\n", indent, "");
  }

  if (list->elements != NULL) {
    WalkSymbolElements(list->elements, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  free(list);

  if (print_tree) {
    printf("%*s</list>\n", indent, "");
  }
}

static void WalkSymbolElements(SymbolElements *const elements,
                               const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<elements>\n", indent, "");
  }

  if (elements->elements != NULL) {
    WalkSymbolElements(elements->elements, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  WalkSymbolExpression(elements->expression, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(elements);

  if (print_tree) {
    printf("%*s</elements>\n", indent, "");
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
  case SYMBOL_TYPE_DICT:
    WalkSymbolDict((SymbolDict *)atom->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_LIST:
    WalkSymbolList((SymbolList *)atom->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_EXPRESSION:
    WalkSymbolExpression((SymbolExpression *)atom->symbol, print_tree,
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

  WalkSymbolExpression(arguments->expression, print_tree,
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

  WalkSymbolExpression(subscription->expression, print_tree,
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
    printf("%*s<slice left_expression=\"%s\" right_expression=\"%s\">\n",
           indent, "", (slice->left_expression != NULL) ? "true" : "false",
           (slice->right_expression != NULL) ? "true" : "false");
  }

  WalkSymbolPrimary(slice->primary, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);

  if (slice->left_expression != NULL) {
    WalkSymbolExpression(slice->left_expression, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  if (slice->right_expression != NULL) {
    WalkSymbolExpression(slice->right_expression, print_tree,
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

  WalkSymbolComparrison(less_than->comparrison, print_tree,
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

  WalkSymbolComparrison(greater_than->comparrison, print_tree,
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

  WalkSymbolComparrison(equal->comparrison, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);

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

  WalkSymbolComparrison(less_equal->comparrison, print_tree,
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

  WalkSymbolComparrison(greater_equal->comparrison, print_tree,
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

  WalkSymbolComparrison(not_equal->comparrison, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolTerm(not_equal->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(not_equal);

  if (print_tree) {
    printf("%*s</not_equal>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolComparrison(SymbolComparrison *const comparrison,
                                  const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<comparrison>\n", indent, "");
  }

  switch (comparrison->symbol->type) {
  case SYMBOL_TYPE_TERM:
    WalkSymbolTerm((SymbolTerm *)comparrison->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_LESS_THAN:
    WalkSymbolLessThan((SymbolLessThan *)comparrison->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_GREATER_THAN:
    WalkSymbolGreaterThan((SymbolGreaterThan *)comparrison->symbol, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_EQUAL:
    WalkSymbolEqual((SymbolEqual *)comparrison->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_LESS_EQUAL:
    WalkSymbolLessEqual((SymbolLessEqual *)comparrison->symbol, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_GREATER_EQUAL:
    WalkSymbolGreaterEqual((SymbolGreaterEqual *)comparrison->symbol,
                           print_tree, indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_NOT_EQUAL:
    WalkSymbolNotEqual((SymbolNotEqual *)comparrison->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", comparrison->symbol->type);
  }

  free(comparrison);

  if (print_tree) {
    printf("%*s</comparrison>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolAnd(SymbolAnd *const and, const bool print_tree,
                          const int indent) {
  if (print_tree) {
    printf("%*s<and>\n", indent, "");
  }

  WalkSymbolCondition(and->condition, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolComparrison(and->comparrison, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(and);

  if (print_tree) {
    printf("%*s</and>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolCondition(SymbolCondition *const condition,
                                const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<condition>\n", indent, "");
  }

  switch (condition->symbol->type) {
  case SYMBOL_TYPE_COMPARRISON:
    WalkSymbolComparrison((SymbolComparrison *)condition->symbol, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_AND:
    WalkSymbolAnd((SymbolAnd *)condition->symbol, print_tree,
                  indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", condition->symbol->type);
  }

  free(condition);

  if (print_tree) {
    printf("%*s</condition>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolOr(SymbolOr *const or, const bool print_tree,
                         const int indent) {
  if (print_tree) {
    printf("%*s</or>\n", indent, "");
  }

  WalkSymbolExpression(or->expression, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolCondition(or->condition, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(or);

  if (print_tree) {
    printf("%*s</or>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolExpression(SymbolExpression *const expression,
                                 const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<expression>\n", indent, "");
  }

  switch (expression->symbol->type) {
  case SYMBOL_TYPE_CONDITION:
    WalkSymbolCondition((SymbolCondition *)expression->symbol, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_OR:
    WalkSymbolOr((SymbolOr *)expression->symbol, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", expression->symbol->type);
  }

  free(expression);

  if (print_tree) {
    printf("%*s</expression>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolDatatype(SymbolDatatype *const datatype,
                               const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<datatype>\n", indent, "");
  }

  WalkSymbolIdentifier(datatype->identifier, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(datatype);

  if (print_tree) {
    printf("%*s</datatype>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolMutable(SymbolMutable *const mutable,
                              const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<mutable>\n", indent, "");
  }

  WalkSymbolDatatype(mutable->datatype, print_tree,
                     indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(mutable);

  if (print_tree) {
    printf("%*s</mutable>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolReference(SymbolReference *const reference,
                                const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<reference>\n", indent, "");
  }

  switch (reference->symbol->type) {
  case SYMBOL_TYPE_DATATYPE:
    WalkSymbolDatatype((SymbolDatatype *)reference->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_MUTABLE:
    WalkSymbolMutable((SymbolMutable *)reference->symbol, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", reference->symbol->type);
  }

  free(reference);

  if (print_tree) {
    printf("%*s</reference>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolDecl(SymbolDecl *const decl, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<decl>\n", indent, "");
  }

  switch (decl->symbol->type) {
  case SYMBOL_TYPE_REFERENCE:
    WalkSymbolReference((SymbolReference *)decl->symbol, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_MUTABLE:
    WalkSymbolMutable((SymbolMutable *)decl->symbol, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_DATATYPE:
    WalkSymbolDatatype((SymbolDatatype *)decl->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", decl->symbol->type);
  }

  WalkSymbolIdentifier(decl->identifier, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(decl);

  if (print_tree) {
    printf("%*s</decl>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolVariable(SymbolVariable *const variable,
                               const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<variable>\n", indent, "");
  }

  switch (variable->symbol->type) {
  case SYMBOL_TYPE_IDENTIFIER:
    WalkSymbolIdentifier((SymbolIdentifier *)variable->symbol, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_VARIABLE:
    WalkSymbolVariable((SymbolVariable *)variable->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", variable->symbol->type);
  }

  if (variable->expression != NULL) {
    WalkSymbolExpression(variable->expression, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  free(variable);

  if (print_tree) {
    printf("%*s</variable>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolAssignment(SymbolAssignment *const assignment,
                                 const bool print_tree, const int indent) {
  if (print_tree) {
    printf("%*s<assignment>\n", indent, "");
  }

  switch (assignment->symbol->type) {
  case SYMBOL_TYPE_VARIABLE:
    WalkSymbolVariable((SymbolVariable *)assignment->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_DECL:
    WalkSymbolDecl((SymbolDecl *)assignment->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", assignment->symbol->type);
  }

  WalkSymbolExpression(assignment->expression, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(assignment);

  if (print_tree) {
    printf("%*s</assignment>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolStmt(SymbolStmt *const stmt, const bool print_tree,
                           const int indent) {
  if (print_tree) {
    printf("%*s<stmt>\n", indent, "");
  }

  switch (stmt->symbol->type) {
  case SYMBOL_TYPE_ASSIGNMENT:
    WalkSymbolAssignment((SymbolAssignment *)stmt->symbol, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_DECL:
    WalkSymbolDecl((SymbolDecl *)stmt->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_EXPRESSION:
    WalkSymbolExpression((SymbolExpression *)stmt->symbol, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", stmt->symbol->type);
  }

  free(stmt);

  if (print_tree) {
    printf("%*s</stmt>\n", indent, "");
  }
}

/****************************************************************************/

void WalkSyntaxTree(SymbolStmt *const stmt, const bool print_tree) {
  if (stmt != NULL) {
    WalkSymbolStmt(stmt, print_tree, 0);
  }
}
