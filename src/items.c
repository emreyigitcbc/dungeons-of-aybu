#include "items.h"

char *NameList_1[] = { "Cebeci's", "Reptile", "God's", "AYBU's", "Rat", "Bear", "Big" };
char *NameList_2[] = { "Bracelet", "Necklace", "Bone", "Golden Ring" };
/* @
 * item_create_random: void
 * -------------------------
 * Generates a random item and assigns its attributes based on the item type.
 *
 * Parameters:
 * - i: Item* - Pointer to the Item structure to be initialized.
 *
 * Notes:
 * - Randomly determines the item type (SWORD, SHIELD, ELIXIR, GENERAL, NONE).
 * - For ITEM_SWORD and ITEM_SHIELD, properties such as strength or defence are assigned.
 * - ITEM_ELIXIR can either be a "Small Elixir" (25 health) or a "Big Elixir" (50 health).
 * - Names are randomly generated using the `item_get_random_name` function.
 */

void item_create_random(Item *i){
    int type  = rand() % 5;
    // Set basic properties
    switch(type) {
        case ITEM_SWORD:
            i->type = ITEM_SWORD;
            i->name = item_get_random_name(NameList_1, " Sword");
            i->strength = 3 + rand() % 11;
            i->crit_rate = 0.05 + ((rand() % 11 + 0.5) / 100);
            break;
        case ITEM_SHIELD:
            i->type = ITEM_SHIELD;
            i->name = item_get_random_name(NameList_1, " Shield");
            i->defence = 3 + rand() % 11; 
            break;
        case ITEM_ELIXIR:
            i->type = ITEM_ELIXIR;
            int elixir_type  = (rand() % 2);
            if (elixir_type) {
                i->health = 50;
                i->name = "Big Elixir";
            } else {
                i->health = 25;
                i->name = "Small Elixir";
            }
            break;
        case ITEM_GENERAL:
            i->type = ITEM_GENERAL;
            i->name = item_get_random_name(NameList_2, "");
            break;
        case ITEM_NONE:
            i->type = ITEM_NONE;
            break;
    }
}
/* @
 * item_get_random_name: char*
 * ----------------------------
 * Generates a random name for an item by combining a random prefix from a list
 * and a specified suffix.
 *
 * Parameters:
 * - list: char*[] - Array of string pointers to be used as prefixes.
 * - last: char* - The suffix to append to the randomly chosen prefix.
 *
 * Returns:
 * - A dynamically allocated string containing the combined item name.
 * - Returns NULL if memory allocation fails.
 *
 * Notes:
 * - Ensures random selection of a string from the given list using `rand()`.
 */
char *item_get_random_name(char* list[], char* last) {
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