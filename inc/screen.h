#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <stdarg.h>
#endif

#include "room.h"
#include "player.h"
#include "items.h"

// Offset settings for game area or inventory & input area
#define ROOM_OFFSET_N 1
#define ROOM_OFFSET_S 7
#define ROOM_OFFSET_E 1
#define ROOM_OFFSET_W 1

#define CMD_OFFSET_W 3
#define CMD_OFFSET_S 2

#define CMD_OUTPUT_OFFSET_W 3
#define CMD_OUTPUT_OFFSET_S 1

#define CMD_STRING_OFFSET_W 12
#define CMD_STRING_OFFSET_S 2

// Function headers
void get_terminal_size();

void draw_borders();
void draw_text(const char *text, int x, int y);
void draw_text_center(const char *text, int y, ...);
void draw_input_text();
void draw_output_text(const char *format, ...);

void draw_dungeon(Room *r);
void draw_player();
void draw_mobs(Enemy *mobs);
void draw_item(Item *i);
void draw_player_stats(Player *pl);
void draw_inventory(Player *pl);
void draw_war_info(Player *pl);
void draw_game_over(Player *pl);

void move_cursor(int x, int y);
void move_cursor_default();
void move_cursor_output();
void move_cursor_info_1();
void move_cursor_info_2();

void clear_player_stats();
void clear_item_drawing();
void clear_mobs();
void clear_input();
void clear_info_1();
void clear_info_2();

void change_info_to_room(Room *r);
void change_info_to_help();
void change_info_title(char* text);

void game_print_north_wall(unsigned char isOpen);
void game_print_south_wall(unsigned char isOpen);
void game_print_west_wall(unsigned char isOpen);
void game_print_east_wall(unsigned char isOpen);

void open_door_north();
void open_door_south();
void open_door_west();
void open_door_east();
#endif 
