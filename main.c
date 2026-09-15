#include "helpers/piece_map.h"
#include "raylib/include/raylib.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define TILE_SIZE 75

typedef enum
{
    NONE,
    SELECTING,
    APPLYING
} StatusType;

StatusType status = NONE;

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
    DrawCircle((x + 0.5) * TILE_SIZE, (y + 0.5) * TILE_SIZE, floor(TILE_SIZE) / 8, light_red);
}

void mouse_interactions()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = GetMousePosition();
        int x_pos = floor(mouse.x / TILE_SIZE);
        int y_pos = floor(mouse.y / TILE_SIZE);
        selected_piece = find_entry_at_xy_pos(&map, x_pos, y_pos);
        status = SELECTING;
    }
}

int route_check_rook(RouteCheckObj routes[14], PieceMapEntry rook)
{
    int count = 0;
    // RIGHT
    for (int x = rook.x_pos + 1; x < 8; x++)
    {
        RouteStatus status = piece_route_check(&map, x, rook.y_pos, rook.color);

        if (status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
        }
        else if (status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
            break;
        }
        else
        {
            break;
        }
    }
    // LEFT
    for (int x = rook.x_pos - 1; x > 0; x--)
    {
        RouteStatus status = piece_route_check(&map, x, rook.y_pos, rook.color);

        if (status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
        }
        else if (status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){x, rook.y_pos};
            break;
        }
        else
        {
            break;
        }
    }
    // UP
    for (int y = rook.y_pos - 1; y > 0; y--)
    {
        RouteStatus status = piece_route_check(&map, rook.x_pos, y, rook.color);

        if (status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
        }
        else if (status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
            break;
        }
        else
        {
            break;
        }
    }
    // DOWN
    for (int y = rook.y_pos + 1; y < 8; y++)
    {
        RouteStatus status = piece_route_check(&map, rook.x_pos, y, rook.color);

        if (status == EMPTY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
        }
        else if (status == ENEMY)
        {
            routes[count++] = (RouteCheckObj){rook.x_pos, y};
            break;
        }
        else
        {
            break;
        }
    }
    return count;
}

void calculate_piece_route()
{
    RouteCheckObj routes[14];
    // No NULL check
    int piece_type_index = selected_piece->type_index;
    char piece_move_count = selected_piece->move_count;
    char piece_color = selected_piece->color;
    bool is_white = (piece_color == 'w');

    {
        switch (piece_type_index)
        {
        case 1:
            // PAWN
            if (is_white)
            {
                draw_piece_route(selected_piece->x_pos, selected_piece->y_pos - 1);
            }
            else
            {
                draw_piece_route(selected_piece->x_pos, selected_piece->y_pos + 1);
            }
            if (piece_move_count == 0)
            {
                if (is_white)
                {
                    draw_piece_route(selected_piece->x_pos, selected_piece->y_pos - 2);
                }
                else
                {
                    draw_piece_route(selected_piece->x_pos, selected_piece->y_pos + 2);
                }
            }
            break;
        case 2:
            for (int i = 0; i < route_check_rook(routes, *selected_piece); i++)
            {
                draw_piece_route(routes[i].x, routes[i].y);
            }
            break;
        }
    }
}

void board_details()
{
    if (!!selected_piece)
    {
        int x_pos = selected_piece->x_pos;
        int y_pos = selected_piece->y_pos;
        draw_piece_selection(x_pos, y_pos);
        calculate_piece_route();
    }
}

void draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_board();
    draw_pieces();
    mouse_interactions();
    board_details();
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
