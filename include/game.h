#ifndef GAME_H
#define GAME_H

#include "./defs.h"
#include <stdbool.h>

extern PieceMap map;
extern OpenRoutes open_routes;
extern GameState game_state;
extern PieceMapEntry *current_piece;
extern MoveHistory move_history;

void init_map(void);
void init_promotion_pieces(void);
void handle_click(float mouse_x, float mouse_y);
bool is_king_in_check(char color);
bool calculate_square_attacked(XYPosition pos, char color);
void calculate_piece_route(PieceMapEntry *piece);
void draw_game_over_overlay();
void reset_game(void);
void flip_board(void);
void undo_move(void);
void move_piece(PieceMapEntry *piece, int x, int y);
PieceMapEntry *get_entry_at_xy_pos(int x, int y);

#endif