#include "../include/game.h"
#include "./draw.h"
#include <stdio.h>
#include <string.h>

StarterPiece starter_pieces[MAX_PIECES] = {
    // Black
    {ROOK, 'b', 0, 0},
    {KNIGHT, 'b', 1, 0},
    {BISHOP, 'b', 2, 0},
    {QUEEN, 'b', 3, 0},
    {KING, 'b', 4, 0},
    {BISHOP, 'b', 5, 0},
    {KNIGHT, 'b', 6, 0},
    {ROOK, 'b', 7, 0},

    {PAWN, 'b', 0, 1},
    {PAWN, 'b', 1, 1},
    {PAWN, 'b', 2, 1},
    {PAWN, 'b', 3, 1},
    {PAWN, 'b', 4, 1},
    {PAWN, 'b', 5, 1},
    {PAWN, 'b', 6, 1},
    {PAWN, 'b', 7, 1},

    // White
    {PAWN, 'w', 0, 6},
    {PAWN, 'w', 1, 6},
    {PAWN, 'w', 2, 6},
    {PAWN, 'w', 3, 6},
    {PAWN, 'w', 4, 6},
    {PAWN, 'w', 5, 6},
    {PAWN, 'w', 6, 6},
    {PAWN, 'w', 7, 6},

    {ROOK, 'w', 0, 7},
    {KNIGHT, 'w', 1, 7},
    {BISHOP, 'w', 2, 7},
    {QUEEN, 'w', 3, 7},
    {KING, 'w', 4, 7},
    {BISHOP, 'w', 5, 7},
    {KNIGHT, 'w', 6, 7},
    {ROOK, 'w', 7, 7},
};

PieceMap map;
OpenRoutes open_routes = {0};
GameState game_state = {1, 'w', NONE};
PieceMapEntry *current_piece;

void init_map()
{
    map.count = MAX_PIECES;
    for (int i = 0; i < MAX_PIECES; i++)
    {
        StarterPiece starter_piece = starter_pieces[i];
        map.entries[i] = (PieceMapEntry){i,
                                         starter_piece.type,
                                         starter_piece.color,
                                         starter_piece.x,
                                         starter_piece.y,
                                         true,
                                         get_piece_texture(starter_piece.type, starter_piece.color),
                                         0};
    }
}

PieceMapEntry *find_entry_at_xy_pos(int x, int y)
{
    for (int i = 0; i < MAX_PIECES; i++)
    {
        if (map.entries[i].x_pos == x && map.entries[i].y_pos == y)
        {
            return &map.entries[i];
        }
    }
    return NULL;
}

SquareStatusEnum get_square_status_at_xy_pos(int x, int y, char color)
{
    if (x < 8 && x >= 0 && y < 8 && y >= 0)
    {
        for (int i = 0; i < MAX_PIECES; i++)
        {
            if (map.entries[i].x_pos == x && map.entries[i].y_pos == y && map.entries[i].is_alive)
            {
                if (map.entries[i].color == color)
                {
                    return FRIENDLY;
                }
                else
                {
                    return ENEMY;
                }
            }
        }
        return EMPTY;
    }
    else
    {
        return OUTSIDE;
    }
}

XYPosition *is_xy_in_open_routes(int x, int y)
{
    for (int i = 0; i < MAX_MOVES; i++)
    {
        if (open_routes.routes[i].x == x && open_routes.routes[i].y == y)
        {
            return &open_routes.routes[i];
        }
    }
    return NULL;
}

void calculate_pawn_route(PieceMapEntry *pawn)
{
    int direction = pawn->color == 'w' ? -1 : 1;
    int directions[2] = {-1, 1};

    int x = pawn->x_pos;
    int y = pawn->y_pos;
    int count = 0;
    SquareStatusEnum status = get_square_status_at_xy_pos(x, y + direction, pawn->color);
    if (status == EMPTY)
    {
        open_routes.routes[count++] = (XYPosition){x, y + direction, false};
    }

    if (pawn->move_count == 0)
    {
        status = get_square_status_at_xy_pos(x, y + direction * 2, pawn->color);
        if (status == EMPTY)
        {
            open_routes.routes[count++] = (XYPosition){x, y + (direction * 2), false};
        }
    }
    for (int i = 0; i < 2; i++)
    {
        x = pawn->x_pos + directions[i];
        y = pawn->y_pos;
        status = get_square_status_at_xy_pos(x, y + direction, pawn->color);
        if (status == ENEMY)
        {
            open_routes.routes[count++] = (XYPosition){x, y + direction, true};
        }
    }
    open_routes.count = count;
}

void calculate_knight_route(PieceMapEntry *knight)
{
    int count = 0;

    int directions[8][2] = {
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
        SquareStatusEnum status = get_square_status_at_xy_pos(
            knight->x_pos + directions[i][0], knight->y_pos + directions[i][1], knight->color);
        if (status == ENEMY || status == EMPTY)
        {
            open_routes.routes[count++] =
                (XYPosition){knight->x_pos + directions[i][0], knight->y_pos + directions[i][1],
                             status == ENEMY};
        }
    }
    open_routes.count = count;
}

void calculate_bishop_and_or_rook_route(PieceMapEntry *piece, bool is_queen)
{
    int count = 0;
    int index = 0;
    int directions[8][2];
    int bishop_directions[4][2] = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
    int rook_directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    if (piece->type == BISHOP || is_queen)
    {
        memcpy(&directions[index], bishop_directions, sizeof(bishop_directions));
        index += 4;
    }

    if (piece->type == ROOK || is_queen)
    {
        memcpy(&directions[index], rook_directions, sizeof(rook_directions));
        index += 4;
    }
    for (int i = 0; i < index; i++)
    {
        bool flag = true;
        int x = piece->x_pos + directions[i][0];
        int y = piece->y_pos + directions[i][1];
        while (flag)
        {
            SquareStatusEnum status = get_square_status_at_xy_pos(x, y, piece->color);
            if (status == EMPTY || status == ENEMY)
            {
                open_routes.routes[count++] = (XYPosition){x, y, status == ENEMY};
                if (status == ENEMY)
                {
                    flag = false;
                }
                else
                {
                    x += directions[i][0];
                    y += directions[i][1];
                }
            }
            else
            {
                flag = false;
            }
        }
    }
    open_routes.count = count;
}

void calculate_king_route(PieceMapEntry *king)
{
    int count = 0;
    int directions[8][2] = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}, {0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    for (int i = 0; i < 8; i++)
    {
        int x = king->x_pos + directions[i][0];
        int y = king->y_pos + directions[i][1];
        SquareStatusEnum status = get_square_status_at_xy_pos(x, y, king->color);
        if (status == EMPTY || status == ENEMY)
        {
            open_routes.routes[count++] = (XYPosition){x, y, status == ENEMY};
        }
    }
    open_routes.count = count;
}

void calculate_piece_route(PieceMapEntry *piece)
{
    switch (piece->type)
    {
    case PAWN:
        calculate_pawn_route(piece);
        break;
    case KNIGHT:
        calculate_knight_route(piece);
        break;
    case BISHOP:
    case ROOK:
        calculate_bishop_and_or_rook_route(piece, false);
        break;
    case QUEEN:
        calculate_bishop_and_or_rook_route(piece, true);
        break;
    case KING:
        calculate_king_route(piece);
        break;
    }
    current_piece = piece;
}

void capture_piece(XYPosition *pos)
{
    printf("CAPTURE\n");
    PieceMapEntry piece = *find_entry_at_xy_pos(pos->x, pos->y);
    piece.is_alive = false;
}

void move_piece(PieceMapEntry *piece, int x, int y)
{
    XYPosition *open_route = is_xy_in_open_routes(x, y);
    if (!(open_route == NULL) && piece)
    {
        if (open_route->is_enemy)
        {
            capture_piece(open_route);
        }
        printf("TEST\n");
        piece->x_pos = x;
        piece->y_pos = y;
        piece->move_count += 1;
        game_state.move += 1;
        game_state.color = game_state.color == 'w' ? 'b' : 'w';
        set_header_texture();
    }
}

void board_handle_click(PieceMap *map, float mouse_x, float mouse_y)
{
    if (mouse_y < 100)
        return;

    int x = (int)(mouse_x / TILE_SIZE);
    int y = (int)((mouse_y - 100) / TILE_SIZE);

    if (x < 0 || x >= 8 || y < 0 || y >= 8)
        return;

    PieceMapEntry *piece = find_entry_at_xy_pos(x, y);

    if (game_state.status == NONE)
    {
        if (piece != NULL && piece->color == game_state.color)
        {
            calculate_piece_route(piece);
            game_state.status = SELECTING;
        }
    }
    else if (game_state.status == SELECTING)
    {
        if (current_piece->color == game_state.color)
        {
            move_piece(current_piece, x, y);
        }
        game_state.status = NONE;
    }
}
