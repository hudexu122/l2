%locations
%{
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"


// 插入节点函数，argc表示参数个数，argv为参数数组
TreeNode* insertNode(int linenum, NodeType type, char* name, char* val, int argc, ...) {
    
    if (argc < 0) {
        return NULL;
    }
    
    Data* data = (Data*)malloc(sizeof(Data));
    data-> linenum = linenum;
    data-> type = type;
    data-> name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    data-> name = name;
    if(val != NULL){
        data-> val = (char*)malloc(sizeof(char) * (strlen(val) + 1));
        strncpy(data->val, val, sizeof(char) * (strlen(val) + 1));
    }
    else data-> val = NULL;

    TreeNode* parent = (TreeNode*)malloc(sizeof(TreeNode));
    parent->data = data;
    parent->firstChild = NULL;
    parent->nextSibling = NULL;

    va_list argv;
    va_start(argv, argc);

    if(argc > 0){
        TreeNode* tempNode = va_arg(argv, TreeNode*);
        parent->firstChild = tempNode;
        //printf("arg0:%x\n",(void*)tempNode);
        for (int i = 1; i < argc; i++) {
            if(tempNode == NULL){printf("arg %d wrong at insertNode %s, line:%d!\n",i-1,name,linenum); continue;}
            tempNode->nextSibling = va_arg(argv, TreeNode*);
            if (tempNode->nextSibling != NULL) {
                tempNode = tempNode->nextSibling;
            }
            //printf("arg%d:%x\n",i,(void*)tempNode);
        }
    }

    va_end(argv);

    //printf("insertNode %s, line:%d ,of %x, val %s\n",name,linenum,(void*)parent,val);

    return parent;
}

// 删除节点函数
void deleteNode(TreeNode *node) {
    if (node == NULL) {
        return;
    }

    deleteNode(node->firstChild); // 递归删除子节点
    deleteNode(node->nextSibling); // 递归删除兄弟节点

    free(node);
}

// 打印节点函数
void printTree(TreeNode *node, int height) {
    if (node == NULL) {
        return;
    }

    if(node->data == NULL) assert(0);
    if(node->data->name == NULL) assert(0);
    for(int i = 1; i <= height; i++){
        printf("  ");
    }
    printf("%s", node->data->name);
    //printf("%s,%x,%d", node->data->name,(void*)node,height);
    NodeType curtype = node->data->type;
    if (curtype == TYPE_NO) {
        printf(" (%d)", node->data->linenum);
    } else if (curtype == TYPE_TYPE || curtype == TYPE_ID || curtype == TYPE_INT) {
        printf(": %s", node->data->val);
    } else if (curtype == TYPE_FLOAT) {
        printf(": %f", strtof(node->data->val, NULL));
    }
    printf("\n");
    printTree(node->firstChild, height + 1); // 打印子节点
    //printf("%s,%x,%d\n", node->data->name,(void*)node,height);
    printTree(node->nextSibling, height); // 打印兄弟节点
}

TreeNode* root = NULL; //根节点

#include "lex.yy.c"
// int yylex();
extern void semantic_check(TreeNode* root);
bool has_error = 0; // 存在错误
void yyerror(char *s){
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, s);
};

%}

/* declared types */
%union {
    TreeNode* type_node;
}

/* declared tokens */
%type <type_node> Program ExtDefList ExtDef ExtDecList   //  High-level Definitions
%type <type_node> Specifier StructSpecifier OptTag Tag   //  Specifiers
%type <type_node> VarDec FunDec VarList ParamDec         //  Declarators
%type <type_node> CompSt StmtList Stmt                   //  Statements
%type <type_node> DefList Def Dec DecList                //  Local Definitions
%type <type_node> Exp Args                               //  Expressions 
%token <type_node> TYPE PLUS MINUS STAR DIV LB LC LP RB RC RP SEMI ASSIGNOP COMMA AND OR NOT RELOP DOT STRUCT RETURN IF ELSE WHILE 
%token <type_node> INT
%token <type_node> FLOAT
%token <type_node> ID

//优先级

%right ASSIGNOP
%left OR
%left AND
%left RELOP 
%left PLUS MINUS
%left STAR DIV
%right NOT UNMINUS
%left LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

// High-level Definitions

Program : ExtDefList {$$ = insertNode(@$.first_line,TYPE_NO,"Program",NULL,1,$1); root = $$; if(!has_error){printTree(root, 0);semantic_check(root); }}
    ;

ExtDefList : ExtDef ExtDefList {$$ = insertNode(@$.first_line,TYPE_NO,"ExtDefList",NULL,2,$1,$2); }
    | {$$ = NULL;}
    ; 
    
ExtDef : Specifier ExtDecList SEMI {$$ = insertNode(@$.first_line,TYPE_NO,"ExtDef",NULL,3,$1,$2,$3); }
    | Specifier SEMI {$$ = insertNode(@$.first_line,TYPE_NO,"ExtDef",NULL,2,$1,$2); }
    | Specifier FunDec CompSt {$$ = insertNode(@$.first_line,TYPE_NO,"ExtDef",NULL,3,$1,$2,$3); }
    | Specifier FunDec SEMI {$$ = insertNode(@$.first_line,TYPE_NO,"ExtDef",NULL,3,$1,$2,$3); }//declare
    | error SEMI                              { has_error = 1; }
    ;

ExtDecList : VarDec {$$ = insertNode(@$.first_line,TYPE_NO,"ExtDecList",NULL,1,$1); }
    | VarDec COMMA ExtDecList {$$ = insertNode(@$.first_line,TYPE_NO,"ExtDecList",NULL,3,$1,$2,$3); }
    ;

// Specifiers

Specifier : TYPE {$$ = insertNode(@$.first_line,TYPE_NO,"Specifier",NULL,1,$1); }
    | StructSpecifier {$$ = insertNode(@$.first_line,TYPE_NO,"Specifier",NULL,1,$1); }
    ;

StructSpecifier : STRUCT OptTag LC DefList RC {$$ = insertNode(@$.first_line,TYPE_NO,"StructSpecifier",NULL,5,$1,$2,$3,$4,$5); }
    | STRUCT Tag {$$ = insertNode(@$.first_line,TYPE_NO,"StructSpecifier",NULL,2,$1,$2); }
    ;
    
OptTag : ID {$$ = insertNode(@$.first_line,TYPE_NO,"OptTag",NULL,1,$1); }
    | {$$ = NULL;}
    ;

Tag : ID {$$ = insertNode(@$.first_line,TYPE_NO,"Tag",NULL,1,$1); }
    ;

// Declarators

VarDec : ID {$$ = insertNode(@$.first_line,TYPE_NO,"VarDec",NULL,1,$1); }
    | VarDec LB INT RB {$$ = insertNode(@$.first_line,TYPE_NO,"VarDec",NULL,4,$1,$2,$3,$4); }
    | error RB                                { has_error = 1; }
    ;

FunDec : ID LP VarList RP {$$ = insertNode(@$.first_line,TYPE_NO,"FunDec",NULL,4,$1,$2,$3,$4); }
    | ID LP RP {$$ = insertNode(@$.first_line,TYPE_NO,"FunDec",NULL,3,$1,$2,$3); }
    | error RP                                { has_error = 1; }
    ;

VarList : ParamDec COMMA VarList {$$ = insertNode(@$.first_line,TYPE_NO,"VarList",NULL,3,$1,$2,$3); }
    | ParamDec {$$ = insertNode(@$.first_line,TYPE_NO,"VarList",NULL,1,$1); }
    ;

ParamDec : Specifier VarDec {$$ = insertNode(@$.first_line,TYPE_NO,"ParamDec",NULL,2,$1,$2); }
    ;

// Statements

CompSt : LC DefList StmtList RC {$$ = insertNode(@$.first_line,TYPE_NO,"CompSt",NULL,4,$1,$2,$3,$4); }
    | error RC                                { has_error = 1; }
    ;

StmtList : Stmt StmtList {$$ = insertNode(@$.first_line,TYPE_NO,"StmtList",NULL,2,$1,$2); }
    | {$$ = NULL;}
    ;

Stmt : Exp SEMI {$$ = insertNode(@$.first_line,TYPE_NO,"Stmt",NULL,2,$1,$2); }
    | CompSt {$$ = insertNode(@$.first_line,TYPE_NO,"Stmt",NULL,1,$1); }
    | RETURN Exp SEMI {$$ = insertNode(@$.first_line,TYPE_NO,"Stmt",NULL,3,$1,$2,$3); }
    | IF LP Exp RP Stmt {$$ = insertNode(@$.first_line,TYPE_NO,"Stmt",NULL,5,$1,$2,$3,$4,$5); }
    | IF LP Exp RP Stmt ELSE Stmt {$$ = insertNode(@$.first_line,TYPE_NO,"Stmt",NULL,7,$1,$2,$3,$4,$5,$6,$7); }
    | WHILE LP Exp RP Stmt {$$ = insertNode(@$.first_line,TYPE_NO,"Stmt",NULL,5,$1,$2,$3,$4,$5); }
    | error SEMI                              { has_error = 1; }
    ;

// Local Definitions

DefList : Def DefList {$$ = insertNode(@$.first_line,TYPE_NO,"DefList",NULL,2,$1,$2); }
    | {$$ = NULL;}
    ;

Def : Specifier DecList SEMI {$$ = insertNode(@$.first_line,TYPE_NO,"Def",NULL,3,$1,$2,$3); }
    ;

DecList : Dec {$$ = insertNode(@$.first_line,TYPE_NO,"DecList",NULL,1,$1); }
    | Dec COMMA DecList {$$ = insertNode(@$.first_line,TYPE_NO,"DecList",NULL,3,$1,$2,$3); }
    ;

Dec : VarDec {$$ = insertNode(@$.first_line,TYPE_NO,"Dec",NULL,1,$1); }
    | VarDec ASSIGNOP Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Dec",NULL,3,$1,$2,$3); }

// Expressions

Exp : Exp ASSIGNOP Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | Exp AND Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | Exp OR Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | Exp RELOP Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | Exp PLUS Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | Exp MINUS Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | Exp STAR Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | Exp DIV Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | LP Exp RP {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | MINUS Exp %prec UNMINUS {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,2,$1,$2); }
    | NOT Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,2,$1,$2); }
    | ID LP Args RP {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,4,$1,$2,$3,$4); }
    | ID LP RP {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | Exp LB Exp RB {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,4,$1,$2,$3,$4); }
    | Exp DOT ID {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,3,$1,$2,$3); }
    | ID {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,1,$1); }
    | INT {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,1,$1); }
    | FLOAT {$$ = insertNode(@$.first_line,TYPE_NO,"Exp",NULL,1,$1); }
    ;

Args : Exp COMMA Args {$$ = insertNode(@$.first_line,TYPE_NO,"Args",NULL,3,$1,$2,$3); }
    | Exp {$$ = insertNode(@$.first_line,TYPE_NO,"Args",NULL,1,$1); }
    ;

%%

