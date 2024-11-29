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
struct PlayerStats {
    int health;
    int strength;
    char *inventory[10]; 
};

// Item structure with a name and description
struct Item {
    char name[50];
    char info[100];
};

// Creature structure with health, strength, and description
struct Creature {
    char name[50];
    char info[100];
    int health;
    int strength;   
};

// Room structure representing each room in the dungeon
struct Room {
    char name[50];
    char info[200];
    struct Item items[10]; // Store up to 10 items per room
    struct Creature creature;  // Creature in the room
};

// Matrix of rooms, a 4x3 grid
extern struct Room rooms[ROOMS_ROWS][ROOMS_COLS];
extern struct Creature creatures[CREATURES_ROWS][CREATURES_COLS];
extern struct Room currentRoom; 

// Function prototypes for in-game and menu actions
void move(const char *direction);
void look();
void inventory();
void pickup(const char *itemName);
void saveGame(struct PlayerStats *player, int row, int col, const char *filepath);
void loadGame(struct PlayerStats *player, int *row, int *col, const char *filepath);
void exitGame();
void readRoomData(const char *filename);

#endif // DUNGEONGAME_H
