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
void freeAllNodes(BST *tree)
{
    if (tree == NULL)
    {
        return;
    }
    if (tree->root)
    {
        /* code */
    }
    
    freeAllNodes(tree->root->left);
    freeAllNodes(tree->root->right);

    bstFree((tree->root), tree->freeData);
}

// Deletes The Tree and frees all the memory
void deleteTree(BST *tree)
{
    freeAllNodes(tree);
    free(tree);
}
