%locations
%{
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "lex.yy.c"
// int yylex();
bool has_error = 0; // 存在错误
void yyerror(char *s){
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, s);
};

%}

/* declared types */
%union {
    int type_int;
    float type_float;
    double type_double;
}

/* declared tokens */
%token TYPE PLUS MINUS STAR DIV LB LC LP RB RC RP SEMI ASSIGNOP COMMA AND OR NOT RELOP DOT STRUCT RETURN IF ELSE WHILE 
%token <type_int> INT
%token <type_float> FLOAT
%token <type_string> ID

//优先级

%right ASSIGNOP
%left OR
%left AND
%left RELOP 
%left STAR DIV
%left PLUS MINUS
%right NOT
%left LP RP LB RB DOT 
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

// High-level Definitions

Program : ExtDefList
    ;

ExtDefList : ExtDef ExtDefList
    |
    ; 
    
ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;

ExtDecList : VarDec
    | VarDec COMMA ExtDecList
    ;

// Specifiers

Specifier : TYPE
    | StructSpecifier
    ;

StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;
    
OptTag : ID
    | 
    ;

Tag : ID
    ;

// Declarators

VarDec : ID
    | VarDec LB INT RB
    ;

FunDec : ID LP VarList RP
    | ID LP RP
    ;

VarList : ParamDec COMMA VarList
    | ParamDec

ParamDec : Specifier VarDec
    ;

// Statements

CompSt : LC DefList StmtList RC
    ;

StmtList : Stmt StmtList
    | 
    ;

Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;

// Local Definitions

DefList : Def DefList
    | 
    ;

Def : Specifier DecList SEMI
    ;

DecList : Dec
    | Dec COMMA DecList

Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;

// Expressions

Exp : Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    ;

Args : Exp COMMA Args
    | Exp
    ;

%%

