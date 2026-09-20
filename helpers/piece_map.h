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

bool black_king_in_danger = false;
bool white_king_in_danger = false;

typedef struct
{
    char *name;
    int count;
} PieceType;

const PieceType pieces[] = {{"PAWN", 8},   {"ROOK", 2},  {"KNIGHT", 2},
                            {"BISHOP", 2}, {"QUEEN", 1}, {"KING", 1}};

typedef struct
{
    PieceMapEntry entries[TABLE_SIZE];
} PieceMap;

typedef struct
{
    const char *type;
    char color;
    int x;
    int y;
    int type_index;
} StarterPiece;

StarterPiece starter_pieces[] = {
    {"ROOK", 'b', 0, 0, 2},   {"KNIGHT", 'b', 1, 0, 3}, {"BISHOP", 'b', 2, 0, 4},
    {"QUEEN", 'b', 3, 0, 5},  {"KING", 'b', 4, 0, 6},   {"BISHOP", 'b', 5, 0, 4},
    {"KNIGHT", 'b', 6, 0, 3}, {"ROOK", 'b', 7, 0, 2},

    {"PAWN", 'b', 0, 1, 1},   {"PAWN", 'b', 1, 1, 1},   {"PAWN", 'b', 2, 1, 1},
    {"PAWN", 'b', 3, 1, 1},   {"PAWN", 'b', 4, 1, 1},   {"PAWN", 'b', 5, 1, 1},
    {"PAWN", 'b', 6, 1, 1},   {"PAWN", 'b', 7, 1, 1},

    {"PAWN", 'w', 0, 6, 1},   {"PAWN", 'w', 1, 6, 1},   {"PAWN", 'w', 2, 6, 1},
    {"PAWN", 'w', 3, 6, 1},   {"PAWN", 'w', 4, 6, 1},   {"PAWN", 'w', 5, 6, 1},
    {"PAWN", 'w', 6, 6, 1},   {"PAWN", 'w', 7, 6, 1},

    {"ROOK", 'w', 0, 7, 2},   {"KNIGHT", 'w', 1, 7, 3}, {"BISHOP", 'w', 2, 7, 4},
    {"QUEEN", 'w', 3, 7, 5},  {"KING", 'w', 4, 7, 6},   {"BISHOP", 'w', 5, 7, 4},
    {"KNIGHT", 'w', 6, 7, 3}, {"ROOK", 'w', 7, 7, 2},
};

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

void assign_default_map_values(char *str, int index, int type_index, int x_pos, int y_pos,
                               char color, const char *type, PieceMap *map)
{
    int len = snprintf(NULL, 0, "%s_%d", str, index);

    map->entries[index].key = malloc(len + 1);

    snprintf(map->entries[index].key, len + 1, "%s_%d", str, index);

    char lower_type[32];

    strcpy(lower_type, type);

    toLowerString(lower_type);

    len =
        snprintf(NULL, 0, "assets/%s-%s.png", strcmp(&color, "b") ? "white" : "black", lower_type);

    map->entries[index].path_name = malloc(len + 1);

    snprintf(map->entries[index].path_name, len + 1, "assets/%s-%s.png",
             strcmp(&color, "b") ? "white" : "black", lower_type);

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

void reset_map(PieceMap *map)
{
    // for (int i = 0; i < TABLE_SIZE; i++)
    // {
    //     map->entries[i].
    // }
}

void free_map(PieceMap *map)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        free(map->entries[i].key);
        free(map->entries[i].path_name);
    }
}

void print_map(PieceMap *map)
{
    printf("-----------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        printf("%s , %i , %i", map->entries[i].key, map->entries[i].x_pos, map->entries[i].y_pos);
        printf("\n");
    }
}

void init_map(PieceMap *map)
{
    for (int i = 0; i < sizeof(starter_pieces) / sizeof(starter_pieces[0]); i++)
    {
        StarterPiece *piece = &starter_pieces[i];

        char key[32];

        snprintf(key, sizeof(key), "%c_%s", piece->color, piece->type);

        assign_default_map_values(key, i, piece->type_index, piece->x, piece->y, piece->color,
                                  piece->type, map);
    }
    print_map(map);
}

void print_routes(RouteCheckObj routes[27], int *indexer)
{
    printf("-----------------------\n");
    for (int i = 0; i < *indexer; i++)
    {
        printf("x = %i, y = %i", routes[i].x, routes[i].y);
        printf("\n");
    }
}

bool is_entry_at_xy_pos(PieceMap *map, int x, int y)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (map->entries[i].x_pos == x && map->entries[i].y_pos == y)
        {
            return true;
        }
    }

    return false;
}

PieceMapEntry *find_entry_at_xy_pos(PieceMap *map, int x, int y)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (map->entries[i].x_pos == x && map->entries[i].y_pos == y && map->entries[i].is_alive)
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
        if (map->entries[i].is_alive && map->entries[i].x_pos == x && map->entries[i].y_pos == y)
        {
            if (map->entries[i].color == color)
            {
                return FRIENDLY;
                break;
            }

            return ENEMY;
            break;
        }
    }

    return EMPTY;
}

int route_check_pawn(RouteCheckObj routes[27], PieceMapEntry pawn, PieceMap *map, int *indexer)
{
    int count = 0;

    bool is_white = (pawn.color == 'w');

    int direction;

    if (is_white && !flip)
        direction = -1;
    else
        direction = 1;

    RouteStatus route_status =
        piece_route_check(map, pawn.x_pos, pawn.y_pos + direction, pawn.color);

    if (route_status == EMPTY)
    {
        routes[count++] = (RouteCheckObj){pawn.x_pos, pawn.y_pos + direction};

        if (pawn.move_count == 0)
        {
            RouteStatus two_step_route_status =
                piece_route_check(map, pawn.x_pos, pawn.y_pos + direction * 2, pawn.color);

            if (two_step_route_status == EMPTY)
            {
                routes[count++] = (RouteCheckObj){pawn.x_pos, pawn.y_pos + direction * 2};
            }
        }
    }

    RouteCheckObj capture[] = {{1, direction}, {-1, direction}};

    for (int i = 0; i < 2; i++)
    {
        RouteStatus route_capture_status = piece_route_check(map, pawn.x_pos + capture[i].x,
                                                             pawn.y_pos + capture[i].y, pawn.color);

        if (route_capture_status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){pawn.x_pos + capture[i].x, pawn.y_pos + capture[i].y};
        }
    }
    *indexer = count;
    return count;
}

int route_check_rook(RouteCheckObj routes[27], PieceMapEntry rook, PieceMap *map, int *indexer)
{
    int count = 0;
    // RIGHT
    for (int x = rook.x_pos + 1; x < 8; x++)
    {
        RouteStatus route_status = piece_route_check(map, x, rook.y_pos, rook.color);

        if (route_status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
        }
        else if (route_status == ENEMY)
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
        RouteStatus route_status = piece_route_check(map, x, rook.y_pos, rook.color);

        if (route_status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){
                x,
                rook.y_pos,
            };
        }
        else if (route_status == ENEMY)
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
        RouteStatus route_status = piece_route_check(map, rook.x_pos, y, rook.color);

        if (route_status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
        }
        else if (route_status == ENEMY)
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
        RouteStatus route_status = piece_route_check(map, rook.x_pos, y, rook.color);

        if (route_status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
        }
        else if (route_status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
            break;
        }
        else
        {
            break;
        }
    }
    *indexer = count;
    return count;
}

int route_check_knight(RouteCheckObj routes[27], PieceMapEntry knight, PieceMap *map, int *indexer)
{
    int count = 0;

    int x_pos = knight.x_pos;
    int y_pos = knight.y_pos;

    const RouteCheckObj moves[8] = {
        {
            -2,
            1,
        },
        {-1, 2},
        {
            1,
            2,
        },
        {2, 1},
        {
            -2,
            -1,
        },
        {-1, -2},
        {
            1,
            -2,
        },
        {2, -1},
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
            RouteStatus route_status = piece_route_check(map, x_pos + x, y_pos + y, knight.color);
            if (route_status == EMPTY || route_status == ENEMY)
            {
                routes[count++] = (RouteCheckObj){x_pos + x, y_pos + y};
            }
        }
    }
    *indexer = count;
    return count;
}

int route_check_bishop(RouteCheckObj routes[27], PieceMapEntry bishop, PieceMap *map, int *indexer,
                       int _count)
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
            RouteStatus route_status = piece_route_check(map, track_x, track_y, bishop.color);
            if (route_status == EMPTY)
            {
                routes[count++] = (RouteCheckObj){track_x, track_y};
            }
            else if (route_status == ENEMY)
            {
                routes[count++] = (RouteCheckObj){track_x, track_y};
                break;
            }
            else if (route_status == FRIENDLY && track_x != x_pos && track_y != y_pos)
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
    *indexer = count;
    return count;
}

int route_check_queen(RouteCheckObj routes[27], PieceMapEntry queen, PieceMap *map, int *indexer)
{
    int count = 0;
    count = route_check_rook(routes, queen, map, indexer);
    return route_check_bishop(routes, queen, map, indexer, count);
}

int route_check_king(RouteCheckObj routes[27], PieceMapEntry king, PieceMap *map, int *indexer)
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
            RouteStatus route_status = piece_route_check(map, x_pos + x, y_pos + y, king.color);
            if (route_status == EMPTY || route_status == ENEMY)
            {
                routes[count++] = (RouteCheckObj){x_pos + x, y_pos + y};
            }
        }
    }
    *indexer = count;
    return count;
}

void check_capture_piece(PieceMap *map, RouteCheckObj route, PieceMapEntry *piece)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (map->entries[i].x_pos == route.x && map->entries[i].y_pos == route.y &&
            map->entries[i].color != piece->color)
        {
            map->entries[i].is_alive = false;
            return;
        }
    };
    return;
}

bool check_valid_move(RouteCheckObj available_routes[27], RouteCheckObj route, int *indexer,
                      PieceMap *map, PieceMapEntry *piece)
{
    bool flag = false;
    for (int i = 0; i < *indexer; i++)
    {
        if (available_routes[i].x == route.x && available_routes[i].y == route.y)
        {
            flag = true;
            break;
        }
    }
    return flag;
}

bool attacked_by_rook(PieceMap *map, int x, int y, char color)
{
    const int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    for (int i = 0; i < 4; i++)
    {
        int check_x = x + directions[i][0];
        int check_y = y + directions[i][1];

        while (check_x >= 0 && check_x < 8 && check_y >= 0 && check_y < 8)
        {
            PieceMapEntry *entry = find_entry_at_xy_pos(map, check_x, check_y);

            if (entry == NULL || !entry->is_alive)
            {
                check_x += directions[i][0];
                check_y += directions[i][1];
                continue;
            }

            if (entry->color == color && (entry->type_index == 2 || entry->type_index == 5))
            {
                return true;
            }

            break;
        }
    }
    return false;
}

bool attacked_by_bishop(PieceMap *map, int x, int y, char color)
{
    const int directions[4][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};

    for (int i = 0; i < 4; i++)
    {
        int check_x = x + directions[i][0];
        int check_y = y + directions[i][1];

        while (check_x >= 0 && check_x < 8 && check_y >= 0 && check_y < 8)
        {
            PieceMapEntry *entry = find_entry_at_xy_pos(map, check_x, check_y);

            if (entry == NULL || !entry->is_alive)
            {
                check_x += directions[i][0];
                check_y += directions[i][1];
                continue;
            }

            if (entry->color == color && (entry->type_index == 4 || entry->type_index == 5))
            {
                return true;
            }

            break;
        }
    }
    return false;
}

bool attacked_by_piece_type(PieceMap *map, int index, int x, int y, char color)
{
    switch (index)
    {
    case 1:
    {
        // Pawn
        int direction = color == 'w' ? 1 : -1;

        RouteCheckObj capture[] = {{1, direction}, {-1, direction}};

        for (int z = 0; z < 2; z++)
        {
            PieceMapEntry *entry = find_entry_at_xy_pos(map, x + capture[z].x, y + capture[z].y);

            if (entry && entry->is_alive && entry->color == color && entry->type_index == 1)
            {
                return true;
            }
        }

        break;
    }

    case 2:
    {
        // Rook
        return attacked_by_rook(map, x, y, color);
        break;
    }

    case 3:
    {
        // Knight
        const int directions[8][2] = {{-2, 1},  {-1, 2},  {1, 2},  {2, 1},
                                      {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};

        for (int i = 0; i < 8; i++)
        {
            int check_x = x + directions[i][0];
            int check_y = y + directions[i][1];

            if (check_x < 0 || check_x >= 8 || check_y < 0 || check_y >= 8)
            {
                continue;
            }

            PieceMapEntry *entry = find_entry_at_xy_pos(map, check_x, check_y);

            if (entry && entry->is_alive && entry->color == color && entry->type_index == 3)
            {
                return true;
            }
        }

        break;
    }

    case 4:
    {
        // Bishop
        return attacked_by_bishop(map, x, y, color);
        break;
    }
    case 5:
        // Queen
        break;
    }

    return false;
}

bool square_is_attacked(PieceMap *map, int x, int y, char color)
{
    bool flag = false;
    for (int i = 1; i <= 6; i++)
    {
        flag = attacked_by_piece_type(map, i, x, y, color);
        if (flag)
            break;
    }
    return flag;
}

bool move_piece(int x, int y, PieceMapEntry *piece, RouteCheckObj available_routes[27],
                PieceMap *map, int *indexer)
{

    if (piece == NULL || map == NULL)
    {
        return false;
    }
    if (piece->type_index <= 0)
    {
        return false;
    }
    if (!check_valid_move(available_routes, (RouteCheckObj){x, y}, indexer, map, piece))
    {
        return false;
    }

    check_capture_piece(map, (RouteCheckObj){x, y}, piece);

    piece->x_pos = x;
    piece->y_pos = y;
    piece->move_count++;
    return true;
}
