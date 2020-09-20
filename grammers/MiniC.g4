// use the defination of decaf-lang, a compiler lab of Tsinghua
// https://decaf-lang.github.io/minidecaf-tutorial/

grammar MiniC;

program : (function | declaration)*;

function
    : type identifier '(' ')' '{' statement '}';

type
    :   'void'
    |   'char'
    |   'short'
    |   'int'
    |   'long'
    |   'float'
    |   'double'
    |   'signed'
    |   'unsigned'
    |   type '*'
    ;

parameterList
    : (type identifier (',' type identifier)*)?
    ;

compoundStatement
    : '{' blockItem* '}'
    ;

blockItem
    : statement
    | declaration
    ;

statement
    : 'return' expression ';'
    | expression? ';'
    | 'if' '(' expression ')' statement ('else' statement)?
    | compoundStatement
    | 'for' '(' expression? ';' expression? ';' expression? ')' statement
    | 'for' '(' declaration expression? ';' expression? ')' statement
    | 'while' '(' expression ')' statement
    | 'do' statement 'while' '(' expression ')' ';'
    | 'break' ';'
    | 'continue' ';'
    ;
declaration
    : type identifier ('[' Integer ']')* ('=' expression)? ';'
    ;

expressionList
    : (expression (',' expression)*)?
    ;

expression : assignment ;
assignment
    : conditional
    | unary '=' expression
    ;

conditional
    : logicalOr
    | logicalOr '?' expression ':' conditional
    ;

logicalOr
    : logicalAnd
    | logicalOr '||' logicalAnd
    ;

logicalAnd
    : equality
    | logicalAnd '&&' equality
    ;
equality
    : relational
    | equality ('=='|'!=') relational
    ;
relational
    : additive
    | relational ('<'|'>'|'<='|'>=') additive
    ;

additive
    : multiplicative
    | additive ('+'|'-') multiplicative
    ;

multiplicative
    : unary
    | multiplicative ('*'|'/'|'%') unary
    ;

unary
    : postfix
    | ('-'|'~'|'!'|'&'|'*') unary
    | '(' type ')' unary
    ;

postfix
    : primary
    | identifier '(' expressionList ')'
    | postfix '[' expression ']'
    ;

primary
    : Integer
    | '(' expression ')'
    | identifier
    ;
    
identifier
    :   NO_DIGIT (NO_DIGIT | DIGIT)*;

Integer : '0' | [1-9][0-9]* ; 


fragment NO_DIGIT:[a-zA-Z_];
fragment DIGIT:[0-9];

WS : [ \t\n\r]+ -> skip;