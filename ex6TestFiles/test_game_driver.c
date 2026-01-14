#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "game.h"
#include "bst.h"

// --- MOCKING INPUT SYSTEM ---
#define MAX_INPUTS 500 // הגדלנו את המקום לקלטים

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

void clearInputs() {
    inputs.intHead = 0;
    inputs.intTail = 0;
    for (int i = 0; i < inputs.strTail; i++) free(inputs.stringQueue[i]);
    inputs.strHead = 0;
    inputs.strTail = 0;
}

// --- OVERRIDING UTILS ---
int getInt(const char* prompt) {
    // printf("[TEST INPUT] %s", prompt); // צמצמנו את ההדפסה כדי שהמפה תבלוט
    if (inputs.intHead < inputs.intTail) {
        int val = inputs.intQueue[inputs.intHead++];
        // printf(" >> %d\n", val);
        return val;
    }
    printf("\nError: Test ran out of int inputs! Req: '%s'\n", prompt);
    exit(1);
}

char* getString(const char* prompt) {
    if (inputs.strHead < inputs.strTail) {
        return strdup(inputs.stringQueue[inputs.strHead++]);
    }
    exit(1);
}

void clearBuffer() {}
char* getStringScanf() { return NULL; }

// --- HELPER FOR VISUAL TEST ---
// פונקציה שמוסיפה חדר ידנית ללא שימוש ב-scanf
void force_add_room(GameState *g, int id, int x, int y) {
    Room* r = (Room*)malloc(sizeof(Room));
    r->id = id;
    r->x = x;
    r->y = y;
    r->visited = 0;
    r->monster = NULL;
    r->item = NULL;
    r->next = NULL;

    if (g->rooms == NULL) {
        g->rooms = r;
    } else {
        Room* curr = g->rooms;
        while(curr->next) curr = curr->next;
        curr->next = r;
    }
    g->roomCount++;
}

// --- TESTS ---

void test_spiral_map() {
    printf("\n=== TEST 4: Large Spiral Map Visualization ===\n");
    clearInputs();
    GameState game = {0};

    // 1. בניית ספירלה ידנית של 50 חדרים
    int x = 0, y = 0;
    int steps = 1;
    int id = 0;
    
    // מוסיפים את החדר הראשון
    force_add_room(&game, id++, x, y);

    // אלגוריתם ליצירת ספירלה
    for (int i = 0; i < 12; i++) { // מספר לולאות
        // Right
        for(int j=0; j<steps; j++) force_add_room(&game, id++, ++x, y);
        // Up (y-1)
        for(int j=0; j<steps; j++) force_add_room(&game, id++, x, --y);
        steps++;
        // Left
        for(int j=0; j<steps; j++) force_add_room(&game, id++, --x, y);
        // Down (y+1)
        for(int j=0; j<steps; j++) force_add_room(&game, id++, x, ++y);
        steps++;
    }

    printf("Generated %d rooms in a spiral pattern.\n", id);
    printf("Triggering displayMap via addRoom...\n");

    // 2. הכנת קלט שיגרום ל-addRoom להדפיס ואז לצאת
    // אנחנו מנסים לחבר חדר לחדר 0 בכיוון שכבר תפוס (למשל Right שזה (1,0))
    // זה יגרום לפונקציה להדפיס "Room exists there" ולסיים, אבל אחרי שהדפיסה את המפה!
    
    pushInt(0); // Attach to ID 0
    pushInt(3); // Direction Right (שם נמצא חדר ID 1 בספירלה שלנו)
    
    // קריאה לפונקציה
    addRoom(&game);

    // ניקוי (פשוט מאוד)
    Room* curr = game.rooms;
    while(curr) {
        Room* next = curr->next;
        free(curr);
        curr = next;
    }
}

int main() {
    test_spiral_map();
    return 0;
}