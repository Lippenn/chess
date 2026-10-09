#include "game.h"
#include "draw.h"
#include "moves.h"
#include "textures.h"
#include <stdbool.h>

void init_game_state(GameState *game_state)
{
    Board board;
    load_fen(game_state, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    game_state->board_status = NONE;
}

bool move_piece(GameState *game_state, Piece piece, int x, int y)
{
    SelectedPiece selected_piece = game_state->selected_piece;
    Move player_move = {
        .from_x = selected_piece.x, .from_y = selected_piece.y, .to_x = x, .to_y = y};
    Move moves[MAX_MOVES];
    int *counter = 0;
    generate_legal_moves_by_piece(game_state, piece, &moves, counter, x, y);
    printf("COUNTER : %i\n", *counter);

    for (int i = 0; i < *counter; i++)
    {
        if (moves_equal(moves[i], player_move))
        {
            make_move(game_state, player_move);
            return true;
        };
    }
    return false;
}

void select_piece(GameState *game_state, Piece piece, int x, int y)
{
    game_state->selected_piece = (SelectedPiece){piece.type, piece.color, x, y};
}

Piece return_piece(GameState *game_state, int x, int y) { return game_state->board.squares[y][x]; }

void handle_board_click(GameState *game_state, int x, int y)
{
    Piece piece = game_state->board.squares[y][x];
    if (game_state->board_status == NONE)
    {
        select_piece(game_state, piece, x, y);
        game_state->board_status = SELECTING;
    }
    else if (game_state->board_status == SELECTING)
    {
        Piece piece = return_piece(game_state, x, y);
        if (piece.type != EMPTY && piece.color == game_state->turn)
        {
            select_piece(game_state, piece, x, y);
        }
        else
        {
            bool is_move = move_piece(game_state, piece, x, y);
        }
    }
}

void draw(GameState *game_state)
{
    draw_textures();
    if (game_state->board_status == SELECTING)
    {
        SelectedPiece piece = game_state->selected_piece;
        if (piece.type != EMPTY && piece.x && piece.y)
        {
            Move moves[MAX_MOVES];
            int counter = 0;
            draw_piece_selection(piece.x, piece.y);
            generate_legal_moves_by_piece(game_state, game_state->board.squares[piece.y][piece.x],
                                          &moves, &counter, piece.x, piece.y);
            draw_piece_route(moves, counter);
        }
    }

    draw_pieces(game_state);
}