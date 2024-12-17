#Dungeon Game - Text-based Adventure

##Overview
Dungeon Game is a text-based adventure game where the player explores a dungeon, battles creatures, collects items, and solves puzzles to progress. The game features a grid of rooms, each with its own unique characteristics, such as creatures, items, and hidden secrets. The player can move between rooms, interact with their environment, and save/load game progress.

##Features
Explore the Dungeon: Move through different rooms in the dungeon, each with its own name, description, and challenges.
Combat System: Battle creatures that reside in the rooms, using items and weapons to enhance the player's strength.
Inventory System: Collect and use items like weapons, armor, and keys to aid in your journey.
Save and Load: Save your game progress and resume from where you left off.
Menu System: Start a new game, load an existing game, or exit the game using the menu.

##Gameplay
The player can move in four directions: left, right, up, and down. In each room, there may be items to pick up and creatures to fight. The player can use a variety of items to increase their health or strength and gain advantages in combat.

##How to Play
Start the Game: Upon running the game, you will be presented with the main menu where you can choose to start a new game, load a saved game, or exit.
Explore the Dungeon: Once you start the game, use the move() function to navigate the dungeon rooms.
Look Around: Use the look() function to examine the current room and check for items or creatures.
Fight Creatures: If a creature is present in the room, use the attack() function to engage in battle.
Use Items: Use the inventory() and pickup() functions to manage your items and keep track of what you've collected.
Save/Load Progress: Save your progress using saveGame() and load a previously saved game using loadGame().

##Installation
###Prerequisites
-C Compiler (e.g., GCC)
-Text editor or IDE for editing the code
###Steps
Clone this repository:
```bash
git clone https://github.com/mehmetgokgul/dungeon-game.git
```
Navigate to the project directory:
```
cd dungeon-game
```
Compile the code:
```
gcc -o dungeon_game main.c
```
Run the game:
```
./dungeon_game
```
##Code Structure
###DungeonGame.h
This header file defines the key structures and function prototypes used in the game. It includes the player's stats, creatures, rooms, and functions for gameplay actions like moving, attacking, and interacting with items.

###main.c
This file contains the main game loop, where the player navigates the dungeon and interacts with the environment. It includes function definitions like move(), attack(), inventory(), and saveGame().

###DungeonGameData.txt
This file holds the data for the dungeon's rooms, creatures, and items, including descriptions, attributes, and interactions for each element in the game.

##Function List
-move(): Allows the player to move between rooms.
-look(): Displays information about the current room.
-inventory(): Lists the items in the player’s inventory.
-pickup(): Adds an item to the player’s inventory.
-attack(): Engages in combat with a creature.
-saveGame(): Saves the current game state to a file.
loadGame(): Loads a previously saved game state.
