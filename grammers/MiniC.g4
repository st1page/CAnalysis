// change from decaf-lang, a compiler lab of Tsinghua
// https://decaf-lang.github.io/minidecaf-tutorial/

grammar MiniC;

// keyword
Void : 'void';
Int : 'int';
UInt : 'uint';
Char : 'char';
UChar : 'uchar';
Return : 'return';
If : 'if';
Else : 'else';
For : 'for';
While : 'while';
Do : 'do';
Break : 'break';
Continue : 'continue'; 

// operator
Lparen : '(' ;
Rparen : ')' ;
Lbrkt : '[' ;
Rbrkt : ']' ;
Lbrace : '{' ;
Rbrace : '}' ;
Comma : ',' ;
Semicolon : ';';
Question : '?';
Colon : ':';

Minus : '-';
Exclamation : '!';
Tilde : '~';
Addition : '+';
Multiplication : '*';
AND : '&';
Division : '/';
Modulo : '%';
LAND : '&&';
LOR : '||';
EQ : '==';
NEQ : '!=';
LT : '<';
LE : '<=';
GT : '>';
GE : '>=';


// integer, identifier
Ident
    :   NO_DIGIT (NO_DIGIT | DIGIT)*;

Integer : '0' | [1-9][0-9]* ; 

fragment NO_DIGIT:[a-zA-Z_];
fragment DIGIT:[0-9];

COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
WS : [ \t\n\r]+ -> skip;



prog : func* EOF;

func
    : type Ident '(' (varDecl ',')* varDecl? ')' ('{' blockItem* '}' | ';')
    ;

varDecl
    : type Ident ('[' Integer ']')*
    ;
blockItem
    : stmt
    ;

stmt
    : varDecl ('=' expr)? ';'                                   # varDef
    | 'return' expr ';'                                         # returnStmt
    | expr? ';'                                                 # singleExpr
    | '{' blockItem* '}'                                        # block
    | 'if' '(' expr ')' stmt ('else' stmt)                      # ifStmt
    | 'while' '(' expr ')' stmt                                 # whileLoop
    | 'do' stmt 'while' '(' expr ')' ';'                        # doWhile
    | 'for' '(' (varDecl | expr)? ';' (expr)? ';' (expr)? ')'   # forLoop
    | 'break' ';'                                               # break
    | 'continue' ';'                                            # continue
    | ';'                                                       # nullStmt
    ;

expr
    : Ident '(' (expr ',')* (expr)? ')'                         # funcCall
    | ('!' | '~' | '-') expr                                    # unaryOp
    | expr ('*' | '/' | '%') expr                               # mulDiv
    | expr ('+' | '-') expr                                     # addSub
    | expr ('<' | '<=' | '>' | '>=') expr                       # LessGreat
    | expr ('==' | '!=') expr                                   # equalNot
    | expr '&&' expr                                            # logicAnd
    | expr '||' expr                                            # logicOr
    | '(' expr ')'                                              # atomParen 
    | Ident '=' expr                                            # assign
    | Ident                                                     # sigIdent
    | Integer                                                   # sigInt
    ;

type
    :   'void'   # voidType
    |   'char'   # charType
    |   'uchar'  # uCharType
    |   'int'    # intType
    |   'uint'   # uIntType
    |   type ('*')+ # pointerType
    ;

