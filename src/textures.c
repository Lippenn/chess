#include "textures.h"
#include "draw.h"

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

void set_textures(void)
{
    header_texture = LoadRenderTexture(HEADER_WIDTH, HEADER_HEIGHT);
    board_texture = LoadRenderTexture(BOARD_WIDTH, BOARD_HEIGHT);
    footer_texture = LoadRenderTexture(FOOTER_WIDTH, FOOTER_HEIGHT);

    set_header_texture();
    set_board_texture();
    set_footer_texture();
    load_textures();
}

void draw_render_texture(RenderTexture2D texture, int x, int y)
{
    DrawTextureRec(texture.texture,
                   (Rectangle){0, 0, texture.texture.width, -texture.texture.height},
                   (Vector2){x, y}, WHITE);
}

void draw_textures(void)
{
    ClearBackground(RAYWHITE);

    draw_render_texture(header_texture, 0, 0);
    draw_render_texture(board_texture, 0, HEADER_HEIGHT);
    draw_render_texture(footer_texture, 0, HEADER_HEIGHT + BOARD_HEIGHT);
}

Texture2D get_piece_texture(Piece piece)
{
    if (piece.color == PIECE_WHITE)
    {
        switch (piece.type)
        {
        case PAWN:
            return white_pawn;
        case KNIGHT:
            return white_knight;
        case BISHOP:
            return white_bishop;
        case ROOK:
            return white_rook;
        case QUEEN:
            return white_queen;
        case KING:
            return white_king;
        case EMPTY:
            break;
        }
    }
    else
    {
        switch (piece.type)
        {
        case PAWN:
            return black_pawn;
        case KNIGHT:
            return black_knight;
        case BISHOP:
            return black_bishop;
        case ROOK:
            return black_rook;
        case QUEEN:
            return black_queen;
        case KING:
            return black_king;
        case EMPTY:
            break;
        }
    }
    return (Texture2D){0};
}