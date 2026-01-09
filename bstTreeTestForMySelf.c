#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

// --- 1. Mock Data Structure (Simulating an Item) ---
typedef struct
{
    char *name;
    int value;
    int type; // 0 for ARMOR, 1 for SWORD
} MockItem;

// --- 2. Implement Helper Functions (Required by BST) ---

// Create a new mock item
MockItem *create_item(const char *name, int value, int type)
{
    MockItem *new_item = (MockItem *)malloc(sizeof(MockItem));
    new_item->name = strdup(name); // Uses string.h (allowed)
    new_item->value = value;
    new_item->type = type;
    return new_item;
}

// Compare function: Name -> Value -> Type
int compare_items(void *a, void *b)
{
    MockItem *itemA = (MockItem *)a;
    MockItem *itemB = (MockItem *)b;

    // 1. Compare Names
    int nameObj = strcmp(itemA->name, itemB->name);
    if (nameObj != 0)
        return nameObj;

    // 2. Compare Values (If names are equal)
    if (itemA->value != itemB->value)
    {
        return itemA->value - itemB->value;
    }

    // 3. Compare Type (If values are equal)
    return itemA->type - itemB->type;
}

// Print function
void print_item(void *data)
{
    MockItem *item = (MockItem *)data;
    printf("[%s] Val:%d Type:%d\n", item->name, item->value, item->type);
}

// Free function
void free_item(void *data)
{
    MockItem *item = (MockItem *)data;
    if (item)
    {
        free(item->name); // Free the internal string
        free(item);       // Free the struct itself
    }
}

// --- 3. Main Test Loop ---
int main()
{
    printf("=== Starting BST Test ===\n");

    // Initialize BST
    // You likely have a function like init_bst(compare, print, free)
    // Adjust the function name below to match your bst.h exactly!
    BST *tree = init_bst(compare_items, print_item, free_item);

    // Test Data
    printf("Inserting items...\n");
    // Should go to Root
    insert(tree, create_item("Midas", 10, 1));
    // Should go Left (Alphabetically 'A' < 'M')
    insert(tree, create_item("Armor", 5, 0));
    // Should go Right (Alphabetically 'Z' > 'M')
    insert(tree, create_item("Zelda", 10, 1));
    // Duplicate Name test: Same name, diff value
    // 'Midas', 5 < 10 -> Should go Left of Root
    insert(tree, create_item("Midas", 5, 1));

    // Test Traversals
    printf("\n--- Preorder (Root, Left, Right) ---\n");
    print_tree(tree, 0); // Assuming 0 is Preorder enum/const

    printf("\n--- Inorder (Left, Root, Right) ---\n");
    print_tree(tree, 1); // Assuming 1 is Inorder

    printf("\n--- Postorder (Left, Right, Root) ---\n");
    print_tree(tree, 2); // Assuming 2 is Postorder

    // Clean up
    printf("\nFreeing tree...\n");
    free_tree(tree); // Ensure this frees nodes AND data

    printf("Test finished. Run with Valgrind to check for leaks.\n");
    return 0;
}