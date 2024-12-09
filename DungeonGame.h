#ifndef DUNGEONGAME_H
#define DUNGEONGAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROOMS_ROWS 4
#define ROOMS_COLS 3
#define CREATURES_ROWS 4
#define CREATURES_COLS 3

// Player structure holding health, strength, and inventory
typedef struct {
    int health;
    int strength;
    char *inventory[10]; 
}PlayerStats;

// Item structure with a name and description
typedef struct {
    char name[50];
    char info[100];
    int healthBonus;
    int strengthBonus;
}Item;

// Creature structure with health, strength, and description
typedef struct {
    char name[50];
    char info[100];
    int health;
    int strength;   
}Creature;

// Room structure representing each room in the dungeon
typedef struct {
    char name[50];
    char info[200];
    Item items[10]; // Store up to 10 items per room
    Creature creature;  // Creature in the room
}Room;

// Matrix of rooms, a 4x3 grid
extern Room rooms[ROOMS_ROWS][ROOMS_COLS];
extern Creature creatures[CREATURES_ROWS][CREATURES_COLS];
extern Room currentRoom; 

// Function prototypes for in-game and menu actions
void move(const char *direction);
void look();
void inventory();
void pickup(const char *itemName);
void attack();

void list();
void saveGame(const char *filepath);
void loadGame(const char *filepath);
void exitGame();
void readRoomData(const char *filename);

#endif // DUNGEONGAME_H
