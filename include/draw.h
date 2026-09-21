#ifndef DRAW_H
#define DRAW_H

#include "defs.h"

extern RenderTexture2D board_texture;
extern RenderTexture2D header_texture;
extern Rectangle board;

void set_header_texture(void);
void set_board_texture(void);
void draw_pieces(void);
Texture2D get_piece_texture(PieceTypeEnum type, char color);
void draw_piece_selection(int x, int y);
void draw_king_danger(int x, int y);
void draw_piece_route(int x, int y);
void draw_image(Texture2D texture, int x, int y);
void draw_open_routes();

#endif