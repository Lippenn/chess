#include "game.h"
#include "draw.h"
#include "moves.h"
#include "textures.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void init_game_state(GameState *game_state)
{
    Board board;
    load_fen(game_state, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // load_fen(game_state, "4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 1");
    game_state->board_status = NONE;
}

void reset_game(GameState *game_state)
{
    memset(game_state, 0, sizeof(*game_state));
    init_game_state(game_state);
    printf("RESET - %i\n", game_state->is_checkmate || game_state->is_stalemate);
}

bool move_piece(GameState *game_state, Piece piece, int x, int y)
{
    SelectedPiece selected_piece = game_state->selected_piece;
    Move player_move = {
        .from_x = selected_piece.x, .from_y = selected_piece.y, .to_x = x, .to_y = y};
    Move moves[MAX_MOVES];
    int counter = 0;
    generate_legal_moves_by_piece(game_state,
                                  game_state->board.squares[selected_piece.y][selected_piece.x],
                                  &moves, &counter, selected_piece.x, selected_piece.y);
    for (int i = 0; i < counter; i++)
    {
        if (moves_equal(moves[i], player_move))
        {
            make_move(game_state, moves[i]);
            return true;
        };
    }
    return false;
}

void select_piece(GameState *game_state, Piece piece, int x, int y)
{
    if (piece.type != EMPTY && game_state->turn == piece.color)
    {
        game_state->selected_piece = (SelectedPiece){piece.type, piece.color, x, y};
    }
}

Piece return_piece(GameState *game_state, int x, int y) { return game_state->board.squares[y][x]; }

void handle_board_click(GameState *game_state, int x, int y)
{

    Piece piece = game_state->board.squares[y][x];
    if (game_state->board_status == NONE && piece.type != EMPTY && piece.color == game_state->turn)
    {
        select_piece(game_state, piece, x, y);
        game_state->board_status = SELECTING;
    }
    else if (game_state->board_status == SELECTING)
    {
        Piece piece = return_piece(game_state, x, y);
        SelectedPiece selected_piece = game_state->selected_piece;

        if (selected_piece.x == x && selected_piece.y == y)
        {
            game_state->board_status = NONE;
        }
        if (piece.type != EMPTY && piece.color == game_state->turn)
        {
            select_piece(game_state, piece, x, y);
        }
        else
        {
            bool is_move = move_piece(game_state, piece, x, y);
            if (is_move)
                game_state->board_status = NONE;
        }
    }
}

SelectedPiece get_king_by_turn_color(GameState *game_state)
{
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (game_state->board.squares[y][x].color == game_state->turn &&
                game_state->board.squares[y][x].type == KING)
            {
                return (SelectedPiece){KING, game_state->turn, x, y};
            }
        }
    }
    return (SelectedPiece){KING, game_state->turn, -1, -1};
}

bool detect_game_over(GameState *game_state)
{
    if (game_state->is_checkmate || game_state->is_stalemate)
    {
        return true;
    }
    SelectedPiece king = get_king_by_turn_color(game_state);
    Move moves[MAX_MOVES];
    int counter = 0;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (game_state->board.squares[y][x].type != EMPTY &&
                game_state->board.squares[y][x].color == game_state->turn)
            {
                generate_legal_moves_by_piece(game_state, game_state->board.squares[y][x], &moves,
                                              &counter, king.x, king.y);
            }
            if (counter > 0)
            {
                return false;
            }
        }
    }
    if (!is_king_in_check(*game_state, game_state->turn))
    {
        game_state->is_stalemate = true;
    }
    else
    {
        game_state->is_checkmate = true;
    }
    game_state->winner = game_state->turn == PIECE_WHITE ? PIECE_BLACK : PIECE_WHITE;
    return true;
}

void draw(GameState *game_state)
{
    draw_textures();
    draw_king_attacked(game_state);
    bool is_game_over = detect_game_over(game_state);
    if (game_state->board_status == SELECTING)
    {
        SelectedPiece piece = game_state->selected_piece;
        if (piece.type != EMPTY)
            draw_piece_selection(piece.x, piece.y);
    }
    draw_pieces(game_state);
    if (game_state->board_status == SELECTING)
    {
        SelectedPiece piece = game_state->selected_piece;
        if (piece.type != EMPTY)
        {
            Move moves[MAX_MOVES];
            int counter = 0;
            generate_legal_moves_by_piece(game_state, game_state->board.squares[piece.y][piece.x],
                                          &moves, &counter, piece.x, piece.y);
            draw_piece_route(moves, counter);
        }
    }
    if (game_state->promotion_overlay_open)
    {
        draw_promotion_overlay(game_state);
    }
    if (game_state->is_checkmate || game_state->is_stalemate)
    {
        draw_game_over_overlay(game_state);
    }
}

void promote_pawn_to_piece(GameState *game_state, PieceType type, PieceColor color, int x, int y)
{
    Piece *piece = &game_state->board.squares[y][x];
    piece->type = type;
    piece->color = color;
}