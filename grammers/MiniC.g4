// use the defination of decaf-lang, a compiler lab of Tsinghua
// https://decaf-lang.github.io/minidecaf-tutorial/

grammar MiniC;

prog : (func | decl)*;

func
    : type Ident '(' (type Ident ',')* (type Ident)? ')' ('{' block* '}' | ';')
    ;
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

block
    : stmt
    | decl
    ;

decl
    : type Ident ('[' Integer ']')* ('=' expr)? ';'
    ;

stmt
    : 'return' expr ';'
    | expr? ';'
    | '{' block* '}' 
    | 'if' '(' expr ')' stmt ('else' stmt)
    | 'while' '(' expr ')' stmt
    | 'do' stmt 'while' '(' expr ')' ';'
    | 'for' '(' (decl | expr)? ';' (expr)? ';' (expr)? ')' stmt
    | 'break' ';'
    | 'continue' ';'
    | ';'
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