#include "./defs.h"
#include "./fen.h"

extern GameState game_state;

void init_game_state(GameState *game_state);
void handle_board_click(GameState *game_state, int x, int y);
void promote_pawn_to_piece(GameState *game_state, PieceType type, PieceColor color, int x, int y);
void draw(GameState *game_state);
void reset_game(GameState *game_state);
void detect_game_over(GameState *game_state);