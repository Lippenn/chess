#include "../raylib/include/raylib.h"
#include "board.h"
#include "game.h"
#include "textures.h"

GameState game_state;

void initalize()
{
    init_game_state(&game_state);
    set_textures();
}

int main()
{
    InitWindow(600, 800, "Chess Game");
    SetTargetFPS(24);

    set_textures();
    initalize();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        draw(&game_state);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            int x, y;

            if (get_board_square(GetMousePosition(), &x, &y))
            {
                handle_board_click(&game_state, x, y);
            }
        }
        EndDrawing();
    }
}