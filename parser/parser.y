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
%}

// Terminals

%union {
  SymbolIdentifier *identifier;
  SymbolIntegerLiteral *integer_literal;
  SymbolFloatLiteral *float_literal;
  SymbolStringLiteral *string_literal;
  SymbolBooleanLiteral *boolean_literal;
  SymbolNoneLiteral *none_literal;
  SymbolMutableSpecifier *mutable_specifier;
}

%token <identifier> IDENTIFIER
%token <integer_literal> INTEGER_LITERAL
%token <float_literal> FLOAT_LITERAL
%token <string_literal> STRING_LITERAL
%token <boolean_literal> BOOLEAN_LITERAL
%token <none_literal> NONE_LITERAL
%token <mutable_specifier> MUTABLE_SPECIFIER
%token EQ_OPER LE_OPER GE_OPER NE_OPER
%token AND_OPER OR_OPER

// Expression

%union {
  SymbolExpression *expression;
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
  SymbolDictDisplay *dict_display;
  SymbolKeyValuePairs *key_value_pairs;
  SymbolListDisplay *list_display;
  SymbolListElements *list_elements;
  SymbolArguments *arguments;
  SymbolSubscription *subscription;
  SymbolSlice *slice;
  SymbolAtom *atom;
}

%type <expression> inner_expression expression;
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
%type <dict_display> dict_display;
%type <key_value_pairs> key_value_pairs;
%type <list_display> list_display;
%type <list_elements> list_elements;
%type <subscription> subscription;
%type <slice> slice;
%type <atom> atom;

// Statements

%union {
  SymbolStmt *stmt;
  SymbolAssignment *assignment;
  SymbolVariable *variable;
  SymbolDecl *decl;
  SymbolReference *reference;
  SymbolMutable *mutable;
  SymbolDatatype *datatype;
}

%type <stmt> stmt;
%type <assignment> assignment;
%type <variable> variable;
%type <decl> decl;
%type <reference> reference;
%type <mutable> mutable;
%type <datatype> datatype;

%%

start
: /* empty */ {
  LOG_DEBUG("start : %%empty");
  P.stmt = NULL;
}
| stmt {
  LOG_DEBUG("start : stmt");
  P.stmt = $1;
}
;

stmt
: assignment ';' {
  LOG_DEBUG("stmt : assignment ';'");
  $$ = xmalloc(sizeof(SymbolStmt));
  $$->type = SYMBOL_TYPE_STMT;
  $$->symbol = (Symbol *)$1;
}
| decl ';' {
  LOG_DEBUG("stmt : decl ';'");
  $$ = xmalloc(sizeof(SymbolStmt));
  $$->type = SYMBOL_TYPE_STMT;
  $$->symbol = (Symbol *)$1;
}
| expression ';' {
  LOG_DEBUG("stmt : expression ';'");
  $$ = xmalloc(sizeof(SymbolStmt));
  $$->type = SYMBOL_TYPE_STMT;
  $$->symbol = (Symbol *)$1;
}
;

assignment
: variable '=' expression {
  LOG_DEBUG("assignment : IDENTIFIER '=' expression");
  $$ = xmalloc(sizeof(SymbolAssignment));
  $$->type = SYMBOL_TYPE_ASSIGNMENT;
  $$->symbol = (Symbol *)$1;
  $$->expression = $3;
}
| decl '=' expression {
  LOG_DEBUG("assignment : decl '=' expression");
  $$ = xmalloc(sizeof(SymbolAssignment));
  $$->type = SYMBOL_TYPE_ASSIGNMENT;
  $$->symbol = (Symbol *)$1;
  $$->expression = $3;
}
;

variable
: IDENTIFIER {
  LOG_DEBUG("variable : IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolVariable));
  $$->type = SYMBOL_TYPE_VARIABLE;
  $$->symbol = (Symbol *)$1;
  $$->expression = NULL;
}
| variable '[' expression ']' {
  LOG_DEBUG("variable : variable '[' expression ']'");
  $$ = xmalloc(sizeof(SymbolVariable));
  $$->type = SYMBOL_TYPE_VARIABLE;
  $$->symbol = (Symbol *)$1;
  $$->expression = $3;
}
;

decl
: reference IDENTIFIER {
  LOG_DEBUG("decl : reference IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolDecl));
  $$->type = SYMBOL_TYPE_DECL;
  $$->identifier = $2;
  $$->symbol = (Symbol *)$1;
}
| mutable IDENTIFIER {
  LOG_DEBUG("decl : mutable IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolDecl));
  $$->type = SYMBOL_TYPE_DECL;
  $$->symbol = (Symbol *)$1;
  $$->identifier = $2;
}
| datatype IDENTIFIER {
  LOG_DEBUG("decl : datatype IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolDecl));
  $$->type = SYMBOL_TYPE_DECL;
  $$->symbol = (Symbol *)$1;
  $$->identifier = $2;
}
;

reference
: datatype '&' {
  LOG_DEBUG("reference : datatype '&'");
  $$ = xmalloc(sizeof(SymbolReference));
  $$->type = SYMBOL_TYPE_REFERENCE;
  $$->symbol = (Symbol *)$1;
}
| mutable '&' {
  LOG_DEBUG("reference : mutable '&'");
  $$ = xmalloc(sizeof(SymbolReference));
  $$->type = SYMBOL_TYPE_REFERENCE;
  $$->symbol = (Symbol *)$1;
}
;

mutable
: MUTABLE_SPECIFIER datatype {
  LOG_DEBUG("mutable : MUTABLE_SPECIFIER datatype");
  $$ = xmalloc(sizeof(SymbolMutable));
  $$->type = SYMBOL_TYPE_MUTABLE;
  $$->datatype = $2;
}
;

datatype
: IDENTIFIER {
  LOG_DEBUG("datatype : IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolDatatype));
  $$->type = SYMBOL_TYPE_DATATYPE;
  $$->identifier = $1;
}
;

expression
: cond {
  LOG_DEBUG("expression : cond");
  $$ = xmalloc(sizeof(SymbolExpression));
  $$->type = SYMBOL_TYPE_EXPRESSION;
  $$->symbol = (Symbol *)$1;
}
| or {
  LOG_DEBUG("expression : or");
  $$ = xmalloc(sizeof(SymbolExpression));
  $$->type = SYMBOL_TYPE_EXPRESSION;
  $$->symbol = (Symbol *)$1;
}
;

or
: expression OR_OPER cond {
  LOG_DEBUG("or : expression OR_OPER cond");
  $$ = xmalloc(sizeof(SymbolOr));
  $$->type = SYMBOL_TYPE_OR;
  $$->expression = $1;
  $$->cond = $3;
}
;

cond
: comp {
  LOG_DEBUG("cond : comp");
  $$ = xmalloc(sizeof(SymbolCond));
  $$->type = SYMBOL_TYPE_COND;
  $$->symbol = (Symbol *)$1;
}
| and {
  LOG_DEBUG("cond : and");
  $$ = xmalloc(sizeof(SymbolCond));
  $$->type = SYMBOL_TYPE_COND;
  $$->symbol = (Symbol *)$1;
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
  $$->symbol = (Symbol *)$1;
}
| less_than {
  LOG_DEBUG("comp : less_than");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = (Symbol *)$1;
}
| greater_than {
  LOG_DEBUG("comp : greater_than");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = (Symbol *)$1;
}
| equal {
  LOG_DEBUG("comp : equal");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = (Symbol *)$1;
}
| less_equal {
  LOG_DEBUG("comp : less_equal");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = (Symbol *)$1;
}
| greater_equal {
  LOG_DEBUG("comp : greater_equal");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = (Symbol *)$1;
}
| not_equal {
  LOG_DEBUG("comp : not_equal");
  $$ = xmalloc(sizeof(SymbolComp));
  $$->type = SYMBOL_TYPE_COMP;
  $$->symbol = (Symbol *)$1;
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
  $$->symbol = (Symbol *)$1;
}
| add {
  LOG_DEBUG("term : add");
  $$ = xmalloc(sizeof(SymbolTerm));
  $$->type = SYMBOL_TYPE_TERM;
  $$->symbol = (Symbol *)$1;
}
| subtract {
  LOG_DEBUG("term : subtract");
  $$ = xmalloc(sizeof(SymbolTerm));
  $$->type = SYMBOL_TYPE_TERM;
  $$->symbol = (Symbol *)$1;
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
  $$->symbol = (Symbol *)$1;
}
| multiply {
  LOG_DEBUG("factor : multiply");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->symbol = (Symbol *)$1;
}
| divide {
  LOG_DEBUG("factor : divide");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_FACTOR;
  $$->symbol = (Symbol *)$1;
}
| modulo {
  LOG_DEBUG("factor : modulo");
  $$ = xmalloc(sizeof(SymbolFactor));
  $$->type = SYMBOL_TYPE_MODULO;
  $$->symbol = (Symbol *)$1;
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
  $$->symbol = (Symbol *)$1;
}
| '+' unary {
  LOG_DEBUG("unary : '+' unary");
  // Ignore '+' token
  $$ = $2;
}
| minus {
  LOG_DEBUG("unary : minus");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = (Symbol *)$1;
}
| negate {
  LOG_DEBUG("unary : negate");
  $$ = xmalloc(sizeof(SymbolUnary));
  $$->type = SYMBOL_TYPE_UNARY;
  $$->symbol = (Symbol *)$1;
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
  LOG_DEBUG("primary : atom");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = (Symbol *)$1;
}
| fncall {
  LOG_DEBUG("primary : fncall");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = (Symbol *)$1;
}
| subscription {
  LOG_DEBUG("primary : subscription");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = (Symbol *)$1;
}
| slice {
  LOG_DEBUG("primary : slice");
  $$ = xmalloc(sizeof(SymbolPrimary));
  $$->type = SYMBOL_TYPE_PRIMARY;
  $$->symbol = (Symbol *)$1;
}
;

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

dict_display
: '{' '}' {
  LOG_DEBUG("dict_display : '{' '}'");
  $$ = xmalloc(sizeof(SymbolDictDisplay));
  $$->type = SYMBOL_TYPE_DICT_DISPLAY;
  $$->key_value_pairs = NULL;
}
| '{' key_value_pairs '}' {
  LOG_DEBUG("dict_display : '{' key_value_pairs '}'");
  $$ = xmalloc(sizeof(SymbolDictDisplay));
  $$->type = SYMBOL_TYPE_DICT_DISPLAY;
  $$->key_value_pairs = $2;
}
| '{' key_value_pairs ',' '}' {
  LOG_DEBUG("dict_display : '{' key_value_pairs ',' '}'");
  $$ = xmalloc(sizeof(SymbolDictDisplay));
  $$->type = SYMBOL_TYPE_DICT_DISPLAY;
  $$->key_value_pairs = $2;
}
;

key_value_pairs
: STRING_LITERAL ':' expression {
  LOG_DEBUG("key_value_pairs : STRING_LITERAL ':' expression");
  $$ = xmalloc(sizeof(SymbolKeyValuePairs));
  $$->type = SYMBOL_TYPE_KEY_VALUE_PAIRS;
  $$->key_value_pairs = NULL;
  $$->string_literal = $1;
  $$->expression = $3;
}
| key_value_pairs ',' STRING_LITERAL ':' expression {
  LOG_DEBUG("key_value_pairs : key_value_pairs ',' STRING_LITERAL ':' expression");
  $$ = xmalloc(sizeof(SymbolKeyValuePairs));
  $$->type = SYMBOL_TYPE_KEY_VALUE_PAIRS;
  $$->key_value_pairs = $1;
  $$->string_literal = $3;
  $$->expression = $5;
}
;

list_display
: '[' ']' {
  LOG_DEBUG("list_display : '[' ']'");
  $$ = xmalloc(sizeof(SymbolListDisplay));
  $$->type = SYMBOL_TYPE_LIST_DISPLAY;
  $$->list_elements = NULL;
}
| '[' list_elements ']' {
  LOG_DEBUG("list_display : '[' list_elements ']'");
  $$ = xmalloc(sizeof(SymbolListDisplay));
  $$->type = SYMBOL_TYPE_LIST_DISPLAY;
  $$->list_elements = $2;
}
| '[' list_elements ',' ']' {
  LOG_DEBUG("list_display : '[' list_elements ',' ']'");
  $$ = xmalloc(sizeof(SymbolListDisplay));
  $$->type = SYMBOL_TYPE_LIST_DISPLAY;
  $$->list_elements = $2;
}
;

list_elements
: expression {
  LOG_DEBUG("list_elements : expression");
  $$ = xmalloc(sizeof(SymbolListElements));
  $$->type = SYMBOL_TYPE_LIST_ELEMENTS;
  $$->list_elements = NULL;
  $$->expression = $1;
}
| list_elements ',' expression {
  LOG_DEBUG("list_elements : list_elements ',' expression");
  $$ = xmalloc(sizeof(SymbolListElements));
  $$->type = SYMBOL_TYPE_LIST_ELEMENTS;
  $$->list_elements = $1;
  $$->expression = $3;
}
;

arguments
: expression {
  LOG_DEBUG("arguments : expression");
  $$ = xmalloc(sizeof(SymbolArguments));
  $$->type = SYMBOL_TYPE_ARGUMENTS;
  $$->arguments = NULL;
  $$->expression = $1;
}
| arguments ',' expression {
  LOG_DEBUG("arguments : arguments ',' expression");
  $$ = xmalloc(sizeof(SymbolArguments));
  $$->type = SYMBOL_TYPE_ARGUMENTS;
  $$->arguments = $1;
  $$->expression = $3;
}
;

subscription
: primary '[' expression ']' {
  LOG_DEBUG("subscription : primary '[' expression ']'");
  $$ = xmalloc(sizeof(SymbolSubscription));
  $$->type = SYMBOL_TYPE_SUBSCRIPTION;
  $$->primary = $1;
  $$->expression = $3;
}
;

slice
: primary '[' expression ':' expression ']' {
  LOG_DEBUG("slice : primary '[' expression ':' expression ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expression = $3;
  $$->right_expression = $5;
}
| primary '[' expression ':' ']' {
  LOG_DEBUG("slice : primary '[' expression ':' ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expression = $3;
  $$->right_expression = NULL;
}
| primary '[' ':' expression ']' {
  LOG_DEBUG("slice : primary '[' ':' expression ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expression = NULL;
  $$->right_expression = $4;
}
| primary '[' ':' ']' {
  LOG_DEBUG("slice : primary '[' ':' ']'");
  $$ = xmalloc(sizeof(SymbolSlice));
  $$->type = SYMBOL_TYPE_SLICE;
  $$->primary = $1;
  $$->left_expression = NULL;
  $$->right_expression = NULL;
}
;

inner_expression
: '(' expression ')' {
  LOG_DEBUG("inner_expression : '(' expression ')'");
  $$ = $2;
}
;

atom
: IDENTIFIER {
  LOG_DEBUG("atom : IDENTIFIER");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->symbol = (Symbol *)$1;
}
| INTEGER_LITERAL {
  LOG_DEBUG("atom : INTEGER_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->symbol = (Symbol *)$1;
}
| FLOAT_LITERAL {
  LOG_DEBUG("atom : FLOAT_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->symbol = (Symbol *)$1;
}
| STRING_LITERAL {
  LOG_DEBUG("atom : STRING_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->symbol = (Symbol *)$1;
}
| BOOLEAN_LITERAL {
  LOG_DEBUG("atom : BOOLEAN_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->symbol = (Symbol *)$1;
}
| NONE_LITERAL {
  LOG_DEBUG("atom : NONE_LITERAL");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->symbol = (Symbol *)$1;
}
| dict_display {
  LOG_DEBUG("atom : dict_display");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->symbol = (Symbol *)$1;
}
| list_display {
  LOG_DEBUG("atom : list_display");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
  $$->symbol = (Symbol *)$1;
}
| inner_expression {
  LOG_DEBUG("atom : inner_expression");
  $$ = xmalloc(sizeof(SymbolAtom));
  $$->type = SYMBOL_TYPE_ATOM;
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
