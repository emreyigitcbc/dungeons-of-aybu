#include "save.h"

void save_player(Player *player, char *filename) {
    char f[50];
    sprintf(f, "save_%s.dat", filename);
    FILE *file = fopen(f, "wb");
    if (file == NULL) {
        perror("The file can not be opened.");
        return;
    }

    // save player struct without pointers
    fwrite(player, sizeof(Player) - sizeof(Room *) - sizeof(char *), 1, file);

    // save room
    if (player->room) {
        short hasRoom = 1; // do we have room?
        fwrite(&hasRoom, sizeof(short), 1, file);

        // save room data
        fwrite(player->room, sizeof(Room), 1, file);

        // save the pointers in the room data
        int nameLen = strlen(player->room->name) + 1;
        int itemNameLen = strlen(player->room->item.name) + 1;
        fwrite(&nameLen, sizeof(int), 1, file);
        fwrite(player->room->name, sizeof(char), nameLen, file);
        fwrite(&itemNameLen, sizeof(int), 1, file);
        fwrite(player->room->item.name, sizeof(char), itemNameLen, file);
        // save item names
        for(int i = 0; i < PLAYER_INV_SIZE; i++) {
            if(player->inventory[i].name != NULL) {
                int invItemNameLen = strlen(player->inventory[i].name) + 1;
                fwrite(&invItemNameLen, sizeof(int), 1, file);
                fwrite(player->inventory[i].name, sizeof(char), invItemNameLen, file);
            }
        }
    } else {
        short hasRoom = 0; // no room
        fwrite(&hasRoom, sizeof(short), 1, file);
    }

    fclose(file);
    printf("Game successfully saved to: '%s'!", f);
}

void load_player(Player *player, char *filename) {
    char f[50];
    sprintf(f, "save_%s.dat", filename);
    FILE *file = fopen(f, "rb");
    if (file == NULL) {
        perror("The file can not be opened.");
        return;
    }

    // read player struct without pointers
    fread(player, sizeof(Player) - sizeof(Room *) - sizeof(char *), 1, file);

    // check if we have room
    short hasRoom;
    fread(&hasRoom, sizeof(short), 1, file);

    if (hasRoom) {
        // allocate memory for room
        player->room = malloc(sizeof(Room));
        if (player->room == NULL) {
            perror("Can't allocate memory!");
            fclose(file);
            return;
        }

        // read room data
        fread(player->room, sizeof(Room), 1, file);

        // read room name char* in room
        int nameLen;
        fread(&nameLen, sizeof(int), 1, file);

        player->room->name = malloc(nameLen);
        if (player->room->name == NULL) {
            perror("Can't allocate memory!");
            fclose(file);
            return;
        }
        fread(player->room->name, sizeof(char), nameLen, file);
        // read item name char* in room
        int itemNameLen;
        fread(&itemNameLen, sizeof(int), 1, file);

        player->room->item.name = malloc(itemNameLen);
        if (player->room->item.name == NULL) {
            perror("Can't allocate memory!");
            fclose(file);
            return;
        }
        fread(player->room->item.name, sizeof(char), itemNameLen, file);
        // read item names
        for(int i = 0; i < PLAYER_INV_SIZE; i++) {
            int invItemNameLen;
            fread(&invItemNameLen, sizeof(int), 1, file);
            player->inventory[i].name = malloc(invItemNameLen);
            fread(player->inventory[i].name, sizeof(char), invItemNameLen, file);
        }
    } else {
        player->room = NULL; // if no room
    }

    fclose(file);
    printf("The game loaded from '%s'!", f);
}

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
#endif

void list_saves() {
    printf("Saved games: ");

#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("save_*.dat", &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No saved games.");
        return;
    }

    do {
        printf("%s ", findFileData.cFileName);
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

#else
    struct dirent *entry;
    DIR *dp = opendir("./");

    if (dp == NULL) {
        perror("Could not open directory!");
        return;
    }

    while ((entry = readdir(dp))) {
        if (strncmp(entry->d_name, "save_", 5) == 0 && strstr(entry->d_name, ".dat")) {
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dp);
#endif
}