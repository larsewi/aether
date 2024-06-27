%{
#include "parser_state.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils/logger.h"
#include "../utils/alloc.h"

#define P PARSER_STATE

extern int yylex();
void yyerror(char *msg);
%}

%union {
  Identifier *identifier;
  IntegerLiteral *integer_literal;
  FloatLiteral *float_literal;
  StringLiteral *string_literal;
  BooleanLiteral *boolean_literal;
  NoneLiteral *none_literal;
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
  Expr *expr;
  LP3 *lp3;
  Or *or;
  LP2 *lp2;
  And *and;
  LP1 *lp1;
  Not *not;
  Comp *comp;
  LessThan *less_than;
  GreaterThan *greater_than;
  Equal *equal;
  LessEqual *less_equal;
  GreaterEqual *greater_equal;
  NotEqual *not_equal;
  Term *term;
  Add *add;
  Subtract *subtract;
  Factor *factor;
  Multiply *multiply;
  Divide *divide;
  Modulo *modulo;
  Unary *unary;
  Minus *minus;
  Primary *primary;
  Fncall *fncall;
  Arguments *arguments;
  Subscription *subscription;
  Slice *slice;
  Atom *atom;
}

%type <expr> inner_expr expr;
%type <lp1> lp1;
%type <or> or;
%type <lp2> lp2;
%type <and> and;
%type <lp3> lp3;
%type <not> not;
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
}
| expr {
  LOG_DEBUG("start : expr");
}
;

expr
: lp1 {
  LOG_DEBUG("expr : lp1");
  $$ = xmalloc(sizeof(Expr));
  $$->type = SYMBOL_TYPE_EXPR;
  $$->value = $1;
}
;

lp1
: lp2 {
  LOG_DEBUG("lp1 : lp2");
  $$ = xmalloc(sizeof(LP1));
  $$->type = SYMBOL_TYPE_LP1;
  $$->symbol = $1;
}
| or {
  LOG_DEBUG("lp1 : or");
  $$ = xmalloc(sizeof(LP1));
  $$->type = SYMBOL_TYPE_LP1;
  $$->symbol = $1;
}
;

or
: lp1 OR_OPER lp2 {
  LOG_DEBUG("or : lp1 OR_OPER lp2");
  $$ = xmalloc(sizeof(Or));
  $$->type = SYMBOL_TYPE_OR;
  $$->lp1 = $1;
  $$->lp2 = $3;
}
;

lp2
: lp3 {
  LOG_DEBUG("lp2 : lp3");
  $$ = xmalloc(sizeof(LP2));
  $$->type = SYMBOL_TYPE_LP2;
  $$->symbol = $1;
}
| and {
  LOG_DEBUG("lp2 : and");
  $$ = xmalloc(sizeof(LP2));
  $$->type = SYMBOL_TYPE_LP2;
  $$->symbol = $1;
}
;

and
: lp2 AND_OPER lp3 {
  LOG_DEBUG("and : lp2 AND_OPER lp3");
  $$ = xmalloc(sizeof(And));
  $$->type = SYMBOL_TYPE_AND;
  $$->lp2 = $1;
  $$->lp3 = $3;
}
;

lp3
: comp {
  LOG_DEBUG("lp3 : comp");
  $$ = xmalloc(sizeof(LP3));
  $$->type = SYMBOL_TYPE_LP3;
  $$->symbol = $1;
}
| not {
  LOG_DEBUG("lp3 : not");
  $$ = xmalloc(sizeof(LP3));
  $$->type = SYMBOL_TYPE_LP3;
  $$->symbol = $1;
}
;

not
: '!' lp3 {
  LOG_DEBUG("not : '!' lp3");
  $$ = xmalloc(sizeof(Not));
  $$->type = SYMBOL_TYPE_NOT;
  $$->lp3 = $2;
}
;

comp
: term {
  LOG_DEBUG("comp : term");
  $$ = xmalloc(sizeof(Comp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| less_than {
  LOG_DEBUG("comp : less_than");
  $$ = xmalloc(sizeof(Comp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| greater_than {
  LOG_DEBUG("comp : greater_than");
  $$ = xmalloc(sizeof(Comp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| equal {
  LOG_DEBUG("comp : equal");
  $$ = xmalloc(sizeof(Comp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| less_equal {
  LOG_DEBUG("comp : less_equal");
  $$ = xmalloc(sizeof(Comp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| greater_equal {
  LOG_DEBUG("comp : greater_equal");
  $$ = xmalloc(sizeof(Comp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
| not_equal {
  LOG_DEBUG("comp : not_equal");
  $$ = xmalloc(sizeof(Comp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = $1;
}
;

less_than
: comp '<' term {
  LOG_DEBUG("less_than : comp '<' term");
  $$ = xmalloc(sizeof(LessThan));
  $$->type = SYMBOL_TYPE_LESS_THAN;
  $$->comp = $1;
  $$->term = $3;
}
;

greater_than
: comp '>' term {
  LOG_DEBUG("greater_than : comp '>' term");
  $$ = xmalloc(sizeof(GreaterThan));
  $$->type = SYMBOL_TYPE_GREATER_THAN;
  $$->comp = $1;
  $$->term = $3;
}
;

equal
: comp EQ_OPER term {
  LOG_DEBUG("equal : comp EQ_OPER term");
  $$ = xmalloc(sizeof(Equal));
  $$->type = SYMBOL_TYPE_EQUAL;
  $$->comp = $1;
  $$->term = $3;
}
;

less_equal
: comp LE_OPER term {
  LOG_DEBUG("less_equal : comp LE_OPER term");
  $$ = xmalloc(sizeof(LessEqual));
  $$->type = SYMBOL_TYPE_LESS_EQUAL;
  $$->comp = $1;
  $$->term = $3;
}
;

greater_equal
: comp GE_OPER term {
  LOG_DEBUG("greater_equal : comp GE_OPER term");
  $$ = xmalloc(sizeof(GreaterEqual));
  $$->type = SYMBOL_TYPE_GREATER_EQUAL;
  $$->comp = $1;
  $$->term = $3;
}
;

not_equal
: comp NE_OPER term {
  LOG_DEBUG("not_equal : comp NE_OPER term");
  $$ = xmalloc(sizeof(NotEqual));
  $$->type = SYMBOL_TYPE_NOT_EQUAL;
  $$->comp = $1;
  $$->term = $3;
}
;

term
: factor {
  LOG_DEBUG("term : factor");
  $$ = xmalloc(sizeof(Term));
  $$->type = SYMBOL_TYPE_TERM;
  $$->symbol = $1;
}
| add {
  LOG_DEBUG("term : add");
  $$ = xmalloc(sizeof(Term));
  $$->type = SYMBOL_TYPE_TERM;
  $$->symbol = $1;
}
| subtract {
  LOG_DEBUG("term : subtract");
  $$ = xmalloc(sizeof(Term));
  $$->type = SYMBOL_TYPE_TERM;
  $$->symbol = $1;
}
;

add
: term '+' factor {
  LOG_DEBUG("add : term '+' factor");
  $$ = xmalloc(sizeof(Add));
  $$->type = SYMBOL_TYPE_ADD;
  $$->term = $1;
  $$->factor = $3;
}
;

subtract
: term '-' factor {
  LOG_DEBUG("subtract : term '-' factor");
  $$ = xmalloc(sizeof(Subtract));
  $$->type = SYMBOL_TYPE_SUBTRACT;
  $$->term = $1;
  $$->factor = $3;
}
;

factor
: unary {
  LOG_DEBUG("factor : unary");
  $$ = xmalloc(sizeof(Factor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->symbol = $1;
}
| multiply {
  LOG_DEBUG("factor : multiply");
  $$ = xmalloc(sizeof(Factor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->symbol = $1;
}
| divide {
  LOG_DEBUG("factor : divide");
  $$ = xmalloc(sizeof(Factor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->symbol = $1;
}
| modulo {
  LOG_DEBUG("factor : modulo");
  $$ = xmalloc(sizeof(Factor));
  $$->type = SYMBOL_TYPE_MODULO;
  $$->symbol = $1;
}
;

multiply
: factor '*' unary {
  LOG_DEBUG("multiply : factor '*' unary");
  $$ = xmalloc(sizeof(Factor));
  $$->type = SYMBOL_TYPE_MULTIPLY;
  $$->factor = $1;
  $$->unary = $3;
}
;

divide
: factor '/' unary {
  LOG_DEBUG("divide : factor '/' unary");
  $$ = xmalloc(sizeof(Factor));
  $$->type = SYMBOL_TYPE_DIVIDE;
  $$->factor = $1;
  $$->unary = $3;
}
;

modulo
: factor '%' unary {
  LOG_DEBUG("divide : factor '%' unary");
  $$ = xmalloc(sizeof(Factor));
  $$->type = SYMBOL_TYPE_MODULO;
  $$->factor = $1;
  $$->unary = $3;
}
;

unary
: primary {
  LOG_DEBUG("primary");
  $$ = xmalloc(sizeof(Unary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = $1;
}
| '+' unary {
  LOG_DEBUG("unary : '+' unary");
  $$ = xmalloc(sizeof(Unary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = $2;
}
| minus {
  LOG_DEBUG("unary : minus");
  $$ = xmalloc(sizeof(Unary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = $1;
}
;

minus
: '-' unary {
  LOG_DEBUG("minus : '-' unary");
  $$ = xmalloc(sizeof(Minus));
  $$->type = SYMBOL_TYPE_MINUS;
  $$->unary = $2;
}
;

primary
: atom {
  LOG_DEBUG("atom");
  $$ = xmalloc(sizeof(Primary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = $1;
}
| fncall {
  LOG_DEBUG("primary : fncall");
  $$ = xmalloc(sizeof(Primary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = $1;
}
| subscription {
  LOG_DEBUG("primary : subscription");
  $$ = xmalloc(sizeof(Primary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = $1;
}
| slice {
  LOG_DEBUG("primary : slice");
  $$ = xmalloc(sizeof(Primary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = $1;
}
;

// TODO: Rename to func_call
fncall : primary '(' ')' {
  LOG_DEBUG("fncall : primary '(' ')'");
  $$ = xmalloc(sizeof(Fncall));
  $$->type = SYMBOL_TYPE_FNCALL;
  $$->primary = $1;
  $$->arguments = NULL;
}
| primary '(' arguments ')' {
  LOG_DEBUG("fncall : primary '(' arguments ')'");
  $$ = xmalloc(sizeof(Fncall));
  $$->type = SYMBOL_TYPE_FNCALL;
  $$->primary = $1;
  $$->arguments = $3;
}
;

arguments
: expr {
  LOG_DEBUG("arglist : expr");
  $$ = xmalloc(sizeof(Arguments));
  $$->type = SYMBOL_TYPE_ARGUMENTS;
  $$->arguments = NULL;
  $$->expr = $1;
}
| arguments ',' expr {
  LOG_DEBUG("arglist : arglist ',' expr");
  $$ = xmalloc(sizeof(Arguments));
  $$->type = SYMBOL_TYPE_ARGUMENTS;
  $$->arguments = $1;
  $$->expr = $3;
}
;

subscription
: primary '[' expr ']' {
  LOG_DEBUG("subscription : primary '[' expr ']'");
  $$ = xmalloc(sizeof(Subscription));
  $$->type = SYMBOL_TYPE_SUBSCRIPTION;
  $$->primary = $1;
  $$->expr = $3;
}
;

slice
: primary '[' expr ':' expr ']' {
  LOG_DEBUG("slice : primary '[' expr ':' expr ']'");
  $$ = xmalloc(sizeof(Slice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expr = $3;
  $$->right_expr = $5;
}
| primary '[' expr ':' ']' {
  LOG_DEBUG("slice : primary '[' expr ':' ']'");
  $$ = xmalloc(sizeof(Slice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expr = $3;
  $$->right_expr = NULL;
}
| primary '[' ':' expr ']' {
  LOG_DEBUG("slice : primary '[' ':' expr ']'");
  $$ = xmalloc(sizeof(Slice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expr = NULL;
  $$->right_expr = $4;
}
| primary '[' ':' ']' {
  LOG_DEBUG("slice : primary '[' ':' ']'");
  $$ = xmalloc(sizeof(Slice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expr = NULL;
  $$->right_expr = NULL;
}
;

inner_expr
: '(' expr ')' {
  LOG_DEBUG("inner_expr : '(' expr ')'");
  $$ = xmalloc(sizeof(Expr));
  $$->type = SYMBOL_TYPE_EXPR;
  $$->value = $2;
}
;

atom
: IDENTIFIER {
  LOG_DEBUG("atom : IDENTIFIER");
  $$ = xmalloc(sizeof(Atom));
  $$->type = SYMBOL_TYPE_IDENTIFIER;
  $$->symbol = $1;
}
| INTEGER_LITERAL {
  LOG_DEBUG("atom : INTEGER_LITERAL");
  $$ = xmalloc(sizeof(Atom));
  $$->type = SYMBOL_TYPE_INTEGER_LITERAL;
  $$->symbol = $1;
}
| FLOAT_LITERAL {
  LOG_DEBUG("atom : FLOAT_LITERAL");
  $$ = xmalloc(sizeof(Atom));
  $$->type = SYMBOL_TYPE_FLOAT_LITERAL;
  $$->symbol = $1;
}
| STRING_LITERAL {
  LOG_DEBUG("atom : STRING_LITERAL");
  $$ = xmalloc(sizeof(Atom));
  $$->type = SYMBOL_TYPE_STRING_LITERAL;
  $$->symbol = $1;
}
| BOOLEAN_LITERAL {
  LOG_DEBUG("atom : BOOLEAN_LITERAL");
  $$ = xmalloc(sizeof(Atom));
  $$->type = SYMBOL_TYPE_BOOLEAN_LITERAL;
  $$->symbol = $1;
}
| NONE_LITERAL {
  LOG_DEBUG("atom : NONE_LITERAL");
  $$ = xmalloc(sizeof(Atom));
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
