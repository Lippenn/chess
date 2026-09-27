#ifndef GAME_H
#define GAME_H

#include "./defs.h"
#include <stdbool.h>

extern PieceMap map;
extern OpenRoutes open_routes;
extern GameState game_state;
extern PieceMapEntry *current_piece;

void init_map(void);
void init_promotion_pieces(void);
void handle_click(float mouse_x, float mouse_y);
bool is_king_in_check(char color);
bool calculate_square_attacked(XYPosition pos, char color);
void draw_game_over_overlay();
void reset_game(void);
void flip_board(void);

#endif