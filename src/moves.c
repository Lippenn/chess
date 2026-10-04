#include "moves.h"

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

bool targeted_by_pawn(GameState *game_state_moves, int king_x, int king_y,
                      PieceColor current_piece_color)
{
    int direction = current_piece_color == WHITE ? 1 : -1;
    for (int dx = -1; dx <= 1; dx += 2)
    {
        int pawn_x = king_x + dx;
        int pawn_y = king_y + direction;
        Piece pawn = game_state_moves->board.squares[pawn_y][pawn_x];
        if (pawn.color != current_piece_color && pawn.type == PAWN)
        {
            return true;
        }
    }
    return false;
}

bool targeted_by_bishop(GameState *game_state_moves, int king_x, int king_y,
                        PieceColor current_piece_color)
{
    for (int i = 0; i < 4; i++)
    {
        int target_x = king_x;
        int target_y = king_y;

        while (target_x >= 0 && target_x <= 7 && target_y >= 0 && target_y <= 7)
        {
            target_x += bishop_directions[i][0];
            target_y += bishop_directions[i][1];
            if (target_x < 0 || target_x > 7 || target_y < 0 || target_y > 7)
                break;

            Piece target = game_state_moves->board.squares[target_y][target_x];
            if (target.type == EMPTY)
                continue;
            if (target.color != current_piece_color &&
                (target.type == BISHOP || target.type == QUEEN))
            {
                return true;
            }
            break;
        }
    }
    return false;
}

bool targeted_by_rook(GameState *game_state_moves, int king_x, int king_y,
                      PieceColor current_piece_color)
{
    for (int i = 0; i < 4; i++)
    {
        int target_x = king_x;
        int target_y = king_y;

        while (target_x >= 0 && target_x <= 7 && target_y >= 0 && target_y <= 7)
        {
            target_x += rook_directions[i][0];
            target_y += rook_directions[i][1];
            if (target_x < 0 || target_x > 7 || target_y < 0 || target_y > 7)
                break;

            Piece target = game_state_moves->board.squares[target_y][target_x];
            if (target.type == EMPTY)
                continue;
            if (target.color != current_piece_color &&
                (target.type == ROOK || target.type == QUEEN))
            {
                return true;
            }
            break;
        }
    }
    return false;
}

bool targeted_by_knight(GameState *game_state_moves, int king_x, int king_y,
                        PieceColor current_piece_color)
{
    for (int i = 0; i < 8; i++)
    {
        int target_x = king_x + knight_directions[i][0];
        int target_y = king_y + knight_directions[i][1];

        if ((target_x > 7 || target_x < 0) && (target_y > 7 || target_y < 0))
            continue;

        Piece target = game_state_moves->board.squares[target_y][target_x];
        if (target.color != current_piece_color && target.type == KNIGHT)
        {
            return true;
        }
    }
    return false;
}

bool targeted_by_king(GameState *game_state_moves, int king_x, int king_y,
                      PieceColor current_piece_color)
{
    for (int i = 0; i < 8; i++)
    {
        int target_x = king_x + king_directions[i][0];
        int target_y = king_y + king_directions[i][1];

        if (target_x < 0 || target_x > 7 || target_y < 0 || target_y > 7)
            continue;

        Piece target = game_state_moves->board.squares[target_y][target_x];

        if (target.color != current_piece_color && target.type == KING)
        {
            return true;
        }
    }
    return false;
}

bool is_king_in_check(GameState game_state_moves, PieceColor color)
{
    int king_x = -1;
    int king_y = -1;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (game_state_moves.board.squares[y][x].color == color &&
                game_state_moves.board.squares[y][x].type == KING)
            {
                king_x = x;
                king_y = y;
                break;
            }
        }
        if (king_x != -1)
            break;
    }
    if (king_x == -1 || king_y == -1)
        return false;
    if (targeted_by_pawn(&game_state_moves, king_x, king_y, color))
        return true;
    if (targeted_by_knight(&game_state_moves, king_x, king_y, color))
        return true;
    if (targeted_by_bishop(&game_state_moves, king_x, king_y, color))
        return true;
    if (targeted_by_rook(&game_state_moves, king_x, king_y, color))
        return true;
    if (targeted_by_king(&game_state_moves, king_x, king_y, color))
        return true;
    return false;
}

void add_move(Move new_move, Move (*pseudo)[MAX_MOVES], int *counter)
{
    (*pseudo)[(*counter)++] = new_move;
}

bool can_pawn_move_two(const Board *board, int x, int y, PieceColor color, int direction)
{
    int start_rank = color == WHITE ? 6 : 1;

    if (y != start_rank)
        return false;
    if (board->squares[y + (direction * 2)][x].type != EMPTY)
        return false;
    return true;
}

void generate_pseudo_pawn_moves(GameState *game_state_moves, Piece piece, Move (*pseudo)[MAX_MOVES],
                                int *counter, int x, int y)
{
    int direction = game_state_moves->turn == WHITE ? -1 : 1;

    if (game_state_moves->board.squares[y + direction][x].type == EMPTY)
    {
        Move new_move = {.from_x = x, .from_y = y, .to_x = x, .to_y = y + direction};
        add_move((Move){(y + direction == 7 && piece.color == BLACK) ||
                                (y + direction == 0 && piece.color == WHITE)
                            ? MOVE_PROMOTION
                            : MOVE_NORMAL,
                        .from_x = x, .from_y = y, .to_x = x, .to_y = y + direction},
                 pseudo, counter);
        if (can_pawn_move_two(&game_state_moves->board, x, y, game_state_moves->turn, direction))
        {
            add_move((Move){(y + (direction * 2) == 7 && piece.color == BLACK) ||
                                    (y + (direction * 2) == 0 && piece.color == WHITE)
                                ? MOVE_PROMOTION
                                : MOVE_NORMAL,
                            .from_x = x, .from_y = y, .to_x = x, .to_y = y + (direction * 2)},
                     pseudo, counter);
        }
    }
    for (int dx = -1; dx <= 1; dx += 2)
    {
        int target_x = x + dx;
        int target_y = y + direction;

        if (target_x < 0 || target_x > 7)
            return;
        Piece target = game_state_moves->board.squares[target_y][target_x];

        if (target.type != EMPTY && target.color != game_state_moves->turn)
        {
            add_move((Move){(target_y == 7 && piece.color == BLACK) ||
                                    (target_y == 0 && piece.color == WHITE)
                                ? MOVE_PROMOTION
                                : MOVE_NORMAL,
                            .from_x = x, .from_y = y, .to_x = target_x, .to_y = target_y},
                     pseudo, counter);
        }
        if (game_state_moves->en_passant_x == target_x &&
            game_state_moves->en_passant_y == target_y)
        {
            add_move((Move){.type = MOVE_EN_PASSANT,
                            .from_x = x,
                            .from_y = y,
                            .to_x = target_x,
                            .to_y = target_y},
                     pseudo, counter);
        }
    }
}

void generate_pseudo_knight_moves(GameState *game_state_moves, Piece piece,
                                  Move (*pseudo)[MAX_MOVES], int *counter, int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        int target_x = x + knight_directions[i][0];
        int target_y = y + knight_directions[i][1];

        if ((target_x > 7 || target_x < 0) && (target_y > 7 || target_y < 0))
            continue;

        Piece target = game_state_moves->board.squares[target_y][target_x];
        if (target.type == EMPTY || target.color != game_state_moves->turn)
        {
            add_move((Move){.type = MOVE_NORMAL,
                            .from_x = x,
                            .from_y = y,
                            .to_x = target_x,
                            .to_y = target_y},
                     pseudo, counter);
        }
    }
}

void generate_pseudo_bishop_moves(GameState *game_state_moves, Piece piece,
                                  Move (*pseudo)[MAX_MOVES], int *counter, int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        int target_x = x;
        int target_y = y;

        while (target_x >= 0 && target_x <= 7 && target_y >= 0 && target_y <= 7)
        {
            target_x += bishop_directions[i][0];
            target_y += bishop_directions[i][1];
            if (target_x < 0 || target_x > 7 || target_y < 0 || target_y > 7)
                break;

            Piece target = game_state_moves->board.squares[target_y][target_x];
            if (target.type == EMPTY)
            {
                add_move((Move){.type = MOVE_NORMAL,
                                .from_x = x,
                                .from_y = y,
                                .to_x = target_x,
                                .to_y = target_y},
                         pseudo, counter);

                continue;
            }

            if (target.color != game_state_moves->turn)
            {
                add_move((Move){.type = MOVE_NORMAL,
                                .from_x = x,
                                .from_y = y,
                                .to_x = target_x,
                                .to_y = target_y},
                         pseudo, counter);
            }

            break;
        }
    }
}

void generate_pseudo_rook_moves(GameState *game_state_moves, Piece piece, Move (*pseudo)[MAX_MOVES],
                                int *counter, int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        int target_x = x;
        int target_y = y;

        while (target_x >= 0 && target_x <= 7 && target_y >= 0 && target_y <= 7)
        {
            target_x += rook_directions[i][0];
            target_y += rook_directions[i][1];
            if (target_x < 0 || target_x > 7 || target_y < 0 || target_y > 7)
                break;

            Piece target = game_state_moves->board.squares[target_y][target_x];
            if (target.type == EMPTY)
            {
                add_move((Move){.type = MOVE_NORMAL,
                                .from_x = x,
                                .from_y = y,
                                .to_x = target_x,
                                .to_y = target_y},
                         pseudo, counter);

                continue;
            }

            if (target.color != game_state_moves->turn)
            {
                add_move((Move){.type = MOVE_NORMAL,
                                .from_x = x,
                                .from_y = y,
                                .to_x = target_x,
                                .to_y = target_y},
                         pseudo, counter);
            }

            break;
        }
    }
}

void generate_pseudo_king_moves(GameState *game_state_moves, Piece piece, Move (*pseudo)[MAX_MOVES],
                                int *counter, int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        int target_x = x + king_directions[i][0];
        int target_y = y + king_directions[i][1];

        if (target_x < 0 || target_x > 7 || target_y < 0 || target_y > 7)
            continue;

        Piece target = game_state_moves->board.squares[target_y][target_x];

        if (target.type == EMPTY || target.color != game_state_moves->turn)
        {
            add_move((Move){.type = MOVE_NORMAL,
                            .from_x = x,
                            .from_y = y,
                            .to_x = target_x,
                            .to_y = target_y},
                     pseudo, counter);
        }
    }

    if (game_state_moves->turn == WHITE)
    {
        if (game_state_moves->white_can_castle_kingside)
        {
            if (game_state_moves->board.squares[7][5].type == EMPTY &&
                game_state_moves->board.squares[7][6].type == EMPTY)
            {
                add_move(
                    (Move){.type = MOVE_CASTLE, .from_x = 4, .from_y = 7, .to_x = 6, .to_y = 7},
                    pseudo, counter);
            }
        }

        if (game_state_moves->white_can_castle_queenside)
        {
            if (game_state_moves->board.squares[7][1].type == EMPTY &&
                game_state_moves->board.squares[7][2].type == EMPTY &&
                game_state_moves->board.squares[7][3].type == EMPTY)
            {
                add_move(
                    (Move){.type = MOVE_CASTLE, .from_x = 4, .from_y = 7, .to_x = 2, .to_y = 7},
                    pseudo, counter);
            }
        }
    }
    else
    {
        if (game_state_moves->black_can_castle_kingside)
        {
            if (game_state_moves->board.squares[0][5].type == EMPTY &&
                game_state_moves->board.squares[0][6].type == EMPTY)
            {
                add_move(
                    (Move){.type = MOVE_CASTLE, .from_x = 4, .from_y = 0, .to_x = 6, .to_y = 0},
                    pseudo, counter);
            }
        }

        if (game_state_moves->black_can_castle_queenside)
        {
            if (game_state_moves->board.squares[0][1].type == EMPTY &&
                game_state_moves->board.squares[0][2].type == EMPTY &&
                game_state_moves->board.squares[0][3].type == EMPTY)
            {
                add_move(
                    (Move){.type = MOVE_CASTLE, .from_x = 4, .from_y = 0, .to_x = 2, .to_y = 0},
                    pseudo, counter);
            }
        }
    }
}

void generate_pseudo_moves_by_piece(GameState *game_state_moves, Piece piece,
                                    Move (*pseudo)[MAX_MOVES], int *counter, int x, int y)
{
    switch (piece.type)
    {
    case PAWN:
        generate_pseudo_pawn_moves(game_state_moves, piece, pseudo, counter, x, y);
        break;
    case KNIGHT:
        generate_pseudo_knight_moves(game_state_moves, piece, pseudo, counter, x, y);
        break;
    case BISHOP:
        generate_pseudo_bishop_moves(game_state_moves, piece, pseudo, counter, x, y);
        break;
    case ROOK:
        generate_pseudo_rook_moves(game_state_moves, piece, pseudo, counter, x, y);
        break;
    case QUEEN:
        generate_pseudo_bishop_moves(game_state_moves, piece, pseudo, counter, x, y);
        generate_pseudo_rook_moves(game_state_moves, piece, pseudo, counter, x, y);
        break;
    case KING:
        generate_pseudo_king_moves(game_state_moves, piece, pseudo, counter, x, y);
        break;
    case EMPTY:
        break;
    }
}

bool in_check_after_move_same_color(GameState *game_state_moves, Move move)
{
    bool in_check = false;

    Piece from_piece = game_state_moves->board.squares[move.from_y][move.from_x];
    Piece to_piece = game_state_moves->board.squares[move.to_y][move.to_x];
    Piece *from_piece_pointer = &game_state_moves->board.squares[move.from_y][move.from_x];
    Piece *to_piece_pointer = &game_state_moves->board.squares[move.to_y][move.to_x];

    to_piece_pointer->color = from_piece.color;
    to_piece_pointer->type = from_piece.type;
    from_piece_pointer->type = EMPTY;

    in_check = is_king_in_check(*game_state_moves, to_piece.color);
    to_piece_pointer->color = to_piece.color;
    to_piece_pointer->type = to_piece.type;
    from_piece_pointer->type = from_piece.type;
    from_piece_pointer->color = from_piece.color;
    return in_check;
}

void generate_legal_moves_by_piece(GameState *game_state_moves, Piece piece,
                                   Move (*pseudo)[MAX_MOVES], int *counter, int x, int y)
{
    generate_pseudo_moves_by_piece(game_state_moves, piece, pseudo, counter, x, y);
    Move legal_moves[MAX_MOVES];
    int legal_move_counter = 0;
    for (int i = 0; i < *counter; i++)
    {
        if (!in_check_after_move_same_color(game_state_moves, (*pseudo)[i]))
        {
            legal_moves[legal_move_counter++] = (*pseudo)[i];
        }
    }
    for (int i = 0; i < legal_move_counter; i++)
    {
        (*pseudo)[i] = legal_moves[i];
    }

    *counter = legal_move_counter;
}

int generate_pseudo_moves(GameState *game_state_moves, Move (*pseudo)[MAX_MOVES])
{
    int counter = 0;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (game_state_moves->board.squares[y][x].color == game_state_moves->turn &&
                game_state_moves->board.squares[y][x].type != EMPTY)
            {
                generate_pseudo_moves_by_piece(game_state_moves,
                                               game_state_moves->board.squares[y][x], pseudo,
                                               &counter, x, y);
            }
        }
    }
    return counter;
}

int generate_legal_moves(GameState *game_state_moves, Move *moves[MAX_MOVES])
{
    Move pseudo_moves[MAX_MOVES];
    int count = generate_pseudo_moves(game_state_moves, &pseudo_moves);
    return 0;
}

void next_move(GameState *game_state)
{
    if (game_state->turn == BLACK)
    {
        game_state->fullmove_number += 1;
    }
    game_state->turn = game_state->turn == WHITE ? BLACK : WHITE;
}

void make_move_normal(Piece *from_piece_pointer, Piece *to_piece_pointer)
{
    to_piece_pointer->color = from_piece_pointer->color;
    to_piece_pointer->type = from_piece_pointer->type;
    from_piece_pointer->type = EMPTY;
}

void make_move_promotion(Piece *from_piece_pointer, Piece *to_piece_pointer)
{
    to_piece_pointer->color = from_piece_pointer->color;
    to_piece_pointer->type = from_piece_pointer->type;
    from_piece_pointer->type = EMPTY;
}

void make_move_en_passant(Piece *from_piece_pointer, Piece *to_piece_pointer, Move move,
                          Board *board)
{
    int direction = move.from_x - move.to_x;
    int captured_x = move.from_x + direction;
    Piece *captured_pawn = &board->squares[move.from_y][captured_x];

    to_piece_pointer->color = from_piece_pointer->color;
    to_piece_pointer->type = from_piece_pointer->type;

    captured_pawn->type = EMPTY;
    from_piece_pointer->type = EMPTY;
}

void make_move_castle(Piece *from_piece_pointer, Piece *to_piece_pointer, Move move, Board *board)
{
    int rook_x = move.to_x > move.from_x ? 7 : 0;
    int rook_to_x = move.to_x > move.from_x ? 5 : 3;

    Piece *castling_rook = &board->squares[move.from_y][rook_x];
    Piece *rook_destination = &board->squares[move.from_y][rook_to_x];

    *to_piece_pointer = *from_piece_pointer;

    *rook_destination = *castling_rook;

    from_piece_pointer->type = EMPTY;

    castling_rook->type = EMPTY;
}

void make_move(GameState *game_state, Move move)
{
    Piece *from_piece_pointer = &game_state->board.squares[move.from_y][move.from_x];
    Piece *to_piece_pointer = &game_state->board.squares[move.to_y][move.to_x];

    switch (move.type)
    {
    case MOVE_NORMAL:
        make_move_normal(from_piece_pointer, to_piece_pointer);
        break;
    case MOVE_PROMOTION:
        make_move_promotion(from_piece_pointer, to_piece_pointer);
        break;
    case MOVE_EN_PASSANT:
        make_move_en_passant(from_piece_pointer, to_piece_pointer, move, &game_state->board);
        break;
    case MOVE_CASTLE:
        make_move_castle(from_piece_pointer, to_piece_pointer, move, &game_state->board);
        break;
    }

    next_move(game_state);
}

bool moves_equal(Move a, Move b)
{
    return a.from_x == b.from_x && a.from_y == b.from_y && a.to_x == b.to_x && a.to_y == b.to_y &&
           a.type == b.type;
}