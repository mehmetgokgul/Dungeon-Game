#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DungeonGame.h"
#include <sys/stat.h>
#include <dirent.h>

PlayerStats player = {10, 5, {NULL}};
int currentRow = 0, currentCol = 0;
int inventoryCount = 0;

Room rooms[4][3];
Creature creatures[4][3];

void loadDefaultGame() {
    strcpy(rooms[0][0].name, "The Hall");
    strcpy(rooms[0][0].info, "Choose the direction you want to go. Right or down? Good luck!");

    strcpy(rooms[0][1].name, "The Piano Room");
    strcpy(rooms[0][1].info, "A grand room with an old piano that fills the dungeon with eerie melodies. You played the piano beautifully and the walls adored the melody. As a reward, you received:");
    strcpy(rooms[0][1].item.name, "Musical Pipe");
    strcpy(rooms[0][1].item.info, "Increases health by 1 hp. Playing the pipe restores your spirit and enhances your connection to the music of the world.");
    rooms[0][1].item.healthBonus = 1;
    rooms[0][1].item.strengthBonus = 0;

    strcpy(rooms[0][2].name, "The Hidden Garden");
    strcpy(rooms[0][2].info, "A beautiful, unexpected garden in the middle of the dungeon. It seems untouched by time, with lush plants growing in every corner. The air feels fresh and the serenity of the garden contrasts sharply with the dungeon's eerie atmosphere.");
    strcpy(rooms[0][2].item.name, "Old Sword");
    strcpy(rooms[0][2].item.info, "A basic weapon that increases strength by 1 sp.");
    rooms[0][2].item.healthBonus = 0;
    rooms[0][2].item.strengthBonus = 1;

    strcpy(rooms[1][0].name, "The Laboratory");
    strcpy(rooms[1][0].info, "A cold, sterile room filled with strange concoctions and equipment. Be cautious.");
    strcpy(rooms[1][0].item.name, "Lab Coat");
    strcpy(rooms[1][0].item.info, "+1 sp. Safety first.");
    rooms[1][0].item.healthBonus = 0;
    rooms[1][0].item.strengthBonus = 1;

    strcpy(rooms[1][1].name, "The Workshop");
    strcpy(rooms[1][1].info, "A room full of tools and workbenches. It looks like something could be crafted here.");
    strcpy(rooms[1][1].item.name, "Mace");
    strcpy(rooms[1][1].item.info, "+2 sp. Increases attack strength moderately.");
    rooms[1][1].item.healthBonus = 0;
    rooms[1][1].item.strengthBonus = 2;

    strcpy(rooms[1][2].name, "The Prison");
    strcpy(rooms[1][2].info, "A dark, damp room with cold stone walls and rusty bars. The air is filled with the scent of mildew and despair.");
    strcpy(rooms[1][2].item.name, "Bone Key");
    strcpy(rooms[1][2].item.info, "Can be used to unlock a chest.");
    rooms[1][2].item.healthBonus = 0;
    rooms[1][2].item.strengthBonus = 0;

    strcpy(rooms[2][0].name, "The Library");
    strcpy(rooms[2][0].info, "The room is filled with dusty, long shelves of books. Many are old and decayed, but some may hold valuable knowledge.");
    strcpy(rooms[2][0].item.name, "Ancient Book");
    strcpy(rooms[2][0].item.info, "+3 sp. Knowledge is power.");
    rooms[2][0].item.healthBonus = 0;
    rooms[2][0].item.strengthBonus = 3;

    strcpy(rooms[2][1].name, "The Bedroom");
    strcpy(rooms[2][1].info, "A grand, opulent room with a massive bed that seems too large for any human. The room is filled with velvet curtains and golden accents. It looks as if it hasn’t been used for centuries.");
    strcpy(rooms[2][1].item.name, "Golden Chest");
    strcpy(rooms[2][1].item.info, "Can be unlocked with the Bone Key.");
    rooms[2][1].item.healthBonus = 0;
    rooms[2][1].item.strengthBonus = 0;

    strcpy(rooms[2][2].name, "The Forge");
    strcpy(rooms[2][2].info, "A hot, fiery room filled with the sound of hammering metal. The walls glow with the heat of a massive forge and armor sets hang from the walls. Sparks fly as molten metal is poured into molds. This is where the finest armors are made.");
    strcpy(rooms[2][2].item.name, "Armor of the Flame");
    strcpy(rooms[2][2].item.info, "+5 hp. Increases defense and provides fire resistance.");
    rooms[2][2].item.healthBonus = 5;
    rooms[2][2].item.strengthBonus = 0;

    strcpy(rooms[3][0].name, "The Secret Chamber");
    strcpy(rooms[3][0].info, "A hidden chamber filled with dark energy. You feel a strange presence here. Whispers echo from every corner, footsteps race through the air and faint voices discuss your every move. The eerie sounds of laughter and weeping fill the space. You are not alone.");
    strcpy(rooms[3][0].item.name, "Shadow Blade");
    strcpy(rooms[3][0].item.info, "+3 sp. A weapon forged in the shadows, it enhances attack power with dark energy.");
    rooms[3][0].item.healthBonus = 0;
    rooms[3][0].item.strengthBonus = 3;

    strcpy(rooms[3][1].name, "The Courtyard");
    strcpy(rooms[3][1].info, "A large, empty space with cold stone floors. A heavy silence fills the air, broken only by the occasional gust of wind that seems to seep in from somewhere. An unmoving figure stands at the far end of the courtyard, its eyes following your every move. As you approach, you realize the figure is a stone-like statue, eerily lifelike, its gaze unblinking.");
    strcpy(rooms[3][1].item.name, "Ebony Shield");
    strcpy(rooms[3][1].item.info, "+5 hp. A rare, dark shield made from ebony, known for its durability and ability to protect the wearer.");
    rooms[3][1].item.healthBonus = 5;
    rooms[3][1].item.strengthBonus = 0;

    strcpy(rooms[3][2].name, "The Dungeon Lord's Lair");
    strcpy(rooms[3][2].info, "The final room. The walls are lined with ominous, blood-stained stone and a cold, oppressive atmosphere fills the air. At the far end, seated upon a throne made of ancient bones, sits the Dungeon Lord, eyes burning with malevolent power. The room is filled with the echoes of countless battles fought and lost. The Dungeon Lord awaits and only the strongest will leave this chamber alive.");
    strcpy(rooms[3][2].item.name, "NULL");
    strcpy(rooms[3][2].item.info, "NULL");
    rooms[3][2].item.healthBonus = 0;
    rooms[3][2].item.strengthBonus = 0;

    // Creatures
    strcpy(creatures[0][0].name, "NULL");
    strcpy(creatures[0][0].info, "NULL");
    creatures[0][0].health = 0;
    creatures[0][0].strength = 0;

    strcpy(creatures[0][1].name, "NULL");
    strcpy(creatures[0][1].info, "NULL");
    creatures[0][1].health = 0;
    creatures[0][1].strength = 0;

    strcpy(creatures[0][2].name, "Blue Snake");
    strcpy(creatures[0][2].info, "It is attacked when you are walking in greenery.");
    creatures[0][2].health = 5;
    creatures[0][2].strength = 2;

    strcpy(creatures[1][0].name, "Crazy Scientist");
    strcpy(creatures[1][0].info, "Stays behind equipment, attacks when approached.");
    creatures[1][0].health = 8;
    creatures[1][0].strength = 3;

    strcpy(creatures[1][1].name, "Crafting Golem");
    strcpy(creatures[1][1].info, "It comes to life when you approach the workbenches.");
    creatures[1][1].health = 10;
    creatures[1][1].strength = 5;

    strcpy(creatures[1][2].name, "Prison Guard");
    strcpy(creatures[1][2].info, "Keeps watch over the prisoners and attacks intruders.");
    creatures[1][2].health = 7;
    creatures[1][2].strength = 4;

    strcpy(creatures[2][0].name, "Knowledge Guardian");
    strcpy(creatures[2][0].info, "A golem made of old books, protects valuable knowledge.");
    creatures[2][0].health = 12;
    creatures[2][0].strength = 2;

    strcpy(creatures[2][1].name, "Sleepy Wraith");
    strcpy(creatures[2][1].info, "It attacks when disturbed in its sleep.");
    creatures[2][1].health = 6;
    creatures[2][1].strength = 3;

    strcpy(creatures[2][2].name, "Molten Fiend");
    strcpy(creatures[2][2].info, "A fiery beast that attacks with its flames.");
    creatures[2][2].health = 15;
    creatures[2][2].strength = 6;

    strcpy(creatures[3][0].name, "Shadow Specter");
    strcpy(creatures[3][0].info, "It moves in darkness and attacks when provoked.");
    creatures[3][0].health = 8;
    creatures[3][0].strength = 4;

    strcpy(creatures[3][1].name, "Stone Sentinel");
    strcpy(creatures[3][1].info, "Statue-like creature that attacks when approached too closely");
    creatures[3][1].health = 20;
    creatures[3][1].strength = 7;

    strcpy(creatures[3][2].name, "Dungeon Lord");
    strcpy(creatures[3][2].info, "He awaits on his throne, the final challenge");
    creatures[3][2].health = 50;
    creatures[3][2].strength = 10;
}

/*void printDungeonData() {
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
*/
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
    printf("\nYou are in %s.\n", rooms[currentRow][currentCol].name);
    printf("You are in %s.\n", rooms[currentRow][currentCol].info);

    if (strcmp(creatures[currentRow][currentCol].name, "NULL") != 0){
        printf("The Creature in this room is %s. %s\n", 
            creatures[currentRow][currentCol].name, 
            creatures[currentRow][currentCol].info);
        printf("HP: %d\nSP: %d\n", 
            creatures[currentRow][currentCol].health, 
            creatures[currentRow][currentCol].strength);
    }

    if (strcmp(rooms[currentRow][currentCol].item.name, "NULL") != 0) {
    printf("Items in this room:\n");
        printf("%s (%s)\n", rooms[currentRow][currentCol].item.name, rooms[currentRow][currentCol].item.info);
    
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

    if (rooms[currentRow][currentCol].items != NULL) {
        if (strcmp(rooms[currentRow][currentCol].items->name, itemName) == 0) {
            for (int j = 0; j < 10; j++) {
                if (player.inventory[j] == NULL) {
                    player.inventory[j] = strdup(rooms[currentRow][currentCol].items->name);
                    player.health += rooms[currentRow][currentCol].items->healthBonus;  
                    player.strength += rooms[currentRow][currentCol].items->strengthBonus;  

                    printf("You have picked up: %s\n", itemName);
                    
                    rooms[currentRow][currentCol].items->name[0] = '\0'; 
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

        strcpy(currentCreature->name, "NULL");
        currentCreature->health = 0;
        currentCreature->strength = 0;

        if (strcmp(rooms[currentRow][currentCol].item.name, "NULL") != 0) {
            pickup(rooms[currentRow][currentCol].item.name);
            strcpy(rooms[currentRow][currentCol].item.name, "NULL");
            strcpy(rooms[currentRow][currentCol].item.info, "NULL");
        }
    } else {
        printf("You were defeated by %s. Game over!\n", currentCreature->name);
        exitGame();
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
        fprintf(file, "CurrentRoom: %s\n", rooms[currentRow][currentCol].name);
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
                if (rooms[i][j].items[0].name[0] != '\0') {
                    fprintf(file, "[%d,%d]: ", i, j);
                    for (int k = 0; k < 10 && rooms[i][j].items[k].name[0] != '\0'; k++) {
                        fprintf(file, "%s", rooms[i][j].items[k].name);
                        if (k < 9 && rooms[i][j].items[k + 1].name[0] != '\0') {
                            fprintf(file, ", ");
                        }
                    }
                    fprintf(file, "\n");
                }
            }
        }

        fclose(file);
    } else {
        printf("Failed to save the game.\n");
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
                sscanf(line, "CurrentRoom: %d,%d", &currentRow, &currentCol);  
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

        printf("Starting game at room [%d, %d].\n", currentRow, currentCol);
    } else {
        printf("Failed to load the game.\n");
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
            printf("\nLoading...\n");
            loadDefaultGame();
            inGame();
            break;
        case 2:
            list();
            printf("Choose your save file: ");
            scanf("%s", saveFileName);  

            char fullPath[100] = "Savegames/";
            strcat(fullPath, saveFileName); 

            if (fileExists(fullPath)) {
                //loadGameData();
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
            exit(0);
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
    while (choice != 6) {
        printf("\n1-Move\n2-Look\n3-Inventory\n4-Pickup\n5-Attack\n6-Back to Menu\n");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            {
                int isError = -1;
                while (isError != 0) {
                    printf("\nWhich direction do you want to go? (up/down/left/right) \n");
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
        case 4:    
            if (strcmp(creatures[currentRow][currentCol].name, "NULL") != 0 && creatures[currentRow][currentCol].health > 0) {
                printf("\nYou must fight first with the %s!\n", creatures[currentRow][currentCol].name);
                return;
            }
    
            printf("Enter the item you wanna pickup\n");
            char itemName[50];
            scanf("%c",itemName);
            pickup(itemName);
            break;
        case 5:
            attack();
            break;
        case 6:
            menu();
            break;
        default:
            printf("\nInvalid choice. Try again.\n");
            break;
        }
    }
}

int main() {
    menu();
    return 0;
}