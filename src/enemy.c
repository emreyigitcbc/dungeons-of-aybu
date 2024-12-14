#include "enemy.h"
/* @
 * enemy_create_random: void
 * --------------------------
 * Generates a random enemy and fills its attributes based on predefined enemy types.
 *
 * Parameters:
 * - e: Enemy* - Pointer to the Enemy structure to be initialized.
 *
 * Notes:
 * - Randomly determines the enemy type (SLIME, ZOMBIE, VAMPIRE, SKELETON).
 * - Assigns health, damage, critical hit properties, and flee chance for the selected type.
 */
void enemy_create_random(Enemy *e)
{
    // set all fields to zero
    memset(e, 0, sizeof(*e));

    unsigned char type = rand() % 4 + 1;
    // Set basic properties
    switch (type)
    {
    case ENEMY_SLIME:
        e->type = ENEMY_SLIME;
        e->health = 20 + rand() % 6;
        e->damage = 8 + rand() % 3;
        e->crit_rate = 1.1;
        e->crit_chance = 0.2;
        e->flee_chance = 0.8;
        break;
    case ENEMY_ZOMBIE:
        e->type = ENEMY_ZOMBIE;
        e->health = 40 + rand() % 11;
        e->damage = 5 + rand() % 5;
        e->crit_rate = 1.5;
        e->crit_chance = 0.4;
        e->flee_chance = 0.70;
        break;
    case ENEMY_VAMPIRE:
        e->type = ENEMY_VAMPIRE;
        e->health = 80 + rand() % 11;
        e->damage = 8 + rand() % 5;
        e->crit_rate = 1.25;
        e->crit_chance = 0.2;
        e->flee_chance = 0.60;
        break;
    case ENEMY_SKELETON:
        e->type = ENEMY_SKELETON;
        e->health = 100;
        e->damage = 15 + rand() % 5;
        e->crit_rate = 1.34;
        e->crit_chance = 0.1;
        e->flee_chance = 0.45;
        break;
    }
}
/* @
 * enemy_get_name: char*
 * ----------------------
 * Returns the formatted name of an enemy type enclosed in brackets.
 *
 * Parameters:
 * - type: EnemyType - The type of enemy (e.g., SLIME, ZOMBIE).
 *
 * Returns:
 * - A string containing the enemy name formatted as "[ NAME ]".
 *
 * Notes:
 * - If the type is unrecognized, returns an empty string.
 */
char *enemy_get_name(EnemyType type)
{
    switch (type)
    {
    case ENEMY_SLIME:
        return "[ SLIME ]";
        break;
    case ENEMY_ZOMBIE:
        return "[ ZOMBIE ]";
        break;
    case ENEMY_VAMPIRE:
        return "[ VAMPIRE ]";
        break;
    case ENEMY_SKELETON:
        return "[ SKELETON ]";
        break;
    default:
        return "";
        break;
    }
    return "ENEMY";
}
/* @
 * enemy_get_simple_name: char*
 * ----------------------------
 * Returns the simple name of an enemy type without formatting.
 *
 * Parameters:
 * - type: EnemyType - The type of enemy (e.g., SLIME, ZOMBIE).
 *
 * Returns:
 * - A string containing the enemy name in plain text.
 *
 * Notes:
 * - If the type is unrecognized, returns an empty string.
 */
char *enemy_get_simple_name(EnemyType type)
{
    switch (type)
    {
    case ENEMY_SLIME:
        return "SLIME";
        break;
    case ENEMY_ZOMBIE:
        return "ZOMBIE";
        break;
    case ENEMY_VAMPIRE:
        return "VAMPIRE";
        break;
    case ENEMY_SKELETON:
        return "SKELETON";
        break;
    default:
        return "";
        break;
    }
    return "ENEMY";
}
/* @
 * enemy_get_hit: void
 * --------------------
 * Reduces the enemy's health by a specified damage amount.
 *
 * Parameters:
 * - e: Enemy* - Pointer to the Enemy structure.
 * - damage: float - The amount of damage to inflict.
 */
void enemy_get_hit(Enemy *e, float damage) {
    e->health -= damage;
}
/* @
 * enemy_is_alive: bool
 * ---------------------
 * Checks whether the enemy is still alive.
 *
 * Parameters:
 * - e: Enemy* - Pointer to the Enemy structure.
 *
 * Returns:
 * - true if the enemy's health is greater than 0; otherwise, false.
 */
bool enemy_is_alive(Enemy *e) {
    return e->health > 0;
}
/* @
 * enemy_attack: float
 * ---------------------
 * Calculates the damage dealt by the enemy, with a chance for a critical hit.
 *
 * Parameters:
 * - e: Enemy* - Pointer to the Enemy structure.
 *
 * Returns:
 * - The amount of damage dealt as a float.
 *
 * Notes:
 * - A random chance determines whether the damage is a critical hit or normal.
 * - Critical hits multiply the damage by the crit_rate.
 */
float enemy_attack(Enemy *e)
{
    float damage = 0;
    srand(time(0));

    int chance = rand() % 100 + 1;
    if (chance <= e->crit_chance * 100)
    {
        // crit!
        damage = e->damage * e->crit_rate;
    }
    else
    {
        // normal
        damage = e->damage;
    }
    return damage;
}