#include <stdlib.h>
#include "bst.h"

// Creates the Binary Tree , get compare ,print and free data Functions,set the root to null
BST *createBST(int (*cmp)(void *, void *), void (*print)(void *), void (*freeData)(void *))
{
    BST *newTree = (BST *)malloc(sizeof(BST)); // allcoating meomry to the tree in the heap

    // Initialize the fields
    newTree->compare = cmp;
    newTree->print = print;
    newTree->freeData = freeData;
    newTree->root = NULL; // the root start as empty
    return newTree;
}

// return the root it was send but update it
BSTNode *bstInsert(BSTNode *root, void *data, int (*cmp)(void *, void *))
{
    if (root == NULL)
    {
        // if root null need to create a root node
        BSTNode *newNode = (BSTNode *)malloc(sizeof(BSTNode));
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    int value = cmp(root->data, data);
    if (value == LEFT_IS_BIGGER)
    {
        root->left = bstInsert(root->left, data, cmp);
    }
    else // if data is bigger than root data (should Not be equal)
    {
        root->right = bstInsert(root->right, data, cmp);
    }

    // return the updaed root
    return root;
}

// Delete root Data and free memory
void bstFree(BSTNode *root, void (*freeData)(void *))
{
    if (root == NULL)
    {
        return;
    }

    if (root->data != NULL)
    {
        freeData(root->data);
    }

    free(root);
}

// free all the nodes
void freeAllNodes(BSTNode *treeNode, void (*freeData)(void *))
{
    if (treeNode == NULL)
    {
        return;
    }

    freeAllNodes(treeNode->left, freeData);
    freeAllNodes(treeNode->right, freeData);

    bstFree((treeNode), freeData);
}

// Deletes The Tree and frees all the memory
void deleteTree(BST *tree)
{
    freeAllNodes(tree->root, tree->freeData);
    free(tree);
}



void *bstFind(BSTNode *root, void *data, int (*cmp)(void *, void *))
{


}



void bstInorder(BSTNode *root, void (*print)(void *));
void bstPreorder(BSTNode *root, void (*print)(void *));
void bstPostorder(BSTNode *root, void (*print)(void *));