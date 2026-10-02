
#include "game.h"

PieceMapValue values[6] = {{100, PAWN}, {310, BISHOP}, {320, KNIGHT},
                           {500, ROOK}, {900, QUEEN},  {20000, KING}};

int evaluate_board()
{
    int white_value_sum = 0;
    int black_value_sum = 0;

    for (int i = 0; i < MAX_PIECES; i++)
    {
        if (map.entries[i].color == 'w')
        {
            white_value_sum += values[map.entries[i].type].value;
        }
        else
        {
            black_value_sum += values[map.entries[i].type].value;
        }
    }

    return white_value_sum - black_value_sum;
}

void generate_moves(MoveHistory(moves)[MAX_POSITION_MOVES])
{
    for (int i = 0; i < MAX_MOVES; i++)
    {
        int from_x = map.entries[i].x_pos;
        int from_y = map.entries[i].y_pos;

        calculate_piece_route(&map.entries[i]);
        for (int j = 0; j < open_routes.count; j++)
        {
            // moves->moves[moves->count] = (Move) { moves->count, -1,from_x,from_y,moves->moves[j].
            // }
        }
    }
}

int search(int depth)
{
    if (depth == 0)
        return evaluate_board();

    MoveHistory moves;
    generate_moves(&moves);

    for (int i = 0; i < moves.count; i++)
    {
    }
}