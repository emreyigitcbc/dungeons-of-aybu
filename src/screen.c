#include "screen.h"

// Declaring width and height here to update dynamically.
int WIDTH, HEIGHT;

/*
clear_console : void
Clears console by sendinjg special chars or commands.
*/
void clear_console()
{
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J");
    printf("\033[H");
#endif
}

/*
get_terminal_size : void
Gets current terminal size from OS.
*/
void get_terminal_size()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        WIDTH = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top;
    }
    else
    {
        WIDTH = 80;
        HEIGHT = 24;
    }
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
    {
        WIDTH = w.ws_col;
        HEIGHT = w.ws_row;
    }
    else
    {
        WIDTH = 80;
        HEIGHT = 24;
    }
#endif
}
/*
###################################
###           MOVE CURSOR       ###
###################################
*/
/*
move_cursor : void
args:
- x : int
- y : int
Moves cursor to desired x and y places. So user can print anything from that location.
*/
void move_cursor(int x, int y)
{
    printf("\033[%d;%dH", y + 1, x + 1);
}

void move_cursor_default()
{
    move_cursor(CMD_STRING_OFFSET_W, HEIGHT - CMD_STRING_OFFSET_S);
}
void move_cursor_output()
{
    move_cursor(CMD_OUTPUT_OFFSET_W, HEIGHT - CMD_OUTPUT_OFFSET_S);
}
void move_cursor_info_1()
{
    move_cursor(CMD_OUTPUT_OFFSET_W, HEIGHT - CMD_OUTPUT_OFFSET_S - 4);
}
void move_cursor_info_2()
{
    move_cursor(CMD_OUTPUT_OFFSET_W, HEIGHT - CMD_OUTPUT_OFFSET_S - 3);
}
/*
###################################
###         CLEAR FUNCTIONS     ###
###################################
*/
void clear_input()
{
    // clear user input
    move_cursor(CMD_STRING_OFFSET_W, HEIGHT - CMD_STRING_OFFSET_S);
    for (int i = CMD_STRING_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf(" ");
    }
    move_cursor(CMD_OUTPUT_OFFSET_W, HEIGHT - CMD_OUTPUT_OFFSET_S);
    for (int i = CMD_OUTPUT_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf(" ");
    }
    move_cursor(CMD_STRING_OFFSET_W, HEIGHT - CMD_STRING_OFFSET_S);
}
void clear_mobs()
{
    draw_text_center("                          ", ROOM_OFFSET_N + 3);
    move_cursor(ROOM_OFFSET_W + 3, (HEIGHT - ROOM_OFFSET_S - ROOM_OFFSET_N) / 2 + ROOM_OFFSET_N);
    printf("            ");
    draw_text_center("                                      ", HEIGHT - (ROOM_OFFSET_S + 3));
    move_cursor(WIDTH - ROOM_OFFSET_E - 15, (HEIGHT - ROOM_OFFSET_S - ROOM_OFFSET_N) / 2 + ROOM_OFFSET_N);
    printf("            ");

    move_cursor_output();
}
void clear_player_stats()
{
    move_cursor(CMD_OFFSET_W, HEIGHT - CMD_OFFSET_S + 1);
    for (int i = CMD_OFFSET_W; i < ROOM_OFFSET_E - 1; i++)
    {
        printf(" ");
    }
    move_cursor_output();
}

void clear_item_drawing()
{
    move_cursor(ROOM_OFFSET_W + 6, ROOM_OFFSET_N + 6);
    printf("                    ");
    move_cursor_output();
}
void clear_info_1()
{
    move_cursor_info_1();
    for (int i = CMD_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf(" ");
    }
    move_cursor_output();
}
void clear_info_2()
{
    move_cursor_info_2();
    for (int i = CMD_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf(" ");
    }
    move_cursor_output();
}
/*
###################################
###      DRAWING FUNCTIONS      ###
###################################
*/
/*
draw_borders : void
Draw game borders and input-output & inventory area. This is for decoration purposes only.
*/
void draw_borders()
{
    clear_console();

    move_cursor(0, 0);

    printf("+"); // left corner
    // top
    for (int i = ROOM_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf("-");
    }
    // right corner
    printf("+");
    // left wall
    for (int i = ROOM_OFFSET_N; i < HEIGHT; i++)
    {
        move_cursor(0, i);
        printf("|");
    }
    move_cursor(0, HEIGHT);
    printf("+");
    // below
    for (int i = ROOM_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf("-");
    }
    move_cursor(WIDTH - ROOM_OFFSET_E, HEIGHT);
    printf("+");
    // right wall
    for (int i = ROOM_OFFSET_W; i < HEIGHT; i++)
    {
        move_cursor(WIDTH, i);
        printf("|");
    }
    // inventory and command prompt
    move_cursor(0, HEIGHT - ROOM_OFFSET_S + 1);
    printf("+");
    for (int i = ROOM_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf("-");
    }
    printf("+");

    change_info_title("> ROOM <");
}
/*
draw_text : void
args:
- text : const char *
- x : int
- y : int
Draws text to desired x and y coordinates in the screen.
*/

void draw_text(const char *text, int x, int y)
{
    move_cursor(x, y);
    printf("%s", text);
}
/*
draw_text_center : void
args:
- text : const char *
- y : int
Draws centered-text to desired row (y).
*/
void draw_text_center(const char *text, int y, ...)
{
    va_list args;
    va_start(args, text);
    move_cursor(WIDTH / 2 - strlen(text) / 2, y);
    vprintf(text, args);
}
/*
draw_text_center : void
Prints "COMMAND: " string to specific place.
*/
void draw_input_text()
{
    move_cursor(CMD_OFFSET_W, HEIGHT - CMD_OFFSET_S);
    printf("COMMAND: ");
}

void draw_war_info(Player *pl)
{
    clear_info_1();
    clear_info_2();
    change_info_title("> ATTACK <");
    move_cursor_info_1();
    printf("Player vs %s! [ Commands : HIT KICK FLEE] (Flee chance: %.1f%%)", enemy_get_simple_name(pl->room->mobs[pl->warIndex].type), pl->room->mobs[pl->warIndex].flee_chance * 100);
    move_cursor_info_2();
    printf("> %s HEALTH: %.1f | STRENGTH: %.1f", enemy_get_simple_name(pl->room->mobs[pl->warIndex].type), pl->room->mobs[pl->warIndex].health, pl->room->mobs[pl->warIndex].damage);
}
void draw_item(Item *i)
{
    if (i->type != ITEM_NONE && i->looted == false)
    {
        move_cursor(ROOM_OFFSET_W + 6, ROOM_OFFSET_N + 6);
        printf("[ %s ]", i->name);
    }
}

void draw_dungeon(Room *r)
{
    clear_item_drawing();
    clear_mobs();
    // Print walls
    game_print_north_wall((r->doors & 0b1000) >> 3);
    game_print_east_wall((r->doors & 0b0100) >> 2);
    game_print_south_wall((r->doors & 0b0010) >> 1);
    game_print_west_wall((r->doors & 0b0001));

    if (r->searched)
    {
        draw_mobs(r->mobs);
        draw_item(&r->item);
    }
    // Print player
    draw_player();

    change_info_to_room(r);

    move_cursor_output();
}
/*
 * draw_player: void
 * This function draws player in the center of room. It is only for decoration.
 */
void draw_player()
{
    draw_text_center("_", HEIGHT / 2 - 4);
    draw_text_center("/ \\", HEIGHT / 2 - 3);
    draw_text_center("\\_/", HEIGHT / 2 - 2);
    draw_text_center(" | ", HEIGHT / 2 - 1);
    draw_text_center("\\|/", HEIGHT / 2);
    draw_text_center(" | ", HEIGHT / 2 + 1);
    draw_text_center("/ \\", HEIGHT / 2 + 2);
}

void draw_mobs(Enemy *mobs)
{
    if (mobs[3].type != ENEMY_NONE && mobs[3].health > 0)
    {
        // north guard
        draw_text_center(enemy_get_name(mobs[3].type), ROOM_OFFSET_N + 3);
    }
    if (mobs[0].type != ENEMY_NONE && mobs[0].health > 0)
    {
        // west guard
        move_cursor(ROOM_OFFSET_W + 3, (HEIGHT - ROOM_OFFSET_S - ROOM_OFFSET_N) / 2 + ROOM_OFFSET_N);
        printf("%s", enemy_get_name(mobs[0].type));
    }
    if (mobs[1].type != ENEMY_NONE && mobs[1].health > 0)
    {
        // south guard
        draw_text_center(enemy_get_name(mobs[1].type), HEIGHT - (ROOM_OFFSET_S + 3));
    }
    if (mobs[2].type != ENEMY_NONE && mobs[2].health > 0)
    {
        // east guard
        move_cursor(WIDTH - ROOM_OFFSET_E - 15, (HEIGHT - ROOM_OFFSET_S - ROOM_OFFSET_N) / 2 + ROOM_OFFSET_N);
        printf("%s", enemy_get_name(mobs[2].type));
    }

    move_cursor_output();
}

void draw_player_stats(Player *pl)
{
    clear_player_stats();
    move_cursor(CMD_OFFSET_W, HEIGHT - CMD_OFFSET_S - 1);
    float total_strength = pl->strength;
    float total_defence = pl->defence;
    float total_crit_rate = pl->crit_rate;
    float total_crit_chance = pl->crit_chance;
    printf("> PLAYER HEALTH: %.1f/%.1f | STRENGTH: %.1f | DEFENCE: %.1f | CRIT RATE: %.1f | CRIT CHANCE: %.1f", pl->health, pl->maxHealth, total_strength, total_defence, total_crit_rate, total_crit_chance);
}
void draw_game_over(Player *pl)
{
    draw_text_center("GAME OVER", ROOM_OFFSET_N + 4);
    draw_text_center("You are dead!", ROOM_OFFSET_N + 6);
    draw_text_center("You killed %d enemies.", ROOM_OFFSET_N + 7, pl->mobs_killed);
    draw_text_center("You walked %d rooms.", ROOM_OFFSET_N + 8, pl->rooms_walked + 1);
    draw_text_center("Press enter to restart!", ROOM_OFFSET_N + 10);
}

void draw_output_text(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    move_cursor_output();
    vprintf(format, args);
    move_cursor_output();
}

void draw_inventory(Player *pl)
{
    clear_info_1();
    clear_info_2();
    move_cursor_info_1();
    char result[150];
    for (int i = 0; i < PLAYER_INV_SIZE; i++)
    {
        memset(result, 0, sizeof(result));
        if (pl->inventory[i].type != ITEM_NONE)
        {
            sprintf(result, ">> [%s] ", pl->inventory[i].name);
            if (pl->inventory[i].health != 0)
            {
                sprintf(result, "%sHP: %.1f ", result, pl->inventory[i].health);
            }
            if (pl->inventory[i].crit_rate != 0)
            {
                sprintf(result, "%sCR: %.1f ", result, pl->inventory[i].crit_rate);
            }
            if (pl->inventory[i].crit_chance != 0)
            {
                sprintf(result, "%sCC: %.1f ", result, pl->inventory[i].crit_chance);
            }
            if (pl->inventory[i].defence != 0)
            {
                sprintf(result, "%sDF: %.1f ", result, pl->inventory[i].defence);
            }
            if (pl->inventory[i].strength != 0)
            {
                sprintf(result, "%sST: %.1f ", result, pl->inventory[i].strength);
            }
            printf("%s<<", result);
        }
        if ((float)(PLAYER_INV_SIZE / 2) == (float)(i + 1))
        {
            move_cursor_info_2();
        }
    }
}
/*
###################################
###          CHANGE INFO        ###
###################################
*/
void change_info_to_room(Room *r)
{
    clear_info_1();
    clear_info_2();
    move_cursor_info_1();
    if (r->searched)
    {
        printf("ROOM NAME: %s | ENEMIES: %s | ITEM: %s | OPEN DOORS: %s", r->name, room_get_enemy_names(r), room_get_item_name(r), room_get_open_doors(r));
    }
    else
    {
        printf("ROOM NAME: ????? | ENEMIES: ????? | ITEM: ????? | OPEN DOORS: ?????");
    }
}
void change_info_to_help()
{
    clear_info_1();
    clear_info_2();
    move_cursor_info_1();
    printf("Available Game Commands: look, move, inventory, attack, pickup, drop");
    move_cursor_info_2();
    printf("Available Menu Commands: list, save, load, exit");
    move_cursor_output();
}
/*
change_info_title : void
args:
- text : char *
Draws text to title place.
*/
void change_info_title(char *text)
{
    move_cursor(0, HEIGHT - ROOM_OFFSET_S + 1);
    printf("+");
    for (int i = ROOM_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf("-");
    }
    printf("+");
    draw_text_center(text, HEIGHT - ROOM_OFFSET_S + 1);
}
/*
###################################
###       ROOM DOOR-OPENING     ###
###################################
*/
void open_door_north()
{
    draw_text_center("       ", 1);
    draw_text_center("       ", 2);
}

void open_door_south()
{
    draw_text_center("       ", HEIGHT - ROOM_OFFSET_S);
    draw_text_center("       ", HEIGHT - ROOM_OFFSET_S - 1);
}

void open_door_west()
{
    for (int i = (HEIGHT - ROOM_OFFSET_S - ROOM_OFFSET_N) / 2 + ROOM_OFFSET_N - 2; i < (HEIGHT - ROOM_OFFSET_S) / 2 + 3 + ROOM_OFFSET_N; i++)
    {
        move_cursor(ROOM_OFFSET_W, i);
        printf("  ");
    }
}

void open_door_east()
{
    for (int i = (HEIGHT - ROOM_OFFSET_S - ROOM_OFFSET_N) / 2 + ROOM_OFFSET_N - 2; i < (HEIGHT - ROOM_OFFSET_S) / 2 + 3 + ROOM_OFFSET_N; i++)
    {
        move_cursor(WIDTH - ROOM_OFFSET_E - 2, i);
        printf("  ");
    }
}
/*
###################################
###      ROOM WALL PRINTING     ###
###################################
*/
void game_print_north_wall(unsigned char isOpen)
{
    move_cursor(ROOM_OFFSET_W, ROOM_OFFSET_N);
    for (int i = ROOM_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf("#");
    }
    move_cursor(ROOM_OFFSET_W, ROOM_OFFSET_N + 1);
    for (int i = ROOM_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf("#");
    }
    if (isOpen)
    {
        open_door_north();
    }
}
void game_print_south_wall(unsigned char isOpen)
{
    move_cursor(ROOM_OFFSET_W, HEIGHT - ROOM_OFFSET_S);
    for (int i = ROOM_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf("#");
    }
    move_cursor(ROOM_OFFSET_W, HEIGHT - ROOM_OFFSET_S - 1);
    for (int i = ROOM_OFFSET_W; i < WIDTH - ROOM_OFFSET_E; i++)
    {
        printf("#");
    }
    if (isOpen)
    {
        open_door_south();
    }
}

void game_print_west_wall(unsigned char isOpen)
{
    for (int i = ROOM_OFFSET_N; i < HEIGHT - ROOM_OFFSET_S; i++)
    {
        move_cursor(1, i);
        printf("##");
    }
    if (isOpen)
    {
        open_door_west();
    }
}

void game_print_east_wall(unsigned char isOpen)
{
    for (int i = ROOM_OFFSET_N; i < HEIGHT - ROOM_OFFSET_S; i++)
    {
        move_cursor(WIDTH - ROOM_OFFSET_E - 2, i);
        printf("##");
    }
    if (isOpen)
    {
        open_door_east();
    }
}