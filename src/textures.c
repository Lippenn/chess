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
    white_pawn = LoadTexture("../assets/white-pawn.png");
    white_knight = LoadTexture("../assets/white-knight.png");
    white_bishop = LoadTexture("../assets/white-bishop.png");
    white_rook = LoadTexture("../assets/white-rook.png");
    white_queen = LoadTexture("../assets/white-queen.png");
    white_king = LoadTexture("../assets/white-king.png");

    black_pawn = LoadTexture("../assets/black-pawn.png");
    black_knight = LoadTexture("../assets/black-knight.png");
    black_bishop = LoadTexture("../assets/black-bishop.png");
    black_rook = LoadTexture("../assets/black-rook.png");
    black_queen = LoadTexture("../assets/black-queen.png");
    black_king = LoadTexture("../assets/black-king.png");
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