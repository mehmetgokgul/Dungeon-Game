#ifndef DUNGEONGAME_H
#define DUNGEONGAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROOMS_ROWS 4
#define ROOMS_COLS 3
#define CREATURES_ROWS 4
#define CREATURES_COLS 3

typedef struct {
    int health;
    int strength;
    char *inventory[10]; 
}PlayerStats;

typedef struct {
    char name[50];
    char info[100];
}Item;

typedef struct {
    char name[50];
    char info[100];
    int health;
    int strength;   
}Creature;

typedef struct {
    char name[50];
    char info[200];
    Item items[10]; 
    Creature creature;  
}Room;

extern Room rooms[ROOMS_ROWS][ROOMS_COLS];
extern Creature creatures[CREATURES_ROWS][CREATURES_COLS];
extern Room currentRoom; 

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

#endif 
