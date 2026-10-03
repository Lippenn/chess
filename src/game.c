#include "../include/game.h"
#include "./draw.h"
#include "textures.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StarterPiece starter_pieces[MAX_PIECES] = {
    // Black
    {ROOK, 'b', 0, 0, true},
    {KNIGHT, 'b', 1, 0, true},
    {BISHOP, 'b', 2, 0, true},
    {QUEEN, 'b', 3, 0, true},
    {KING, 'b', 4, 0, true},
    {BISHOP, 'b', 5, 0, true},
    {KNIGHT, 'b', 6, 0, true},
    {ROOK, 'b', 7, 0, true},

    {PAWN, 'b', 0, 1, true},
    {PAWN, 'b', 1, 1, true},
    {PAWN, 'b', 2, 1, true},
    {PAWN, 'b', 3, 1, true},
    {PAWN, 'b', 4, 1, true},
    {PAWN, 'b', 5, 1, true},
    {PAWN, 'b', 6, 1, true},
    {PAWN, 'b', 7, 1, true},

    // White
    {PAWN, 'w', 0, 6, true},
    {PAWN, 'w', 1, 6, true},
    {PAWN, 'w', 2, 6, true},
    {PAWN, 'w', 3, 6, true},
    {PAWN, 'w', 4, 6, true},
    {PAWN, 'w', 5, 6, true},
    {PAWN, 'w', 6, 6, true},
    {PAWN, 'w', 7, 6, true},

    {ROOK, 'w', 0, 7, true},
    {KNIGHT, 'w', 1, 7, true},
    {BISHOP, 'w', 2, 7, true},
    {QUEEN, 'w', 3, 7, true},
    {KING, 'w', 4, 7, true},
    {BISHOP, 'w', 5, 7, true},
    {KNIGHT, 'w', 6, 7, true},
    {ROOK, 'w', 7, 7, true},
};

PieceValueMapEntry piece_values[6] = {{PAWN, 100}, {KNIGHT, 300}, {BISHOP, 300},
                                      {ROOK, 500}, {QUEEN, 900},  {KING, 2000}};

int pawn_directions[2] = {-1, 1};
int knight_directions[8][2] = {
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
int bishop_directions[4][2] = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
int rook_directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
int king_directions[8][2] = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}, {0, -1}, {1, 0}, {0, 1}, {-1, 0}};
int king_castle_directions[2][2] = {
    {2, 0},
    {-2, 0},
};

PieceMap map;
PromotionPiece promotion_pieces[8];
OpenRoutes open_routes = {0};
GameState game_state = {1, 'w', NONE, false, false, false, false, -1, -1, NIL};
PieceMapEntry *current_piece;
MoveHistory move_history = {-1};

void init_map()
{
    map.count = MAX_PIECES;
    for (int i = 0; i < MAX_PIECES; i++)
    {
        StarterPiece starter_piece = starter_pieces[i];
        int x = starter_piece.x;
        int y = starter_piece.y;
        if (game_state.flip)
        {
            x = 7 - x;
            y = 7 - y;
        }
        map.entries[i] = (PieceMapEntry){i,
                                         starter_piece.type,
                                         starter_piece.color,
                                         x,
                                         y,
                                         starter_piece.is_alive,
                                         get_piece_texture(starter_piece.type, starter_piece.color),
                                         0,
                                         -1};
    }
}

void reset_game()
{
    game_state = (GameState){1, 'w', NONE, false, false, false, true, -1, -1, NIL};
    init_map();
    set_header_texture();
}

void init_promotion_pieces()
{
    int count = 0;
    char color[2] = {'w', 'b'};
    PieceTypeEnum piece_types[4] = {KNIGHT, BISHOP, ROOK, QUEEN};
    for (int z = 0; z < 2; z++)
    {
        for (int i = 0; i < 4; i++)
        {
            promotion_pieces[count++] =
                (PromotionPiece){get_piece_texture(piece_types[i], color[z]), piece_types[i]};
        }
    }
}

void print_map()
{
    printf("---------------------------\n");
    for (int i = 0; i < MAX_PIECES; i++)
    {
        printf("%c\n", map.entries[i].is_alive ? 'T' : 'F');
    }
}

PieceMapEntry *get_entry_at_xy_pos(int x, int y)
{
    for (int i = 0; i < MAX_PIECES; i++)
    {
        if (map.entries[i].x_pos == x && map.entries[i].y_pos == y && map.entries[i].is_alive)
        {
            return &map.entries[i];
        }
    }
    return NULL;
}

PieceMapEntry *get_entry_by_index(int index)
{
    for (int i = 0; i < MAX_PIECES; i++)
    {
        if (map.entries[i].index == index)
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

    for (int i = 0; i < open_routes.count; i++)
    {
        if (open_routes.routes[i].x == x && open_routes.routes[i].y == y)
        {
            return &open_routes.routes[i];
        }
    }
    return NULL;
}

bool simulate_if_king_check(PieceMapEntry *piece, XYPosition pos)
{
    PieceMap temp_map = map;
    SquareStatusEnum square_type = get_square_status_at_xy_pos(pos.x, pos.y, piece->color);
    if (square_type == ENEMY)
    {
        PieceMapEntry *enemy_piece = get_entry_at_xy_pos(pos.x, pos.y);
        if (enemy_piece)
        {
            enemy_piece->is_alive = false;
        }
    }
    piece->x_pos = pos.x;
    piece->y_pos = pos.y;
    bool in_check = is_king_in_check(piece->color);
    map = temp_map;
    return in_check;
}

void add_available_route(int *count, XYPosition pos, PieceMapEntry *piece)
{
    if (!simulate_if_king_check(piece, pos))
    {
        open_routes.routes[(*count)++] = pos;
    }
}

void calculate_pawn_route(PieceMapEntry *pawn)
{
    int direction = pawn->color == 'w' ? -1 : 1;

    if (game_state.flip)
        direction *= -1;

    int x = pawn->x_pos;
    int y = pawn->y_pos;
    int count = 0;
    SquareStatusEnum status = get_square_status_at_xy_pos(x, y + direction, pawn->color);
    if (status == EMPTY)
    {
        add_available_route(&count, (XYPosition){x, y + direction, false}, pawn);
        if (pawn->move_count == 0)
        {
            status = get_square_status_at_xy_pos(x, y + direction * 2, pawn->color);
            if (status == EMPTY)
            {
                add_available_route(&count, (XYPosition){x, y + (direction * 2), false}, pawn);
            }
        }
    }

    for (int i = 0; i < 2; i++)
    {
        x = pawn->x_pos + pawn_directions[i];
        y = pawn->y_pos;
        status = get_square_status_at_xy_pos(x, y + direction, pawn->color);
        if (status == ENEMY)
        {
            add_available_route(&count, (XYPosition){x, y + direction, true}, pawn);
        }
        else if (status == EMPTY)
        {
            SquareStatusEnum status = get_square_status_at_xy_pos(x, pawn->y_pos, pawn->color);
            if (status == ENEMY)
            {
                PieceMapEntry *enemy_piece = get_entry_at_xy_pos(x, pawn->y_pos);
                if (enemy_piece && enemy_piece->type == PAWN && enemy_piece->move_count == 1 &&
                    enemy_piece->double_move_at == game_state.move - 1)
                {
                    add_available_route(&count, (XYPosition){x, pawn->y_pos + direction, true},
                                        pawn);
                }
            }
        }
    }
    open_routes.count = count;
}

void calculate_knight_route(PieceMapEntry *knight)
{
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        SquareStatusEnum status =
            get_square_status_at_xy_pos(knight->x_pos + knight_directions[i][0],
                                        knight->y_pos + knight_directions[i][1], knight->color);
        if (status == ENEMY || status == EMPTY)
        {
            add_available_route(&count,
                                (XYPosition){knight->x_pos + knight_directions[i][0],
                                             knight->y_pos + knight_directions[i][1],
                                             status == ENEMY},
                                knight);
        }
    }
    open_routes.count = count;
}

void calculate_bishop_or_queen_or_rook_route(PieceMapEntry *piece, bool is_queen)
{
    int count = 0;
    int index = 0;
    int directions[8][2];

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
                add_available_route(&count, (XYPosition){x, y, true}, piece);
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
    for (int i = 0; i < 8; i++)
    {
        int x = king->x_pos + king_directions[i][0];
        int y = king->y_pos + king_directions[i][1];
        SquareStatusEnum status = get_square_status_at_xy_pos(x, y, king->color);
        if (status == EMPTY || status == ENEMY)
        {
            add_available_route(&count, (XYPosition){x, y, true}, king);
        }
    }
    if (king->move_count == 0 &&
        !calculate_square_attacked((XYPosition){king->x_pos, king->y_pos}, king->color))
    {
        for (int i = 0; i < 2; i++)
        {
            int direction = king_castle_directions[i][0] > 0 ? 1 : -1;
            int rook_x = direction > 0 ? 7 : 0;

            PieceMapEntry *rook = get_entry_at_xy_pos(rook_x, king->y_pos);

            if (!rook || rook->type != ROOK || !rook->is_alive || rook->color != king->color ||
                rook->move_count != 0)
            {
                continue;
            }

            int middle_x = king->x_pos + direction;
            int destination_x = king->x_pos + direction * 2;

            if (get_square_status_at_xy_pos(middle_x, king->y_pos, king->color) != EMPTY)
            {
                continue;
            }

            if (get_square_status_at_xy_pos(destination_x, king->y_pos, king->color) != EMPTY)
            {
                continue;
            }
            if (direction < 0)
            {
                int rook_path_x = king->x_pos - (game_state.flip ? 2 : 3);
                if (get_square_status_at_xy_pos(rook_path_x, king->y_pos, king->color) != EMPTY)
                {
                    continue;
                }
            }
            if (calculate_square_attacked((XYPosition){middle_x, king->y_pos}, king->color))
            {
                continue;
            }
            if (calculate_square_attacked((XYPosition){destination_x, king->y_pos}, king->color))
            {
                continue;
            }
            add_available_route(&count, (XYPosition){destination_x, king->y_pos, false}, king);
        }
    }
    open_routes.count = count;
}

void calculate_piece_route(PieceMapEntry *piece)
{
    open_routes.count = 0;
    if (piece->is_alive)
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
            calculate_bishop_or_queen_or_rook_route(piece, false);
            break;
        case QUEEN:
            calculate_bishop_or_queen_or_rook_route(piece, true);
            break;
        case KING:
            calculate_king_route(piece);
            break;
        }
        current_piece = piece;
    }
}

PieceMapEntry *capture_piece(XYPosition *pos)
{
    PieceMapEntry *piece = get_entry_at_xy_pos(pos->x, pos->y);
    if (piece)
    {
        piece->is_alive = false;
        return piece;
    }
    return NULL;
}

void check_castle_king(PieceMapEntry *king, int x, int y, Move *move)
{
    int difference = abs(x - king->x_pos);
    int direction = x > king->x_pos ? -1 : 1;

    if (difference > 1)
    {
        PieceMapEntry *rook = get_entry_at_xy_pos(x > 4 ? 7 : 0, y);
        move->sub_x = rook->x_pos;
        move->sub_y = rook->y_pos;
        move->capture_index = rook->index;
        if (rook && rook->is_alive && rook->type == ROOK)
        {
            rook->x_pos = x + direction;
        }
        move->is_castle = true;
    }
}

void check_en_passant(PieceMapEntry *pawn, int x, int y, Move *move)
{
    if ((x - pawn->x_pos) != 0)
    {
        PieceMapEntry *piece = get_entry_at_xy_pos(x, pawn->y_pos);
        if (piece && piece->type == PAWN)
        {
            move->sub_x = piece->x_pos;
            move->sub_y = piece->y_pos;
            move->capture_index = piece->index;
            piece->is_alive = false;
        }
        move->is_castle = true;
    }
}

void check_double_advance(PieceMapEntry *pawn, int x, int y)
{
    int difference = abs(pawn->y_pos - y);
    if (difference > 1)
    {
        pawn->double_move_at = game_state.move;
    }
}

void check_pawn_promotion(PieceMapEntry *pawn, int x, int y, Move *move)
{
    if (y == 0 || y == 7)
    {
        game_state.promotion_active = true;
        game_state.promotion_x = x;
        game_state.promotion_y = y;
        move->is_promotion = true;
    }
}

bool is_king_in_check(char color)
{
    PieceMapEntry king = map.entries[color == 'w' ? 28 : 4];
    return calculate_square_attacked((XYPosition){king.x_pos, king.y_pos}, color);
}

bool has_available_move(char color)
{
    for (int i = 0; i < MAX_PIECES; i++)
    {
        if (map.entries[i].color == color)
        {
            calculate_piece_route(&map.entries[i]);
            if (open_routes.count > 0)
            {
                return true;
            }
        }
    }
    return false;
}

void set_game_status()
{
    char colors[2] = {'w', 'b'};
    for (int i = 0; i < 2; i++)
    {
        bool in_check = is_king_in_check(colors[i]);
        bool has_move = has_available_move(colors[i]);

        if (has_move)
        {
            continue;
        }
        if (in_check)
        {
            game_state.game_over = true;
            game_state.winner = colors[i] == 'w' ? 'b' : 'w';
        }
        else
        {
            game_state.game_over = true;
            game_state.stalemate = true;
        }
    }
}

void add_to_move_history(Move move)
{
    move_history.count += 1;
    move_history.moves[move_history.count] = move;
}

void next_move()
{
    game_state.move += 1;
    game_state.color = game_state.color == 'w' ? 'b' : 'w';
    set_header_texture();
    set_game_status();
}

void previous_move()
{
    game_state.move -= 1;
    game_state.color = game_state.color == 'w' ? 'b' : 'w';
    set_header_texture();
    set_game_status();
}

void move_piece(PieceMapEntry *piece, int x, int y)
{
    XYPosition *open_route = is_xy_in_open_routes(x, y);
    PieceMapEntry *save_piece = current_piece;
    if (!(open_route == NULL) && piece)
    {
        PieceMapEntry *captured_piece = NULL;
        Move move = {piece->index,
                     captured_piece != NULL ? captured_piece->index : -1,
                     piece->x_pos,
                     piece->y_pos,
                     x,
                     y,
                     -1,
                     -1,
                     false,
                     false};
        if (simulate_if_king_check(piece, (XYPosition){x, y}))
        {
            return;
        }
        if (piece->type == KING)
        {
            check_castle_king(piece, x, y, &move);
        }

        if (open_route->is_enemy)
        {
            captured_piece = capture_piece(open_route);
        }
        if (piece->type == PAWN)
        {
            check_pawn_promotion(piece, x, y, &move);
            check_en_passant(piece, x, y, &move);
            check_double_advance(piece, x, y);
        }
        add_to_move_history(move);
        piece->x_pos = x;
        piece->y_pos = y;
        piece->move_count += 1;
        next_move();
        current_piece = save_piece;
    }
}

void handle_board_click(int x, int y)
{
    PieceMapEntry *piece = get_entry_at_xy_pos(x, y);

    if (game_state.status == NONE)
    {
        if (piece != NULL && piece->is_alive && piece->color == game_state.color)
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

void promote_pawn(int index)
{
    PromotionPiece piece = promotion_pieces[index];
    for (int i = 0; i < MAX_PIECES; i++)
    {
        if (map.entries[i].index == current_piece->index)
        {
            map.entries[i].type = piece.type;
            map.entries[i].texture = piece.texture;
        }
    }
    return;
}

void reset_promotion_overlay()
{
    game_state.promotion_active = false;
    game_state.promotion_x = -1;
    game_state.promotion_y = -1;
}

void handle_promotion_overlay_click(float mouse_x, float mouse_y)
{
    int offset = current_piece->color == 'w' ? 0 : 4;

    for (int i = 0; i < 4; i++)
    {
        Rectangle piece_rect = {155 + i * TILE_SIZE, 332, 65, 65};

        if (CheckCollisionPointRec((Vector2){mouse_x, mouse_y}, piece_rect))
        {
            promote_pawn(offset + i);
            reset_promotion_overlay();
            return;
        }
    }
}
void handle_click(float mouse_x, float mouse_y)
{
    if (mouse_y < HEADER_HEIGHT)
    {
        return;
    }
    else if (mouse_y < HEADER_HEIGHT + BOARD_HEIGHT)
    {
        int x = (int)(mouse_x / TILE_SIZE);
        int y = (int)((mouse_y - 100) / TILE_SIZE);

        if (x < 0 || x >= 8 || y < 0 || y >= 8)
            return;

        if (game_state.promotion_active)
        {
            handle_promotion_overlay_click(mouse_x, mouse_y);
        }
        else
        {
            handle_board_click(x, y);
        }
    }
    else if (mouse_y < HEADER_HEIGHT + BOARD_HEIGHT + FOOTER_HEIGHT)
    {
        return;
    }
}

bool check_attacked_by_pawn(XYPosition pos, char color)
{
    int x = pos.x;
    int y = pos.y;
    int direction = color == 'w' ? -1 : 1;

    if (game_state.flip)
        direction *= -1;

    for (int i = 0; i < 2; i++)
    {
        SquareStatusEnum status =
            get_square_status_at_xy_pos(x + pawn_directions[i], y + direction, color);

        if (status == ENEMY)
        {
            PieceMapEntry *enemy_piece = get_entry_at_xy_pos(x + pawn_directions[i], y + direction);
            if (enemy_piece->type == PAWN && enemy_piece->is_alive)
            {
                return true;
            }
        }
    }

    return false;
}

bool check_attacked_by_bishop_and_rook(XYPosition pos, char color)
{
    for (int i = 0; i < 4; i++)
    {
        int x = pos.x + bishop_directions[i][0];
        int y = pos.y + bishop_directions[i][1];

        while (true)
        {
            SquareStatusEnum status = get_square_status_at_xy_pos(x, y, color);

            if (status == OUTSIDE || status == FRIENDLY)
            {
                break;
            }

            if (status == ENEMY)
            {
                PieceMapEntry *enemy_piece = get_entry_at_xy_pos(x, y);
                if (enemy_piece->is_alive &&
                    (enemy_piece->type == BISHOP || enemy_piece->type == QUEEN))
                {
                    return true;
                }
                break;
            }

            x += bishop_directions[i][0];
            y += bishop_directions[i][1];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        int x = pos.x + rook_directions[i][0];
        int y = pos.y + rook_directions[i][1];

        while (true)
        {
            SquareStatusEnum status = get_square_status_at_xy_pos(x, y, color);

            if (status == OUTSIDE || status == FRIENDLY)
            {
                break;
            }

            if (status == ENEMY)
            {
                PieceMapEntry *enemy_piece = get_entry_at_xy_pos(x, y);
                if (enemy_piece->is_alive &&
                    (enemy_piece->type == ROOK || enemy_piece->type == QUEEN))
                {
                    return true;
                }
                break;
            }

            x += rook_directions[i][0];
            y += rook_directions[i][1];
        }
    }

    return false;
}

bool check_attacked_by_knight(XYPosition pos, char color)
{
    for (int i = 0; i < 8; i++)
    {
        int x = pos.x + knight_directions[i][0];
        int y = pos.y + knight_directions[i][1];
        SquareStatusEnum status = get_square_status_at_xy_pos(x, y, color);
        if (status == ENEMY)
        {
            PieceMapEntry *enemy_piece = get_entry_at_xy_pos(x, y);
            if ((enemy_piece->type == KNIGHT) && enemy_piece->is_alive)
            {
                return true;
            }
        }
    }
    return false;
}

bool check_attacked_by_king(XYPosition pos, char color)
{
    for (int i = 0; i < 8; i++)
    {
        int x = pos.x + king_directions[i][0];
        int y = pos.y + king_directions[i][1];
        SquareStatusEnum status = get_square_status_at_xy_pos(x, y, color);
        if (status == ENEMY)
        {
            PieceMapEntry *enemy_piece = get_entry_at_xy_pos(x, y);
            if ((enemy_piece->type == KING) && enemy_piece->is_alive)
            {
                return true;
            }
        }
    }
    return false;
}

bool calculate_square_attacked(XYPosition pos, char color)
{
    if (check_attacked_by_pawn(pos, color))
    {
        return true;
    }
    if (check_attacked_by_bishop_and_rook(pos, color))
    {
        return true;
    }
    if (check_attacked_by_knight(pos, color))
    {
        return true;
    }
    if (check_attacked_by_king(pos, color))
    {
        return true;
    }
    return false;
}

void flip_board()
{
    game_state.flip = !game_state.flip;
    for (int i = 0; i < MAX_PIECES; i++)
    {
        PieceMapEntry *piece = &map.entries[i];
        piece->x_pos = 7 - piece->x_pos;
        piece->y_pos = 7 - piece->y_pos;
    }
}

void undo_move()
{
    if (move_history.count > 0)
    {
        Move *latest_move = &move_history.moves[move_history.count];
        PieceMapEntry *piece = get_entry_by_index(latest_move->index);
        if (latest_move->capture_index >= 0)
        {
            PieceMapEntry *captured_piece = get_entry_by_index(latest_move->capture_index);
            captured_piece->is_alive = true;
        }
        piece->x_pos = latest_move->from_x;
        piece->y_pos = latest_move->from_y;
        piece->move_count -= 1;

        if (latest_move->is_castle)
        {
            PieceMapEntry *rook = get_entry_by_index(latest_move->capture_index);
            if (latest_move->sub_x > -1 && latest_move->sub_y > -1)
            {
                rook->x_pos = latest_move->sub_x;
                rook->y_pos = latest_move->sub_y;
            }
        }
        if (latest_move->is_promotion)
        {
            piece->type = PAWN;
            piece->texture = piece->color ? white_pawn : black_pawn;
        }
        latest_move = NULL;
        move_history.count -= 1;
        previous_move();
    }
}