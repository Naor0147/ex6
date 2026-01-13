#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "game.h"
#include "bst.h"

// --- MOCKING INPUT SYSTEM ---
// מערכת שמדמה את הקלט של המשתמש

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

// פונקציה לאיפוס הקלטים לפני כל בדיקה
void clearInputs() {
    inputs.intHead = 0;
    inputs.intTail = 0;
    
    // ניקוי מחרוזות ישנות (לא קריטי בטסט פשוט אבל טוב ליתר ביטחון)
    for (int i = 0; i < inputs.strTail; i++) {
        free(inputs.stringQueue[i]);
    }
    inputs.strHead = 0;
    inputs.strTail = 0;
}

// --- OVERRIDING UTILS FUNCTIONS ---
// פונקציות אלו מחליפות את utils.c

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
        return strdup(val);
    }
    printf("\nError: Test ran out of string inputs!\n");
    exit(1);
}

void clearBuffer() {}
char* getStringScanf() { return NULL; }

// --- TESTS ---

void test_add_first_room() {
    printf("\n=== TEST 1: Add First Room (ID 0) ===\n");
    clearInputs(); // איפוס הקלט
    
    GameState game = {0}; 
    
    // Inputs: No monster, No item
    pushInt(0); 
    pushInt(0); 

    addRoom(&game);

    assert(game.roomCount == 1);
    assert(game.rooms != NULL);
    assert(game.rooms->id == 0);
    assert(game.rooms->monster == NULL);

    printf(">>> PASS: First room created successfully.\n");
    
    // ניקוי זיכרון חלקי
    removeRoomFromGameState(&game, game.rooms);
}

void test_add_second_room_with_content() {
    printf("\n=== TEST 2: Add Connected Room with Monster & Item ===\n");
    clearInputs(); // איפוס חשוב!
    
    GameState game = {0};

    // --- שלב 1: יצירת החדר הראשון (ID 0) ---
    // נדרש כי לא ניתן להוסיף חדר שני בלי חדר ראשון
    pushInt(0); // Monster? No
    pushInt(0); // Item? No
    addRoom(&game); 

    // --- שלב 2: הוספת החדר השני (ID 1) ---
    // סדר הקלטים הצפוי ב-addRoom לחדר שני:
    // 1. Attach ID
    // 2. Direction
    // 3. Add Monster? (אם כן -> שם, סוג, חיים, התקפה)
    // 4. Add Item? (אם כן -> שם, סוג, ערך)
    
    pushInt(0);          // Attach to room ID: 0
    pushInt(0);          // Direction: 0 (UP)
    
    // פרטי מפלצת
    pushInt(1);          // Add monster? Yes
    pushString("Goblin"); // Monster Name
    pushInt(1);          // Type (SPIDER=1)
    pushInt(50);         // HP
    pushInt(10);         // Attack
    
    // פרטי חפץ
    pushInt(1);          // Add item? Yes
    pushString("SwordOfTruth"); // Item Name
    pushInt(1);          // Type (SWORD=1)
    pushInt(100);        // Value

    addRoom(&game); // הקריאה השנייה לפונקציה

    // --- בדיקות (Assertions) ---
    assert(game.roomCount == 2);
    
    Room* r1 = findRoomByID(game.rooms, 1);
    assert(r1 != NULL);
    // בדיקת מיקום: UP אומר y-1
    assert(r1->x == 0);
    assert(r1->y == -1); 
    
    // בדיקת מפלצת
    assert(r1->monster != NULL);
    assert(strcmp(r1->monster->name, "Goblin") == 0);
    assert(r1->monster->hp == 50);

    // בדיקת חפץ
    assert(r1->item != NULL);
    assert(strcmp(r1->item->name, "SwordOfTruth") == 0);
    assert(r1->item->value == 100);

    // בדיקת קישוריות
    assert(game.rooms->next == r1);

    printf(">>> PASS: Second room connected and populated correctly.\n");
    
    // שחרור זיכרון (ידני כי freeGame עדיין לא מומש)
    removeRoomFromGameState(&game, r1); 
    removeRoomFromGameState(&game, game.rooms); 
}

void test_find_functions() {
    printf("\n=== TEST 3: Find Functions ===\n");
    
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
    
    return 0;
}