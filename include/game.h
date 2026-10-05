#include "./defs.h"
#include "./fen.h"

extern GameState game_state;

void init_game_state(GameState *game_state);
void handle_board_click(GameState *game_state, int x, int y);
void draw(GameState *game_state);
