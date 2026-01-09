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

//return 
BSTNode *bstInsert(BSTNode *root, void *data, int (*cmp)(void *, void *))
{
    if (root=NULL)
    {
        root= (BSTNode *)malloc(sizeof(BSTNode));
        
    }
    int value =cmp(root->data,data);

    
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

    if (root->left != NULL)
    {
        free(root->left);
        root->left = NULL;
    }
    if (root->right != NULL)
    {
        free(root->right);
        root->right = NULL;
    }

    free(root);
}

// free all the nodes
void freeAllNodes(BSTNode *treeNode,void (*freeData)(void *))
{
    if (treeNode == NULL)
    {
        return;
    }

    
    freeAllNodes(treeNode->left,freeData);
    freeAllNodes(treeNode->right,freeData);

    bstFree((treeNode), freeData);
}

// Deletes The Tree and frees all the memory
void deleteTree(BST *tree)
{
    freeAllNodes(tree,tree->freeData);
    free(tree);
}
