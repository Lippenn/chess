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

const char *enumNames[] = {"NONE", "SELECTING", "APPLYING"};

StatusType status = NONE;

PieceMapEntry *selected_piece = NULL;

RouteCheckObj routes[27];

PieceMap map;

Vector2 boardPosition = {0, 100};

Rectangle board = {0, 100, 600, 600};

bool board_dirty = true;

void draw_board()
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Color tileColor = (x + y) % 2 == 0 ? LIGHTGRAY : DARKGRAY;
            DrawRectangle(boardPosition.x + x * TILE_SIZE, boardPosition.y + y * TILE_SIZE,
                          TILE_SIZE, TILE_SIZE, tileColor);
            // DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);
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
    DrawTextureEx(piece, (Vector2){x * 75 + 5 + board.x, y * 75 + 5 + board.y}, 0.0f, scaleX,
                  WHITE);
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
    DrawRectangle(x * TILE_SIZE + board.x, y * TILE_SIZE + board.y, TILE_SIZE, TILE_SIZE,
                  light_yellow);
}

void draw_piece_route(int x, int y)
{
    Color light_red = {255, 0, 0, 255};
    DrawCircle((x + 0.5) * TILE_SIZE + board.x, (y + 0.5) * TILE_SIZE + board.y,
               floor(TILE_SIZE) / 8, light_red);
}

void calculate_piece_route()
{
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
            for (int i = 0; i < route_check_pawn(routes, *selected_piece, &map); i++)
            {
                draw_piece_route(routes[i].x, routes[i].y);
            }
            break;
        case 2:
            // ROOK
            for (int i = 0; i < route_check_rook(routes, *selected_piece, &map); i++)
            {
                draw_piece_route(routes[i].x, routes[i].y);
            }
            break;
        case 3:
            // KNIGHT
            for (int i = 0; i < route_check_knight(routes, *selected_piece, &map); i++)
            {
                draw_piece_route(routes[i].x, routes[i].y);
            }
            break;
        case 4:
            // BISHOP
            for (int i = 0; i < route_check_bishop(routes, *selected_piece, &map, -1); i++)
            {
                draw_piece_route(routes[i].x, routes[i].y);
            }
            break;
        case 5:
            // QUEEN
            for (int i = 0; i < route_check_queen(routes, *selected_piece, &map); i++)
            {
                draw_piece_route(routes[i].x, routes[i].y);
            }
            break;
        case 6:
            // KING
            for (int i = 0; i < route_check_king(routes, *selected_piece, &map); i++)
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
        if (status == SELECTING)
        {
            int x_pos = selected_piece->x_pos;
            int y_pos = selected_piece->y_pos;
            draw_piece_selection(x_pos, y_pos);
            calculate_piece_route();
        }
    }
}

void mouse_interactions()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = GetMousePosition();
        int x_pos = floor((mouse.x - board.x) / TILE_SIZE);
        int y_pos = floor((mouse.y - board.y) / TILE_SIZE);
        if (status == NONE)
        {
            selected_piece = find_entry_at_xy_pos(&map, x_pos, y_pos);
            status = SELECTING;
        }
        else
        {
            move_piece(x_pos, y_pos, selected_piece, routes, &map);
            status = NONE;
        }
    }
}

void draw_header() { DrawText("Chess", 20, 30, 40, BLACK); }

void draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_header();
    draw_board();
    draw_pieces();
    mouse_interactions();
    board_details();
    EndDrawing();
}

int main()
{
    init_map(&map);

    InitWindow(600, 700, "My Game");
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
