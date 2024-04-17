//
// Created by hudexu on 24-4-5.
//




#include "semantic.h"


#define HASH_SIZE 0x3ff
int basic_ = 0;
int debug = 0;

unsigned int hash_pjw(char *name)
{
    unsigned int val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~HASH_SIZE) val = (val ^ (i >> 12)) & HASH_SIZE;
    }
    return val;
}

void semantic_error(int type, int line, char *s){
    fprintf(stderr, "Error type %d at Line %d: %s\n", type, line, s);
}

void semantic_check(TreeNode *root){
    init();

    ExtDefList(root->firstChild);

    decfun_check();

    for(int i = 0; i < HASH_SIZE; i++){
//        node_delete(gTable[i]);
    }
    now_deep = 0;
    func_num = 0;
    func_flag = 0;
    unnamed_func = 0;
}
//init、delete

Type new_type(int kind, ...){
    Type type = (Type)malloc(sizeof(Type_));
    assert(type != NULL);
    type->kind = kind;
    va_list argv;


    if(type->kind == BASIC){
        va_start(argv, 1);
        type->u.basic = va_arg(argv, int);
    }else if(type->kind == ARRAY){
        va_start(argv, 2);
        type->u.array.elem = va_arg(argv, Type);
        type->u.array.size = va_arg(argv, int);
    }else if(type->kind == STRUCTURE){
        Structure structure = (Structure)malloc(sizeof(Structure_));
        type->u.structure = structure;
        va_start(argv, 2);
        type->u.structure->name = va_arg(argv, char*);
        type->u.structure->domain = va_arg(argv, FieldList);
    }else{
        Function function = (Function)malloc(sizeof(Function_));
        type->u.function = function;
        va_start(argv, 4);
        type->u.function->name = va_arg(argv, char*);
        type->u.function->type = va_arg(argv, Type);
        type->u.function->line = va_arg(argv, int);
        type->u.function->param = va_arg(argv, FieldList);
    }
    va_end(argv);
    return type;

}

void init(){
    for(int i = 0; i < HASH_SIZE; i++){
        gTable[i] = NULL;
        sTable[i] = NULL;
        funcTable[i] = NULL;
    }
    structTable = NULL;
    now_deep = 0;
    func_flag = 0;
    func_num = 0;
    unnamed_func = 0;
}

void type_delete(Type type){

    if(type->kind == BASIC){
        type->u.basic = 0;
    }else if(type->kind == ARRAY){
        type_delete(type->u.array.elem);
        type->u.array.elem = NULL;
        type->u.array.size = 0;
    }else if(type->kind == STRUCTURE){
        if(type->u.structure->name != NULL) {
            free(type->u.structure->name);
            type->u.structure->name = NULL;
        }
        field_list_delete(type->u.structure->domain);
        type->u.structure->domain = NULL;
        free(type->u.structure);
        type->u.structure = NULL;
    }else{
        if(type->u.function->name != NULL) {
            free(type->u.function->name);
            type->u.function->name = NULL;
        }
        type->u.function->line = 0;
        field_list_delete(type->u.function->param);
        type->u.function->param = NULL;
        type_delete(type->u.function->type);
        type->u.function->type = NULL;
        free(type->u.function);
        type->u.function = NULL;
    }
    type->kind = 0;
    free(type);
    type = NULL;
}


void field_list_delete(FieldList field_list){
//    printf("CUT1\n");
    if(field_list->tail != NULL){
        field_list_delete(field_list->tail);
        field_list->tail = NULL;
    }
//    printf("CUT2\n");
    if(field_list->type != NULL) {
        type_delete(field_list->type);
        field_list->type = NULL;
    }
//    printf("CUT3\n");
    if(field_list->name != NULL) {
//        printf("%d %s\n", (int)(field_list->name), field_list->name);
        free(field_list->name);
        field_list->name = NULL;
    }
//    printf("CUT4\n");
    free(field_list);
    field_list = NULL;
}

void node_delete(Node node){
    if(node == NULL) return;

    unsigned int name_hash = hash_pjw(node->param->name);

    Node t = NULL;
    for(Node node1 = gTable[name_hash]; node1 != NULL; node1 = node1->hash_tail) {
        if (strcmp(node1->param->name, node->param->name) == 0 && node1->deep == node->deep) {
            if(t == NULL){
                gTable[name_hash] = NULL;
            }else{
                t->hash_tail = NULL;
            }
            break;
        }
        t = node;
    }

    for(Node node1 = structTable; node1 != NULL; node1 = node1->hash_tail) {
        if (strcmp(node1->param->name, node->param->name) == 0 && node1->deep == node->deep) {
            if(t == NULL){
                structTable = NULL;
            }else{
                t->hash_tail = NULL;
            }
            break;
        }
        t = node;
    }

    if(node->param != NULL) {
        field_list_delete(node->param);
        node->param = NULL;
    }
    node->hash_tail = NULL;
    node->deep_tail = NULL;
    free(node);
    node = NULL;
}

//hash

int hash_struct_insert(char *name, Type type, int struct_flag){
    unsigned int name_hash = hash_pjw(name);
    Node newNode = (Node)malloc(sizeof(Node_));
    FieldList newFieldList = (FieldList) malloc(sizeof(FieldList_));
    newFieldList->name = copy_name(name);
    newFieldList->type = copy_type(type);
    newFieldList->tail = NULL;
    newNode->deep = now_deep;
    newNode->param = newFieldList;

    if(debug) printf("hash_struct_insert:%s,%d,%d\n", name, name_hash, now_deep);


    if(struct_flag == 0) {
        Node node = gTable[name_hash];
        if (node == NULL) {
            gTable[name_hash] = newNode;
        } else {
            while (node->hash_tail != NULL) {
                node = node->hash_tail;
            }
            node->hash_tail = newNode;
        }
    }else{
        Node node = structTable;
        if (node == NULL) {
            structTable = newNode;
        } else {
            while (node->hash_tail != NULL) {
                node = node->hash_tail;
            }
            node->hash_tail = newNode;
        }
    }


    newNode->deep_tail = sTable[now_deep];
    sTable[now_deep] = newNode;

    return 0;
}

Node hash_check(char *name){
    unsigned int name_hash = hash_pjw(name);

    Node t = NULL;
    for(Node node = gTable[name_hash]; node != NULL; node = node->hash_tail) {
        if (strcmp(node->param->name, name) == 0) {
            if (debug) printf("hash_check:%s\n", name);
            if(t == NULL || t->deep < node->deep){
                t = node;
            }
        }
    }
    return t;
}

Node struct_check(char *name){

    for(Node node = structTable; node != NULL; node = node->hash_tail) {
        if (strcmp(node->param->name, name) == 0) {
            if (debug) printf("struct_check:%s\n", name);
            return node;
        }
    }
    return NULL;
}

//stack
void print_stack(int deep){
    Node node = sTable[deep];
    while (node != NULL) {
        printf("node[%d]:%s\n", deep, node->param->name);
        printf("node[%d]:%d\n", deep, node->param->type->kind);
        node = node->deep_tail;
    }
}

void delete_stack(int deep){
    Node node = sTable[deep];

    while (node != NULL) {
        Node save = node;
//        if (debug) printf("delete_stack_node:%s\n", save->param->name);
        node = node->deep_tail;
        node_delete(save);
    }
    sTable[deep] = NULL;
}


//type
char* copy_name(char* name){
    if(name == NULL) return NULL;
    int len = strlen(name) + 1;
    char* copy = (char*) malloc(sizeof(char) * len);
    strncpy(copy, name, len);
//    if(debug) printf("copy_name %d %d\n", (int)name, (int)copy);
    return copy;
}

Type copy_type(Type type){
    if(type == NULL) return NULL;

    Type type1 = (Type)malloc(sizeof(Type_));
//    if(debug) printf("copy_type %d\n", type->kind);

    type1->kind = type->kind;

    if(type->kind == BASIC){
        type1->u.basic = type->u.basic;
    }else if(type->kind == ARRAY){
        type1->u.array.elem = copy_type(type->u.array.elem);
        type1->u.array.size = type->u.array.size;
    }else if(type->kind == STRUCTURE){
        Structure structure = (Structure)malloc(sizeof(Structure_));
        type1->u.structure = structure;
        type1->u.structure->name = copy_name(type->u.structure->name);
        type1->u.structure->domain = copy_field_list(type->u.structure->domain);
    }else{
        Function function = (Function)malloc(sizeof(Function_));
        type1->u.function = function;
        type1->u.function->name = copy_name(type->u.function->name);
        type1->u.function->line = type->u.function->line;
        type1->u.function->param = copy_field_list(type->u.function->param);
        type1->u.function->type = copy_type(type->u.function->type);
    }
    return type1;
}


int type_check(Type type_a, Type type_b) {
    if (type_a == NULL && type_b == NULL) {
        return 1;
    }
    if (type_a == NULL || type_b == NULL) {
        return 0;
    }
    if (type_a->kind != type_b->kind) {
        return 0;
    } else if (type_a->kind == BASIC) {
//        if(debug) printf("type_check:%d %d\n", type_a->u.basic, type_b->u.basic);
        if (type_a->u.basic == type_b->u.basic) {
            return 1;
        } else {
            return 0;
        }
    } else if (type_a->kind == ARRAY) {
        return type_check(type_a->u.array.elem, type_b->u.array.elem);
    } else if (type_a->kind == STRUCTURE) {
        FieldList f1 = type_a->u.structure->domain;
        FieldList f2 = type_b->u.structure->domain;
        return param_check(f1, f2);
    }else{
        return 0;
    }
}

//Fieldlist
FieldList copy_field_list(FieldList fieldList){
    if(fieldList == NULL) return NULL;

    FieldList fieldList1 = (FieldList)malloc(sizeof(FieldList_));

    fieldList1->type = copy_type(fieldList->type);
    fieldList1->name = copy_name(fieldList->name);
    if(fieldList->tail != NULL){
        fieldList1->tail = copy_field_list(fieldList->tail);
    }else{
        fieldList1->tail = NULL;
    }
    return fieldList1;

}
//function
int param_check(FieldList f1, FieldList f2){
    FieldList p1 = f1;
    FieldList p2 = f2;
    while(p1 != NULL && p2 != NULL){
//        if(debug) printf("%d %d\n", p1->type->kind, p2->type->kind);
        if(!type_check(p1->type, p2->type)){
            return 0;
        }
        p1 = p1->tail;
        p2 = p2->tail;
    }
    if(p1 == NULL && p2 == NULL){
        return 1;
    }else{
        return 0;
    }

}

void declare_func(Function func){
    if(func == NULL) return;

    for(int i = 0; i < func_num; i++){
        if(strcmp(func->name, funcTable[i]->name) == 0){
            if(type_check(func->type, funcTable[i]->type) == 0 ||
               param_check(func->param, funcTable[i]->param) == 0){
                char s[100] = {0};
                sprintf(s, "Inconsistent declaration of function \"%s\".", func->name);
                semantic_error(19, func->line, s);
            }
            return;
        }
    }
    funcTable[func_num] = func;
    func_num++;
}
Function undeclared_func(Function func){
    if(func == NULL) return NULL;

    for(int i = 0; i < func_num; i++) {
        if (strcmp(func->name, funcTable[i]->name) == 0) {
            return funcTable[i];
            //TODO
        }
    }
    return NULL;
}

void decfun_check(){
    for(int i = 0; i < func_num; i++){
        Node res = hash_check(funcTable[i]->name);
        if(res == NULL || res->param->type->kind != FUNCTION){
            char s[100] = {0};
            sprintf(s, "Undefined function \"%s\".", funcTable[i]->name);
            semantic_error(18, funcTable[i]->line, s);
        }else if(param_check(res->param->type->u.function->param, funcTable[i]->param) == 0){
            char s[100] = {0};
            sprintf(s, "Inconsistent declaration of function \"%s\".", funcTable[i]->name);
            semantic_error(19, funcTable[i]->line, s);
        }
    }
}

//tree

void ExtDefList(TreeNode* node){
    if(node == NULL) return;
    if(node->firstChild == NULL) return;
    if(debug) printf("ExtDefList:\n");
    ExtDef(node->firstChild);//遍历第一个节点
    ExtDefList(node->firstChild->nextSibling);//遍历兄弟节点
    if(debug) printf("ExtDefList:end\n");
}

void ExtDef(TreeNode* node){
    if(node == NULL) return;
    if(debug) printf("ExtDef:%s\n", node->firstChild->nextSibling->data->name);
    Type t = Specifier(node->firstChild);
    TreeNode* sibc_node = node->firstChild->nextSibling;
    if(strcmp(sibc_node->data->name, "ExtDecList") == 0){
        ExtDecList(sibc_node, t);
    }else if(strcmp(sibc_node->data->name, "FunDec") == 0){
        if(strcmp(sibc_node->nextSibling->data->name, "CompSt") == 0){
            Function func = FunDec(sibc_node, t);
            if (func == NULL) return;

            Type type = malloc(sizeof(Type_));
            type->kind = FUNCTION;
            type->u.function = func;
            Function f = undeclared_func(func);
            if(f == NULL){
                hash_struct_insert(func->name, type, 0);
            }else{
                hash_struct_insert(func->name, type, 0);
                if(param_check(f->param, func->param) == 0) {
                    char s[100] = {0};
                    sprintf(s, "Inconsistent declaration of function \"%s\".", f->name);
                    semantic_error(19, f->line, s);
                    f->param = copy_field_list(func->param);
                    f->line = func->line;
                    f->type = copy_type(func->type);
                }
            }
            CompSt(sibc_node->nextSibling, t);
        }else {
            func_flag = 1;
            Function func = FunDec(sibc_node, t);
            if (func == NULL) return;
            declare_func(func);
            func_flag = 0;
        }
    }

    if(debug) printf("ExtDef:end\n");
}

void ExtDecList(TreeNode* node, Type type){
    if(node == NULL) return;
    if(debug) printf("ExtDecList:\n");
    FieldList fieldList = VarDec(node->firstChild, type, 0);
    if(node->firstChild->nextSibling != NULL){
        ExtDecList(node->firstChild->nextSibling->nextSibling, type);
    }
}

Type Specifier(TreeNode* node){
    if(node == NULL) return NULL;
    if(debug) printf("Specifier:%s\n", node->firstChild->data->name);

    Type type = NULL;
    if(strcmp(node->firstChild->data->name, "TYPE") == 0){
        type = malloc(sizeof(Type_));
        type->kind = BASIC;

        if(strcmp(node->firstChild->data->val, "int") == 0){
            type->u.basic = 0;
        }else if(strcmp(node->firstChild->data->val, "float") == 0){
            type->u.basic = 1;
        }
        if(debug) printf("Specifier:%s\n", node->firstChild->data->val);
    }else if(strcmp(node->firstChild->data->name, "StructSpecifier") == 0){
        type = StructSpecifier(node->firstChild);
    }
    if(debug) printf("Specifier:end\n");
    return type;
}

Type StructSpecifier(TreeNode* node){
    if(node == NULL) return NULL;
    if(debug) printf("StructSpecifier:%s\n", node->firstChild->nextSibling->data->name);

    Type type;
    TreeNode* opttag = node->firstChild->nextSibling;

    if(strcmp(opttag->data->name, "Tag") == 0){
        Node tag = struct_check(opttag->firstChild->data->val);
        if(tag == NULL || tag->param->type->kind != STRUCTURE){
            char s[100] = {0};
            sprintf(s, "Undefined struct \"%s\".", opttag->firstChild->data->val);
            semantic_error(17, node->firstChild->data->linenum, s);
            return 0;
        }else{
            type = new_type(STRUCTURE, copy_name(tag->param->name), copy_field_list(tag->param->type->u.structure->domain));
            return type;
        }
    }else{

        type = new_type(STRUCTURE, NULL, NULL);
        Structure structure1 = (Structure)malloc(sizeof(Structure_));
        type->u.structure = structure1;

        if(strcmp(opttag->data->name, "OptTag") == 0) {
            Node p = hash_check(opttag->firstChild->data->val);
//            if (debug) printf("hash_check:%s\n", opttag->firstChild->data->name);
            if(opttag->firstChild->data->val != NULL && (struct_check(opttag->firstChild->data->val) != NULL || (p != NULL && p->deep == now_deep)) ){
                char s[100] = {0};
                sprintf(s, "Duplicated name \"%s\".", opttag->firstChild->data->val);
                semantic_error(16, node->firstChild->data->linenum, s);
                return NULL;
            }else{
                type->u.structure->name = opttag->firstChild->data->val;
                opttag = opttag->nextSibling;
            }
        }else{
            unnamed_func++;
            char s[10] = {0};
            sprintf(s, "%d", unnamed_func);
            type->u.structure->name = s;
        }
        now_deep++;
        //TODO
        if(strcmp(opttag->nextSibling->data->name, "DefList") == 0) {
            type->u.structure->domain = DefList(opttag->nextSibling, 1);
        }else{
            type->u.structure->domain = NULL;
        }

        if(debug) print_stack(now_deep);
        delete_stack(now_deep);
        now_deep--;

        hash_struct_insert(type->u.structure->name, type, 1);
        if(debug) printf("StructSpecifier:%s\n", node->firstChild->nextSibling->data->name);
        return type;

    }

}

FieldList VarDec(TreeNode* node, Type type, int structflags){
    if(node == NULL) return NULL;
    if(debug) printf("VarDec:%s\n", node->firstChild->data->val);

    if(strcmp(node->firstChild->data->name, "ID") == 0){
        Node v = hash_check(node->firstChild->data->val);
        Node st = struct_check(node->firstChild->data->val);
        FieldList fieldList = (FieldList)malloc(sizeof(FieldList_));
        if(structflags == 0 && (st != NULL || (v != NULL && v->deep == now_deep))){
            char s[100] = {0};
            sprintf(s, "Redefined variable \"%s\".", node->firstChild->data->val);
            semantic_error(3, node->firstChild->data->linenum, s);
            fieldList->name = node->firstChild->data->val;
            fieldList->type = type;
            fieldList->tail = NULL;
        }else if(structflags == 1 && (st != NULL || (v != NULL && v->deep == now_deep))){
            char s[100] = {0};
            sprintf(s, "Redefined field \"%s\".", node->firstChild->data->val);
            semantic_error(15, node->firstChild->data->linenum, s);
            fieldList->name = node->firstChild->data->val;
            fieldList->type = type;
            fieldList->tail = NULL;
        }else{
            if(func_flag == 0){
                hash_struct_insert(node->firstChild->data->val, type, 0);
            }
            fieldList->name = node->firstChild->data->val;
            fieldList->type = type;
            fieldList->tail = NULL;
        }
        return fieldList;
    }else{
        Type newtype = new_type(ARRAY, type, atoi(node->firstChild->nextSibling->nextSibling->data->val));
        return VarDec(node->firstChild, newtype, structflags);
    }
}

Function FunDec(TreeNode* node, Type type){
    if(node == NULL) return NULL;
    if(debug) printf("FunDec:\n");

    Function func = (Function)malloc(sizeof(Function_));
    func->type = type;
    func->name = node->firstChild->data->val;
    func->line = node->firstChild->data->linenum;

    Node f = hash_check(func->name);
    if(f != NULL && func_flag == 0 && f->param->type->kind == FUNCTION && f->deep == now_deep){
        char s[100] = {0};
        sprintf(s, "Redefined function \"%s\".", func->name);
        semantic_error(4, node->firstChild->data->linenum, s);
    }
    if(strcmp(node->firstChild->nextSibling->nextSibling->data->name, "VarList") == 0){
        func->param = VarList(node->firstChild->nextSibling->nextSibling);
    }else{
        func->param = NULL;
    }
    if(debug) printf("FunDec:end\n");
    return func;
}

FieldList VarList(TreeNode* node){
    if(node == NULL) return NULL;
    if(debug) printf("VarList:\n");

    FieldList fieldList = ParamDec(node->firstChild);
    if(node->firstChild->nextSibling != NULL){
        if(fieldList == NULL){
            fieldList = VarList(node->firstChild->nextSibling->nextSibling);
        }else {
            fieldList->tail = VarList(node->firstChild->nextSibling->nextSibling);
        }
    }else{
        fieldList->tail = NULL;
    }
    return fieldList;
}

FieldList ParamDec(TreeNode* node){
    if(node == NULL) return NULL;
    if(debug) printf("ParamDec:\n");

    Type t = Specifier(node->firstChild);
    return VarDec(node->firstChild->nextSibling, t, 0);
}

void CompSt(TreeNode* node, Type return_type){
    if(node == NULL) return;
    if(debug) printf("CompSt:\n");


    now_deep++;
    if(debug) print_stack(now_deep);
    if(strcmp(node->firstChild->nextSibling->data->name, "DefList") == 0){
        DefList(node->firstChild->nextSibling, 0);
        if(strcmp(node->firstChild->nextSibling->nextSibling->data->name, "StmtList") == 0){
            StmtList(node->firstChild->nextSibling->nextSibling, return_type);
        }
    }else if(strcmp(node->firstChild->nextSibling->data->name, "StmtList") == 0){
        StmtList(node->firstChild->nextSibling, return_type);
    }
    if(debug) print_stack(now_deep);
    delete_stack(now_deep);
    now_deep--;
    if(debug) printf("CompSt:end\n");
}

void StmtList(TreeNode* node, Type return_type){
    if(node == NULL) return;
    if(debug) printf("StmtList:\n");

    Stmt(node->firstChild, return_type);
    if(node->firstChild->nextSibling != NULL){
        StmtList(node->firstChild->nextSibling, return_type);
    }
}

void Stmt(TreeNode* node, Type return_type){
    if(node == NULL) return;
    if(debug) printf("Stmt:\n");

    if(strcmp(node->firstChild->data->name, "Exp") == 0){
        Exp(node->firstChild);
    }else if(strcmp(node->firstChild->data->name, "CompSt") == 0){
        CompSt(node->firstChild, return_type);
    }else if(strcmp(node->firstChild->data->name, "RETURN") == 0){
        Type type;
        type = Exp(node->firstChild->nextSibling);
        if(type_check(type, return_type) == 0){
            semantic_error(8, node->firstChild->data->linenum, "Type mismatched for return.");
        }
    }else if(strcmp(node->firstChild->data->name, "IF") == 0){
        Exp(node->firstChild->nextSibling->nextSibling);
        TreeNode *node1 = node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling;
        Stmt(node1, return_type);
        if(node1->nextSibling != NULL){
            Stmt(node1->nextSibling->nextSibling, return_type);
        }
    }else if(strcmp(node->firstChild->data->name, "WHILE") == 0){
        Exp(node->firstChild->nextSibling->nextSibling);
        TreeNode *node1 = node->firstChild->nextSibling->nextSibling->nextSibling->nextSibling;
        Stmt(node1, return_type);
    }
}

FieldList DefList(TreeNode* node, int structflags){
    if(node == NULL) return NULL;
    if(debug) printf("DefList:\n");

    FieldList fieldList = Def(node->firstChild, structflags);
    FieldList fieldList1 = fieldList;
    if(node->firstChild->nextSibling != NULL){
        while(fieldList1->tail != NULL){
            fieldList1 = fieldList1->tail;
        }
        fieldList1->tail = DefList(node->firstChild->nextSibling, structflags);
    }
    if(debug) printf("DefList:end\n");
    return fieldList;
}

FieldList Def(TreeNode* node, int structflags){
    if(node == NULL) return NULL;
    if(debug) printf("Def:\n");
    Type type = Specifier(node->firstChild);
    return DecList(node->firstChild->nextSibling, type, structflags);

}

FieldList DecList(TreeNode* node, Type return_type, int structflags){
    if(node == NULL) return NULL;
    if(debug) printf("DecList:\n");

    FieldList fieldList;

    fieldList = Dec(node->firstChild, return_type, structflags);
    if(node->firstChild->nextSibling != NULL){
        fieldList->tail = DecList(node->firstChild->nextSibling->nextSibling, return_type, structflags);
    }else{
        fieldList->tail = NULL;
    }
    if(debug) printf("DecList:end\n");
    return fieldList;
}

FieldList Dec(TreeNode* node, Type type, int structflags){
    if(node == NULL) return NULL;
    if(debug) printf("Dec:\n");

    if(node->firstChild->nextSibling == NULL){
        return VarDec(node->firstChild, type, structflags);
    }else{
        if(structflags != 0){
            semantic_error(15, node->firstChild->data->linenum, "Illegal initialize in struct.");
            return NULL;
        }else{
            FieldList fieldList = VarDec(node->firstChild, type, structflags);
            Type type1 = Exp(node->firstChild->nextSibling->nextSibling);
            if(type_check(type, type1) == 0){
                semantic_error(5, node->firstChild->data->linenum, "Type mismatched for assignment.");
                return NULL;
            }else{
                hash_struct_insert(fieldList->name, fieldList->type, 0);
            }
            if(type1 != NULL) type_delete(type1);
            return fieldList;
        }
    }
}

Type Exp(TreeNode* node){
    if(node == NULL) return NULL;
    if(debug) printf("Exp:\n");

    if(strcmp(node->firstChild->data->name, "Exp") == 0){
        if(strcmp(node->firstChild->nextSibling->data->name, "DOT") == 0 || strcmp(node->firstChild->nextSibling->data->name, "LB") == 0){
            //    | Exp LB Exp RB
            //    | Exp DOT ID
            if(strcmp(node->firstChild->nextSibling->data->name, "LB") == 0){
                Type type1 = Exp(node->firstChild);
                Type type2 = Exp(node->firstChild->nextSibling->nextSibling);
                Type ret = NULL;
                if(type1 != NULL && type1->kind != ARRAY){
                    char s[100] = {0};
                    sprintf(s, "\"%s\" is not an array.", node->firstChild->firstChild->data->val);
                    semantic_error(10, node->firstChild->data->linenum, s);
                }else if(type2 == NULL || type2->kind != BASIC || type2->u.basic != 0){
                    char s[100] = {0};
                    sprintf(s, "\"%s\" is not an integer.", node->firstChild->nextSibling->nextSibling->firstChild->data->val);
                    semantic_error(12, node->firstChild->data->linenum, s);
                }else{
                    ret = copy_type(type1->u.array.elem);
                }
                if(type1) type_delete(type1);
                if(type2) type_delete(type2);
                return ret;
            }else{
                Type type = Exp(node->firstChild);
                Type ret = NULL;
                if(type == NULL || type->kind != STRUCTURE){
                    semantic_error(13, node->firstChild->data->linenum, "Illegal use of \".\".");
                }else{
                    FieldList fieldList = type->u.structure->domain;
                    while(fieldList != NULL){
                        if(strcmp(fieldList->name, node->firstChild->nextSibling->nextSibling->data->val) == 0){
                            break;
                        }
                        fieldList = fieldList->tail;
                    }
                    if(fieldList == NULL){
                        char s[100] = {0};
                        sprintf(s, "Non-existent field \"%s\".", node->firstChild->nextSibling->nextSibling->data->val);
                        semantic_error(14, node->firstChild->data->linenum, s);
                    }else{
                        ret = copy_type(fieldList->type);
                    }
                }
                if(type) type_delete(type);
                return ret;
            }
        }else{
            //    Exp ASSIGNOP Exp
            //    | Exp AND Exp
            //    | Exp OR Exp
            //    | Exp RELOP Exp
            //    | Exp PLUS Exp
            //    | Exp MINUS Exp
            //    | Exp STAR Exp
            //    | Exp DIV Exp
            if(debug) printf("Exp:%s\n", node->firstChild->nextSibling->data->name);
            Type type1 = Exp(node->firstChild);
            Type type2 = Exp(node->firstChild->nextSibling->nextSibling);
            if(type1 == NULL || type2 == NULL){
                return NULL;
            }
            TreeNode *p = node->firstChild->firstChild;
            Type ret = NULL;
            if(strcmp(node->firstChild->nextSibling->data->name, "ASSIGNOP") == 0){
                if((p->nextSibling == NULL && strcmp(p->data->name, "ID") == 0) ||
                   (p->nextSibling != NULL && (strcmp(p->nextSibling->data->name, "DOT") == 0 || strcmp(p->nextSibling->data->name, "LB") == 0))){
                    if (type_check(type1, type2) == 0) {
                        semantic_error(5, node->firstChild->data->linenum, "Type mismatched for assignment.");
                    } else {
                        ret = copy_type(type1);
                    }
                }else{
                    semantic_error(6, node->firstChild->data->linenum, "The left-hand side of an assignment must be a variable.");
                }
            }else{
                if(type1 && type2 && (type1->kind == ARRAY || type2->kind == ARRAY)){
                    semantic_error(7, node->firstChild->data->linenum, "Type mismatched for operands.");
                }else if(type_check(type1, type2) == 0){
                    semantic_error(7, node->firstChild->data->linenum, "Type mismatched for operands.");
                }else{
                    if(type1 && type2){
                        ret = copy_type(type1);
                    }
                }
            }
            if(type1) type_delete(type1);
            if(type2) type_delete(type2);
            return ret;
        }
    }else if(strcmp(node->firstChild->data->name, "ID") == 0){
        if(node->firstChild->nextSibling != NULL){
            //    | ID LP Args RP
            //    | ID LP RP
            Node p = hash_check(node->firstChild->data->val);
            if(p == NULL){
                char s[100] = {0};
                sprintf(s, "Undefined function \"%s\".", node->firstChild->data->val);
                semantic_error(2, node->firstChild->data->linenum, s);
                return NULL;
            }else if(p->param->type->kind != FUNCTION){
                char s[100] = {0};
                sprintf(s, "\"%s\" is not a function.", node->firstChild->data->val);
                semantic_error(11, node->firstChild->data->linenum, s);
                return NULL;
            }else if(strcmp(node->firstChild->nextSibling->nextSibling->data->name, "Args") == 0){
                FieldList fieldList = Args(node->firstChild->nextSibling->nextSibling);
                if(param_check(p->param->type->u.function->param, fieldList) == 0){
                    char s[100] = {0};
                    sprintf(s, "Function \"%s\"() is not applicable for argument.", node->firstChild->data->val);
                    semantic_error(9, node->firstChild->data->linenum, s);
                    return NULL;
                }else{
                    return copy_type(p->param->type->u.function->type);
                }
            }else{
                if(p->param->type->u.function->param != NULL){
                    char s[100] = {0};
                    sprintf(s, "Function \"%s\"() is not applicable for argument.", node->firstChild->data->val);
                    semantic_error(9, node->firstChild->data->linenum, s);
                    return NULL;
                }else{
                    return copy_type(p->param->type->u.function->type);
                }
            }
        }else{
            //ID
            Node p = hash_check(node->firstChild->data->val);
            if(p == NULL){
                char s[100] = {0};
                sprintf(s, "Undefined variable \"%s\".", node->firstChild->data->val);
                semantic_error(1, node->firstChild->data->linenum, s);
                return NULL;
            }else{
                return copy_type(p->param->type);
            }
        }
    }else if(strcmp(node->firstChild->data->name, "LP") == 0){
        //LP EXP RP
        return Exp(node->firstChild->nextSibling);
    }else if(strcmp(node->firstChild->data->name, "NOT") == 0 || strcmp(node->firstChild->data->name, "MINUS") == 0){
        //    | MINUS Exp %prec UNMINUS
        //    | NOT Exp
        Type type = Exp(node->firstChild->nextSibling);
        Type ret = NULL;
        if(type == NULL || type->kind != BASIC){
            semantic_error(7, node->firstChild->data->linenum, "Type mismatched for operands.");
        }else{
            ret = copy_type(type);
        }
        if(type) type_delete(type);
        return ret;
    }else if(strcmp(node->firstChild->data->name, "INT") == 0){
        //INT
        if(debug) printf("Exp:INT\n");
        return new_type(BASIC, TYPE_INT);
    }else{
        //FLOAT
        if(debug) printf("Exp:FLOAT\n");
        return new_type(BASIC, TYPE_FLOAT);
    }
}

FieldList Args(TreeNode* node){
    if(node == NULL) return NULL;
    if(debug) printf("Args:\n");

    FieldList fieldList = (FieldList) malloc(sizeof(FieldList_));
    fieldList->type = Exp(node->firstChild);
    fieldList->name = NULL;

    if(node->firstChild->nextSibling != NULL){
        fieldList->tail = Args(node->firstChild->nextSibling->nextSibling);
    }else{
        fieldList->tail = NULL;
    }
    return fieldList;
}



