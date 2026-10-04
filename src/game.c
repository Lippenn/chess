#include "./game.h"
#include "./moves.h"
#include <stdbool.h>

void init_game_state(GameState *game_state)
{
    Board board;
    load_fen(game_state, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

bool move_piece(GameState *game_state, Move player_move, Move move[MAX_MOVES], int *counter)
{
    for (int i = 0; i < *counter; i++)
    {
        if (moves_equal(move[i], player_move))
        {
            make_move(game_state, player_move);
            return true;
        };
    }
    return false;
}