#ifndef ROOM_H
#define ROOM_H

#include <stdbool.h>
#include <time.h>

#include "enemy.h"
#include "items.h"

typedef struct Room {
    short doors; // Open doors for navigating
    bool searched; // Whether "look" command executed
    char *name;
    Enemy mobs[4];
    Item item;
} Room;

void room_create_random(Room *r, unsigned char open_doors);
bool room_look(Room *r);

char* room_get_enemy_names(Room *r);
char* room_get_open_doors(Room *r);
char* room_get_item_name(Room *r);
char *room_get_random_name(char* list[], char* last);
unsigned char room_get_door_bit(int direction);

#endif