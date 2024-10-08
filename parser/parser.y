%{
#include "syntax.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/logger.h"
#include "../utils/alloc.h"

#define P PARSER_STATE

extern FILE *yyin;
extern void yylex_destroy();
extern int yylex();

void yyerror(char *msg);

ParserState PARSER_STATE = {0};

int yydebug = 1;
%}

%locations

// Terminals

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
%token MUTABLE_KEYWORD
%token EQ_OPER LE_OPER GE_OPER NE_OPER
%token AND_OPER OR_OPER

// Expression

%union {
  SymbolExpression *expression;
  SymbolOr *or;
  SymbolCondition *condition;
  SymbolAnd *and;
  SymbolComparison *comparison;
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
  SymbolDict *dict;
  SymbolEntries *entries;
  SymbolList *list;
  SymbolElements *elements;
  SymbolArguments *arguments;
  SymbolSubscription *subscription;
  SymbolSlice *slice;
  SymbolAtom *atom;
}

%type <expression> inner_expression expression;
%type <or> or;
%type <condition> condition;
%type <and> and;
%type <comparison> comparison;
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
%type <dict> dict;
%type <entries> entries;
%type <list> list;
%type <elements> elements;
%type <subscription> subscription;
%type <slice> slice;
%type <atom> atom;

// Statements

%union {
  SymbolStatement *statement;
  SymbolAssignment *assignment;
  SymbolDeclaration *declaration;
  SymbolReference *reference;
  SymbolMutable *mutable;
  SymbolDatatype *datatype;
}

%type <statement> statement;
%type <assignment> assignment;
%type <declaration> declaration;
%type <reference> reference;
%type <mutable> mutable;
%type <datatype> datatype;

%%

start
: /* empty */ {
  LOG_DEBUG("start : %%empty");
  P.statement = NULL;
}
| statement {
  LOG_DEBUG("start : statement");
  P.statement = $1;
}
;

statement
: assignment ';' {
  LOG_DEBUG("statement : assignment ';'");
  $$ = xmalloc(sizeof(SymbolStatement));
  $$->type = SYMBOL_TYPE_STATEMENT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->symbol = (Symbol *)$1;
}
| declaration ';' {
  LOG_DEBUG("statement : declaration ';'");
  $$ = xmalloc(sizeof(SymbolStatement));
  $$->type = SYMBOL_TYPE_STATEMENT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->symbol = (Symbol *)$1;
}
| expression ';' {
  LOG_DEBUG("statement : expression ';'");
  $$ = xmalloc(sizeof(SymbolStatement));
  $$->type = SYMBOL_TYPE_STATEMENT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->symbol = (Symbol *)$1;
}
;

assignment
: expression '=' expression {
  LOG_DEBUG("assignment : expression '=' expression");
  $$ = xmalloc(sizeof(SymbolAssignment));
  $$->type = SYMBOL_TYPE_ASSIGNMENT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->symbol = (Symbol *)$1;
  $$->expression = $3;
}
| declaration '=' expression {
  LOG_DEBUG("assignment : declaration '=' expression");
  $$ = xmalloc(sizeof(SymbolAssignment));
  $$->type = SYMBOL_TYPE_ASSIGNMENT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->symbol = (Symbol *)$1;
  $$->expression = $3;
}
;

declaration
: reference IDENTIFIER {
  LOG_DEBUG("declaration : reference IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolDeclaration));
  $$->type = SYMBOL_TYPE_DECLARATION;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->identifier = $2;
  $$->symbol = (Symbol *)$1;
}
| mutable IDENTIFIER {
  LOG_DEBUG("declaration : mutable IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolDeclaration));
  $$->type = SYMBOL_TYPE_DECLARATION;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->symbol = (Symbol *)$1;
  $$->identifier = $2;
}
| datatype IDENTIFIER {
  LOG_DEBUG("declaration : datatype IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolDeclaration));
  $$->type = SYMBOL_TYPE_DECLARATION;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->symbol = (Symbol *)$1;
  $$->identifier = $2;
}
;

reference
: datatype '&' {
  LOG_DEBUG("reference : datatype '&'");
  $$ = xmalloc(sizeof(SymbolReference));
  $$->type = SYMBOL_TYPE_REFERENCE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->symbol = (Symbol *)$1;
}
| mutable '&' {
  LOG_DEBUG("reference : mutable '&'");
  $$ = xmalloc(sizeof(SymbolReference));
  $$->type = SYMBOL_TYPE_REFERENCE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->symbol = (Symbol *)$1;
}
;

mutable
: MUTABLE_KEYWORD datatype {
  LOG_DEBUG("mutable : MUTABLE_KEYWORD datatype");
  $$ = xmalloc(sizeof(SymbolMutable));
  $$->type = SYMBOL_TYPE_MUTABLE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->datatype = $2;
}
;

datatype
: IDENTIFIER {
  LOG_DEBUG("datatype : IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolDatatype));
  $$->type = SYMBOL_TYPE_DATATYPE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->identifier = $1;
}
;

expression
: condition {
  LOG_DEBUG("expression : condition");
  $$ = xmalloc(sizeof(SymbolExpression));
  $$->type = SYMBOL_TYPE_EXPRESSION;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| or {
  LOG_DEBUG("expression : or");
  $$ = xmalloc(sizeof(SymbolExpression));
  $$->type = SYMBOL_TYPE_EXPRESSION;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
;

or
: expression OR_OPER condition {
  LOG_DEBUG("or : expression OR_OPER condition");
  $$ = xmalloc(sizeof(SymbolOr));
  $$->type = SYMBOL_TYPE_OR;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->expression = $1;
  $$->condition = $3;
}
;

condition
: comparison {
  LOG_DEBUG("condition : comparison");
  $$ = xmalloc(sizeof(SymbolCondition));
  $$->type = SYMBOL_TYPE_CONDITION;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| and {
  LOG_DEBUG("condition : and");
  $$ = xmalloc(sizeof(SymbolCondition));
  $$->type = SYMBOL_TYPE_CONDITION;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
;

and
: condition AND_OPER comparison {
  LOG_DEBUG("and : condition AND_OPER comparison");
  $$ = xmalloc(sizeof(SymbolAnd));
  $$->type = SYMBOL_TYPE_AND;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->condition = $1;
  $$->comparison = $3;
}
;

comparison
: term {
  LOG_DEBUG("comparison : term");
  $$ = xmalloc(sizeof(SymbolComparison));
  $$->type = SYMBOL_TYPE_COMPARISON;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| less_than {
  LOG_DEBUG("comparison : less_than");
  $$ = xmalloc(sizeof(SymbolComparison));
  $$->type = SYMBOL_TYPE_COMPARISON;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| greater_than {
  LOG_DEBUG("comparison : greater_than");
  $$ = xmalloc(sizeof(SymbolComparison));
  $$->type = SYMBOL_TYPE_COMPARISON;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| equal {
  LOG_DEBUG("comparison : equal");
  $$ = xmalloc(sizeof(SymbolComparison));
  $$->type = SYMBOL_TYPE_COMPARISON;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| less_equal {
  LOG_DEBUG("comparison : less_equal");
  $$ = xmalloc(sizeof(SymbolComparison));
  $$->type = SYMBOL_TYPE_COMPARISON;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| greater_equal {
  LOG_DEBUG("comparison : greater_equal");
  $$ = xmalloc(sizeof(SymbolComparison));
  $$->type = SYMBOL_TYPE_COMPARISON;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| not_equal {
  LOG_DEBUG("comparison : not_equal");
  $$ = xmalloc(sizeof(SymbolComparison));
  $$->type = SYMBOL_TYPE_COMPARISON;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
;

less_than
: comparison '<' term {
  LOG_DEBUG("less_than : comparison '<' term");
  $$ = xmalloc(sizeof(SymbolLessThan));
  $$->type = SYMBOL_TYPE_LESS_THAN;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->comparison = $1;
  $$->term = $3;
}
;

greater_than
: comparison '>' term {
  LOG_DEBUG("greater_than : comparison '>' term");
  $$ = xmalloc(sizeof(SymbolGreaterThan));
  $$->type = SYMBOL_TYPE_GREATER_THAN;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->comparison = $1;
  $$->term = $3;
}
;

equal
: comparison EQ_OPER term {
  LOG_DEBUG("equal : comparison EQ_OPER term");
  $$ = xmalloc(sizeof(SymbolEqual));
  $$->type = SYMBOL_TYPE_EQUAL;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->comparison = $1;
  $$->term = $3;
}
;

less_equal
: comparison LE_OPER term {
  LOG_DEBUG("less_equal : comparison LE_OPER term");
  $$ = xmalloc(sizeof(SymbolLessEqual));
  $$->type = SYMBOL_TYPE_LESS_EQUAL;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->comparison = $1;
  $$->term = $3;
}
;

greater_equal
: comparison GE_OPER term {
  LOG_DEBUG("greater_equal : comparison GE_OPER term");
  $$ = xmalloc(sizeof(SymbolGreaterEqual));
  $$->type = SYMBOL_TYPE_GREATER_EQUAL;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->comparison = $1;
  $$->term = $3;
}
;

not_equal
: comparison NE_OPER term {
  LOG_DEBUG("not_equal : comparison NE_OPER term");
  $$ = xmalloc(sizeof(SymbolNotEqual));
  $$->type = SYMBOL_TYPE_NOT_EQUAL;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->comparison = $1;
  $$->term = $3;
}
;

term
: factor {
  LOG_DEBUG("term : factor");
  $$ = xmalloc(sizeof(SymbolTerm));
  $$->type = SYMBOL_TYPE_TERM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| add {
  LOG_DEBUG("term : add");
  $$ = xmalloc(sizeof(SymbolTerm));
  $$->type = SYMBOL_TYPE_TERM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| subtract {
  LOG_DEBUG("term : subtract");
  $$ = xmalloc(sizeof(SymbolTerm));
  $$->type = SYMBOL_TYPE_TERM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
;

add
: term '+' factor {
  LOG_DEBUG("add : term '+' factor");
  $$ = xmalloc(sizeof(SymbolAdd));
  $$->type = SYMBOL_TYPE_ADD;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->term = $1;
  $$->factor = $3;
}
;

subtract
: term '-' factor {
  LOG_DEBUG("subtract : term '-' factor");
  $$ = xmalloc(sizeof(SymbolSubtract));
  $$->type = SYMBOL_TYPE_SUBTRACT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->term = $1;
  $$->factor = $3;
}
;

factor
: unary {
  LOG_DEBUG("factor : unary");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| multiply {
  LOG_DEBUG("factor : multiply");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| divide {
  LOG_DEBUG("factor : divide");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| modulo {
  LOG_DEBUG("factor : modulo");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_MODULO;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
;

multiply
: factor '*' unary {
  LOG_DEBUG("multiply : factor '*' unary");
  $$ = xmalloc(sizeof(SymbolMultiply));
  $$->type = SYMBOL_TYPE_MULTIPLY;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->factor = $1;
  $$->unary = $3;
}
;

divide
: factor '/' unary {
  LOG_DEBUG("divide : factor '/' unary");
  $$ = xmalloc(sizeof(SymbolDivide));
  $$->type = SYMBOL_TYPE_DIVIDE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->factor = $1;
  $$->unary = $3;
}
;

modulo
: factor '%' unary {
  LOG_DEBUG("divide : factor '%' unary");
  $$ = xmalloc(sizeof(SymbolModulo));
  $$->type = SYMBOL_TYPE_MODULO;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->factor = $1;
  $$->unary = $3;
}
;

unary
: primary {
  LOG_DEBUG("primary");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| '+' unary {
  LOG_DEBUG("unary : '+' unary");
  // Ignore '+' token
  $$ = $2;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
}
| minus {
  LOG_DEBUG("unary : minus");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| negate {
  LOG_DEBUG("unary : negate");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
;

minus
: '-' unary {
  LOG_DEBUG("minus : '-' unary");
  $$ = xmalloc(sizeof(SymbolMinus));
  $$->type = SYMBOL_TYPE_MINUS;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->unary = $2;
}
;

negate
: '!' unary {
  LOG_DEBUG("negate : '!' unary");
  $$ = xmalloc(sizeof(SymbolNegate));
  $$->type = SYMBOL_TYPE_NEGATE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->unary = $2;
}
;

primary
: atom {
  LOG_DEBUG("primary : atom");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| fncall {
  LOG_DEBUG("primary : fncall");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| subscription {
  LOG_DEBUG("primary : subscription");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| slice {
  LOG_DEBUG("primary : slice");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
;

fncall : primary '(' ')' {
  LOG_DEBUG("fncall : primary '(' ')'");
  $$ = xmalloc(sizeof(SymbolFncall));
  $$->type = SYMBOL_TYPE_FNCALL;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->primary = $1;
  $$->arguments = NULL;
}
| primary '(' arguments ')' {
  LOG_DEBUG("fncall : primary '(' arguments ')'");
  $$ = xmalloc(sizeof(SymbolFncall));
  $$->type = SYMBOL_TYPE_FNCALL;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @4.last_line;
  $$->last.column = @4.last_column;
  $$->primary = $1;
  $$->arguments = $3;
}
;

dict
: '{' '}' {
  LOG_DEBUG("dict : '{' '}'");
  $$ = xmalloc(sizeof(SymbolDict));
  $$->type = SYMBOL_TYPE_DICT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->entries = NULL;
}
| '{' entries '}' {
  LOG_DEBUG("dict : '{' entries '}'");
  $$ = xmalloc(sizeof(SymbolDict));
  $$->type = SYMBOL_TYPE_DICT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->entries = $2;
}
| '{' entries ',' '}' {
  LOG_DEBUG("dict : '{' entries ',' '}'");
  $$ = xmalloc(sizeof(SymbolDict));
  $$->type = SYMBOL_TYPE_DICT;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @4.last_line;
  $$->last.column = @4.last_column;
  $$->entries = $2;
}
;

entries
: STRING_LITERAL ':' expression {
  LOG_DEBUG("entries : STRING_LITERAL ':' expression");
  $$ = xmalloc(sizeof(SymbolEntries));
  $$->type = SYMBOL_TYPE_ENTRIES;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->entries = NULL;
  $$->string_literal = $1;
  $$->expression = $3;
}
| entries ',' STRING_LITERAL ':' expression {
  LOG_DEBUG("entries : entries ',' STRING_LITERAL ':' expression");
  $$ = xmalloc(sizeof(SymbolEntries));
  $$->type = SYMBOL_TYPE_ENTRIES;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @5.last_line;
  $$->last.column = @5.last_column;
  $$->entries = $1;
  $$->string_literal = $3;
  $$->expression = $5;
}
;

list
: '[' ']' {
  LOG_DEBUG("list : '[' ']'");
  $$ = xmalloc(sizeof(SymbolList));
  $$->type = SYMBOL_TYPE_LIST;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @2.last_line;
  $$->last.column = @2.last_column;
  $$->elements = NULL;
}
| '[' elements ']' {
  LOG_DEBUG("list : '[' elements ']'");
  $$ = xmalloc(sizeof(SymbolList));
  $$->type = SYMBOL_TYPE_LIST;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->elements = $2;
}
| '[' elements ',' ']' {
  LOG_DEBUG("list : '[' elements ',' ']'");
  $$ = xmalloc(sizeof(SymbolList));
  $$->type = SYMBOL_TYPE_LIST;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @4.last_line;
  $$->last.column = @4.last_column;
  $$->elements = $2;
}
;

elements
: expression {
  LOG_DEBUG("elements : expression");
  $$ = xmalloc(sizeof(SymbolElements));
  $$->type = SYMBOL_TYPE_ELEMENTS;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->elements = NULL;
  $$->expression = $1;
}
| elements ',' expression {
  LOG_DEBUG("elements : elements ',' expression");
  $$ = xmalloc(sizeof(SymbolElements));
  $$->type = SYMBOL_TYPE_ELEMENTS;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->elements = $1;
  $$->expression = $3;
}
;

arguments
: expression {
  LOG_DEBUG("arguments : expression");
  $$ = xmalloc(sizeof(SymbolArguments));
  $$->type = SYMBOL_TYPE_ARGUMENTS;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->arguments = NULL;
  $$->expression = $1;
}
| arguments ',' expression {
  LOG_DEBUG("arguments : arguments ',' expression");
  $$ = xmalloc(sizeof(SymbolArguments));
  $$->type = SYMBOL_TYPE_ARGUMENTS;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
  $$->arguments = $1;
  $$->expression = $3;
}
;

subscription
: primary '[' expression ']' {
  LOG_DEBUG("subscription : primary '[' expression ']'");
  $$ = xmalloc(sizeof(SymbolSubscription));
  $$->type = SYMBOL_TYPE_SUBSCRIPTION;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @4.last_line;
  $$->last.column = @4.last_column;
  $$->primary = $1;
  $$->expression = $3;
}
;

slice
: primary '[' expression ':' expression ']' {
  LOG_DEBUG("slice : primary '[' expression ':' expression ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @6.last_line;
  $$->last.column = @6.last_column;
  $$->primary = $1;
  $$->left_expression = $3;
  $$->right_expression = $5;
}
| primary '[' expression ':' ']' {
  LOG_DEBUG("slice : primary '[' expression ':' ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @5.last_line;
  $$->last.column = @5.last_column;
  $$->primary = $1;
  $$->left_expression = $3;
  $$->right_expression = NULL;
}
| primary '[' ':' expression ']' {
  LOG_DEBUG("slice : primary '[' ':' expression ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @5.last_line;
  $$->last.column = @5.last_column;
  $$->primary = $1;
  $$->left_expression = NULL;
  $$->right_expression = $4;
}
| primary '[' ':' ']' {
  LOG_DEBUG("slice : primary '[' ':' ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @4.last_line;
  $$->last.column = @4.last_column;
  $$->primary = $1;
  $$->left_expression = NULL;
  $$->right_expression = NULL;
}
;

inner_expression
: '(' expression ')' {
  LOG_DEBUG("inner_expression : '(' expression ')'");
  $$ = $2;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @3.last_line;
  $$->last.column = @3.last_column;
}
;

atom
: IDENTIFIER {
  LOG_DEBUG("atom : IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| INTEGER_LITERAL {
  LOG_DEBUG("atom : INTEGER_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| FLOAT_LITERAL {
  LOG_DEBUG("atom : FLOAT_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| STRING_LITERAL {
  LOG_DEBUG("atom : STRING_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| BOOLEAN_LITERAL {
  LOG_DEBUG("atom : BOOLEAN_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| NONE_LITERAL {
  LOG_DEBUG("atom : NONE_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| dict {
  LOG_DEBUG("atom : dict");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| list {
  LOG_DEBUG("atom : list");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
| inner_expression {
  LOG_DEBUG("atom : inner_expression");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->first.line = @1.first_line;
  $$->first.column = @1.first_column;
  $$->last.line = @1.last_line;
  $$->last.column = @1.last_column;
  $$->symbol = (Symbol *)$1;
}
;

%%

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

void yyerror(char *msg) {
  LOG_ERROR(msg);
  exit(1);
}
