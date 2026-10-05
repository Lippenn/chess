#include "board.h"
#include "../raylib/include/raylib.h"

bool get_board_square(Vector2 mouse, int *x, int *y)
{
    if (mouse.x < 0 || mouse.x >= BOARD_WIDTH || mouse.y < BOARD_Y ||
        mouse.y >= BOARD_Y + BOARD_HEIGHT)
    {
        return false;
    }

    *x = (int)(mouse.x / TILE_SIZE);
    *y = (int)((mouse.y - BOARD_Y) / TILE_SIZE);

    return true;
}
