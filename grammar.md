# Grammar
Non-terminals are illustarted by boxes with sharp edges and contain an
identifier. They expand into other non-terminals or terminals. Terminals are
illustrated by boxes with regular edges and contain a regular expressions.

## Program

### program
```mermaid
graph LR
    start(( ))
    --> func
    --> E-o-F
    --> stop(( ))
    func --> func
```

## Statements

### stmt
```mermaid
graph LR
    start(( ))
    --> assignment
    --> return_stmt
    --> semicolon(;)
    --> stop(( ))
```

## Simple statements

### assignment
```mermaid
graph LR
    start(( ))
    --> type
    --> identifier
    --> subscription
    --> assignment_opr(=)
    --> expr
    --> semicolon(;)
    --> stop(( ))
    start --> identifier
    identifier --> assignment_opr
    subscription --> subscription
```

### return_stmt
```mermaid
graph LR
    start(( ))
    --> return(return)
    --> expr
    --> semicolon(;)
    --> stop(( ))
```

## Compund statements
Compound statements contain other statements.

### compound_stmt
```mermaid
graph LR
    start(( )) --> for_stmt --> stop(( ))
    start --> if_stmt --> stop
    start --> while_stmt --> stop
    start --> do_while_stmt --> stop
```

### if_stmt
```mermaid
graph LR
    start(( ))
    --> if
    --> if_expr[inner_expr]
    --> if_scope[scope]
    --> stop(( ))
    if_scope --> elif[else if]
    --> elif_expr[inner_expr]
    --> elif_scope[scope]
    --> stop
    if_scope --> else
    --> else_expr[inner_expr]
    --> else_scope[scope]
    --> stop
    elif_scope --> else

```

### scope
```mermaid
graph LR
    start(( ))
    --> left_curly("{")
    --> stmt
    --> stmt
    --> right_curly("}")
    --> stop(( ))
```

### for_stmt
```mermaid
graph LR
    start(( ))
    --> for(for)
    --> left_param("\(")
    --> first_assignment[assignment]
    --> fist_semicolon(;)
    --> expr
    --> second_semicolon(;)
    --> second_assignment[assignment]
    --> right_param("\)")
    --> scope
    --> stop(( ))
```

### while_stmt
```mermaid
graph LR
    start(( ))
    --> while(while)
    --> inner_expr
    --> scope
    --> stop(( ))
```

### do_while_stmt
```mermaid
graph LR
    start(( ))
    --> do(do)
    --> scope
    --> while(while)
    --> inner_expr
    --> stop(( ))
```

## Functions

### func
```mermaid
graph LR
    start(( ))
    --> type
    --> identifier
    --> params
    --> scope
    --> stop(( ))
```

### params
```mermaid
graph LR
    start(( ))
    --> left_paren("\(")
    --> type
    --> identifier
    --> right_paren("\)")
    --> stop(( ))
    left_paren --> right_paren
    identifier --> comma(,)
    comma --> type
```

## Types
These are the built in types of the language.

### int8
```mermaid
graph LR
    start(( )) --> int(int8) --> stop(( ))
    start --> mut(mut)
    mut --> int
```

### int16
```mermaid
graph LR
    start(( ))
    --> int(int16)
    --> stop(( ))
    start --> mut(mut)
    mut --> int
```

### int32
```mermaid
graph LR
    start(( )) --> type(int32) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### int64
```mermaid
graph LR
    start(( )) --> type(int64) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### uint8
```mermaid
graph LR
    start(( )) --> type(uint8) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### uint16
```mermaid
graph LR
    start(( )) --> type(uint16) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### uint32
```mermaid
graph LR
    start(( )) --> type(uint32) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### int64
```mermaid
graph LR
    start(( )) --> type(uint64) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### float
```mermaid
graph LR
    start(( )) --> type(float) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### double
```mermaid
graph LR
    start(( )) --> type(double) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### string
```mermaid
graph LR
    start(( )) --> type(string) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

### bool
```mermaid
graph LR
    start(( )) --> type(bool) --> stop(( ))
    start --> mut(mut)
    mut --> type
```

## Expressions
Expressions computes values. They are defined using many non-terminals in order
to control precedence.

### expr
```mermaid
graph LR;
    start(( )) --> and_test --> stop(( ))
    and_test --> or(or) --> and_test
```

### and_test
```mermaid
graph LR;
    start(( )) --> not_test --> stop(( ))
    not_test --> and(and) --> not_test
```

### not_test
```mermaid
graph LR;
    start(( )) --> not(not) --> comparison --> stop(( ))
    start --> comparison
```

### comparison
```mermaid
graph LR;
    start(( )) --> term --> stop(( ))
    term --> comp_opr
    --> term
```

### comp_opr
```mermaid
graph LR;
    start(( )) --> less_than(<) --> stop(( ))
    start --> greater_than(>) --> stop
    start --> equal(==) --> stop
    start --> less_equal(<=) --> stop
    start --> greater_equal(>=) --> stop
    start --> not_equal(!=) --> stop
```

### term
```mermaid
graph LR;
    start(( )) --> factor --> stop(( ))
    factor --> term_opr
    --> factor
```

### term_opr
```mermaid
graph LR;
    start(( )) --> plus(\+) --> stop(( ))
    start --> minus("-") --> stop
```

### factor
```mermaid
graph LR;
    start(( )) --> factor_prefix
    --> primary
    --> stop(( ))
    start --> primary
    --> factor_opr
    --> factor_prefix
```

### factor_prefix
```mermaid
graph LR;
    start(( )) --> plus(\+) --> stop(( ))
    start --> minus("-") --> stop
```

### factor_opr
```mermaid
graph LR;
    start(( )) --> multiply(\*) --> stop(( ))
    start --> divide(\/) --> stop
    start --> modulo(%) --> stop
    start --> floor_division(\/\/) --> stop
```

### primary
```mermaid
graph LR;
    start(( )) --> atom
    --> primary_suffix
    --> stop(( ))
    primary_suffix --> primary_suffix
    atom --> stop
```

### primary_suffix
```mermaid
graph LR;
    start(( )) --> arguments --> stop(( ))
    start --> subscription --> stop
```

### atom
```mermaid
graph LR;
    start(( )) --> identifier --> stop(( ))
    start --> integer_literal --> stop
    start --> float_literal --> stop
    start --> string_literal --> stop
    start --> boolean_literal --> stop
    start --> none_literal --> stop
    start --> inner_expr --> stop
    start --> list_display --> stop
    start --> dict_display --> stop
```

### inner_expr
```mermaid
graph LR;
    start(( )) --> left_bracket("\(") --> expr --> right_bracket("\)") -->
    stop(( ))
```

### list_display
```mermaid
graph LR;
    start(( )) --> left_bracket("\[")
    --> expr
    --> right_bracket("\]")
    --> stop(( ))
    expr --> comma(,)
    --> expr
    comma --> right_bracket
```

### dict_display
```mermaid
graph LR;
    start(( )) --> left_brace("{")
    --> string_literal
    --> colon(:)
    --> expr
    --> right_brace("}")
    left_brace --> right_brace
    expr --> comma(,)
    --> string_literal
    comma --> right_brace
```

### arguments
```mermaid
graph LR;
    start(( )) --> left_paren("\(")
    --> expr
    --> right_paren("\)")
    --> stop(( ))
    expr --> comma(,)
    comma --> expr
    comma --> right_paren
```

### subscription
```mermaid
graph LR;
    start(( )) --> left_bracket("\[") --> expr --> right_bracket("\]") -->
    stop(( ))
```

## Literals
Literals specifies values.

### integer_literal
```mermaid
graph LR;
    start(( )) --> integer_literal("(0|[1-9][0-9]*)") --> stop(( ))
```

### float_literal
```mermaid
graph LR;
    start(( )) --> float_literal("(0|[1-9][0-9]*)\.[0-9]*") --> stop(( ))
```

### string_literal
```mermaid
graph LR;
    start(( )) --> string_literal("\#34;(\\.|[^#34;\\])*\#34;") --> stop(( ))
```

### boolean_literal
```mermaid
graph LR;
    start(( )) --> true_literal(true) --> stop(( ))
    start(( )) --> false_literal(false) --> stop(( ))
```

### none_literal
```mermaid
graph LR;
    start(( )) --> none_literal(none) --> stop(( ))
```

## Identifier
Identifies functions and variables.

### identifier
```mermaid
graph LR
    start(( )) --> identifier("[_a-zA-Z][_a-zA-Z0-9]*") --> stop(( ))
```
