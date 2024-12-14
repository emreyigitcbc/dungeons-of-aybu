#ifndef PLAYER_H
#define PLAYER_H

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "items.h"
#include "room.h"

#define PLAYER_INV_SIZE 6

// Player structure
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

void player_start(Player *pl);
void player_calculate_stats(Player *pl);
void player_get_hit(Player *pl, float damage);
void player_start_attack(Player *pl, int index);

bool player_check_alive(Player *pl);
bool player_check_inv_full(Player *pl);
bool player_check_has_same(Player *pl);
bool player_check_inv_has(Player *pl, char *arg);
bool player_drop_item(Player *pl, char *arg);
bool player_move(Player *pl, int direction);

int player_get_item(Player *pl);
int player_get_inv_size(Player *pl);
int player_init_attack(Player *pl, char *arg);
float player_attack(Player *pl, int multiplier);


// to fix typedef imports from screen.h;
// screen.c compiled before before.c so it looks for Player typedef but it cant, so we import it at the end
#include "screen.h"
#include "commands.h"

#endif