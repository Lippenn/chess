#include "../raylib/include/raylib.h"
#include "string.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 32

typedef struct
{
    char *key;
    int value;
    int x_pos;
    int y_pos;
    int move_count;
    bool is_alive;
    bool is_occupied;
    const char *color;
    const char *type;
    char *path_name;
    Texture2D texture;
} PieceMapEntry;

typedef struct
{
    PieceMapEntry entries[TABLE_SIZE];
} PieceMap;

const char *path_names[] = {
    "../assets/white-pawn.png",   "../assets/white-rook.png",  "../assets/white-knight.png",
    "../assets/white-bishop.png", "../assets/white-queen.png", "../assets/white-king.png",
    "../assets/black-pawn.png",   "../assets/black-rook.png",  "../assets/black-knight.png",
    "../assets/black-bishop.png", "../assets/black-queen.png", "../assets/black-king.png"};

char *toLowerString(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}

void assign_default_map_values(char *str, int index, int count, int x_pos, int y_pos,
                               const char *color, const char *type, PieceMap *map)
{
    int len = snprintf(NULL, 0, "%s_%d", str, count);

    map->entries[index].key = malloc(len + 1);

    snprintf(map->entries[index].key, len + 1, "%s_%d", str, count);

    char lower_color[6];
    char lower_type[32];

    strcpy(lower_color, color);
    strcpy(lower_type, type);

    toLowerString(lower_color);
    toLowerString(lower_type);

    len = snprintf(NULL, 0, "assets/%s-%s.png", lower_color, lower_type);

    map->entries[index].path_name = malloc(len + 1);

    snprintf(map->entries[index].path_name, len + 1, "assets/%s-%s.png", lower_color, lower_type);

    map->entries[index].value = index;
    map->entries[index].is_occupied = false;
    map->entries[index].is_alive = true;
    map->entries[index].x_pos = x_pos;
    map->entries[index].y_pos = y_pos;
    map->entries[index].type = type;
    map->entries[index].color = color;
    map->entries[index].move_count = 0;
}

void map_free(PieceMap *map)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        free(map->entries[i].key);
        free(map->entries[i].path_name);
    }

    free(map->entries);
}

void init_map(PieceMap *map)
{
    typedef struct
    {
        char *name;
        int count;
    } PieceType;

    const PieceType pieces[] = {{"PAWN", 8},   {"ROOK", 2},  {"KNIGHT", 2},
                                {"BISHOP", 2}, {"QUEEN", 1}, {"KING", 1}};

    for (int z = 0; z < 2; z++)
    {
        int square = z * 16;

        for (int p = 0; p < 6; p++)
        {
            for (int n = 1; n <= pieces[p].count; n++)
            {
                char key[32];
                const char *type = pieces[p].name;
                const char *color = z == 0 ? "WHITE" : "BLACK";

                snprintf(key, sizeof(key), "%s_%s", color, pieces[p].name);

                int x_pos = square % 8;
                int y_pos = z == 0 ? square / 8 : 6 + ((square - 16) / 8);

                assign_default_map_values(key, square++, n, x_pos, y_pos, color, type, map);
            }
        }
    }
}

void print_map(PieceMap *map)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        printf("%s , %i , %i", map->entries[i].key, map->entries[i].x_pos, map->entries[i].y_pos);
        printf("\n");
    }
}

PieceMapEntry *find_entry_at_xy_pos(PieceMap *map, int x, int y)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (map->entries[i].x_pos == x && map->entries[i].y_pos == y)
        {
            return &map->entries[i];
        }
    }

    return NULL;
}