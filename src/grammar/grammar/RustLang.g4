grammar RustLang;

fragment DIGITNOZERO : [1-9] ;
fragment DIGIT       : [0-9] ;
fragment SYMBOL      : [_a-zA-Z] ;
fragment NAME        : SYMBOL ( SYMBOL | DIGIT )* ;

INTEGER : '0' | DIGITNOZERO ( DIGIT )* ;
FLOAT   : INTEGER '.' INTEGER;
STRING  : BR (' ' | '%' | '\\' | SYMBOL | DIGIT)* BR ;

LET    : 'let'    ;
MUT    : 'mut'    ;
CONST  : 'const'  ;
FN     : 'fn'     ;
RETURN : 'return' ;
BREAK  : 'break'  ;
IF     : 'if'     ;
ELSE   : 'else'   ;
MATCH  : 'match'  ;
LOOP   : 'loop'   ;
WHILE  : 'while'  ;
FOR    : 'for'    ;
IN     : 'in'     ;
STRUCT : 'struct' ;
IMPL   : 'impl'   ;
TRAIT  : 'trait'  ;
USE    : 'use'    ;
PUB    : 'pub'    ;
MOD    : 'mod'    ;
AS     : 'as'     ;

DCOLON : '::' ;
COLON  : ':'  ;
SEMI   : ';'  ;
COMMA  : ','  ;
POINT  : '.'  ;
DPOINT : '..' ;
BR     : '"'  ;
APSTRF : '\'' ;
LPAREN : '('  ;
RPAREN : ')'  ;
LCBR   : '{'  ;
RCBR   : '}'  ;
LSBR   : '['  ;
RSBR   : ']'  ;
LESS   : '<'  ;
GR     : '>'  ;
LESSEQ : '<=' ;
GREQ   : '>=' ;
EQ     : '==' ;
NEQ    : '!=' ;
ASSIGN : '='  ;

PLUS   : '+'  ;
MINUS  : '-'  ;
STAR   : '*'  ;
SLASH  : '/'  ;
PERCNT : '%'  ;
MPRSND : '&'  ;
STICK  : '|'  ;
UARROW : '^'  ;
LSHIFT : '<<' ;
RSHIFT : '>>' ;

ARROW  : '->' ;
EXCLM  : '!'  ;

ID  : NAME EXCLM?;

WS : [ \t\r\n]+ -> skip ;


program: expressions* EOF ;

expressions
    : assign_stmt
    | func_def
    | mod_def
    | struct_def
    | struct_impl
    | trait_def
    | trait_impl
    ;

statements
    : func_call SEMI
    | assign_stmt
    | var_def SEMI
    | if_stmt
    | loop_stmt
    | while_stmt
    | for_stmt
    | return_stmt
    | break_stmt
    | scope
    ;

unit
    : func_call
    | arr_access
    | ID
    | INTEGER
    | FLOAT
    | STRING
    ;

func_call: ID LPAREN (expr_op (COMMA expr_op)*)? RPAREN;

if_stmt: IF expr_op scope (ELSE IF expr_op scope)* (ELSE scope)?;

while_stmt: WHILE expr_op scope;

expr_op
    : expr_op ( STAR | SLASH | PERCNT ) expr_op
    | expr_op ( PLUS | MINUS ) expr_op
    | expr_op ( LSHIFT | RSHIFT ) expr_op
    | expr_op ( LESS | LESSEQ | GR | GREQ ) expr_op
    | expr_op ( EQ | NEQ ) expr_op
    | expr_op ( MPRSND ) expr_op
    | expr_op ( UARROW ) expr_op
    | expr_op ( STICK ) expr_op
    | expr_op ( MPRSND MPRSND ) expr_op
    | expr_op ( STICK STICK ) expr_op
    | LPAREN expr_op RPAREN
    | ( PLUS | MINUS)? unit
    ;

agr_list : LSBR (expr_op (COMMA expr_op)*)? RSBR ;

arr_access : ID LSBR expr_op RSBR ;

for_stmt: FOR ID IN expr_op DPOINT expr_op scope;

assign_stmt: (ID | var_def | arr_access) ASSIGN (expr_op | agr_list) SEMI;

return_stmt
    : RETURN expr_op? SEMI
    | expr_op
    ;

break_stmt : BREAK expr_op? SEMI ;

scope: LCBR (statements)* RCBR ;

struct_def:  STRUCT ID LCBR (var_typedec COMMA)* RCBR ;
struct_impl: IMPL ID LCBR expressions* RCBR ;

trait_def:  TRAIT ID LCBR expressions* RCBR ;
trait_impl: IMPL ID FOR ID LCBR expressions* RCBR ;

loop_stmt: LOOP scope ;

var_typedec: ID COLON ID ;

mod_def: MOD ID LCBR expressions* RCBR ;

var_def: LET (MUT)? ID (COLON (ID | LSBR ID SEMI INTEGER RSBR))?;

func_def: FN ID LPAREN (var_typedec (COMMA var_typedec)*)? RPAREN (ARROW ID)? scope ;
