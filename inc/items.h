#ifndef ITEMS_H
#define ITEMS_H

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Item structure
typedef struct Item {
    float health;
    float strength;
    float defence;
    float crit_rate;
    float crit_chance;
    int type;
    char *name;
    bool looted;
} Item;

// Item types stored as integers created by enum
typedef enum {
    ITEM_NONE,
    ITEM_SWORD,
    ITEM_SHIELD,
    ITEM_ELIXIR,
    ITEM_GENERAL
} ItemType;

void item_create_random(Item *i);
char *item_get_random_name(char* list[], char* last);

#endif