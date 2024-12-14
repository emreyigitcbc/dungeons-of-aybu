#include "room.h"

char *Room_Names[] = { "Dungeon", "Big", "Small", "Medium", "Haunted", "Rocky", "Cold" };
/* @
 * room_create_random: void
 * -------------------------
 * Creates a random Room object and assigns values for its properties, including
 * doors, name, items, and enemies. It checks the open_doors parameter to determine
 * which doors should be open, represented by the first 4 bits.
 * 
 * The bitwise interpretation for doors is as follows:
 * - 0b0001 : WEST DOOR
 * - 0b0010 : SOUTH DOOR
 * - 0b0100 : EAST DOOR
 * - 0b1000 : NORTH DOOR
 *
 * Parameters:
 * - r: Room* - Pointer to the Room structure to be initialized.
 * - open_doors: unsigned char - Bitmask indicating which doors should be open.
 */
void room_create_random(Room *r, unsigned char open_doors){
    // set all values to 0
    memset(r, 0, sizeof(*r));
    // SEEDING RANDOM
    srand(time(0));
    // random doors
    bool direction_n  = (rand() % 2);
    bool direction_s  = (rand() % 2);
    bool direction_e  = (rand() % 2);
    bool direction_w  = (rand() % 2);
    // if no door opened, open north door
    if(direction_n + direction_s + direction_e + direction_w <= 0 && open_doors == 0) {
        direction_n = 1;
    }
    // Open doors
    r->name = room_get_random_name(Room_Names, " Room");
    r->doors = direction_n << 3 | direction_e << 2 | direction_s << 1 | direction_w | open_doors;
    r->searched = false;
    // Put random item
    item_create_random(&r->item);
    // put enemies
    unsigned char enemyCount = rand() % 5;
    //enemyCount = 0; // for debugging purposes
    unsigned char i = 0;
    while(i < enemyCount) {
        unsigned char pos = (unsigned char)(rand() % 4); // random position
        if(r->mobs[pos].type == ENEMY_NONE) {
            enemy_create_random(&r->mobs[pos]);
            i++;
        }
    }
    return;
}
/* @
 * room_look: bool
 * ----------------
 * Marks the room as searched if it has not been searched before. Returns
 * false if the room has already been searched, otherwise returns true.
 *
 * Parameters:
 * - r: Room* - Pointer to the Room structure to be searched.
 *
 * Returns:
 * - true if the room is successfully marked as searched, false if already searched.
 */
bool room_look(Room *r){
    if(r->searched) {
        return false;
    }
    r->searched = true;
    return true;
}
/* @
 * room_get_door_bit: unsigned char
 * ---------------------------------
 * Returns the bitmask for a specific door direction based on the given direction.
 * The direction values are as follows:
 * - 0: EAST
 * - 1: NORTH
 * - 2: WEST
 * - 3: SOUTH
 *
 * Parameters:
 * - direction: int - The direction for which to get the door bitmask.
 *
 * Returns:
 * - A bitmask representing the door in the specified direction.
 */
unsigned char room_get_door_bit(int direction) {
    if(direction == 0) return 0b0100;
    if(direction == 1) return 0b1000;
    if(direction == 2) return 0b0001;
    if(direction == 3) return 0b0010;
    return 0;
}
/* @
 * room_get_enemy_count: int
 * ---------------------------
 * Returns the number of enemies present in the room.
 *
 * Parameters:
 * - r: Room* - Pointer to the Room structure.
 *
 * Returns:
 * - The count of enemies in the room.
 */
int room_get_enemy_count(Room *r){
    int c = 0;
    for(int i = 0; i < 4; i++) {
        if(r->mobs[i].type != ENEMY_NONE) {
            c++;
        }
    }
    return c;
}
/* @
 * room_get_enemy_names: char*
 * ----------------------------
 * Returns a string containing the names of all enemies present in the room.
 * If there are multiple enemies, their names are separated by spaces.
 * The result string is dynamically allocated and must be freed by the caller.
 *
 * Parameters:
 * - r: Room* - Pointer to the Room structure.
 *
 * Returns:
 * - A string containing the names of enemies, or NULL if memory allocation fails.
 */
char* room_get_enemy_names(Room *r) {
    // Allocate memory for the result string
    char *result = malloc(100);
    if (!result) {
        return NULL;
    }
    memset(result, 0, 100);

    for (int i = 0; i < 4; i++) {
        if (r->mobs[i].type != ENEMY_NONE) {
            strcat(result, enemy_get_simple_name(r->mobs[i].type));
            strcat(result, " "); // Add a space between names
        }
    }
    return result;
}
/* @
 * room_get_open_doors: char*
 * ---------------------------
 * Returns a string describing the open doors in the room. The result string
 * contains the directions of open doors, separated by spaces (e.g., "up right").
 * The result string is dynamically allocated and must be freed by the caller.
 *
 * Parameters:
 * - r: Room* - Pointer to the Room structure.
 *
 * Returns:
 * - A string describing the open doors, or NULL if memory allocation fails.
 */
char* room_get_open_doors(Room *r) {
    // Allocate memory for the result string
    char *result = malloc(100);
    if (!result) {
        return NULL; // Handle allocation failure
    }
    memset(result, 0, 100);

    // Check door directions and append to result
    if ((r->doors & 0b1000) > 0) {
        strcat(result, "up ");
    }
    if ((r->doors & 0b0100) > 0) {
        strcat(result, "right ");
    }
    if ((r->doors & 0b0010) > 0) {
        strcat(result, "down ");
    }
    if ((r->doors & 0b0001) > 0) {
        strcat(result, "left ");
    }

    return result;
}
/* @
 * room_get_item_name: char*
 * ---------------------------
 * Returns the name of the item in the room. If there is no item or the item
 * has already been looted, returns "NONE".
 *
 * Parameters:
 * - r: Room* - Pointer to the Room structure.
 *
 * Returns:
 * - A string representing the item's name, or "NONE" if no item is available.
 */
char* room_get_item_name(Room *r) {
    if(r->item.type == ITEM_NONE || r->item.looted) {
        return "NONE";
    } else {
        return r->item.name;
    }
}
/* @
 * room_get_random_name: char*
 * ----------------------------
 * Selects a random name for the room from a list of possible names and appends
 * the provided suffix to it.
 *
 * Parameters:
 * - list: char*[] - List of possible room names.
 * - last: char* - Suffix to be appended to the selected name.
 *
 * Returns:
 * - A dynamically allocated string containing the room's name, or NULL if memory allocation fails.
 */
char *room_get_random_name(char* list[], char* last) {
    int list_size = 0;
    while (list[list_size] != NULL) {
        list_size++;
    }

    int random_index = rand() % list_size;

    char* selected_string = list[random_index];

    size_t result_length = strlen(selected_string) + strlen(last) + 1;
    char* result = (char*)malloc(result_length * sizeof(char));

    if (!result) {
        return NULL;
    }
    strcpy(result, selected_string);
    strcat(result, last);

    return result;
}