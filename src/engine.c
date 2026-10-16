#include "engine.h"
#include "fen.h"
#include "moves.h"

double calculate_piece_score(PieceType type)
{
    switch (type)
    {
    case PAWN:
        return 1.0;
    case KNIGHT:
        return 3.0;
    case BISHOP:
        return 3.1;
    case ROOK:
        return 5.0;
    case QUEEN:
        return 9.0;
    case KING:
        return 20.0;
    case EMPTY:
        return 0.0;
    }
}

// double calculate_material

double calculate_position_score(GameState game_state)
{
    int score = 0;
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            if (game_state.board.squares[y][x].type != EMPTY)
            {
                if (game_state.board.squares[y][x].color == PIECE_WHITE)
                {
                    score += calculate_piece_score(game_state.board.squares[y][x].type);
                }
                else
                {
                    score -= calculate_piece_score(game_state.board.squares[y][x].type);
                }
            }
        }
    }
    return score;
}

double min_max(int depth, GameState *game_state, bool is_maximising)
{
    if (depth == 0)
    {
        return calculate_position_score(*game_state);
    }
    if (is_maximising)
    {
        int max_score = -INF;
        Move moves[MAX_MOVES];
        int counter = generate_moves(game_state, &moves);
        for (int i = 0; i < counter; i++)
        {
            char fen[1024];
            generate_fen(game_state, fen);
            make_move_wo_next_move_ai(game_state, moves[i]);
            double score = min_max(depth - 1, game_state, false);
            if (score > max_score)
            {
                max_score = score;
            }
            load_fen(game_state, fen);
        }
        return max_score;
    }
    else
    {
        int min_score = INF;
        Move moves[MAX_MOVES];
        int counter = generate_moves(game_state, &moves);
        for (int i = 0; i < counter; i++)
        {
            char fen[1024];
            generate_fen(game_state, fen);
            make_move_wo_next_move_ai(game_state, moves[i]);
            double score = min_max(depth - 1, game_state, true);
            if (score < min_score)
            {
                min_score = score;
            }
            load_fen(game_state, fen);
        }
        return min_score;
    }
}

Move evaluate_position(GameState *game_state)
{
    int depth = 4;
    Move best_move;
    int best_score = 0;
    if (game_state->turn == PIECE_WHITE)
    {
        best_score = -INF;
        Move moves[MAX_MOVES];
        int counter = generate_moves(game_state, &moves);
        for (int i = 0; i < counter; i++)
        {
            char fen[1024];
            generate_fen(game_state, fen);
            make_move_wo_next_move_ai(game_state, moves[i]);
            int score = min_max(depth - 1, game_state, true);
            load_fen(game_state, fen);

            if ((score > best_score) || i == 0)
            {
                best_score = score;
                best_move = moves[i];
            }
        }
    }
    else
    {
        best_score = INF;
        Move moves[MAX_MOVES];
        int counter = generate_moves(game_state, &moves);
        for (int i = 0; i < counter; i++)
        {
            char fen[1024];
            generate_fen(game_state, fen);
            make_move_wo_next_move_ai(game_state, moves[i]);
            int score = min_max(depth - 1, game_state, false);
            load_fen(game_state, fen);
            // printf("SCORE : %i,%i, MOVE X : %i - %i, Y : %i - %i\n", score, best_score,
            //        moves[i].from_x, moves[i].to_x, moves[i].from_y, moves[i].to_y);
            // printf("----------------------------------------------------------------------!\n");
            if ((score < best_score) || i == 0)
            {
                // printf("BEST\n");
                best_score = score;
                best_move = moves[i];
            }
        }
    }
    // printf("BEST MOVE X : %i - %i, Y : %i - %i\n", best_move.from_x, best_move.to_x,
    //        best_move.from_y, best_move.to_y);
    // printf("BEST SCORE : %i\n", best_score);
    // printf("----------------------------------------------------------------------!\n");
    return best_move;
}