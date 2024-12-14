#ifndef ENEMY_H
#define ENEMY_H

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Enemy structure
typedef struct Enemy {
    float health;
    float damage;
    float crit_rate;
    float crit_chance;
    float flee_chance;
    unsigned char type;
} Enemy;

// Enemy types are stored as integers, creted by enum
typedef enum {
    ENEMY_NONE,
    ENEMY_SLIME,
    ENEMY_ZOMBIE,
    ENEMY_VAMPIRE,
    ENEMY_SKELETON
} EnemyType;


void enemy_create_random(Enemy *e);
void enemy_create_none(Enemy *e);
void enemy_get_hit(Enemy *e, float damage);

char *enemy_get_name(EnemyType type);
char *enemy_get_simple_name(EnemyType type);

bool enemy_is_alive(Enemy *e);

float enemy_attack(Enemy *e);

#endif