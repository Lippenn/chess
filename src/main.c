#include "../include/draw.h"
#include "../include/game.h"
#include "../include/textures.h"
#include "../raylib/include/raylib.h"

void set_textures()
{
    header_texture = LoadRenderTexture(HEADER_WIDTH, HEADER_HEIGHT);
    board_texture = LoadRenderTexture(BOARD_WIDTH, BOARD_HEIGHT);

    set_header_texture();
    set_board_texture();

    load_textures();
}

void initalize() { init_map(); }

void draw()
{
    ClearBackground(RAYWHITE);
    DrawTextureRec(header_texture.texture, (Rectangle){0, 0, HEADER_WIDTH, -HEADER_HEIGHT},
                   (Vector2){0, 0}, WHITE);
    DrawTextureRec(board_texture.texture, (Rectangle){0, 0, BOARD_WIDTH, -BOARD_HEIGHT},
                   (Vector2){0, 100}, WHITE);
    draw_pieces();
    if (game_state.status == SELECTING)
    {
        draw_open_routes();
    }
}

int main()
{
    InitWindow(600, 700, "My Game");
    SetTargetFPS(24);

    set_textures();
    initalize();

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse = GetMousePosition();

            board_handle_click(&map, mouse.x, mouse.y);
        }

        BeginDrawing();
        draw();
        EndDrawing();
    }
}