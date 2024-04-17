//
// Created by hudexu on 24-4-12.
//

#ifndef LAB_TREE_H
#define LAB_TREE_H

#include <stdbool.h>
// 定义节点类型
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_ID,
    TYPE_TYPE,
    TYPE_TOKEN,
    TYPE_NO,
} NodeType;

// 定义数据结构
typedef struct Data {
    int linenum; // 行号
    NodeType type; // 类型
    char* name; //名称
    char* val;  //值
} Data;

// 定义多叉树的节点结构
typedef struct TreeNode {
    Data *data;
    struct TreeNode *firstChild; // 第一个子节点
    struct TreeNode *nextSibling; // 下一个兄弟节点
} TreeNode;


#endif //LAB_TREE_H
