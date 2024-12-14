# Dungeons of AYBU
## Overview
This is a console-based dungeon exploration game written in C. The player begins in a randomly generated dungeon room, with at least one open door. Moving through a door does not close it, but returning will reset the room due to memory management constraints.

Each room may contain one item and up to four monsters. To reveal these, the player must inspect the room using the `look` command.

## Game Mechanics

### Monsters

- **Types:** Slime, Zombie, Vampire, Skeleton

### Items

- **Types:** Sword, Shield, Elixir, General
  - **Sword:** Increases damage.
  - **Shield:** Reduces incoming damage.
  - **Elixir:** Restores health.
  - **General:** No specific purpose.

### Combat

- If the player encounters a monster, they can choose to fight or flee using the `flee` command.

### Commands

#### In-Game Commands (7)

- `move <direction>`: Moves between rooms (`up`, `down`, `left`, `right`).
- `look`: Inspects the current room for details.
- `inventory`: Displays the player's inventory.
- `pickup <item>`: Picks up an item from the room.
- `drop <item>`: Removes an item from the inventory.
- `attack <direction|monster|empty>`: Attacks a specific monster or the nearest one.
- `help`: Displays all commands.

#### Menu Commands (4)

- `list`: Lists saved game files in the current directory.
- `save <filename>`: Saves the current game state.
- `load <filename>`: Loads a saved game.
- `exit`: Exits the game without saving.

#### Combat Commands (3)

- `hit`: Strikes the monster.
- `kick`: Delivers a powerful blow with double critical rate but half critical chance.
- `flee`: Attempts to escape the fight. Success removes the monster from the room.

### Game Over

- If the player dies, a game-over screen displays the number of monsters killed and rooms explored.
- Pressing any key restarts the game.

## Code Structure

The game operates through a `Player` structure:

```c
typedef struct Player {
    float health;
    float maxHealth;
    float strength;
    float defence;
    float crit_rate;
    float crit_chance;
    int rooms_walked;
    int mobs_killed;
    bool onWar;
    int warIndex;
    Item inventory[PLAYER_INV_SIZE];
    Room *room;
} Player;
```

- Adjust PLAYER_INV_SIZE in player.h (recommended size: 6-8).

### Files and Descriptions
- `screen.c:` Manages screen rendering using dynamic sizing based on terminal dimensions.
- `commands.c:` Handles command parsing and execution.
- `save.c:` Manages game saving and loading, storing structure data and associated pointers sequentially.
### Building the Game
A Makefile is included. Simply typing `make` will build the project.

Compiles and works on, Windows 11, Linux Ubuntu 24, MacOS 10.14 Mojave!
