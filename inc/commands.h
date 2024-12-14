#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "player.h"
#include "enemy.h"
#include "main.h"
#include "save.h"

// Define max command and arg length to determine max char array size
#define MAX_COMMAND_LENGTH 100
#define MAX_ARG_LENGTH 50

void command_handle_war(const char *input, Player *pl);
void command_handle(const char *input, Player *pl);

#endif