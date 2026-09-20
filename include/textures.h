#ifndef TEXTURES_H
#define TEXTURES_H

#include "../raylib/include/raylib.h"

extern Texture2D white_pawn;
extern Texture2D white_knight;
extern Texture2D white_bishop;
extern Texture2D white_rook;
extern Texture2D white_queen;
extern Texture2D white_king;

extern Texture2D black_pawn;
extern Texture2D black_knight;
extern Texture2D black_bishop;
extern Texture2D black_rook;
extern Texture2D black_queen;
extern Texture2D black_king;

void load_textures(void);
void unload_textures(void);

#endif