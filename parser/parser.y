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
}

%token IDENTIFIER
%token NONE_LITERAL BOOLEAN_LITERAL STRING_LITERAL FLOAT_LITERAL INTEGER_LITERAL
%token EQ_OPER LE_OPER GE_OPER NE_OPER
%token AND_OPER OR_OPER

%%

start : %empty                 { LOG_DEBUG("start : %%empty"); }
      | expr                   { LOG_DEBUG("start : expr"); }
      ;

expr : and                     { LOG_DEBUG("expr : and"); }
     | expr OR_OPER and        { LOG_DEBUG("expr : expr OR_OPER and"); }

and : not                      { LOG_DEBUG("and : not"); }
    | and AND_OPER not         { LOG_DEBUG("and : and AND_OPER not"); }

not : comp                     { LOG_DEBUG("not : comp"); }
    | '!' not                  { LOG_DEBUG("not : '!' not"); }

comp : term                    { LOG_DEBUG("comp : term"); }
     | comp '<' term           { LOG_DEBUG("comp : comp '<' term"); }
     | comp '>' term           { LOG_DEBUG("comp : comp '>' term"); }
     | comp EQ_OPER term       { LOG_DEBUG("comp : comp EQ_OPER term"); }
     | comp LE_OPER term       { LOG_DEBUG("comp : comp LE_OPER term"); }
     | comp GE_OPER term       { LOG_DEBUG("comp : comp GE_OPER term"); }
     | comp NE_OPER term       { LOG_DEBUG("comp : comp NE_OPER term"); }

term : factor                  { LOG_DEBUG("term : factor"); }
     | term '+' factor         { LOG_DEBUG("term : term '+' factor"); }
     | term '-' factor         { LOG_DEBUG("term : term '-' factor"); }
     ;

factor : primary               { LOG_DEBUG("primary"); }
       | factor '*' primary    { LOG_DEBUG("factor : factor '*' primary"); }
       | factor '/' primary    { LOG_DEBUG("factor : factor '/' primary"); }
       | factor '%' primary    { LOG_DEBUG("factor : factor '%' primary"); }
       ;


unary : primary                { LOG_DEBUG("primary"); }
      | '-' primary            { LOG_DEBUG("unary : '+' primary"); }
      | '+' primary            { LOG_DEBUG("unary : '-' primary"); }
      ;

primary : atom                 { LOG_DEBUG("atom"); }
        | primary arguments    { LOG_DEBUG("primary : primary arguments"); }
        | primary subscription { LOG_DEBUG("primary : primary subscription"); }
        ;

arguments : '(' ')'            { LOG_DEBUG("arguments : '(' ')'"); }
          | '(' arglist ')'    { LOG_DEBUG("arguments : '(' arglist ')'"); }
          ;

arglist : expr                 { LOG_DEBUG("arglist : expr"); }
         | arglist ',' expr    { LOG_DEBUG("arglist : arglist ',' expr"); }
         ;

subscription : '[' expr ']'    { LOG_DEBUG("subscription : '[' expr ']'"); }

inner_expr : '(' expr ')'      { LOG_DEBUG("inner_expr : '(' expr ')'"); }

atom : IDENTIFIER              { LOG_DEBUG("atom : IDENTIFIER"); }
     | INTEGER_LITERAL         { LOG_DEBUG("atom : INTEGER_LITERAL"); }
     | FLOAT_LITERAL           { LOG_DEBUG("atom : FLOAT_LITERAL"); }
     | STRING_LITERAL          { LOG_DEBUG("atom : STRING_LITERAL"); }
     | BOOLEAN_LITERAL         { LOG_DEBUG("atom : BOOLEAN_LITERAL"); }
     | NONE_LITERAL            { LOG_DEBUG("atom : NONE_LITERAL"); }
     | inner_expr              { LOG_DEBUG("atom : inner_expr"); }
     ;

%%

void yyerror(char *msg) {
  LOG_ERROR(msg);
  exit(1);
}
