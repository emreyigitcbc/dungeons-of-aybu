#ifndef SAVE_H
#define SAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "player.h"

void save_player(Player *player, char *filename);
void load_player(Player *player, char *filename);
void list_saves();

#endif