#include "../include/draw.h"
#include "../raylib/include/raylib.h"
#include "./textures.h"
#include "game.h"
#include <math.h>
#include <stdio.h>

RenderTexture2D board_texture;
RenderTexture2D header_texture;
RenderTexture2D footer_texture;

Rectangle board = {0, 100, BOARD_WIDTH, BOARD_HEIGHT};

extern PieceMap map;
extern GameState game_state;
extern OpenRoutes open_routes;
extern PieceMapEntry *current_piece;

void set_header_texture()
{
    BeginTextureMode(header_texture);
    ClearBackground((Color){35, 35, 35, 255});
    DrawRectangle(0, HEADER_HEIGHT - 3, 600, 3, (Color){207, 135, 65, 255});

    DrawText("CHESS", 25, 18, 32, RAYWHITE);
    DrawText("Classic Chess", 25, 55, 20, (Color){160, 160, 160, 255});

    char turn_text[32];
    snprintf(turn_text, sizeof(turn_text), "TURN %d", (game_state.move + 1) / 2);

    DrawText(turn_text, 320, 24, 18, (Color){180, 180, 180, 255});
    const char *turn_text_color = game_state.color == 'w' ? "WHITE TO MOVE" : "BLACK TO MOVE";
    DrawText(turn_text_color, 320, 50, 20, RAYWHITE);

    EndTextureMode();
}

void set_footer_texture()
{
    BeginTextureMode(footer_texture);
    ClearBackground((Color){35, 35, 35, 255});
    DrawRectangle(0, 0, FOOTER_WIDTH, 3, (Color){207, 135, 65, 255});
    EndTextureMode();
}

void set_board_texture()
{
    BeginTextureMode(board_texture);

    ClearBackground(RAYWHITE);

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                          (x + y) % 2 == 0 ? LIGHTGRAY : DARKGRAY);
        }
    }

    EndTextureMode();
}

void draw_screen_image(Texture2D texture, int x, int y)
{
    float scale = 65.0f / texture.width;
    DrawTextureEx(texture, (Vector2){x, y}, 0.0f, scale, WHITE);
}

void draw_promotion_overlay()
{
    int offset = current_piece->color == 'w' ? 0 : 4;
    Rectangle panel = {125, 275, 350, 150};
    DrawRectangle(0, 0, 600, HEADER_HEIGHT + 600, Fade(BLACK, 0.6f));
    DrawRectangleRec(panel, RAYWHITE);
    DrawText("PROMOTE", 225, 290, 30, BLACK);
    for (int i = offset; i < 4 + offset; i++)
    {
        draw_screen_image(promotion_pieces[i].texture, 155 + (i - offset) * TILE_SIZE, 332);
    }
}

Texture2D get_piece_texture(PieceTypeEnum type, char color)
{
    if (color == 'w')
    {
        switch (type)
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
        }
    }
    else
    {
        switch (type)
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
        }
    }

    return (Texture2D){0};
}

void draw_piece_selection(int x, int y)
{
    Color light_yellow = {255, 250, 202, 196};
    DrawRectangle(x * TILE_SIZE + board.x, y * TILE_SIZE + board.y, TILE_SIZE, TILE_SIZE,
                  light_yellow);
}

void draw_king_danger(int x, int y)
{
    Color light_yellow = {255, 250, 202, 196};
    DrawRectangle(x * TILE_SIZE + board.x, y * TILE_SIZE + board.y, TILE_SIZE, TILE_SIZE, RED);
}

void draw_piece_route(int x, int y)
{
    Color light_red = {255, 0, 0, 255};
    DrawCircle((x + 0.5) * TILE_SIZE + board.x, (y + 0.5) * TILE_SIZE + board.y,
               floor(TILE_SIZE) / 8, light_red);
}

void draw_image(Texture2D texture, int x, int y)
{
    Texture2D piece = texture;

    float scaleX = 65.0f / piece.width;
    float scaleY = 65.0f / piece.height;
    DrawTextureEx(piece, (Vector2){x * 75 + 5 + board.x, y * 75 + 5 + board.y}, 0.0f, scaleX,
                  WHITE);
}

void draw_pieces()
{
    for (int i = 0; i < MAX_PIECES; i++)
    {
        PieceMapEntry *entry = &map.entries[i];
        if (entry->is_alive)
        {
            draw_image(entry->texture, entry->x_pos, entry->y_pos);
        }
    }
}

void draw_open_routes()
{
    for (int i = 0; i < open_routes.count; i++)
    {
        draw_piece_route(open_routes.routes[i].x, open_routes.routes[i].y);
    }
}

void draw_selection()
{
    if (current_piece && current_piece->is_alive && game_state.status == SELECTING)
    {
        draw_piece_selection(current_piece->x_pos, current_piece->y_pos);
    }
}

void draw_king_attacked()
{
    PieceMapEntry *white_king = &map.entries[28];
    PieceMapEntry *black_king = &map.entries[4];

    if (white_king != NULL &&
        calculate_square_attacked((XYPosition){white_king->x_pos, white_king->y_pos},
                                  white_king->color))
    {
        draw_king_danger(white_king->x_pos, white_king->y_pos);
    }
    if (black_king != NULL &&
        calculate_square_attacked((XYPosition){black_king->x_pos, black_king->y_pos},
                                  black_king->color))
    {
        draw_king_danger(black_king->x_pos, black_king->y_pos);
    }
}