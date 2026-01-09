#include <stdlib.h>
#include "bst.h"



BST* createBST(int (*cmp)(void*, void*), void (*print)(void*), void (*freeData)(void*)){
    BST * newTree=(BST * )malloc(sizeof(BST));//creates the tree in the heap 
}
