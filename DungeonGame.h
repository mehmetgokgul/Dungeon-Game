#ifndef DUNGEONGAME_H
#define DUNGEONGAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 4
#define MAX_COLS 3

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

// Room structure representing each room in the dungeon
struct Room {
    char name[50];
    char description[200];
    struct Item items[10]; // Store up to 10 items per room
};

// Matrix of rooms, a 4x3 grid
extern struct Room rooms[MAX_ROWS][MAX_COLS];

// Function prototypes for in-game and menu actions
void move(int *row, int *col, const char *direction);
void look(int row, int col);
void inventory(struct PlayerStats *player);
void pickup(struct PlayerStats *player, int row, int col, const char *itemName);
void saveGame(struct PlayerStats *player, int row, int col, const char *filepath);
void loadGame(struct PlayerStats *player, int *row, int *col, const char *filepath);
void exitGame();
void readRoomData(const char *filename);

#endif // DUNGEONGAME_H
