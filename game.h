#ifndef GAME_H
#define GAME_H

#include "bst.h"

typedef enum
{
    ARMOR,
    SWORD
} ItemType;
typedef enum
{
    PHANTOM,
    SPIDER,
    DEMON,
    GOLEM,
    COBRA
} MonsterType;

// custom
typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} directions;
// their value are set automatly (0=Up,1=Down,2=Left,3=Right)
#define TRUE 1
#define FALSE 0

typedef struct Item
{
    char *name;
    ItemType type;
    int value;
} Item;

typedef struct Monster
{
    char *name;
    MonsterType type;
    int hp;
    int maxHp;
    int attack;
} Monster;

typedef struct Room
{
    int id;
    int x, y;
    int visited;
    Monster *monster;
    Item *item;
    struct Room *next;
} Room;

typedef struct Player
{
    int hp;
    int maxHp;
    int baseAttack;
    BST *bag;
    BST *defeatedMonsters;
    Room *currentRoom;
} Player;

typedef struct
{
    Room *rooms;
    Player *player;
    int roomCount;
    int configMaxHp;
    int configBaseAttack;
} GameState;

// Monster functions
void freeMonster(void *data);
int compareMonsters(void *a, void *b);
void printMonster(void *data);

// Item functions
void freeItem(void *data);
int compareItems(void *a, void *b);
void printItem(void *data);

// Game functions
void addRoom(GameState *g);
void initPlayer(GameState *g);
void playGame(GameState *g);
void freeGame(GameState *g);

// custom

// Room Management
void addRoom(GameState *g);
Room *createRoom(int x, int y, int roomCount);
void addRoomHelperGameState(GameState *g, Room *roomToAdd);
void removeRoomFromGameState(GameState *g, Room *roomToRemove);

// Search Functions
Room *findRoomByID(Room *head, int id);
Room *findRoomByPostion(Room *head, int x, int y);

// Entity Creation
Monster *addMonsterFunction(int userInputValue);
Item *addItemFunction(int userInputValue);

// Memory & Utils
void removeRoomFromMemory(Room *roomToRemove);
void freeMonster(void *data);


// print
void displayPlayerRoomDetails(Player *player);
void printMonster(void *data);
void printItem(void *a);
void showDefeated(GameState *g);
void openBag(GameState *g);

void printBST(BST *tree);


//move
int updatePosBaseOnDirection(int *x,int *y,int direction);
void movePlayer(GameState *g);




//compare
int compareMonsters(void *left, void *right);
int compareItems(void *left, void *right);

//fight 
void fightMonster(GameState *g);
void checkWin(GameState *g);

//pick up item 
void pickupItem(GameState *g);


#endif
