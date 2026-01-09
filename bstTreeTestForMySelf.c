#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

// --- Helper Functions for Integer Data ---

// Compare two integers
// Returns > 0 if a > b, < 0 if a < b, 0 if a == b
int compareInts(void *a, void *b)
{
    int intA = *(int *)a;
    int intB = *(int *)b;
    return intA - intB;
}

// Print an integer
void printInt(void *data)
{
    printf("%d ", *(int *)data);
}

// Free an integer
void freeInt(void *data)
{
    free(data);
}

// Wrapper to allocate memory for an int and insert it into the tree
// We MUST allocate memory because deleteTree calls freeData()
void insertIntValue(BST *tree, int value)
{
    int *newData = (int *)malloc(sizeof(int));
    if (!newData)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    *newData = value;
    
    // Note: We must update tree->root because the root might change (if tree was empty)
    tree->root = bstInsert(tree->root, newData, tree->compare);
}

// --- Main Test Driver ---

int main()
{
    printf("--- Starting BST Test ---\n");

    // 1. Create the BST
    BST *tree = createBST(compareInts, printInt, freeInt);
    if (tree == NULL)
    {
        printf("Failed to create tree.\n");
        return 1;
    }
    printf("Tree created successfully.\n");

    // 2. Insert Data
    // Tree structure plan:
    //      10
    //     /  \
    //    5    15
    //   / \   / \
    //  3   7 12  18
    
    printf("Inserting values: 10, 5, 15, 3, 7, 12, 18\n");
    insertIntValue(tree, 10);
    insertIntValue(tree, 5);
    insertIntValue(tree, 15);
    insertIntValue(tree, 3);
    insertIntValue(tree, 7);
    insertIntValue(tree, 12);
    insertIntValue(tree, 18);

    // 3. Test Traversals
    printf("\n--- Traversals ---\n");
    
    printf("Inorder (Should be sorted): ");
    bstInorder(tree->root, printInt);
    printf("\n");

    printf("Preorder (Root first):      ");
    bstPreorder(tree->root, printInt);
    printf("\n");

    printf("Postorder (Root last):      ");
    bstPostorder(tree->root, printInt);
    printf("\n");

    // 4. Test Find
    printf("\n--- Testing Find ---\n");
    
    int valToFind = 7;
    int valNotPresent = 99;

    // We pass &valToFind just for comparison logic; the tree won't free this pointer
    int *result = (int *)bstFind(tree->root, &valToFind, compareInts);
    
    if (result != NULL && *result == 7)
    {
        printf("PASS: Found value %d\n", *result);
    }
    else
    {
        printf("FAIL: Could not find value %d\n", valToFind);
    }

    result = (int *)bstFind(tree->root, &valNotPresent, compareInts);
    if (result == NULL)
    {
        printf("PASS: Correctly returned NULL for missing value %d\n", valNotPresent);
    }
    else
    {
        printf("FAIL: Found value %d but it should not exist\n", valNotPresent);
    }

    // 5. Memory Cleanup
    printf("\n--- Testing Cleanup ---\n");
    printf("Deleting tree (and freeing all integer data)...\n");
    deleteTree(tree);
    
    printf("Tree deleted. If you see this, no crashes occurred during free().\n");
    printf("Run with Valgrind to ensure no memory leaks.\n");

    return 0;
}