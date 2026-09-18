#include "../raylib/include/raylib.h"
#include "string.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 32

extern Rectangle board;

typedef struct
{
    char *key;
    int value;
    int x_pos;
    int y_pos;
    int move_count;
    bool is_alive;
    bool is_occupied;
    char color;
    const char *type;
    int type_index;
    char *path_name;
    Texture2D texture;
} PieceMapEntry;

typedef struct
{
    int x;
    int y;
} RouteCheckObj;

typedef enum
{
    EMPTY,
    FRIENDLY,
    ENEMY
} RouteStatus;

bool flip = false;

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

void assign_default_map_values(char *str, int index, int type_index, int count, int x_pos,
                               int y_pos, char color, const char *type, PieceMap *map)
{
    int len = snprintf(NULL, 0, "%s_%d", str, count);

    map->entries[index].key = malloc(len + 1);

    snprintf(map->entries[index].key, len + 1, "%s_%d", str, count);

    char lower_type[32];

    strcpy(lower_type, type);

    toLowerString(lower_type);

    len =
        snprintf(NULL, 0, "assets/%s-%s.png", strcmp(&color, "w") ? "white" : "black", lower_type);

    map->entries[index].path_name = malloc(len + 1);

    snprintf(map->entries[index].path_name, len + 1, "assets/%s-%s.png",
             strcmp(&color, "w") ? "white" : "black", lower_type);

    map->entries[index].value = index;
    map->entries[index].is_occupied = false;
    map->entries[index].is_alive = true;
    map->entries[index].x_pos = x_pos;
    map->entries[index].y_pos = y_pos;
    map->entries[index].type = type;
    map->entries[index].color = color;
    map->entries[index].type_index = type_index;
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
                char color = *(z == 0 ? "w" : "b");

                snprintf(key, sizeof(key), "%s_%s", &color, pieces[p].name);
                int x_pos = 0;
                if (p > 0)
                {
                    if (p > 3)
                    {
                        if (!flip)
                        {
                            x_pos = p == 5 ? 3 : 4;
                        }
                        else
                        {
                            x_pos = p == 5 ? 4 : 3;
                        }
                    }
                    else
                    {
                        x_pos = n % 2 == 0 ? p - 1 : 8 - p;
                    }
                }
                else
                {
                    x_pos = square % 8;
                }

                int y_pos = z == 0 ? abs(-1 + square / 8) : 6 + ((square - 16) / 8);

                assign_default_map_values(key, square++, p + 1, n, x_pos, y_pos, color, type, map);
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

RouteStatus piece_route_check(PieceMap *map, int x, int y, char color)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (map->entries[i].x_pos == x && map->entries[i].y_pos == y)
        {
            if (map->entries[i].color == color)
                return FRIENDLY;

            return ENEMY;
        }
    }

    return EMPTY;
}

int route_check_pawn(RouteCheckObj routes[27], PieceMapEntry pawn, PieceMap *map)
{
    int count = 0;
    char piece_color = pawn.color;
    bool is_white = (piece_color == 'w');

    if (!is_white)
    {
        routes[count++] = (RouteCheckObj){pawn.x_pos, pawn.y_pos - 1};
        RouteCheckObj capture[] = {{1, -1}, {-1, -1}};
        for (int i = 0; i < 2; i++)
        {
            RouteStatus status = piece_route_check(map, pawn.x_pos + capture[i].x,
                                                   pawn.y_pos + capture[i].y, pawn.color);
            if (status == ENEMY)
            {
                routes[count++] = (RouteCheckObj){
                    pawn.x_pos + capture[i].x,
                    pawn.y_pos + capture[i].y,
                };
            }
        }
    }
    else
    {
        routes[count++] = (RouteCheckObj){pawn.x_pos, pawn.y_pos + 1};
        RouteCheckObj capture[] = {{1, 1}, {-1, 1}};
        for (int i = 0; i < 2; i++)
        {
            RouteStatus status = piece_route_check(map, pawn.x_pos + capture[i].x,
                                                   pawn.y_pos + capture[i].y, pawn.color);
            if (status == ENEMY)
            {
                routes[count++] = (RouteCheckObj){
                    pawn.x_pos + capture[i].x,
                    pawn.y_pos + capture[i].y,
                };
            }
        }
    }
    if (pawn.move_count == 0)
    {
        if (!is_white)
        {
            routes[count++] = (RouteCheckObj){pawn.x_pos, pawn.y_pos - 2};
        }
        else
        {
            routes[count++] = (RouteCheckObj){pawn.x_pos, pawn.y_pos + 2};
        }
    }
    return count;
}

int route_check_rook(RouteCheckObj routes[27], PieceMapEntry rook, PieceMap *map)
{
    int count = 0;
    // RIGHT
    for (int x = rook.x_pos + 1; x < 8; x++)
    {
        RouteStatus status = piece_route_check(map, x, rook.y_pos, rook.color);

        if (status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
        }
        else if (status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
            break;
        }
        else
        {
            break;
        }
    }
    // LEFT
    for (int x = rook.x_pos - 1; x >= 0; x--)
    {
        RouteStatus status = piece_route_check(map, x, rook.y_pos, rook.color);

        if (status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
        }
        else if (status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
            break;
        }
        else
        {
            break;
        }
    }
    // UP
    for (int y = rook.y_pos - 1; y >= 0; y--)
    {
        RouteStatus status = piece_route_check(map, rook.x_pos, y, rook.color);

        if (status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
        }
        else if (status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
            break;
        }
        else
        {
            break;
        }
    }
    // DOWN
    for (int y = rook.y_pos + 1; y < 8; y++)
    {
        RouteStatus status = piece_route_check(map, rook.x_pos, y, rook.color);

        if (status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
        }
        else if (status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
            break;
        }
        else
        {
            break;
        }
    }
    return count;
}

int route_check_knight(RouteCheckObj routes[27], PieceMapEntry knight, PieceMap *map)
{
    int count = 0;

    int x_pos = knight.x_pos;
    int y_pos = knight.y_pos;

    const RouteCheckObj moves[8] = {
        {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, 1},
    };

    for (int i = 0; i < 8; i++)
    {
        bool flag = true;
        int x = moves[i].x;
        int y = moves[i].y;

        if ((x_pos + x) < 0 || (x_pos + x) >= 8)
        {
            flag = false;
        }
        if ((y_pos + y) < 0 || (y_pos + y) >= 8)
        {
            flag = false;
        }
        if (flag)
        {
            RouteStatus status = piece_route_check(map, x_pos + x, y_pos + y, knight.color);
            if (status == EMPTY || status == ENEMY)
            {
                routes[count++] = (RouteCheckObj){x_pos + x, y_pos + y};
            }
        }
    }
    return count;
}

int route_check_bishop(RouteCheckObj routes[27], PieceMapEntry bishop, PieceMap *map, int _count)
{
    int count = (_count == -1) ? 0 : _count;

    int x_pos = bishop.x_pos;
    int y_pos = bishop.y_pos;

    for (int i = 0; i < 4; i++)
    {
        int track_x = x_pos;
        int track_y = y_pos;
        while (track_x >= 0 && track_y >= 0 && track_x < 8 && track_y < 8)
        {
            RouteStatus status = piece_route_check(map, track_x, track_y, bishop.color);
            if (status == EMPTY)
            {
                routes[count++] = (RouteCheckObj){track_x, track_y};
            }
            else if (status == ENEMY)
            {
                routes[count++] = (RouteCheckObj){track_x, track_y};
                break;
            }
            else if (status == FRIENDLY && track_x != x_pos && track_y != y_pos)
            {
                break;
            }
            switch (i)
            {
            case 0:
                track_x = track_x - 1;
                track_y = track_y - 1;
                break;
            case 1:
                track_x = track_x + 1;
                track_y = track_y - 1;
                break;
            case 2:
                track_x = track_x - 1;
                track_y = track_y + 1;
                break;
            case 3:
                track_x = track_x + 1;
                track_y = track_y + 1;
            }
        }
    }
    return count;
}

int route_check_queen(RouteCheckObj routes[27], PieceMapEntry queen, PieceMap *map)
{
    int count = 0;
    count = route_check_rook(routes, queen, map);
    return route_check_bishop(routes, queen, map, count);
}

int route_check_king(RouteCheckObj routes[27], PieceMapEntry king, PieceMap *map)
{
    const RouteCheckObj moves[8] = {
        {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0},
    };

    int x_pos = king.x_pos;
    int y_pos = king.y_pos;
    int count = 0;

    for (int i = 0; i < 8; i++)
    {
        bool flag = true;
        int x = moves[i].x;
        int y = moves[i].y;

        if ((x_pos + x) < 0 && (x_pos + x) >= 8)
        {
            flag = false;
        }
        if ((y_pos + y) < 0 && (y_pos + y) >= 8)
        {
            flag = false;
        }
        if (flag)
        {
            RouteStatus status = piece_route_check(map, x_pos + x, y_pos + y, king.color);
            if (status == EMPTY || status == ENEMY)
            {
                routes[count++] = (RouteCheckObj){x_pos + x, y_pos + y};
            }
        }
    }
    return count;
}

bool check_valid_move(RouteCheckObj available_routes[27], RouteCheckObj route)
{
    bool flag = false;
    for (int i = 0; i < 27; i++)
    {
        if (available_routes[i].x == route.x && available_routes[i].y == route.y)
        {
            flag = true;
        }
    }
    return flag;
}

void move_piece(int x, int y, PieceMapEntry *piece, RouteCheckObj available_routes[27],
                PieceMap *map)
{

    if (piece == NULL || map == NULL)
    {
        return;
    }
    if (piece->type_index <= 0)
    {
        return;
    }
    if (!check_valid_move(available_routes, (RouteCheckObj){x, y}))
    {
        return;
    }

    piece->x_pos = x;
    piece->y_pos = y;
    piece->move_count++;
}
