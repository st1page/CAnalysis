// use the defination of decaf-lang, a compiler lab of Tsinghua
// https://decaf-lang.github.io/minidecaf-tutorial/

grammar MiniC;

import CommonLex;

prog : (func | decl)*;

func
    : type Ident '(' (type Ident ',')* (type Ident)? ')' ('{' blockItem* '}' | ';')
    ;
type
    :   'void'
    |   'char'
    |   'uchar'
    |   'int'
    |   'uint'
    |   type '*'
    ;

blockItem
    : stmt
    | decl
    ;

decl
    : type Ident ('[' Integer ']')* ('=' expr)? ';'             # varDef
    ;

stmt
    : 'return' expr ';'                                         # returnStmt
    | expr? ';'                                                 # singleExpr
    | '{' blockItem* '}'                                        # block
    | 'if' '(' expr ')' stmt ('else' stmt)                      # ifStmt
    | 'while' '(' expr ')' stmt                                 # whileLoop
    | 'do' stmt 'while' '(' expr ')' ';'                        # doWhile
    | 'for' '(' (decl | expr)? ';' (expr)? ';' (expr)? ')' stmt # forLoop
    | 'break' ';'                                               # break
    | 'continue' ';'                                            # continue
    | ';'                                                       # nullStmt
    ;
expr
    : Ident '(' (expr ',')* (expr)? ')'
    | ('!' | '~' | '-') expr
    | expr ('*' | '/' | '%') expr
    | expr ('+' | '-') expr
    | expr ('<' | '<=' | '>' | '>=') expr
    | expr ('==' | '!=') expr
    | expr '&&' expr
    | expr '||' expr
    | expr '?' expr ':' expr
    | '(' expr ')'
    | Ident '=' expr
    | Ident
    | Integer
    ;
    
Ident
    :   NO_DIGIT (NO_DIGIT | DIGIT)*;

Integer : '0' | [1-9][0-9]* ; 

fragment NO_DIGIT:[a-zA-Z_];
fragment DIGIT:[0-9];

COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
WS : [ \t\n\r]+ -> skip;