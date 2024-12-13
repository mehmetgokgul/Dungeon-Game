#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DungeonGame.h"
#include <sys/stat.h>
#include <dirent.h>

PlayerStats player = {10, 5, {NULL}};
int currentRow = 0, currentCol = 0;

Room rooms[ROOMS_ROWS][ROOMS_COLS];
Creature creatures[CREATURES_ROWS][CREATURES_COLS];
Room currentRoom;

void loadDefaultGame(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (!file) return;

    char line[256];
    int currentRoomSet = 0;
    int row, col;  
    char roomName[100], roomInfo[100], itemName[100], itemInfo[100];  

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "#Rooms", 6) == 0) continue;
        else if (strncmp(line, "#Creatures", 10) == 0) continue;
        else if (strncmp(line, "#Items", 6) == 0) continue;

        if (sscanf(line, "[%d,%d]-%[^-]-%[^-]-%[^-]-%[^-]", &row, &col, roomName, roomInfo, itemName, itemInfo) == 6) {
            strcpy(rooms[row][col].name, roomName);
            strcpy(rooms[row][col].info, roomInfo);
            strcpy(rooms[row][col].item.name, itemName);
            strcpy(rooms[row][col].item.info, itemInfo);

            if (!currentRoomSet) {
                currentRow = row;
                currentCol = col;
                currentRoomSet = 1;
            }
        }
    }
    fclose(file);
    printf("Default game loaded from %s\n", fileName);
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
int move(const char *direction) {
    if (strcmp(direction, "up") == 0 && currentRow > 0) {
        currentRow--;
    } else if (strcmp(direction, "down") == 0 && currentRow < ROOMS_ROWS - 1) {
        currentRow++;
    } else if (strcmp(direction, "right") == 0 && currentCol < ROOMS_COLS - 1) {
        currentCol++;
    } else if (strcmp(direction, "left") == 0 && currentCol > 0) {
        currentCol--;
    } else {
        printf("You can't move further in this direction. Try again.\n");
        return -1; 
    }
    printf("You moved to [%d,%d]\n", currentRow, currentCol);
    return 0; 
}


void look() {
    printf("You are in %s. %s\n", 
        rooms[currentRow][currentCol].name, 
        rooms[currentRow][currentCol].info);

    if (strcmp(rooms[currentRow][currentCol].creature.name, "NULL") != 0) {
        printf("The Creature in this room is %s. %s\n", 
            rooms[currentRow][currentCol].creature.name, 
            rooms[currentRow][currentCol].creature.info);
        printf("HP: %d\nSP: %d\n", 
            rooms[currentRow][currentCol].creature.health, 
            rooms[currentRow][currentCol].creature.strength);
    }

    if (rooms[currentRow][currentCol].items[0].name[0] != '\0') {
        printf("Items in this room:\n");
        for (int i = 0; i < 10 && rooms[currentRow][currentCol].items[i].name[0] != '\0'; i++) {
            printf("  - %s (%s)\n", 
                rooms[currentRow][currentCol].items[i].name, 
                rooms[currentRow][currentCol].items[i].info);
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
                    player.health += rooms[currentRow][currentCol].items[i].healthBonus;  
                    player.strength += rooms[currentRow][currentCol].items[i].strengthBonus;  
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

void list() {
    DIR *dir = opendir("Savegames");
    if (!dir) {
        perror("Could not open Savegames directory");
        return;
    }

    struct dirent *entry;
    int savedGamesFound = 0;
    printf("Saved Games:\n");

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt") != NULL) {
            printf("%s\n", entry->d_name);
            savedGamesFound = 1;
        }
    }

    if (!savedGamesFound) {
        printf("No saved games found.\n");
    }

    closedir(dir);
}

void save(const char *filepath) {
    struct stat st = {0};
    if (stat("Savegames", &st) == -1) {
        mkdir("Savegames", 0700);
    }

    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "Savegames/%s", filepath);

    FILE *file = fopen(fullPath, "w");
    if (file) {
        fprintf(file, "CurrentRoom: %s\n", currentRoom.name);
        fprintf(file, "PlayerStats: HP=%d, SP=%d, Inventory={", player.health, player.strength);
        for (int i = 0; i < 10 && player.inventory[i] != NULL; i++) {
            fprintf(file, "%s", player.inventory[i]);
            if (i < 9 && player.inventory[i + 1] != NULL) {
                fprintf(file, ", ");
            }
        }
        fprintf(file, "}\n");

        fprintf(file, "CreaturesStats:\n");
        for (int i = 0; i < ROOMS_ROWS; i++) {
            for (int j = 0; j < ROOMS_COLS; j++) {
                if (strcmp(rooms[i][j].creature.name, "") == 0) {
                    fprintf(file, "[%d,%d]:NULL\n", i, j);
                } else if (rooms[i][j].creature.health <= 0) {
                    fprintf(file, "[%d,%d]:DEAD\n", i, j);
                } else {
                    fprintf(file, "[%d,%d]:%s\n", i, j, rooms[i][j].creature.name);
                }
            }
        }

        fprintf(file, "RoomsInventory:\n");
        for (int i = 0; i < ROOMS_ROWS; i++) {
            for (int j = 0; j < ROOMS_COLS; j++) {
                fprintf(file, "[%d,%d]:", i, j);
                for (int k = 0; k < 10 && rooms[i][j].items[k].name[0] != '\0'; k++) {
                    fprintf(file, "%s ", rooms[i][j].items[k].name);
                }
                fprintf(file, "\n");
            }
        }

        fclose(file);
    }
}


void load(const char *filepath) {
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "Savegames/%s", filepath);

    FILE *file = fopen(fullPath, "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "CurrentRoom:", 12) == 0) {
                sscanf(line, "CurrentRoom: %s", currentRoom.name);
            } else if (strncmp(line, "PlayerStats:", 12) == 0) {
                sscanf(line, "PlayerStats: HP=%d, SP=%d", &player.health, &player.strength);
                char inventory[256];
                fgets(inventory, sizeof(inventory), file);
                char *item = strtok(inventory, "{} ,\n");
                int i = 0;
                while (item && i < 10) {
                    player.inventory[i++] = item;
                    item = strtok(NULL, "{} ,\n");
                }
            } else if (strncmp(line, "CreaturesStats:", 15) == 0) {
                for (int i = 0; i < ROOMS_ROWS; i++) {
                    for (int j = 0; j < ROOMS_COLS; j++) {
                        fgets(line, sizeof(line), file);
                        if (strstr(line, ":NULL")) {
                            rooms[i][j].creature.name[0] = '\0';
                        } else if (strstr(line, ":DEAD")) {
                            rooms[i][j].creature.health = 0;
                        } else {
                            sscanf(line, "[%d,%d]:%s", &i, &j, rooms[i][j].creature.name);
                        }
                    }
                }
            } else if (strncmp(line, "RoomsInventory:", 15) == 0) {
                for (int i = 0; i < ROOMS_ROWS; i++) {
                    for (int j = 0; j < ROOMS_COLS; j++) {
                        fgets(line, sizeof(line), file);
                        char *item = strtok(line, ": \n");
                        int k = 0;
                        while (item) {
                            if (k < 10) {
                                strcpy(rooms[i][j].items[k++].name, item);
                            }
                            item = strtok(NULL, ": \n");
                        }
                    }
                }
            }
        }
        fclose(file);
    }
}

void exitGame() {
    printf("Exiting the game...\n");
    exit(0);
}

int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r"); 
    if (file) {
        fclose(file); 
        return 1; 
    }
    return 0; 
}

void menu() {
    printf("Welcome to Dungeon");
    int choice = 0;
    char saveFileName[50]; 

    while (choice != 5) {
        printf("\n1-New Game\n2-Load Game\n3-Save Game\n4-List Saved Games\n5-Exit\n");
        
        scanf("%d", &choice);  

        switch (choice) {
        case 1:
            printf("Loading...\n");
            loadDefaultGame("DungeonGameData.txt");
            inGame();
            break;
        case 2:
            list();
            printf("Choose your save file: ");
            scanf("%s", saveFileName);  

            char fullPath[100] = "Savegames/";
            strcat(fullPath, saveFileName); 

            if (fileExists(fullPath)) {
                load(fullPath);
            } else {
                printf("Error loading file\n");
            }
            break;
        case 3:
            printf("Enter your saved file name: ");
            scanf("%s", saveFileName); 
            save(saveFileName);
            break;
        case 4:
            printf("Here is your list of saved games:\n");
            list();
            break;
        case 5:
            printf("Exiting\n");
            break;
        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    }
}

void inGame() {
    printf("Welcome to the Dungeon. If you want to win, find the Lord and destroy him.\n");
    
    int choice = 0;
    while (choice != 5) {
        printf("\n1-Move\n2-Look\n3-Inventory\n");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            {
                int isError = -1;
                while (isError != 0) {
                    printf("Which direction do you want to go? (up/down/left/right) ");
                    char direction[50];
                    scanf("%s", direction);
                    isError = move(direction);
                }
            }
            break;
        case 2:
            look();
            break;
        case 3:
            inventory();
            break;
        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    }
}

int main() {
    menu();
    return 0;
}
