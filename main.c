#include "helpers/piece_map.h"
#include "raylib/include/raylib.h"
#include <math.h>
#include <string.h>

#define TILE_SIZE 75

typedef enum
{
    IMG_BLACK_PAWN,
    IMG_COUNT
} ImageID;

Texture2D images[IMG_COUNT];

PieceMapEntry *selected_piece = NULL;

PieceMap map;

bool board_dirty = true;

void draw_board()
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Color tileColor = (x + y) % 2 == 0 ? LIGHTGRAY : DARKGRAY;
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);
        }
    }
}

void load_piece_textures()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        PieceMapEntry *entry = &map.entries[i];

        entry->texture = LoadTexture(entry->path_name);
    }
}

void unload_piece_textures()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        UnloadTexture(map.entries[i].texture);
    }
}

void draw_image(Texture2D texture, int x, int y)
{
    Texture2D piece = texture;

    float scaleX = 65.0f / piece.width;
    float scaleY = 65.0f / piece.height;
    DrawTextureEx(piece, (Vector2){x * 75 + 5, y * 75 + 5}, 0.0f, scaleX, WHITE);
}

int draw_pieces()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        PieceMapEntry *entry = &map.entries[i];
        draw_image(entry->texture, entry->x_pos, entry->y_pos);
    }
    return 0;
}

void draw_piece_selection(int x, int y)
{
    Color light_yellow = {255, 250, 202, 196};
    DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, light_yellow);
}

void draw_piece_route(int x, int y)
{
    Color light_red = {255, 0, 0, 255};

    DrawCircle((x + 0.5) * TILE_SIZE, (y - 0.5) * TILE_SIZE, floor(TILE_SIZE) / 7, light_red);
}

void mouse_interactions()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = GetMousePosition();
        int x_pos = floor(mouse.x / TILE_SIZE);
        int y_pos = floor(mouse.y / TILE_SIZE);
        selected_piece = find_entry_at_xy_pos(&map, x_pos, y_pos);
    }
}

void board_details()
{
    if (!!selected_piece)
    {
        // printf("%i,%i,%s\n", selected_piece->x_pos, selected_piece->y_pos, selected_piece->key);
        int x_pos = selected_piece->x_pos;
        int y_pos = selected_piece->y_pos;
        draw_piece_selection(x_pos, y_pos);
        draw_piece_route(x_pos, y_pos);
    }
}

void draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw_board();
    board_details();
    draw_pieces();
    mouse_interactions();
    EndDrawing();
}

int main()
{
    init_map(&map);

    InitWindow(600, 600, "My Game");
    SetTargetFPS(30);

    load_piece_textures();

    while (!WindowShouldClose())
    {
        draw();
    }
    void unload_piece_textures();

    map_free(&map);

    CloseWindow();
    return 0;
}
