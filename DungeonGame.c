#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DungeonGame.h"

struct Room rooms[MAX_ROWS][MAX_COLS];

void readRoomData(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    char line[256];
    int x, y;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "[%d,%d] - %[^\n]", &x, &y, rooms[x][y].name) == 3) {
            // Read info
            fgets(rooms[x][y].info, sizeof(rooms[x][y].info), file);
            rooms[x][y].info[strcspn(rooms[x][y].info, "\n")] = '\0';

            // Read items
            int item_count = 0;
            while (fgets(line, sizeof(line), file) && line[0] != '[') {
                line[strcspn(line, "\n")] = '\0';  
                if (strlen(line) == 0) continue;

                // Parse items in the format "Name (Effect)"
                char item_name[50], item_info[100];
                if (sscanf(line, "%[^'('] (%[^')'])", item_name, item_info) == 2) {
                    strcpy(rooms[x][y].items[item_count].name, item_name);
                    strcpy(rooms[x][y].items[item_count].info, item_info);
                    item_count++;
                }
            }
        }
    }

    fclose(file);

    // Print all room data for testing
    int i, j;
    for (i = 0; i < MAX_ROWS; i++) {
        for (j = 0; j < MAX_COLS; j++) {
            if (rooms[i][j].name[0] == '\0') continue;  
            printf("\n--> %s\n", rooms[i][j].name);
            if (i == 0 && j == 0) printf("\n           Welcome to the Dungeon!\n");
            printf("> %s\n", rooms[i][j].info);

            // Print items if they exist
            if (rooms[i][j].items[0].name[0] != '\0') {
                printf("\nNew items:\n");
                for (int k = 0; k < 10 && rooms[i][j].items[k].name[0] != '\0'; k++) {
                    printf(" - %s(%s)\n", rooms[i][j].items[k].name, rooms[i][j].items[k].info);
                }
            }
        }
    }
}

int main() {
    readRoomData("DungeonGameData.txt");
    return 0;
}
