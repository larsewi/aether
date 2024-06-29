%{
#include "syntax.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils/logger.h"
#include "../utils/alloc.h"

#define P PARSER_STATE

extern int yylex();
void yyerror(char *msg);
%}

%union {
  SymbolIdentifier *identifier;
  SymbolIntegerLiteral *integer_literal;
  SymbolFloatLiteral *float_literal;
  SymbolStringLiteral *string_literal;
  SymbolBooleanLiteral *boolean_literal;
  SymbolNoneLiteral *none_literal;
}

%token <identifier> IDENTIFIER
%token <integer_literal> INTEGER_LITERAL
%token <float_literal> FLOAT_LITERAL
%token <string_literal> STRING_LITERAL
%token <boolean_literal> BOOLEAN_LITERAL
%token <none_literal> NONE_LITERAL
%token EQ_OPER LE_OPER GE_OPER NE_OPER
%token AND_OPER OR_OPER

%union {
  SymbolExpr *expr;
  SymbolOr *or;
  SymbolCond *cond;
  SymbolAnd *and;
  SymbolComp *comp;
  SymbolLessThan *less_than;
  SymbolGreaterThan *greater_than;
  SymbolEqual *equal;
  SymbolLessEqual *less_equal;
  SymbolGreaterEqual *greater_equal;
  SymbolNotEqual *not_equal;
  SymbolTerm *term;
  SymbolAdd *add;
  SymbolSubtract *subtract;
  SymbolFactor *factor;
  SymbolMultiply *multiply;
  SymbolDivide *divide;
  SymbolModulo *modulo;
  SymbolUnary *unary;
  SymbolMinus *minus;
  SymbolNegate *negate;
  SymbolPrimary *primary;
  SymbolFncall *fncall;
  SymbolArguments *arguments;
  SymbolSubscription *subscription;
  SymbolSlice *slice;
  SymbolAtom *atom;
}

%type <expr> inner_expr expr;
%type <or> or;
%type <cond> cond;
%type <and> and;
%type <comp> comp;
%type <less_than> less_than;
%type <greater_than> greater_than;
%type <equal> equal;
%type <less_equal> less_equal;
%type <greater_equal> greater_equal;
%type <not_equal> not_equal;
%type <term> term;
%type <add> add;
%type <subtract> subtract;
%type <factor> factor;
%type <multiply> multiply;
%type <divide> divide;
%type <modulo> modulo;
%type <unary> unary;
%type <minus> minus;
%type <negate> negate;
%type <primary> primary;
%type <fncall> fncall;
%type <arguments> arguments;
%type <subscription> subscription;
%type <slice> slice;
%type <atom> atom;

%%

start
: /* empty */ {
  LOG_DEBUG("start : %%empty");
  P.expr = NULL;
}
| expr {
  LOG_DEBUG("start : expr");
  P.expr = $1;
}
;

expr
: cond {
  LOG_DEBUG("expr : cond");
  $$ = xmalloc(sizeof(SymbolExpr));
  $$->type = SYMBOL_TYPE_EXPR;
  $$->symbol = $1;
}
| or {
  LOG_DEBUG("expr : or");
  $$ = xmalloc(sizeof(SymbolExpr));
  $$->type = SYMBOL_TYPE_EXPR;
  $$->symbol = $1;
}
;

or
: expr OR_OPER cond {
  LOG_DEBUG("or : expr OR_OPER cond");
  $$ = xmalloc(sizeof(SymbolOr));
  $$->type = SYMBOL_TYPE_OR;
  $$->expr = $1;
  $$->cond = $3;
}
;

cond
: comp {
  LOG_DEBUG("cond : comp");
  $$ = xmalloc(sizeof(SymbolCond));
  $$->type = SYMBOL_TYPE_COND;
  $$->symbol = $1;
}
| and {
  LOG_DEBUG("cond : and");
  $$ = xmalloc(sizeof(SymbolCond));
  $$->type = SYMBOL_TYPE_COND;
  $$->symbol = $1;
}
;

and
: cond AND_OPER comp {
  LOG_DEBUG("and : cond AND_OPER comp");
  $$ = xmalloc(sizeof(SymbolAnd));
  $$->type = SYMBOL_TYPE_AND;
  $$->cond = $1;
  $$->comp = $3;
}
;

comp
: term {
  LOG_DEBUG("comp : term");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| less_than {
  LOG_DEBUG("comp : less_than");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| greater_than {
  LOG_DEBUG("comp : greater_than");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| equal {
  LOG_DEBUG("comp : equal");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| less_equal {
  LOG_DEBUG("comp : less_equal");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| greater_equal {
  LOG_DEBUG("comp : greater_equal");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| not_equal {
  LOG_DEBUG("comp : not_equal");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
;

less_than
: comp '<' term {
  LOG_DEBUG("less_than : comp '<' term");
  $$ = xmalloc(sizeof(SymbolLessThan));
  $$->type = SYMBOL_TYPE_LESS_THAN;
  $$->comp = $1;
  $$->term = $3;
}
;

greater_than
: comp '>' term {
  LOG_DEBUG("greater_than : comp '>' term");
  $$ = xmalloc(sizeof(SymbolGreaterThan));
  $$->type = SYMBOL_TYPE_GREATER_THAN;
  $$->comp = $1;
  $$->term = $3;
}
;

equal
: comp EQ_OPER term {
  LOG_DEBUG("equal : comp EQ_OPER term");
  $$ = xmalloc(sizeof(SymbolEqual));
  $$->type = SYMBOL_TYPE_EQUAL;
  $$->comp = $1;
  $$->term = $3;
}
;

less_equal
: comp LE_OPER term {
  LOG_DEBUG("less_equal : comp LE_OPER term");
  $$ = xmalloc(sizeof(SymbolLessEqual));
  $$->type = SYMBOL_TYPE_LESS_EQUAL;
  $$->comp = $1;
  $$->term = $3;
}
;

greater_equal
: comp GE_OPER term {
  LOG_DEBUG("greater_equal : comp GE_OPER term");
  $$ = xmalloc(sizeof(SymbolGreaterEqual));
  $$->type = SYMBOL_TYPE_GREATER_EQUAL;
  $$->comp = $1;
  $$->term = $3;
}
;

not_equal
: comp NE_OPER term {
  LOG_DEBUG("not_equal : comp NE_OPER term");
  $$ = xmalloc(sizeof(SymbolNotEqual));
  $$->type = SYMBOL_TYPE_NOT_EQUAL;
  $$->comp = $1;
  $$->term = $3;
}
;

term
: factor {
  LOG_DEBUG("term : factor");
  $$ = xmalloc(sizeof(SymbolTerm));
  $$->type = SYMBOL_TYPE_TERM;
  $$->symbol = $1;
}
| add {
  LOG_DEBUG("term : add");
  $$ = xmalloc(sizeof(SymbolTerm));
  $$->type = SYMBOL_TYPE_TERM;
  $$->symbol = $1;
}
| subtract {
  LOG_DEBUG("term : subtract");
  $$ = xmalloc(sizeof(SymbolTerm));
  $$->type = SYMBOL_TYPE_TERM;
  $$->symbol = $1;
}
;

add
: term '+' factor {
  LOG_DEBUG("add : term '+' factor");
  $$ = xmalloc(sizeof(SymbolAdd));
  $$->type = SYMBOL_TYPE_ADD;
  $$->term = $1;
  $$->factor = $3;
}
;

subtract
: term '-' factor {
  LOG_DEBUG("subtract : term '-' factor");
  $$ = xmalloc(sizeof(SymbolSubtract));
  $$->type = SYMBOL_TYPE_SUBTRACT;
  $$->term = $1;
  $$->factor = $3;
}
;

factor
: unary {
  LOG_DEBUG("factor : unary");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->symbol = $1;
}
| multiply {
  LOG_DEBUG("factor : multiply");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->symbol = $1;
}
| divide {
  LOG_DEBUG("factor : divide");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->symbol = $1;
}
| modulo {
  LOG_DEBUG("factor : modulo");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_MODULO;
  $$->symbol = $1;
}
;

multiply
: factor '*' unary {
  LOG_DEBUG("multiply : factor '*' unary");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_MULTIPLY;
  $$->factor = $1;
  $$->unary = $3;
}
;

divide
: factor '/' unary {
  LOG_DEBUG("divide : factor '/' unary");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_DIVIDE;
  $$->factor = $1;
  $$->unary = $3;
}
;

modulo
: factor '%' unary {
  LOG_DEBUG("divide : factor '%' unary");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_MODULO;
  $$->factor = $1;
  $$->unary = $3;
}
;

unary
: primary {
  LOG_DEBUG("primary");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = $1;
}
| '+' unary {
  LOG_DEBUG("unary : '+' unary");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = $2;
}
| minus {
  LOG_DEBUG("unary : minus");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = $1;
}
| negate {
  LOG_DEBUG("unary : negate");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = $1;
}
;

minus
: '-' unary {
  LOG_DEBUG("minus : '-' unary");
  $$ = xmalloc(sizeof(SymbolMinus));
  $$->type = SYMBOL_TYPE_MINUS;
  $$->unary = $2;
}
;

negate
: '!' unary {
  LOG_DEBUG("negate : '!' unary");
  $$ = xmalloc(sizeof(SymbolNegate));
  $$->type = SYMBOL_TYPE_NEGATE;
  $$->unary = $2;
}
;

primary
: atom {
  LOG_DEBUG("atom");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = $1;
}
| fncall {
  LOG_DEBUG("primary : fncall");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = $1;
}
| subscription {
  LOG_DEBUG("primary : subscription");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = $1;
}
| slice {
  LOG_DEBUG("primary : slice");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = $1;
}
;

// TODO: Rename to func_call
fncall : primary '(' ')' {
  LOG_DEBUG("fncall : primary '(' ')'");
  $$ = xmalloc(sizeof(SymbolFncall));
  $$->type = SYMBOL_TYPE_FNCALL;
  $$->primary = $1;
  $$->arguments = NULL;
}
| primary '(' arguments ')' {
  LOG_DEBUG("fncall : primary '(' arguments ')'");
  $$ = xmalloc(sizeof(SymbolFncall));
  $$->type = SYMBOL_TYPE_FNCALL;
  $$->primary = $1;
  $$->arguments = $3;
}
;

arguments
: expr {
  LOG_DEBUG("arglist : expr");
  $$ = xmalloc(sizeof(SymbolArguments));
  $$->type = SYMBOL_TYPE_ARGUMENTS;
  $$->arguments = NULL;
  $$->expr = $1;
}
| arguments ',' expr {
  LOG_DEBUG("arglist : arglist ',' expr");
  $$ = xmalloc(sizeof(SymbolArguments));
  $$->type = SYMBOL_TYPE_ARGUMENTS;
  $$->arguments = $1;
  $$->expr = $3;
}
;

subscription
: primary '[' expr ']' {
  LOG_DEBUG("subscription : primary '[' expr ']'");
  $$ = xmalloc(sizeof(SymbolSubscription));
  $$->type = SYMBOL_TYPE_SUBSCRIPTION;
  $$->primary = $1;
  $$->expr = $3;
}
;

slice
: primary '[' expr ':' expr ']' {
  LOG_DEBUG("slice : primary '[' expr ':' expr ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expr = $3;
  $$->right_expr = $5;
}
| primary '[' expr ':' ']' {
  LOG_DEBUG("slice : primary '[' expr ':' ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expr = $3;
  $$->right_expr = NULL;
}
| primary '[' ':' expr ']' {
  LOG_DEBUG("slice : primary '[' ':' expr ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expr = NULL;
  $$->right_expr = $4;
}
| primary '[' ':' ']' {
  LOG_DEBUG("slice : primary '[' ':' ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expr = NULL;
  $$->right_expr = NULL;
}
;

inner_expr
: '(' expr ')' {
  LOG_DEBUG("inner_expr : '(' expr ')'");
  $$ = xmalloc(sizeof(SymbolExpr));
  $$->type = SYMBOL_TYPE_EXPR;
  $$->symbol = $2;
}
;

atom
: IDENTIFIER {
  LOG_DEBUG("atom : IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_IDENTIFIER;
  $$->symbol = $1;
}
| INTEGER_LITERAL {
  LOG_DEBUG("atom : INTEGER_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_INTEGER_LITERAL;
  $$->symbol = $1;
}
| FLOAT_LITERAL {
  LOG_DEBUG("atom : FLOAT_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_FLOAT_LITERAL;
  $$->symbol = $1;
}
| STRING_LITERAL {
  LOG_DEBUG("atom : STRING_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_STRING_LITERAL;
  $$->symbol = $1;
}
| BOOLEAN_LITERAL {
  LOG_DEBUG("atom : BOOLEAN_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_BOOLEAN_LITERAL;
  $$->symbol = $1;
}
| NONE_LITERAL {
  LOG_DEBUG("atom : NONE_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_NONE_LITERAL;
  $$->symbol = $1;
}
| inner_expr {
  LOG_DEBUG("atom : inner_expr");
  $$->symbol = $1;
}
;

%%

void yyerror(char *msg) {
  LOG_ERROR(msg);
  exit(1);
}
