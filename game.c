#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "utils.h"

static void printRoomLegend(GameState *g);
static void printLegendRecursive(Room *room);
static void displayMap(GameState *g);


static void printRoomLegend(GameState *g)
{
    if (g->roomCount > 0)
    {

        printf("=== ROOM LEGEND ===\n");
        printLegendRecursive(g->rooms);
        printf("===================\n");
    }

    if (g->player != NULL)
    {
        displayPlayerRoomDetails(g->player);
    }
}

// Helper function to print legend in descending order (Recursive)
static void printLegendRecursive(Room *room)
{
    if (room == NULL)
        return;

    // Recurse first to reach the end of the list
    printLegendRecursive(room->next);

    // Print details on the way back 
    char monsterStatus = (room->monster != NULL) ? 'V' : 'X';
    char itemStatus = (room->item != NULL) ? 'V' : 'X';

    printf("ID %d: [M:%c] [I:%c]\n", room->id, monsterStatus, itemStatus);
}

// Map display functions
static void displayMap(GameState *g)
{
    if (!g->rooms)
        return;

    // Find bounds
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    for (Room *r = g->rooms; r; r = r->next)
    {
        if (r->x < minX)
            minX = r->x;
        if (r->x > maxX)
            maxX = r->x;
        if (r->y < minY)
            minY = r->y;
        if (r->y > maxY)
            maxY = r->y;
    }

    int width = maxX - minX + 1;
    int height = maxY - minY + 1;

    // Create grid
    int **grid = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++)
    {
        grid[i] = malloc(width * sizeof(int));
        for (int j = 0; j < width; j++)
            grid[i][j] = -1;
    }

    for (Room *r = g->rooms; r; r = r->next)
        grid[r->y - minY][r->x - minX] = r->id;

    printf("=== SPATIAL MAP ===\n");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j] != -1)
                printf("[%2d]", grid[i][j]);
            else
                printf("    ");
        }
        printf("\n");
    }

    for (int i = 0; i < height; i++)
        free(grid[i]);
    free(grid);
}


/*==========
MAIN FUNCTION
===========*/


void playGame(GameState *g)
{
    if (g == NULL || g->player == NULL)
    {
        printf("Init player first\n");
        return;
    }
    int playing=1;

    while (playing)
    {
        g->player->currentRoom->visited = 1;//mark the room as visitied
        displayMap(g);
        printRoomLegend(g);    

        // 2. Sub-menu

        int choice = getInt("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\n");

        switch (choice) {
            case 1: movePlayer(g); break;
            case 2: fightMonster(g); break;
            case 3: pickupItem(g); break;
            case 4: openBag(g); break;
            case 5: showDefeated(g); break;
            case 6: playing = 0; break; // Go back to Main Menu
            default: break;
        }

    }
    

}

// Displays the current room details and player status
/* like
--- Room 0 ---
Monster: Jhony (HP:13)
Item: Midas
HP: 100/100
*/
void displayPlayerRoomDetails(Player *player)
{
    if (player == NULL || player->currentRoom == NULL)
    {
        return;
    }

    Room *currentRoom = player->currentRoom;

    printf("--- Room %d ---\n", currentRoom->id);

    // Print Monster details if one exists in the room
    if (currentRoom->monster != NULL)
    {
        printf("Monster: %s (HP:%d)\n", currentRoom->monster->name, currentRoom->monster->hp);
    }

    // Print Item details if one exists in the room
    if (currentRoom->item != NULL)
    {
        printf("Item: %s\n", currentRoom->item->name);
    }

    // Print Player HP status
    printf("HP: %d/%d\n", player->hp, player->maxHp);
}

GameState initGameState();

void addRoom(GameState *g)
{
    if (g->roomCount == 0)
    {
        Room *newRoom = createRoom(0, 0, 0);
        addRoomHelperGameState(g, newRoom);
        return;
    }

    displayMap(g);

    int roomId = getInt("Attach to room ID: ");
    Room *theRoom = findRoomByID(g->rooms, roomId);
    if (theRoom == NULL)
    {
        printf("Invalid ID\n");
        return;
    }
    int direction = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
    int x = theRoom->x;
    int y = theRoom->y;
    int isValidUpdate =updatePosBaseOnDirection(&x,&y,direction);
    if(isValidUpdate==FALSE)
    {
        printf("Room exists there\n");
        return;
    }

    if (findRoomByPostion(g->rooms, x, y) != NULL)
    {
        printf("Room exists there\n");
        return;
    }

    Room *newRoom = createRoom(x, y, g->roomCount);
    addRoomHelperGameState(g, newRoom);
    return;
}

Room *createRoom(int x, int y, int roomCount)
{
    int userAddMonster = getInt("Add monster? (1=Yes, 0=No): ");
    Monster *monster = addMonsterFunction(userAddMonster);
    int userAddItem = getInt("Add item? (1=Yes, 0=No): ");
    Item *item = addItemFunction(userAddItem);

    // create the room
    Room *newRoom = (Room *)malloc(sizeof(Room));
    newRoom->id = roomCount;
    newRoom->x = x;
    newRoom->y = y;
    newRoom->visited = FALSE;
    newRoom->monster = monster;
    newRoom->item = item;

    newRoom->next = NULL;

    return newRoom;
}

void addRoomHelperGameState(GameState *g, Room *roomToAdd)
{

    if (g == NULL)
        return;

    // If list is empty, new room becomes head
    if (g->rooms == NULL)
    {
        g->rooms = roomToAdd;
    }
    else
    {
        // Iterate to the end of the list using pointers
        Room *current = g->rooms;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = roomToAdd;
    }

    g->roomCount++;
    printf("Created room %d at (%d,%d)\n", roomToAdd->id, roomToAdd->x, roomToAdd->y);
}

Room *findRoomByID(Room *Room, int id)
{
    if (Room == NULL)
    {
        return NULL;
    }
    if (Room->id == id)
    {
        return Room;
    }
    return findRoomByID(Room->next, id);
}
Room *findRoomByPostion(Room *Room, int x, int y)
{
    if (Room == NULL)
    {
        return NULL;
    }
    if (Room->x == x && Room->y == y)
    {
        return Room;
    }
    return findRoomByPostion(Room->next, x, y);
}

Monster *addMonsterFunction(int userInputValue)
{
    if (userInputValue != TRUE)
    {
        return NULL;
    }
    // gets data from the user
    char *monsterName = getString("Monster name: ");
    int monsterType = getInt("Type (0-4): ");
    int monsterMaxHp = getInt("HP: ");
    int monsterAttack = getInt("Attack: ");

    // init memory allocation
    Monster *newMonster = (Monster *)malloc(sizeof(Monster));

    // Initialize all members
    newMonster->name = monsterName;
    newMonster->type = (MonsterType)monsterType; // Cast to enum type
    newMonster->maxHp = monsterMaxHp;
    newMonster->hp = monsterMaxHp;
    newMonster->attack = monsterAttack;

    return newMonster;
}
void freeMonster(void *data)
{
    if (data == NULL)
    {
        return;
    }
    Monster *monsterToRemove = (Monster *)data;
    // frees the name from the memory
    if (monsterToRemove->name != NULL)
    {
        free(monsterToRemove->name);
    }

    free(monsterToRemove);
    return;
}

Item *addItemFunction(int userInputValue)
{
    if (userInputValue != TRUE)
    {
        return NULL;
    }
    // gets data from the user
    char *itemName = getString("Item name: ");
    int itemType = getInt("Type (0=Armor, 1=Sword): ");
    int itemValue = getInt("Value: ");

    // init memory allocation
    Item *newItem = (Item *)malloc(sizeof(Item));

    // Initialize all members
    newItem->name = itemName;
    newItem->type = (ItemType)itemType; // Cast to enum type
    newItem->value = itemValue;

    return newItem;
}

void freeItem(void *data)
{
    if (data == NULL)
    {
        return;
    }
    Item *ItemToRemove = (Item *)data;
    // frees the name from the memory
    if (ItemToRemove->name != NULL)
    {
        free(ItemToRemove->name);
    }

    free(ItemToRemove);
    return;
}

void removeRoomFromGameState(GameState *g, Room *roomToRemove)
{
    if (g == NULL || g->rooms == NULL || roomToRemove == NULL)
    {
        return;
    }

    // Removing the head (first node)
    if (g->rooms == roomToRemove)
    {
        g->rooms = roomToRemove->next;
        removeRoomFromMemory(roomToRemove);
        g->roomCount--; // update count
        return;
    }

    // removing from middle or end
    Room *current = g->rooms;
    // looks at the next node in order to check if equal
    while (current->next != NULL && current->next != roomToRemove)
    {
        current = current->next;
    }

    // If we found it
    if (current->next == roomToRemove)
    {
        current->next = roomToRemove->next; // Unlink it
        removeRoomFromMemory(roomToRemove);
        g->roomCount--;
    }
}
void removeRoomFromMemory(Room *roomToRemove)
{
    if (roomToRemove == NULL)
    {
        return;
    }
    freeMonster(roomToRemove->monster);
    freeItem(roomToRemove->item);
    free(roomToRemove);
    return;
}

void initPlayer(GameState *g)
{
    if (g == NULL)
    {
        return;
    }
    if (g->rooms == NULL)
    {
        printf("Create rooms first\n");
        return;
    }
    if (g->player!=NULL)
    {
        printf("Player exists\n");
        return;
    }
    

    Player *newPlayer = (Player *)malloc(sizeof(Player));
    newPlayer->maxHp = g->configMaxHp;
    newPlayer->hp = g->configMaxHp;
    newPlayer->baseAttack = g->configBaseAttack;
    newPlayer->currentRoom = g->rooms;

    newPlayer->bag = (BST *)malloc(sizeof(BST));
    newPlayer->bag->compare = compareItems;
    newPlayer->bag->print = printItem;
    newPlayer->bag->freeData = freeItem;
    newPlayer->bag->root = NULL;

    newPlayer->defeatedMonsters = (BST *)malloc(sizeof(BST));
    newPlayer->defeatedMonsters->root = NULL;
    newPlayer->defeatedMonsters->compare = compareMonsters;
    newPlayer->defeatedMonsters->print = printMonster;
    newPlayer->defeatedMonsters->freeData = freeMonster;

    g->player = newPlayer;
}

/*==========
Bag Functions
=============*/

/*
 * Compares
 * 1. Name
 * 2. Value
 * 3. Type
 */
int compareItems(void *left, void *right)
{
    // Cast the generic void pointers back to Item pointers
    Item *itemLeft = (Item *)left;
    Item *itemRight = (Item *)right;

    // Sort: Name
    int nameCmp = strcmp(itemLeft->name, itemRight->name);
    if (nameCmp != 0)
    {
        return nameCmp;
    }

    // Sort: Value
    if (itemLeft->value != itemRight->value)
    {
        return itemLeft->value - itemRight->value;
    }

    // Sort: Type
    // ARMOR (0) comes before SWORD (1)
    // Since ARMOR is 0 and SWORD is 1
    // (0 - 1) is negative (Left), (1 - 0) is positive (Right)
    // Return 0 if equal
    return itemLeft->type - itemRight->type;
}
void printItem(void *a)
{
    Item *item = (Item *)a;

    const char *itemType = "ARMOR";
    if (item->type == SWORD)
    {
        itemType = "SWORD";
    }
    printf("[%s] %s - Value: %d\n", itemType, item->name, item->value);
}

/*
 * Compares
 * 1. Name
 * 2. Attack
 * 3. HP
 * 4. Type
 */
int compareMonsters(void *left, void *right)
{
    Monster *monsterLeft = (Monster *)left;
    Monster *monsterRight = (Monster *)right;
    int nameCmp = strcmp(monsterLeft->name, monsterRight->name);
    if (nameCmp != 0)
    {
        return nameCmp;
    }
    int attackDif = monsterLeft->attack - monsterRight->attack;
    if (attackDif != 0)
    {
        return attackDif;
    }
    if (monsterLeft->hp != monsterRight->hp)
    {
        return monsterLeft->hp - monsterRight->hp;
    }
    return monsterLeft->type - monsterRight->type;
}

void printMonster(void *data)
{
    Monster *monster = (Monster *)data;

    // Convert enum to string for printing
    const char *typeStr = "Unknown";
    switch (monster->type)
    {
    case PHANTOM:
        typeStr = "Phantom";
        break;
    case SPIDER:
        typeStr = "Spider";
        break;
    case DEMON:
        typeStr = "Demon";
        break;
    case GOLEM:
        typeStr = "Golem";
        break;
    case COBRA:
        typeStr = "Cobra";
        break;
    }
    printf("[%s] Type: %s, Attack: %d, HP: %d\n", monster->name, typeStr, monster->attack, monster->hp);
}

// free memory
void freeGame(GameState *g)
{
    if (g == NULL)
    {
        return;
    }
    if (g->player != NULL)
    {
        deleteTree(g->player->bag);
        deleteTree(g->player->defeatedMonsters);
        free(g->player);
        g->player = NULL;
    }
    if (g->rooms != NULL)
    {
        Room *currentRoom = g->rooms;
        while (currentRoom != NULL)
        {
            Room *next = currentRoom->next;
            removeRoomFromMemory(currentRoom);
            currentRoom = next;
        }
    }
    free(g);
}


void movePlayer(GameState *g){
    int direction = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
    int x=g->player->currentRoom->x;
    int y=g->player->currentRoom->y;
    updatePosBaseOnDirection(&x,&y,direction);
    Room* room = findRoomByPostion(g->rooms,x,y);
    if (room==NULL)
    {
        return;
    }
    g->player->currentRoom=room;
    return;
    
    
    

}

int updatePosBaseOnDirection(int *x,int *y,int direction){
    if (direction == UP)
    {
        --(*y);
        return TRUE;
    }
    else if (direction == DOWN)
    {
        ++(*y);
        return TRUE;

    }
    else if (direction == RIGHT)
    {
        ++(*x);
        return TRUE;

    }
    else if (direction == LEFT)
    {
        --(*x);
        return TRUE;

    }
    return FALSE;
}

void fightMonster(GameState *g)
{
    Player * myPlayer=g->player;
    Monster* monster = myPlayer->currentRoom->monster;
    if (monster==NULL)
    {
        printf("No monster\n");
        return;
    }

    while (1)
    {
        monster->hp-=myPlayer->baseAttack;
        if (monster->hp<=0)
        {
            //monster died
            printf("You deal %d damage. Monster HP: 0\n",myPlayer->baseAttack);
            printf("Monster defeated! ");
            freeMonster(monster);
            monster=NULL;
            myPlayer->currentRoom->monster=NULL;
            return;
        }
        printf("You deal %d damage. Monster HP: %d\n",myPlayer->baseAttack,monster->hp);
        myPlayer->hp-=monster->attack;
        

        
            
    }
    

    
}
