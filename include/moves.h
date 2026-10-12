#include "./defs.h"

void generate_pseudo_moves_by_piece(GameState *game_state_moves, Piece piece,
                                    Move (*pseudo)[MAX_MOVES], int *counter, int x, int y);
void generate_legal_moves_by_piece(GameState *game_state_moves, Piece piece,
                                   Move (*pseudo)[MAX_MOVES], int *counter, int x, int y);
void make_move(GameState *game_state, Move move);
void make_move_wo_next_move_ai(GameState *game_state, Move move);
bool is_king_in_check(GameState game_state_moves, PieceColor color);
bool moves_equal(Move a, Move b);
void next_move(GameState *game_state);
int generate_moves(GameState *game_state_moves, Move (*pseudo)[MAX_MOVES]);