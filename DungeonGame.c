#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DungeonGame.h"

PlayerStats player = {10, 5, {NULL}};
int currentRow = 0, currentCol = 0;

Room rooms[ROOMS_ROWS][ROOMS_COLS];
Creature creatures[CREATURES_ROWS][CREATURES_COLS];


void readRoomData(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    int c = fgetc(file);
    if (c == EOF) {
        if (feof(file)) {
            printf("Warning: Room data file is empty.\n");
        } else {
            perror("Error while reading the file");
        }
        fclose(file);
        return;
    }
    ungetc(c, file);

    char line[256];
    int x, y;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "[%d,%d] - %[^\n]", &x, &y, rooms[x][y].name) == 3) {
            fgets(rooms[x][y].info, sizeof(rooms[x][y].info), file);
            rooms[x][y].info[strcspn(rooms[x][y].info, "\n")] = '\0';

            int item_count = 0;
            while (fgets(line, sizeof(line), file) && line[0] != '[' && strncmp(line, "Creature:", 9) != 0) {
                line[strcspn(line, "\n")] = '\0';
                if (strlen(line) == 0) continue;

                char item_name[50], item_info[100];
                if (sscanf(line, "%[^'('] (%[^')'])", item_name, item_info) == 2) {
                    strcpy(rooms[x][y].items[item_count].name, item_name);
                    strcpy(rooms[x][y].items[item_count].info, item_info);
                    item_count++;
                }
            }

            if (strncmp(line, "Creature:", 9) == 0) {
                char creature_name[50], creature_info[100];
                int creature_health, creature_strength;
                if (sscanf(line + 9, "%[^:]:%[^:]:%d:%d", creature_name, creature_info, &creature_health, &creature_strength) == 4) {
                    strcpy(rooms[x][y].creature.name, creature_name);
                    strcpy(rooms[x][y].creature.info, creature_info);
                    rooms[x][y].creature.health = creature_health;
                    rooms[x][y].creature.strength = creature_strength;
                }
            } else {
                strcpy(rooms[x][y].creature.name, "NULL");
            }
        }
    }

    fclose(file);
}


void printDungeonData() {
    printf("=== Dungeon Map ===\n");
    for (int i = 0; i < ROOMS_ROWS; i++) {
        for (int j = 0; j < ROOMS_COLS; j++) {
            if (rooms[i][j].name[0] == '\0') continue;
            printf("\nRoom [%d,%d] - %s\n", i, j, rooms[i][j].name);
            printf("Description: %s\n", rooms[i][j].info);

            if (rooms[i][j].items[0].name[0] != '\0') {
                printf("Items:\n");
                for (int k = 0; k < 10 && rooms[i][j].items[k].name[0] != '\0'; k++) {
                    printf("  - %s (%s)\n", rooms[i][j].items[k].name, rooms[i][j].items[k].info);
                }
            }

            if (rooms[i][j].creature.name[0] != '\0' && strcmp(rooms[i][j].creature.name, "NULL") != 0) {
                printf("Creature: %s\n", rooms[i][j].creature.name);
                printf("  Description: %s\n", rooms[i][j].creature.info);
                printf("  Health: %d, Strength: %d\n", rooms[i][j].creature.health, rooms[i][j].creature.strength);
            }
        }
    }
    printf("\n=== End of Dungeon Map ===\n");
}

//In-Game Commands

void move(const char *direction) {
    if (strcmp(direction, "up") == 0 && currentRow > 0) {
        currentRow--;
    } else if (strcmp(direction, "down") == 0 && currentRow < ROOMS_ROWS - 1) {
        currentRow++;
    } else if (strcmp(direction, "right") == 0 && currentCol < ROOMS_COLS - 1) {
        currentCol++;
    } else if (strcmp(direction, "left") == 0 && currentCol > 0) {
        currentCol--;
    } else {
        printf("Invalid move.\n");
    }
}

void look() {
    printf("You are in %s. %s\n", rooms[currentRow][currentCol].name, rooms[currentRow][currentCol].info);

    if (strcmp(rooms[currentRow][currentCol].creature.name, "NULL") != 0) {
        printf("The Creature in this room is %s. %s\n", rooms[currentRow][currentCol].creature.name,
               rooms[currentRow][currentCol].creature.info);
        printf("HP: %d\nSP: %d\n", rooms[currentRow][currentCol].creature.health, rooms[currentRow][currentCol].creature.strength);
    }

    if (rooms[currentRow][currentCol].items[0].name[0] != '\0') {
        printf("Items in this room:\n");
        for (int i = 0; i < 10 && rooms[currentRow][currentCol].items[i].name[0] != '\0'; i++) {
            printf("  - %s (%s)\n", rooms[currentRow][currentCol].items[i].name, rooms[currentRow][currentCol].items[i].info);
        }
    }
}

void inventory() {
    printf("Your items:\n");
    for (int i = 0; i < 10 && player.inventory[i] != NULL; i++) {
        printf("%s\n", player.inventory[i]);
    }
}

void pickup(const char *itemName) {
    Creature *currentCreature = &creatures[currentRow][currentCol];

    if (strcmp(currentCreature->name, "NULL") != 0 && currentCreature->health > 0) {
        printf("You must fight first with the %s!\n", currentCreature->name);
        return;
    }

    for (int i = 0; i < 10 && rooms[currentRow][currentCol].items[i].name[0] != '\0'; i++) {
        if (strcmp(rooms[currentRow][currentCol].items[i].name, itemName) == 0) {
            for (int j = 0; j < 10; j++) {
    if (player.inventory[j] == NULL) {
        player.inventory[j] = strdup(rooms[currentRow][currentCol].items[i].name);
        printf("You have picked up: %s\n", itemName);
        rooms[currentRow][currentCol].items[i].name[0] = '\0'; 
        return;
    }
}

            printf("Your inventory is full!\n");
            return;
        }
    }

    printf("Item '%s' not found in this room.\n", itemName);
}

void attack() {
    Creature *currentCreature = &creatures[currentRow][currentCol];

    if (strcmp(currentCreature->name, "NULL") == 0 || currentCreature->health <= 0) {
        printf("No creature here to fight!\n");
        return;
    }

    printf("Battle begins with %s!\n", currentCreature->name);

    while (currentCreature->health > 0 && player.health > 0) {
        currentCreature->health -= player.strength;
        if (currentCreature->health < 0) currentCreature->health = 0;
        printf("%s takes %d damage. Remaining health: %d\n", currentCreature->name, player.strength, currentCreature->health);

        if (currentCreature->health > 0) {
            player.health -= currentCreature->strength;
            if (player.health < 0) player.health = 0;
            printf("You take %d damage. Remaining health: %d\n", currentCreature->strength, player.health);
        }
    }

    if (player.health > 0) {
        printf("You defeated %s!\n", currentCreature->name);
        currentCreature->health = 0; 
    } else {
        printf("You were defeated by %s. Game over!\n", currentCreature->name);
        exit(0);
    }
}



//Menu Commands

void list(){


    FILE *file = fopen("DungeonGameSavegames", "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    


}

void save(const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if (!file) {
        perror("Could not open save file");
        return;
    }

    fprintf(file, "CurrentRoom: [%d,%d]\n", currentRow, currentCol);
    fprintf(file, "PlayerStats: HP=%d, SP=%d, Inventory={", player.health, player.strength);
    int inventoryWritten = 0;
    for (int i = 0; i < 10; i++) {
        if (player.inventory[i] != NULL) {
            if (inventoryWritten > 0) {
                fprintf(file, ",");
            }
            fprintf(file, "%s", player.inventory[i]);
            inventoryWritten++;
        }
    }
    fprintf(file, "}\n");

    fprintf(file, "CreaturesStats:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (strcmp(creatures[i][j].name, "NULL") == 0) {
                fprintf(file, "[%d,%d]:NULL\n", i, j);
            } else if (creatures[i][j].health <= 0) {
                fprintf(file, "[%d,%d]:DEAD\n", i, j);
            } else {
                fprintf(file, "[%d,%d]:%s\n", i, j, creatures[i][j].name);
            }
        }
    }

    fprintf(file, "RoomsInventory:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(file, "[%d,%d]:", i, j);
            int itemWritten = 0;
            for (int k = 0; k < 10; k++) {
                if (rooms[i][j].items[k].name[0] != '\0') {
                    if (itemWritten > 0) {
                        fprintf(file, ",");
                    }
                    fprintf(file, "%s", rooms[i][j].items[k].name);
                    itemWritten++;
                }
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
    printf("Game saved successfully to %s\n", fileName);
}


void load(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Could not open save file");
        return;
    }

    int character = fgetc(file);
    if (character == EOF) {
        if (feof(file)) {
            printf("The save file is empty!\n");
        } else {
            printf("Error reading the save file!\n");
        }
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_SET);

    char line[256];

    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "CurrentRoom: [%d,%d]", &currentRow, &currentCol);
    }

    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "PlayerStats: HP=%d, SP=%d", &player.health, &player.strength);
        if (fgets(line, sizeof(line), file)) {
            char itemName[50];
            int inventoryIndex = 0;
            while (fgets(line, sizeof(line), file)) {
                if (strchr(line, '}')) break;
                sscanf(line, "%s", itemName);
                player.inventory[inventoryIndex++] = strdup(itemName);
            }
        }
    }

    if (fgets(line, sizeof(line), file)) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                if (fgets(line, sizeof(line), file)) {
                    int health;
                    char creatureName[50];
                    if (sscanf(line, "[%d,%d]:%s", &i, &j, creatureName) == 3) {
                        if (strcmp(creatureName, "NULL") != 0 && strcmp(creatureName, "DEAD") != 0) {
                            creatures[i][j].health = health;
                            strcpy(creatures[i][j].name, creatureName);
                        }
                    }
                }
            }
        }
    }

    fclose(file);
}




/*
void exitGame();
void readRoomData(const char *filename);
*/

int main() {
    //readRoomData("DungeonGameData.txt");
    //printDungeonData();
    
    save("DungeonGameSavegames.txt");
    load("DungeonGameSavegames.txt");

    return 0;
}
