#include <stdlib.h>
#include "bst.h"

//Creates the Binary Tree , get compare ,print and free data Functions,set the root to null
BST *createBST(int (*cmp)(void *, void *), void (*print)(void *), void (*freeData)(void *))
{
    BST *newTree = (BST *)malloc(sizeof(BST)); // allcoating meomry to the tree in the heap

    //Initialize the fields
    newTree->compare = cmp;
    newTree->print = print;
    newTree->freeData = freeData;
    newTree->root = NULL; //the root start as empty
    return newTree;
}
