#include "main.h"

/* @
 * init_game: void
 * ---------------
 * Initializes the game by setting up the terminal, drawing the screen layout,
 * and creating the initial room and player state.
 *
 * Parameters:
 * - pl: Player* - Pointer to the Player structure to be initialized.
 *
 * Notes:
 * - Calls helper functions to get the terminal size and draw game borders.
 * - Creates the first room randomly and sets up the player's initial stats.
 * - Displays the dungeon and player stats on the screen.
 */
void init_game(Player *pl){
    // For better quality, get the terminal size from OS.
    get_terminal_size();

    // Drawing borders of game screen
    draw_borders();

    // Draw title
    draw_text_center("> Dungeons of AYBU <", 0);
    
    // Draw input text
    draw_input_text();

    // Create first room
    Room *r = (Room*)malloc(sizeof(Room));
    room_create_random(r, 0);
    // Create player
    memset(pl, 0, sizeof(*pl));
    player_start(pl);
    pl->room = r;
    draw_dungeon(r);
    draw_player_stats(pl);
}
/* @
 * main: int
 * ---------
 * The entry point for the game. Initializes the game and enters the command handling loop.
 *
 * Returns:
 * - 0 on successful execution, -1 on error during input.
 *
 * Notes:
 * - Allocates memory for the player and initializes the game state.
 * - Enters a loop to read user input, process commands, and update the game state.
 * - Commands are handled by `command_handle` function, with input sanitized to remove newline characters.
 */
int main() {
    Player *pl = (Player*)malloc(sizeof(Player));
    init_game(pl);
    // COMMAND HANDLING
    char input[128];
    while(1) {
        move_cursor_default();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Exiting.\n");
            return -1;
        }
        // Remove newline character from input
        input[strcspn(input, "\n")] = '\0';

        clear_input();
        move_cursor_default();
        command_handle(input, pl);

    }
    return 0;
}
