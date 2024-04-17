//
// Created by hudexu on 24-4-5.
//

#ifndef LAB_SEMANTIC_H
#define LAB_SEMANTIC_H
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

typedef struct node_ *Node;

typedef struct type_ *Type;
typedef struct fieldList_ *FieldList;
typedef struct function_ *Function;
typedef struct structure_ *Structure;

typedef struct type_ {
    enum { BASIC, ARRAY, STRUCTURE, FUNCTION } kind;

    union{
        int basic;//int=0, float=1
        struct {
            Type elem;
            int size;
        } array;//数组
        Structure structure;//结构体
        Function function;
    }u;

}Type_;

typedef struct fieldList_ {
    char *name;//名字
    Type type;//类型
    FieldList tail;//下一个域
}FieldList_;

typedef struct structure_ {
    char *name;//名字
    FieldList domain;//域
}Structure_;

typedef struct function_ {
    char *name;//名字
    Type type;//返回值类型
    int line;//行号
    FieldList param;//参数
}Function_;

typedef struct node_ {
    FieldList param;
    Node deep_tail;
    Node hash_tail;
    int deep;
}Node_;

Node gTable[1024];
Node sTable[1024];
Node structTable;
Function funcTable[1024];
int now_deep;//当前栈深
int func_flag;//是否函数声明
int func_num;//函数声明数量
int unnamed_func;//无名结构体数量


unsigned int hash_pjw(char *name);

void semantic_error(int type, int line, char *s);

void semantic_check(TreeNode *root);

//init、delete
Type new_type(int kind, ...);
void init();
void type_delete(Type type);
void field_list_delete(FieldList field_list);
void node_delete(Node node);

//hash

int hash_struct_insert(char *name, Type type, int struct_flag);
Node hash_check(char *name);
Node struct_check(char *name);

//stack
void print_stack(int deep);
void delete_stack(int deep);

//type
char* copy_name(char* name);
Type copy_type(Type type);
int type_check(Type type_a, Type type_b);

//Fieldlist

FieldList copy_field_list(FieldList fieldList);

//function
int param_check(FieldList f1, FieldList f2);
void declare_func(Function func);
Function undeclared_func(Function func);
void decfun_check();
//tree

void ExtDefList(TreeNode* node);
void ExtDef(TreeNode* node);
void ExtDecList(TreeNode* node, Type type);
Type Specifier(TreeNode* node);
Type StructSpecifier(TreeNode* node);
FieldList VarDec(TreeNode* node, Type type, int structflags);
Function FunDec(TreeNode* node, Type type);
FieldList VarList(TreeNode* node);
FieldList ParamDec(TreeNode* node);
void CompSt(TreeNode* node, Type return_type);
void StmtList(TreeNode* node, Type return_type);
void Stmt(TreeNode* node, Type return_type);

FieldList DefList(TreeNode* node, int structflags);
FieldList Def(TreeNode* node, int structflags);
FieldList DecList(TreeNode* node, Type return_type, int structflags);
FieldList Dec(TreeNode* node, Type type, int structflags);
Type Exp(TreeNode* node);
FieldList Args(TreeNode* node);
#endif //LAB_SEMANTIC_H
