#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "utils.h"

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

void addRoom(GameState *g)
{
    if (g == NULL)
    {

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
    if (direction == UP)
    {
        --y;
    }
    else if (direction == DOWN)
    {
        ++y;
    }
    else if (direction == RIGHT)
    {
        ++x;
    }
    else if (direction == LEFT)
    {
        --x;
    }
    else
    {
        printf("Room exists there\n");
        return;
    }
    if (findRoomByPostion(g->rooms, x, y) != NULL)
    {
        printf("Room exists there\n");
        return;
    }

    // if we got here it means the it is a valid room placement
    int userAddMonster = getInt("Add monster? (1=Yes, 0=No): ");
    Monster *monster = addMonsterFunction(userAddMonster);
    int userAddItem = getInt("Add item? (1=Yes, 0=No): ");
    Item *item = addItem(userAddItem);

    // create the room
    Room *newRoom = (Room *)malloc(sizeof(Room));
    newRoom->id = g->roomCount;
    newRoom->x = x;
    newRoom->y = y;
    newRoom->visited=FALSE;
    newRoom->monster=monster;
    newRoom->item=item;

    addRoomHelper(g,newRoom);
    return;
    
}

void addRoomHelper(GameState *g, Room *roomToAdd)
{
    // empty gamesate
    if (g == NULL)
    {
        return;
    }
    // no rooms
    if (g->rooms == NULL)
    {
        g->rooms = roomToAdd;
        ++(g->roomCount);
        return;
    }
    // find the last id
    Room *theLastRoom = findRoomByID(g->rooms, g->roomCount - 1);

    theLastRoom->next = roomToAdd;
    ++(g->roomCount);
    return;
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

void RemoveMonster(Monster *monsterToRemove)
{
    if (monsterToRemove == NULL)
    {
        return;
    }
    // frees the name from the memory
    free(monsterToRemove->name);
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
