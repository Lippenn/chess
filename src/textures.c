#include "textures.h"

#include "../raylib/include/raylib.h"

Texture2D white_pawn;
Texture2D white_knight;
Texture2D white_bishop;
Texture2D white_rook;
Texture2D white_queen;
Texture2D white_king;

Texture2D black_pawn;
Texture2D black_knight;
Texture2D black_bishop;
Texture2D black_rook;
Texture2D black_queen;
Texture2D black_king;

void load_textures(void)
{
    white_pawn = LoadTexture(ROOT_PATH "/assets/white-pawn.png");
    white_knight = LoadTexture(ROOT_PATH "/assets/white-knight.png");
    white_bishop = LoadTexture(ROOT_PATH "/assets/white-bishop.png");
    white_rook = LoadTexture(ROOT_PATH "/assets/white-rook.png");
    white_queen = LoadTexture(ROOT_PATH "/assets/white-queen.png");
    white_king = LoadTexture(ROOT_PATH "/assets/white-king.png");

    black_pawn = LoadTexture(ROOT_PATH "/assets/black-pawn.png");
    black_knight = LoadTexture(ROOT_PATH "/assets/black-knight.png");
    black_bishop = LoadTexture(ROOT_PATH "/assets/black-bishop.png");
    black_rook = LoadTexture(ROOT_PATH "/assets/black-rook.png");
    black_queen = LoadTexture(ROOT_PATH "/assets/black-queen.png");
    black_king = LoadTexture(ROOT_PATH "/assets/black-king.png");
}

void unload_textures(void)
{
    UnloadTexture(white_pawn);
    UnloadTexture(white_knight);
    UnloadTexture(white_bishop);
    UnloadTexture(white_rook);
    UnloadTexture(white_queen);
    UnloadTexture(white_king);
    UnloadTexture(black_pawn);
    UnloadTexture(black_knight);
    UnloadTexture(black_bishop);
    UnloadTexture(black_rook);
    UnloadTexture(black_queen);
    UnloadTexture(black_king);
}