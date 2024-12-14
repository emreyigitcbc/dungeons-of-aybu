#include "player.h"

#include "room.h"

/* @
 * player_start: void
 * ------------------
 * Initializes the player's basic stats such as health, strength, defense, 
 * and critical hit properties.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure to be initialized.
 */
void player_start(Player *pl)
{
    pl->health = 100;
    pl->maxHealth = 100;
    pl->defence = 0;
    pl->strength = 10;
    pl->crit_chance = 0.1;
    pl->crit_rate = 1.1;
}
/* @
 * player_check_alive: bool
 * --------------------------
 * Checks if the player is still alive based on their health.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 *
 * Returns:
 * - true if the player's health is greater than zero, false otherwise.
 */
bool player_check_alive(Player *pl) {
    return pl->health > 0;
}
/* @
 * player_get_item: int
 * ----------------------
 * Attempts to add an item from the room to the player's inventory.
 * Returns an integer status code:
 * - 0 if the item is successfully added.
 * - 1 if the inventory is full or there is an existing item in the inventory.
 * - 2 if the player already has the same item.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 *
 * Returns:
 * - Integer status code.
 */
int player_get_item(Player *pl)
{
    if (player_check_inv_full(pl))
    {
        return 1;
    }
    if (player_check_has_same(pl))
    {
        return 2;
    }
    if (pl->room->item.looted == false)
    {
        for (int i = 0; i < PLAYER_INV_SIZE; i++)
        {
            if (pl->inventory[i].type == ITEM_NONE)
            {
                pl->room->item.looted = true;
                pl->inventory[i] = pl->room->item;
                player_calculate_stats(pl);

                return 0;
            }
        }
    }
    return 1;
}
/* @
 * player_calculate_stats: void
 * ------------------------------
 * Recalculates the player's stats based on the items in their inventory.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure whose stats will be recalculated.
 */
void player_calculate_stats(Player *pl)
{
    player_start(pl);
    for (int i = 0; i < PLAYER_INV_SIZE; i++)
    {
        if (pl->inventory[i].type != ITEM_NONE)
        {
            // update player stats
            pl->health += pl->inventory[i].health;
            pl->maxHealth += pl->inventory[i].health;
            pl->defence += pl->inventory[i].defence;
            pl->crit_rate += pl->inventory[i].crit_rate;
            pl->crit_chance += pl->inventory[i].crit_chance;
            pl->strength += pl->inventory[i].strength;
        }
    }
}
/* @
 * player_check_inv_has: bool
 * ----------------------------
 * Checks if the player has a specific item in their inventory.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 * - arg: char* - Name of the item to check.
 *
 * Returns:
 * - true if the player has the item, false otherwise.
 */
bool player_check_inv_has(Player *pl, char *arg)
{
    for (int i = 0; i < PLAYER_INV_SIZE; i++)
    {
        if (strcasecmp(pl->inventory[i].name, arg) == 0)
        {
            return true;
        }
    }
    return false;
}
/* @
 * player_drop_item: bool
 * ------------------------
 * Attempts to remove an item from the player's inventory.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 * - arg: char* - Name of the item to drop.
 *
 * Returns:
 * - true if the item is found and removed, false otherwise.
 */
bool player_drop_item(Player *pl, char *arg)
{
    for (int i = 0; i < PLAYER_INV_SIZE; i++)
    {
        if (strcasecmp(pl->inventory[i].name, arg) == 0)
        {
            memset(&pl->inventory[i], 0, sizeof(pl->inventory[i]));
            player_calculate_stats(pl);
            return true;
        }
    }
    return false;
}
/* @
 * player_get_inv_size: int
 * ---------------------------
 * Returns the number of items in the player's inventory.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 *
 * Returns:
 * - Integer representing the number of items in the player's inventory.
 */
int player_get_inv_size(Player *pl)
{
    int size = 0;
    for (int i = 0; i < PLAYER_INV_SIZE; i++)
    {
        if (pl->inventory[i].type != ITEM_NONE)
        {
            size++;
        }
    }
    return size;
}
/* @
 * player_check_inv_full: bool
 * -----------------------------
 * Checks if the player's inventory is full.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 *
 * Returns:
 * - true if the inventory is full, false otherwise.
 */
bool player_check_inv_full(Player *pl)
{
    for (int i = 0; i < PLAYER_INV_SIZE; i++)
    {
        if (pl->inventory[i].type == ITEM_NONE)
        {
            return false;
        }
    }
    return true;
}
/* @
 * player_check_has_same: bool
 * -----------------------------
 * Checks if the player already has the same item in their inventory that
 * is present in the room and has not been looted yet.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 *
 * Returns:
 * - true if the item already exists in the inventory, false otherwise.
 */
bool player_check_has_same(Player *pl)
{
    for (int i = 0; i < PLAYER_INV_SIZE; i++)
    {
        if (pl->inventory[i].name == pl->room->item.name && pl->room->item.looted == false)
        {
            return true;
        }
    }
    return false;
}
/* @
 * player_move: bool
 * -------------------
 * Attempts to move the player in a specified direction. If the direction 
 * contains an enemy, the player will engage in combat. Otherwise, the player
 * will move to a new room.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 * - direction: int - Direction to move (0: left, 1: down, 2: right, 3: up).
 *
 * Returns:
 * - true if the move is successful, false otherwise.
 */
bool player_move(Player *pl, int direction)
{
    // check if enemy exists in that direction
    if (pl->room->mobs[direction].type != ENEMY_NONE)
    {
        // enemy exists, attack!
        player_start_attack(pl, direction);
    }
    else
    {
        if ((pl->room->doors & (0b0001 << direction)) == 0)
        {
            return false;
        }
        // move direction
        Room *r = (Room *)malloc(sizeof(Room));
        room_create_random(r, room_get_door_bit(direction));
        free(pl->room);
        pl->room = r;
        pl->rooms_walked += 1;
        if(pl->health != pl->maxHealth) {
            pl->health = pl->maxHealth;
            draw_output_text("Your health regenerated!");
            draw_player_stats(pl);
        }
        
        draw_dungeon(pl->room);
    }
    return true;
}
/* @
 * player_attack: float
 * ----------------------
 * Calculates and returns the damage dealt by the player based on critical 
 * hit chance and strength. The player's attack will either be a normal or 
 * critical hit depending on the outcome of a random chance.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 * - multiplier: int - Multiplier for critical damage.
 *
 * Returns:
 * - The damage dealt by the player.
 */
float player_attack(Player *pl, int multiplier)
{
    float damage = 0;
    srand(time(0));

    int chance = rand() % 100 + 1;
    if (chance * multiplier <= pl->crit_chance * 100)
    {
        // crit!
        damage = pl->strength * pl->crit_rate * multiplier;
    }
    else
    {
        // normal
        damage = pl->strength;
    }
    enemy_get_hit(&pl->room->mobs[pl->warIndex], damage);
    return damage;
}
/* @
 * player_get_hit: void
 * ----------------------
 * Reduces the player's health based on the damage they received, factoring 
 * in their defense.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 * - damage: float - The damage taken by the player.
 */
void player_get_hit(Player *pl, float damage) {
    pl->health += (0.25 * pl->defence - damage);
}
/* @
 * player_init_attack: int
 * -------------------------
 * Initializes an attack against an enemy, either by direction or by enemy name.
 * Returns the index of the enemy to attack.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 * - arg: char* - Direction or enemy name to attack.
 *
 * Returns:
 * - The index of the enemy to attack, or -1 if no valid enemy was found.
 */
int player_init_attack(Player *pl, char *arg)
{
    // check random attack
    int attack_index = -1;
    if (strlen(arg) <= 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (pl->room->mobs[i].type != ENEMY_NONE)
            {
                if ((pl->room->doors & (0b0001 << i)) > 0)
                {
                    // attack the mob in front of the door
                    return i;
                }
                else
                {
                    // attack other mob
                    attack_index = i;
                }
            }
        }
    }
    else if (strcasecmp(arg, "up") == 0)
    {
        if (pl->room->mobs[3].type != ENEMY_NONE)
        {
            attack_index = 3;
        }
    }
    else if (strcasecmp(arg, "right") == 0)
    {
        if (pl->room->mobs[2].type != ENEMY_NONE)
        {
            attack_index = 2;
        }
    }
    else if (strcasecmp(arg, "down") == 0)
    {
        if (pl->room->mobs[1].type != ENEMY_NONE)
        {
            attack_index = 1;
        }
    }
    else if (strcasecmp(arg, "left") == 0)
    {
        if (pl->room->mobs[0].type != ENEMY_NONE)
        {
            attack_index = 0;
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            if (pl->room->mobs[i].type != ENEMY_NONE && strcasecmp(enemy_get_simple_name(pl->room->mobs[i].type), arg) == 0)
            {
                attack_index = i;
            }
        }
    }
    return attack_index;
}
/* @
 * player_start_attack: void
 * ---------------------------
 * Prepares the player for combat with a specific enemy by setting the war state 
 * and updating the UI with relevant war information.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure.
 * - index: int - The index of the enemy to attack.
 */
void player_start_attack(Player *pl, int index)
{
    pl->onWar = true;
    pl->warIndex = index;
    draw_war_info(pl);
}