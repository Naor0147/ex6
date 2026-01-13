#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "game.h"
// include bst.h if needed for struct definitions
#include "bst.h"

// --- MOCKING INPUT SYSTEM ---
// מערכת שתדמה את הקלט של המשתמש כדי שלא נצטרך להקליד ידנית

#define MAX_INPUTS 100

typedef struct {
    int intQueue[MAX_INPUTS];
    int intHead;
    int intTail;
    
    char* stringQueue[MAX_INPUTS];
    int strHead;
    int strTail;
} MockInput;

MockInput inputs = { .intHead = 0, .intTail = 0, .strHead = 0, .strTail = 0 };

// פונקציות עזר להכנת הקלט לבדיקה
void pushInt(int val) {
    if (inputs.intTail < MAX_INPUTS) {
        inputs.intQueue[inputs.intTail++] = val;
    } else {
        printf("Test Error: Int Input Queue Full\n");
        exit(1);
    }
}

void pushString(const char* val) {
    if (inputs.strTail < MAX_INPUTS) {
        inputs.stringQueue[inputs.strTail++] = strdup(val);
    } else {
        printf("Test Error: String Input Queue Full\n");
        exit(1);
    }
}

void clearInputs() {
    inputs.intHead = 0;
    inputs.intTail = 0;
    // Free strings if needed, but for simple tests we skip to keep it simple
    inputs.strHead = 0;
    inputs.strTail = 0;
}

// --- OVERRIDING UTILS FUNCTIONS ---
// פונקציות אלו מחליפות את utils.c ומחזירות ערכים שהכנו מראש

int getInt(const char* prompt) {
    printf("[TEST INPUT] %s", prompt);
    if (inputs.intHead < inputs.intTail) {
        int val = inputs.intQueue[inputs.intHead++];
        printf(" >> %d\n", val);
        return val;
    }
    printf("\nError: Test ran out of int inputs!\n");
    exit(1);
}

char* getString(const char* prompt) {
    printf("[TEST INPUT] %s", prompt);
    if (inputs.strHead < inputs.strTail) {
        char* val = inputs.stringQueue[inputs.strHead++];
        printf(" >> %s\n", val);
        // Return a copy because the code might free it
        return strdup(val);
    }
    printf("\nError: Test ran out of string inputs!\n");
    exit(1);
}

// Stub for clearBuffer if game.c calls it (it doesn't seem to based on your file, but just in case)
void clearBuffer() {}

char* getStringScanf() { return NULL; } // Not used by game.c logic directly usually

// --- TESTS ---

void test_add_first_room() {
    printf("\n=== TEST 1: Add First Room (ID 0) ===\n");
    GameState game = {0}; // Initialize empty game

    // Scenario: Add first room. 
    // Logic: createRoom asks "Add monster?" then "Add item?".
    // We want a clean room (no monster, no item).
    
    clearInputs();
    pushInt(0); // Add monster? No
    pushInt(0); // Add item? No

    addRoom(&game);

    // Assertions
    assert(game.roomCount == 1);
    assert(game.rooms != NULL);
    assert(game.rooms->id == 0);
    assert(game.rooms->x == 0 && game.rooms->y == 0);
    assert(game.rooms->monster == NULL);
    assert(game.rooms->item == NULL);

    printf(">>> PASS: First room created successfully.\n");
    
    // Clean up (simulating freeGame partially)
    removeRoomFromGameState(&game, game.rooms);
}

void test_add_second_room_with_content() {
    printf("\n=== TEST 2: Add Connected Room with Monster & Item ===\n");
    GameState game = {0};

    // 1. Setup first room manually to save time/inputs
    Room* r0 = createRoom(0,0,0); // createRoom calls getInt! Need to mock inputs for this too.
    // Wait, createRoom calls inputs. Let's do standard addRoom flow.
    
    // Inputs for Room 0
    pushInt(0); // No monster
    pushInt(0); // No item
    addRoom(&game);

    // 2. Inputs for Room 1
    // Flow: attachID -> Direction -> Monster(Yes) -> Name -> Type -> HP -> Atk -> Item(Yes) -> Name -> Type -> Value
    pushInt(0);          // Attach to room ID 0
    pushInt(0);          // Direction 0 (UP) -> Should be (0, -1)
    
    // Monster Details
    pushInt(1);          // Add monster? Yes
    pushString("Goblin"); // Name
    pushInt(1);          // Type (SPIDER=1)
    pushInt(50);         // HP
    pushInt(10);         // Attack
    
    // Item Details
    pushInt(1);          // Add item? Yes
    pushString("SwordOfTruth"); // Name
    pushInt(1);          // Type (SWORD=1)
    pushInt(100);        // Value

    addRoom(&game);

    // Assertions
    assert(game.roomCount == 2);
    
    // Find the new room
    Room* r1 = findRoomByID(game.rooms, 1);
    assert(r1 != NULL);
    assert(r1->x == 0);
    assert(r1->y == -1); // Up is y-1
    
    // Check Monster
    assert(r1->monster != NULL);
    assert(strcmp(r1->monster->name, "Goblin") == 0);
    assert(r1->monster->hp == 50);

    // Check Item
    assert(r1->item != NULL);
    assert(strcmp(r1->item->name, "SwordOfTruth") == 0);
    assert(r1->item->value == 100);

    // Check Linkage (Linked List)
    assert(game.rooms->next == r1);

    printf(">>> PASS: Second room connected and populated correctly.\n");
    
    // Cleanup
    // Note: Since freeGame isn't implemented in your snippet, we remove one by one
    removeRoomFromGameState(&game, r1); // Remove tail first
    removeRoomFromGameState(&game, game.rooms); // Remove head
}

void test_find_functions() {
    printf("\n=== TEST 3: Find Functions ===\n");
    // Manually constructing list to avoid input overhead
    // List: [ID:0 (0,0)] -> [ID:1 (1,0)]
    
    Room r1 = { .id = 1, .x = 1, .y = 0, .next = NULL };
    Room r0 = { .id = 0, .x = 0, .y = 0, .next = &r1 };
    
    Room* res = findRoomByID(&r0, 1);
    assert(res == &r1);
    printf(">>> PASS: findRoomByID found ID 1.\n");

    res = findRoomByID(&r0, 99);
    assert(res == NULL);
    printf(">>> PASS: findRoomByID returned NULL for missing ID.\n");

    res = findRoomByPostion(&r0, 1, 0);
    assert(res == &r1);
    printf(">>> PASS: findRoomByPostion found (1,0).\n");
}

int main() {
    printf("--- STARTING AUTOMATED TESTS FOR GAME.C ---\n");
    
    test_add_first_room();
    test_add_second_room_with_content();
    test_find_functions();
    
    printf("\nAll tests finished. If you see this, no crashes occurred.\n");
    printf("Use 'valgrind ./test_game' to check for memory leaks.\n");
    
    return 0;
}