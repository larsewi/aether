#include "interpreter.h"
#include "config.h"

#include <assert.h>
#include <stdio.h>

#include "../utils/logger.h"

static void WalkSymbolExpression(SymbolExpression *expression, bool print_tree,
                                 int indent);
static void WalkSymbolCondition(SymbolCondition *condition, bool print_tree,
                                int indent);
static void WalkSymbolComparison(SymbolComparison *comparison, bool print_tree,
                                 int indent);
static void WalkSymbolTerm(SymbolTerm *term, bool print_tree, int indent);
static void WalkSymbolFactor(SymbolFactor *factor, bool print_tree, int indent);
static void WalkSymbolUnary(SymbolUnary *unary, bool print_tree, int indent);
static void WalkSymbolPrimary(SymbolPrimary *primary, bool print_tree,
                              int indent);
static void WalkSymbolEntries(SymbolEntries *entries, bool print_tree,
                              int indent);
static void WalkSymbolElements(SymbolElements *elements, bool print_tree,
                               int indent);

/****************************************************************************/

static void WalkSymbolIdentifier(SymbolIdentifier *const identifier,
                                 const bool print_tree, const int indent) {
  assert(identifier->type == SYMBOL_TYPE_IDENTIFIER);

  if (print_tree) {
    printf("%*s<IDENTIFIER ln=\"%d\" col=\"%d\">\n", indent, "",
           identifier->first.line, identifier->first.column);
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
  assert(integer_literal->type == SYMBOL_TYPE_INTEGER_LITERAL);

  if (print_tree) {
    printf("%*s<INTEGER_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           integer_literal->first.line, integer_literal->first.line);
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
  assert(float_literal->type == SYMBOL_TYPE_FLOAT_LITERAL);

  if (print_tree) {
    printf("%*s<FLOAT_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           float_literal->first.line, float_literal->first.column);
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
  assert(string_literal->type == SYMBOL_TYPE_STRING_LITERAL);

  if (print_tree) {
    printf("%*s<STRING_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           string_literal->first.line, string_literal->first.column);
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
  assert(boolean_literal->type == SYMBOL_TYPE_BOOLEAN_LITERAL);

  if (print_tree) {
    printf("%*s<BOOLEAN_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           boolean_literal->first.line, boolean_literal->first.column);
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
  assert(none_literal->type == SYMBOL_TYPE_NONE_LITERAL);

  if (print_tree) {
    printf("%*s<NONE_LITERAL ln=\"%d\" col=\"%d\">\n", indent, "",
           none_literal->first.line, none_literal->first.column);
  }

  free(none_literal);

  if (print_tree) {
    printf("%*s</NONE_LITERAL>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolDict(SymbolDict *const dict, const bool print_tree,
                           const int indent) {
  assert(dict->type == SYMBOL_TYPE_DICT);

  if (print_tree) {
    printf("%*s<dict ln=\"%d\" col=\"%d\">\n", indent, "", dict->first.line,
           dict->first.column);
  }

  WalkSymbolEntries(dict->entries, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(dict);

  if (print_tree) {
    printf("%*s<dict>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolEntries(SymbolEntries *const entries,
                              const bool print_tree, const int indent) {
  assert(entries->type == SYMBOL_TYPE_ENTRIES);

  if (print_tree) {
    printf("%*s<entries ln=\"%d\" col=\"%d\">\n", indent, "",
           entries->first.line, entries->first.column);
  }

  if (entries->entries != NULL) {
    WalkSymbolEntries(entries->entries, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);
  }

  WalkSymbolStringLiteral(entries->string_literal, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolExpression(entries->expression, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(entries);

  if (print_tree) {
    printf("%*s<entries>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolList(SymbolList *const list, const bool print_tree,
                           const int indent) {
  assert(list->type == SYMBOL_TYPE_LIST);

  if (print_tree) {
    printf("%*s<list ln=\"%d\" col=\"%d\">\n", indent, "", list->first.line,
           list->first.column);
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
  assert(elements->type == SYMBOL_TYPE_ELEMENTS);

  if (print_tree) {
    printf("%*s<elements ln=\"%d\" col=\"%d\">\n", indent, "",
           elements->first.line, elements->first.column);
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
  assert(atom->type == SYMBOL_TYPE_ATOM);

  if (print_tree) {
    printf("%*s<atom ln=\"%d\" col=\"%d\">\n", indent, "", atom->first.line,
           atom->first.column);
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
  assert(arguments->type == SYMBOL_TYPE_ARGUMENTS);

  if (print_tree) {
    printf("%*s<arguments ln=\"%d\" col=\"%d\">\n", indent, "",
           arguments->first.line, arguments->first.column);
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
  assert(fncall->type == SYMBOL_TYPE_FNCALL);

  if (print_tree) {
    printf("%*s<fncall ln=\"%d\" col=\"%d\">\n", indent, "", fncall->first.line,
           fncall->first.column);
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
  assert(subscription->type == SYMBOL_TYPE_SUBSCRIPTION);

  if (print_tree) {
    printf("%*s<subscription ln=\"%d\" col=\"%d\">\n", indent, "",
           subscription->first.line, subscription->first.column);
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
  assert(slice->type == SYMBOL_TYPE_SLICE);

  if (print_tree) {
    printf("%*s<slice ln=\"%d\" col=\"%d\" left_expression=\"%s\" "
           "right_expression=\"%s\">\n",
           indent, "", slice->first.line, slice->first.column,
           (slice->left_expression != NULL) ? "true" : "false",
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
  assert(primary->type == SYMBOL_TYPE_PRIMARY);

  if (print_tree) {
    printf("%*s<primary ln=\"%d\" col=\"%d\">\n", indent, "",
           primary->first.line, primary->first.column);
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
  assert(minus->type == SYMBOL_TYPE_MINUS);

  if (print_tree) {
    printf("%*s<minus ln=\"%d\" col=\"%d\">\n", indent, "", minus->first.line,
           minus->first.column);
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
  assert(negate->type == SYMBOL_TYPE_NEGATE);

  if (print_tree) {
    printf("%*s<negate ln=\"%d\" col=\"%d\">\n", indent, "", negate->first.line,
           negate->first.column);
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
  assert(unary->type == SYMBOL_TYPE_UNARY);

  if (print_tree) {
    printf("%*s<unary ln=\"%d\" col=\"%d\">\n", indent, "", unary->first.line,
           unary->first.column);
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
  assert(multiply->type == SYMBOL_TYPE_MULTIPLY);

  if (print_tree) {
    printf("%*s<multiply ln=\"%d\" col=\"%d\">\n", indent, "",
           multiply->first.line, multiply->first.column);
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
  assert(divide->type == SYMBOL_TYPE_DIVIDE);

  if (print_tree) {
    printf("%*s<divide ln=\"%d\" col=\"%d\">\n", indent, "", divide->first.line,
           divide->first.column);
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
  assert(modulo->type == SYMBOL_TYPE_MODULO);

  if (print_tree) {
    printf("%*s<modulo ln=\"%d\" col=\"%d\">\n", indent, "", modulo->first.line,
           modulo->first.column);
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
  assert(factor->type == SYMBOL_TYPE_FACTOR);

  if (print_tree) {
    printf("%*s<factor ln=\"%d\" col=\"%d\">\n", indent, "", factor->first.line,
           factor->first.column);
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
  assert(add->type == SYMBOL_TYPE_ADD);

  if (print_tree) {
    printf("%*s<add ln=\"%d\" col=\"%d\">\n", indent, "", add->first.line,
           add->first.column);
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
  assert(subtract->type == SYMBOL_TYPE_SUBTRACT);

  if (print_tree) {
    printf("%*s<subtract ln=\"%d\" col=\"%d\">\n", indent, "",
           subtract->first.line, subtract->first.column);
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
  assert(term->type == SYMBOL_TYPE_TERM);

  if (print_tree) {
    printf("%*s<term ln=\"%d\" col=\"%d\">\n", indent, "", term->first.line,
           term->first.column);
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
  assert(less_than->type == SYMBOL_TYPE_LESS_THAN);

  if (print_tree) {
    printf("%*s<less_than ln=\"%d\" col=\"%d\">\n", indent, "",
           less_than->first.line, less_than->first.column);
  }

  WalkSymbolComparison(less_than->comparison, print_tree,
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
  assert(greater_than->type == SYMBOL_TYPE_GREATER_THAN);

  if (print_tree) {
    printf("%*s<greater_than ln=\"%d\" col=\"%d\">\n", indent, "",
           greater_than->first.line, greater_than->first.column);
  }

  WalkSymbolComparison(greater_than->comparison, print_tree,
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
  assert(equal->type == SYMBOL_TYPE_EQUAL);

  if (print_tree) {
    printf("%*s<equal ln=\"%d\" col=\"%d\">\n", indent, "", equal->first.line,
           equal->first.column);
  }

  WalkSymbolComparison(equal->comparison, print_tree,
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
  assert(less_equal->type == SYMBOL_TYPE_LESS_EQUAL);

  if (print_tree) {
    printf("%*s<less_equal ln=\"%d\" col=\"%d\">\n", indent, "",
           less_equal->first.line, less_equal->first.column);
  }

  WalkSymbolComparison(less_equal->comparison, print_tree,
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
  assert(greater_equal->type == SYMBOL_TYPE_GREATER_EQUAL);

  if (print_tree) {
    printf("%*s<greater_equal ln=\"%d\" col=\"%d\">\n", indent, "",
           greater_equal->first.line, greater_equal->first.column);
  }

  WalkSymbolComparison(greater_equal->comparison, print_tree,
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
  assert(not_equal->type == SYMBOL_TYPE_NOT_EQUAL);

  if (print_tree) {
    printf("%*s<not_equal ln=\"%d\" col=\"%d\">\n", indent, "",
           not_equal->first.line, not_equal->first.column);
  }

  WalkSymbolComparison(not_equal->comparison, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolTerm(not_equal->term, print_tree,
                 indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(not_equal);

  if (print_tree) {
    printf("%*s</not_equal>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolComparison(SymbolComparison *const comparison,
                                 const bool print_tree, const int indent) {
  assert(comparison->type == SYMBOL_TYPE_COMPARISON);

  if (print_tree) {
    printf("%*s<comparison ln=\"%d\" col=\"%d\">\n", indent, "",
           comparison->first.line, comparison->first.column);
  }

  switch (comparison->symbol->type) {
  case SYMBOL_TYPE_TERM:
    WalkSymbolTerm((SymbolTerm *)comparison->symbol, print_tree,
                   indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_LESS_THAN:
    WalkSymbolLessThan((SymbolLessThan *)comparison->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_GREATER_THAN:
    WalkSymbolGreaterThan((SymbolGreaterThan *)comparison->symbol, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_EQUAL:
    WalkSymbolEqual((SymbolEqual *)comparison->symbol, print_tree,
                    indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_LESS_EQUAL:
    WalkSymbolLessEqual((SymbolLessEqual *)comparison->symbol, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_GREATER_EQUAL:
    WalkSymbolGreaterEqual((SymbolGreaterEqual *)comparison->symbol, print_tree,
                           indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_NOT_EQUAL:
    WalkSymbolNotEqual((SymbolNotEqual *)comparison->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", comparison->symbol->type);
  }

  free(comparison);

  if (print_tree) {
    printf("%*s</comparison>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolAnd(SymbolAnd *const and, const bool print_tree,
                          const int indent) {
  assert(and->type == SYMBOL_TYPE_AND);

  if (print_tree) {
    printf("%*s<and ln=\"%d\" col=\"%d\">\n", indent,
           "", and->first.line, and->first.column);
  }

  WalkSymbolCondition(and->condition, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);

  WalkSymbolComparison(and->comparison, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(and);

  if (print_tree) {
    printf("%*s</and>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolCondition(SymbolCondition *const condition,
                                const bool print_tree, const int indent) {
  assert(condition->type == SYMBOL_TYPE_CONDITION);

  if (print_tree) {
    printf("%*s<condition ln=\"%d\" col=\"%d\">\n", indent, "",
           condition->first.line, condition->first.column);
  }

  switch (condition->symbol->type) {
  case SYMBOL_TYPE_COMPARISON:
    WalkSymbolComparison((SymbolComparison *)condition->symbol, print_tree,
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
  assert(or->type == SYMBOL_TYPE_OR);

  if (print_tree) {
    printf("%*s</or ln=\"%d\" col=\"%d\">\n", indent, "", or->first.line,
           or->first.column);
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
  assert(expression->type == SYMBOL_TYPE_EXPRESSION);

  if (print_tree) {
    printf("%*s<expression ln=\"%d\" col=\"%d\">\n", indent, "",
           expression->first.line, expression->first.column);
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
  assert(datatype->type == SYMBOL_TYPE_DATATYPE);

  if (print_tree) {
    printf("%*s<datatype ln=\"%d\" col=\"%d\">\n", indent, "",
           datatype->first.line, datatype->first.column);
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
  assert(mutable->type == SYMBOL_TYPE_MUTABLE);

  if (print_tree) {
    printf("%*s<mutable ln=\"%d\" col=\"%d\">\n", indent, "",
           mutable->first.line, mutable->first.column);
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
  assert(reference->type == SYMBOL_TYPE_REFERENCE);

  if (print_tree) {
    printf("%*s<reference ln=\"%d\" col=\"%d\">\n", indent, "",
           reference->first.line, reference->first.column);
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

static void WalkSymbolDeclaration(SymbolDeclaration *const declaration,
                                  const bool print_tree, const int indent) {
  assert(declaration->type == SYMBOL_TYPE_DECLARATION);

  if (print_tree) {
    printf("%*s<declaration ln=\"%d\" col=\"%d\">\n", indent, "",
           declaration->first.line, declaration->first.column);
  }

  switch (declaration->symbol->type) {
  case SYMBOL_TYPE_REFERENCE:
    WalkSymbolReference((SymbolReference *)declaration->symbol, print_tree,
                        indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_MUTABLE:
    WalkSymbolMutable((SymbolMutable *)declaration->symbol, print_tree,
                      indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_DATATYPE:
    WalkSymbolDatatype((SymbolDatatype *)declaration->symbol, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", declaration->symbol->type);
  }

  WalkSymbolIdentifier(declaration->identifier, print_tree,
                       indent + DEFAULT_SYNTAX_TREE_INDENT);

  free(declaration);

  if (print_tree) {
    printf("%*s</declaration>\n", indent, "");
  }
}

/****************************************************************************/

static void WalkSymbolAssignment(SymbolAssignment *const assignment,
                                 const bool print_tree, const int indent) {
  assert(assignment->type == SYMBOL_TYPE_ASSIGNMENT);

  if (print_tree) {
    printf("%*s<assignment ln=\"%d\" col=\"%d\">\n", indent, "",
           assignment->first.line, assignment->first.column);
  }

  switch (assignment->symbol->type) {
  case SYMBOL_TYPE_EXPRESSION:
    WalkSymbolExpression((SymbolExpression *)assignment->symbol, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_DECLARATION:
    WalkSymbolDeclaration((SymbolDeclaration *)assignment->symbol, print_tree,
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

static void WalkSymbolStatement(SymbolStatement *const statement,
                                const bool print_tree, const int indent) {
  assert(statement->type == SYMBOL_TYPE_STATEMENT);

  if (print_tree) {
    printf("%*s<statement ln=\"%d\" col=\"%d\">\n", indent, "",
           statement->first.line, statement->first.column);
  }

  switch (statement->symbol->type) {
  case SYMBOL_TYPE_ASSIGNMENT:
    WalkSymbolAssignment((SymbolAssignment *)statement->symbol, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_DECLARATION:
    WalkSymbolDeclaration((SymbolDeclaration *)statement->symbol, print_tree,
                          indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  case SYMBOL_TYPE_EXPRESSION:
    WalkSymbolExpression((SymbolExpression *)statement->symbol, print_tree,
                         indent + DEFAULT_SYNTAX_TREE_INDENT);
    break;
  default:
    LOG_CRITICAL("Unexpected symbol type %d", statement->symbol->type);
  }

  free(statement);

  if (print_tree) {
    printf("%*s</statement>\n", indent, "");
  }
}

/****************************************************************************/

void WalkSyntaxTree(SymbolStatement *const statement, const bool print_tree) {
  if (statement != NULL) {
    WalkSymbolStatement(statement, print_tree, 0);
  }
}
