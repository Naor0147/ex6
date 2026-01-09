#ifndef BST_H
#define BST_H

//custom 
#define LEFT_IS_BIGGER 1
#define RIGHT_IS_BIGGER -1
#define EQUAL 0

typedef struct BSTNode
{
    void *data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct
{
    BSTNode *root;
    int (*compare)(void *, void *);//return 1 if left > right ,-1 if left<right , 0 left==right 
    void (*print)(void *);
    void (*freeData)(void *);
} BST;

BST *createBST(int (*cmp)(void *, void *), void (*print)(void *), void (*freeData)(void *));
BSTNode *bstInsert(BSTNode *root, void *data, int (*cmp)(void *, void *));
void *bstFind(BSTNode *root, void *data, int (*cmp)(void *, void *));
void bstInorder(BSTNode *root, void (*print)(void *));
void bstPreorder(BSTNode *root, void (*print)(void *));
void bstPostorder(BSTNode *root, void (*print)(void *));
void bstFree(BSTNode *root, void (*freeData)(void *));

// Custom
void DeleteTree(BST *tree); // delete The tree and free the memory
void freeAllNodes(BSTNode *treeNode,void (*freeData)(void *));

#endif
