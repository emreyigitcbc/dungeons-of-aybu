#include "commands.h"
/* @
 * command_handle_war: void
 * -------------------------
 * Handles player commands during an active fight.
 * It processes commands such as "hit", "kick", and "flee" to deal damage, attempt to flee, or manage combat.
 *
 * Parameters:
 * - input: const char* - The player's input command during combat.
 * - pl: Player* - Pointer to the Player structure.
 *
 * Notes:
 * - Executes attack logic for "hit" and "kick" with varying multipliers.
 * - Manages enemy and player damage calculations and checks for game-over conditions.
 * - Handles fleeing attempts with random success based on the enemy's flee chance.
 */
void command_handle_war(const char *input, Player *pl)
{
    move_cursor_output();
    if (strcasecmp(input, "hit") == 0 || strcasecmp(input, "kick") == 0)
    {
        int multiplier = 1;
        if (strcasecmp(input, "kick") == 0)
            multiplier = 2;
        float dmg = player_attack(pl, multiplier);
        enemy_get_hit(&pl->room->mobs[pl->warIndex], dmg);
        if (enemy_is_alive(&pl->room->mobs[pl->warIndex]))
        {
            // enemy still alive
            float e_dmg = enemy_attack(&pl->room->mobs[pl->warIndex]);
            player_get_hit(pl, e_dmg);
            // check player still alive
            if (player_check_alive(pl))
            {
                draw_output_text("You hit '%.1f' damage, the enemy hit you '%.1f' damage!", dmg, e_dmg);
                draw_war_info(pl);
                draw_player_stats(pl);
            }
            else
            {
                // GAME OVER
                draw_game_over(pl);
            }
        }
        else
        {
            memset(&pl->room->mobs[pl->warIndex], 0, sizeof(pl->room->mobs[pl->warIndex]));
            pl->onWar = false;
            pl->mobs_killed += 1;
            change_info_to_room(pl->room);
            draw_dungeon(pl->room);
            draw_output_text("You hit '%.1f' and killed the enemy! Fight is over.", dmg);
        }
    }
    else if (strcasecmp(input, "flee") == 0)
    {
        srand(time(NULL));
        int chance = rand() % 100 + 1;
        if (chance <= pl->room->mobs[pl->warIndex].flee_chance * 100)
        {
            pl->onWar = false;
            // delete that mob
            memset(&pl->room->mobs[pl->warIndex], 0, sizeof(pl->room->mobs[pl->warIndex]));
            draw_dungeon(pl->room);
            draw_output_text("You succesfully run away from enemy!");
        }
        else
        {
            pl->room->mobs[pl->warIndex].flee_chance -= 0.1;
            int e_dmg = enemy_attack(&pl->room->mobs[pl->warIndex]);
            player_get_hit(pl, e_dmg);
            draw_war_info(pl);
            draw_output_text("You were unsuccessfull while trying to flee.");
        }
    }
    else
    {
        draw_output_text("Unknown fight command! Available commands are: hit, kick, flee");
    }
}
/* @
 * command_handle: void
 * ---------------------
 * Parses general player commands and executes corresponding actions in non-combat situations.
 *
 * Parameters:
 * - input: const char* - The player's input string, including command and optional arguments.
 * - pl: Player* - Pointer to the Player structure.
 *
 * Notes:
 * - Splits the input string into command and argument parts using space (" ") as a delimiter.
 * - Supports a wide range of commands, including:
 *   - "move" to navigate.
 *   - "look" to examine the surroundings.
 *   - "inventory" to view items.
 *   - "pickup" and "drop" for inventory management.
 *   - "attack" to initiate combat.
 *   - "save" and "load" to handle game state persistence.
 *   - "help" to display available commands.
 * - If a fight is active (`pl->onWar`), delegates command processing to `command_handle_war`.
 */
void command_handle(const char *input, Player *pl)
{
    if (pl->health <= 0)
    {
        init_game(pl);
        return;
    }
    move_cursor_output();
    char command[MAX_ARG_LENGTH];
    char arg[MAX_ARG_LENGTH];
    memset(arg, 0, sizeof(arg));
    memset(command, 0, sizeof(command));

    // Tokenize the input to extract the command and optional argument
    const char *delimiter = " ";
    char *token = strtok((char *)input, delimiter);

    if (token != 0)
    {
        strncpy(command, token, MAX_ARG_LENGTH);
        command[MAX_ARG_LENGTH - 1] = '\0'; // Ensure null-termination

        token = strtok(0, ""); // Get the rest of the line as the argument
        if (token != 0)
        {
            strncpy(arg, token, MAX_ARG_LENGTH);
            arg[MAX_ARG_LENGTH - 1] = '\0';
        }

        // Determine command
        if (pl->onWar)
        {
            command_handle_war(command, pl);
        }
        else if (strcasecmp(command, "move") == 0)
        {
            if (strlen(arg) == 0)
            {
                draw_output_text("Usage: move <direction: up, right, down, left>\n");
            }
            else
            {
                if (strcasecmp(arg, "u") == 0 || strcasecmp(arg, "up") == 0)
                {
                    if (player_move(pl, 3))
                    {
                    }
                    else
                    {
                        draw_output_text("There is no way!");
                    }
                }
                else if (strcasecmp(arg, "r") == 0 || strcasecmp(arg, "right") == 0)
                {
                    if (player_move(pl, 2))
                    {
                    }
                    else
                    {
                        draw_output_text("There is no way!");
                    }
                }
                else if (strcasecmp(arg, "d") == 0 || strcasecmp(arg, "down") == 0)
                {
                    if (player_move(pl, 1))
                    {
                    }
                    else
                    {
                        draw_output_text("There is no way!");
                    }
                }
                else if (strcasecmp(arg, "l") == 0 || strcasecmp(arg, "left") == 0)
                {
                    if (player_move(pl, 0))
                    {
                    }
                    else
                    {
                        draw_output_text("There is no way!");
                    }
                }
                else
                {
                    draw_output_text("Usage: move <direction: up, right, down, left>\n");
                }
            }
        }
        else if (strcasecmp(command, "look") == 0)
        {
            change_info_title("> ROOM <");
            if (room_look(pl->room))
            {
                int enemyCount = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (pl->room->mobs[i].type != ENEMY_NONE)
                    {
                        enemyCount++;
                    }
                }
                char item_text[100];
                memset(item_text, 0, sizeof(item_text));
                if (pl->room->item.type != ITEM_NONE)
                {
                    sprintf(item_text, "Also you saw '%s' on the ground! Pick it up!", pl->room->item.name);
                    draw_item(&pl->room->item);
                }
                draw_output_text("You looked around and saw %d enemies! %s", enemyCount, item_text);
                draw_mobs(pl->room->mobs);
            }
            else
            {
                draw_output_text("You already looked around!");
            }
            change_info_to_room(pl->room);
        }
        else if (strcasecmp(command, "inventory") == 0)
        {
            int inv_size = player_get_inv_size(pl);
            if (inv_size == 0)
            {
                draw_output_text("No items yet!");
            }
            else
            {
                change_info_title("> INVENTORY <");
                draw_inventory(pl);
            }
        }
        else if (strcasecmp(command, "pickup") == 0)
        {
            if (strlen(arg) == 0)
            {
                draw_output_text("Usage: pickup <item>\n");
            }
            else
            {
                if (strcasecmp(arg, pl->room->item.name) == 0 && pl->room->item.looted == false)
                {
                    int result = player_get_item(pl);
                    if (result == 0)
                    {
                        clear_item_drawing();
                        change_info_to_room(pl->room);
                        clear_player_stats();
                        draw_output_text("Picking up '%s'.\n", pl->room->item.name);
                        draw_player_stats(pl);
                    }
                    else if (result == 1)
                    {
                        draw_output_text("INVENTORY FULL! TRY DROPPING ITEMS!");
                    }
                    else
                    {
                        draw_output_text("You have same item named '%s'!", pl->room->item.name);
                    }
                }
                else
                {
                    draw_output_text("No item found named '%s'.", arg);
                }
            }
        }
        else if (strcasecmp(command, "drop") == 0)
        {
            if (strlen(arg) == 0)
            {
                draw_output_text("Usage: drop <item>\n");
            }
            else
            {
                if (player_check_inv_has(pl, arg))
                {
                    int result = player_drop_item(pl, arg);
                    if (result)
                    {
                        draw_player_stats(pl);
                        draw_output_text("Item '%s' succesfully dropped!", arg);
                    }
                    else
                    {
                        draw_output_text("Well, you can't drop it!");
                    }
                }
                else
                {
                    draw_output_text("No item found named '%s'.", arg);
                }
            }
        }
        else if (strcasecmp(command, "attack") == 0)
        {
            int a_status = player_init_attack(pl, arg);
            if (a_status == -1)
            {
                draw_output_text("No enemy found in that direction/name!");
            }
            else
            {
                player_start_attack(pl, a_status);
            }
        }
        else if (strcasecmp(command, "list") == 0)
        {
            list_saves();
        }
        else if (strcasecmp(command, "save") == 0)
        {
            if (strlen(arg) == 0)
            {
                draw_output_text("Usage: save <filepath>\n");
            }
            else
            {
                save_player(pl, arg);
            }
        }
        else if (strcasecmp(command, "load") == 0)
        {
            if (strlen(arg) == 0)
            {
                draw_output_text("Usage: load <filepath>\n");
            }
            else
            {
                Player *player = (Player *)malloc(sizeof(Player));
                memset(player, 0, sizeof(*player));
                load_player(player, arg);
                draw_dungeon(player->room);
                draw_player_stats(player);
                *pl = *player;
            }
        }
        else if (strcasecmp(command, "help") == 0)
        {
            change_info_title("> HELP PAGE <");
            change_info_to_help();
        }
        else if (strcasecmp(command, "exit") == 0)
        {
            draw_output_text("Game is closing... See you later!\n");
            exit(0);
        }
        else
        {
            draw_output_text("Unknown command! Type help to see all commands!");
        }
    }
}
