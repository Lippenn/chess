// #include "../include/draw.h"
// #include "../include/textures.h"
#include "../raylib/include/raylib.h"

void set_textures()
{
    // header_texture = LoadRenderTexture(HEADER_WIDTH, HEADER_HEIGHT);
    // board_texture = LoadRenderTexture(BOARD_WIDTH, BOARD_HEIGHT);
    // footer_texture = LoadRenderTexture(FOOTER_WIDTH, FOOTER_HEIGHT);

    // set_header_texture();
    // set_board_texture();
    // set_footer_texture();
    // load_textures();
}

void initalize() {}

void draw() {}

int main()
{
    InitWindow(600, 800, "Chess Game");
    SetTargetFPS(24);

    set_textures();
    initalize();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        draw();
        EndDrawing();
    }
}