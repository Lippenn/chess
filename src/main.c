#include "../include/draw.h"
#include "../include/game.h"
#include "../include/textures.h"
#include "../raylib/include/raylib.h"

void set_textures()
{
    header_texture = LoadRenderTexture(HEADER_WIDTH, HEADER_HEIGHT);
    board_texture = LoadRenderTexture(BOARD_WIDTH, BOARD_HEIGHT);
    footer_texture = LoadRenderTexture(FOOTER_WIDTH, FOOTER_HEIGHT);

    set_header_texture();
    set_board_texture();
    set_footer_texture();
    load_textures();
}

void initalize()
{
    init_map();
    init_promotion_pieces();
}

void draw()
{

    ClearBackground(RAYWHITE);
    DrawTextureRec(header_texture.texture, (Rectangle){0, 0, HEADER_WIDTH, -HEADER_HEIGHT},
                   (Vector2){0, 0}, WHITE);
    DrawTextureRec(board_texture.texture, (Rectangle){0, 0, BOARD_WIDTH, -BOARD_HEIGHT},
                   (Vector2){0, HEADER_HEIGHT}, WHITE);
    DrawTextureRec(footer_texture.texture, (Rectangle){0, 0, HEADER_WIDTH, -HEADER_HEIGHT},
                   (Vector2){0, HEADER_HEIGHT + BOARD_HEIGHT}, WHITE);
    draw_selection();
    draw_king_attacked();
    draw_pieces();
    if (game_state.status == SELECTING)
    {
        draw_open_routes();
    }
    if (game_state.game_over)
    {
        draw_game_over_overlay();
    }
    else if (game_state.promotion_active)
    {
        draw_promotion_overlay();
    }
}

int main()
{
    InitWindow(600, 800, "Chess Game");
    SetTargetFPS(24);

    set_textures();
    initalize();

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse = GetMousePosition();
            handle_click(mouse.x, mouse.y);
        }

        BeginDrawing();
        draw();
        EndDrawing();
    }
}