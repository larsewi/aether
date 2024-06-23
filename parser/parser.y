%{
#include "parser_state.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils/logger.h"

extern int yylex();
void yyerror(char *msg);

%}

%union {
  Atom *atom;
}

%token <atom> FLOAT_LITERAL STRING_LITERAL INTEGER_LITERAL
%token EQ_OPERATOR LE_OPERATOR GE_OPERATOR NE_OPERATOR
%token AND_OPERATOR OR_OPERATOR

%%

prog : /* empty */
     | expr
     ;

expr : and OR_OPERATOR expr
     | and
     ;

and : not AND_OPERATOR and
    | not
    ;

not : '!' comparison
    | comparison
    ;

comparison : term comp_operator comparison
           | term
           ;

comp_operator : '<'
              | '>'
              | EQ_OPERATOR
              | LE_OPERATOR
              | GE_OPERATOR
              | NE_OPERATOR
              ;

term : factor term_operator term
     | factor
     ;

term_operator : '+'
              | '-'
              ;

factor : factor_prefix atom factor_operator
       | atom factor_operator factor
       | atom factor_operator
       | atom
       ;

factor_prefix : '+'
              | '-'
              ;

factor_operator : '*'
                | '/'
                | '%'
                ;

atom : FLOAT_LITERAL
     | INTEGER_LITERAL
     | STRING_LITERAL
     ;

%%

void yyerror(char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}
