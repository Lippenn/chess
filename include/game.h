#ifndef GAME_H
#define GAME_H

#include "./defs.h"

extern PieceMap map;
extern OpenRoutes open_routes;
extern GameState game_state;
extern PieceMapEntry *current_piece;

void init_map(void);
void board_handle_click(PieceMap *map, float mouse_x, float mouse_y);

#endif