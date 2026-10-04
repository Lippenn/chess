// #include "../include/draw.h"
// #include "../raylib/include/raylib.h"
// #include "./textures.h"
// #include "game.h"
// #include <math.h>

// RenderTexture2D board_texture;
// RenderTexture2D header_texture;
// RenderTexture2D footer_texture;

// Rectangle board = {0, 100, BOARD_WIDTH, BOARD_HEIGHT};

// void set_header_texture()
// {
//     BeginTextureMode(header_texture);
//     ClearBackground((Color){35, 35, 35, 255});
//     DrawRectangle(0, HEADER_HEIGHT - 3, 600, 3, (Color){207, 135, 65, 255});

//     DrawText("CHESS", 25, 18, 32, RAYWHITE);
//     DrawText("Classic Chess", 25, 55, 20, (Color){160, 160, 160, 255});

//     // char turn_text[32];
//     // snprintf(turn_text, sizeof(turn_text), "TURN %d", (game_state.move + 1) / 2);

//     // DrawText(turn_text, 320, 24, 18, (Color){180, 180, 180, 255});
//     // const char *turn_text_color = game_state.color == 'w' ? "WHITE TO MOVE" : "BLACK TO MOVE";
//     // DrawText(turn_text_color, 320, 50, 20, RAYWHITE);

//     EndTextureMode();
// }

// void set_footer_texture()
// {
//     BeginTextureMode(footer_texture);
//     ClearBackground((Color){35, 35, 35, 255});
//     DrawRectangle(0, 0, FOOTER_WIDTH, 3, (Color){207, 135, 65, 255});
//     EndTextureMode();
// }

// void draw_flip_button()
// {
//     Rectangle flip_button = {20, HEADER_HEIGHT + BOARD_HEIGHT + 25, 100, 50};

//     if (draw_button(flip_button, "FLIP", 20))
//     {
//     }
// }

// void draw_undo_button()
// {
//     Rectangle flip_button = {140, HEADER_HEIGHT + BOARD_HEIGHT + 25, 100, 50};

//     if (draw_button(flip_button, "UNDO", 20))
//     {
//     }
// }

// void set_board_texture()
// {
//     BeginTextureMode(board_texture);

//     ClearBackground(RAYWHITE);

//     for (int y = 0; y < 8; y++)
//     {
//         for (int x = 0; x < 8; x++)
//         {
//             DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE,
//                           (x + y) % 2 == 0 ? LIGHTGRAY : DARKGRAY);
//         }
//     }

//     EndTextureMode();
// }

// void draw_screen_image(Texture2D texture, int x, int y)
// {
//     float scale = 65.0f / texture.width;
//     DrawTextureEx(texture, (Vector2){x, y}, 0.0f, scale, WHITE);
// }

// void draw_promotion_overlay()
// {
//     // int offset = current_piece->color == 'w' ? 0 : 4;

//     // Rectangle panel = {125, 275, 350, 150};

//     // DrawRectangle(0, 0, 600, HEADER_HEIGHT + 600, Fade(BLACK, 0.6f));
//     // DrawRectangleRec(panel, RAYWHITE);

//     // const char *title = "PROMOTE";
//     // int title_size = 30;
//     // int title_width = MeasureText(title, title_size);

//     // DrawText(title, panel.x + (panel.width - title_width) / 2, 295, title_size, BLACK);

//     // float button_size = 75;
//     // float spacing = 5;

//     // float total_width = button_size * 4 + spacing * 3;
//     // float start_x = panel.x + (panel.width - total_width) / 2;

//     // for (int i = offset; i < 4 + offset; i++)
//     // {
//     //     int index = i - offset;

//     //     Rectangle button = {start_x + index * (button_size + spacing), 335, button_size,
//     //                         button_size};

//     //     draw_image_button(button, promotion_pieces[i].texture);
//     // }
// }
// void draw_game_over_overlay()
// {
//     // Rectangle panel = {125, 275, 350, 150};
//     // DrawRectangle(0, 0, 600, HEADER_HEIGHT + 600, Fade(BLACK, 0.6f));
//     // DrawRectangleRec(panel, RAYWHITE);
//     // char main_text[20] = "WINS";
//     // char sub_text[50] = "by checkmate";

//     // if (!game_state.stalemate)
//     // {
//     //     memmove(main_text + strlen(game_state.winner == 'w' ? "WHITE " : "BLACK "), main_text,
//     //             strlen(main_text) + 1);
//     //     memcpy(main_text, (game_state.winner == 'w' ? "WHITE " : "BLACK "),
//     //            strlen(game_state.winner == 'w' ? "WHITE " : "BLACK "));
//     // }
//     // else
//     // {
//     //     memcpy(main_text, "DRAW", strlen("DRAW"));
//     //     memcpy(sub_text + 3, "stale", strlen("stale"));
//     // }

//     // int main_text_width = MeasureText(main_text, 30);
//     // int sub_text_width = MeasureText(sub_text, 20);

//     // DrawText(main_text, BOARD_WIDTH / 2 - main_text_width / 2, 295, 30, BLACK);
//     // DrawText(sub_text, BOARD_WIDTH / 2 - sub_text_width / 2, 330, 20, BLACK);

//     // Rectangle reset_button = {225, 365, 150, 40};

//     // if (draw_button(reset_button, "RESET", 20))
//     // {
//     // }
// }

// Texture2D get_piece_texture(PieceTypeEnum type, char color)
// {
//     if (color == 'w')
//     {
//         switch (type)
//         {
//         case PAWN:
//             return white_pawn;
//         case KNIGHT:
//             return white_knight;
//         case BISHOP:
//             return white_bishop;
//         case ROOK:
//             return white_rook;
//         case QUEEN:
//             return white_queen;
//         case KING:
//             return white_king;
//         }
//     }
//     else
//     {
//         switch (type)
//         {
//         case PAWN:
//             return black_pawn;
//         case KNIGHT:
//             return black_knight;
//         case BISHOP:
//             return black_bishop;
//         case ROOK:
//             return black_rook;
//         case QUEEN:
//             return black_queen;
//         case KING:
//             return black_king;
//         }
//     }

//     return (Texture2D){0};
// }

// void draw_piece_selection(int x, int y)
// {
//     Color light_yellow = {255, 250, 202, 196};
//     DrawRectangle(x * TILE_SIZE + board.x, y * TILE_SIZE + board.y, TILE_SIZE, TILE_SIZE,
//                   light_yellow);
// }

// void draw_king_danger(int x, int y)
// {
//     Color light_yellow = {255, 250, 202, 196};
//     DrawRectangle(x * TILE_SIZE + board.x, y * TILE_SIZE + board.y, TILE_SIZE, TILE_SIZE, RED);
// }

// void draw_piece_route(int x, int y)
// {
//     Color light_red = {255, 0, 0, 255};
//     DrawCircle((x + 0.5) * TILE_SIZE + board.x, (y + 0.5) * TILE_SIZE + board.y,
//                floor(TILE_SIZE) / 8, light_red);
// }

// void draw_image(Texture2D texture, int x, int y)
// {
//     Texture2D piece = texture;

//     float scaleX = 65.0f / piece.width;
//     float scaleY = 65.0f / piece.height;
//     DrawTextureEx(piece, (Vector2){x * 75 + 5 + board.x, y * 75 + 5 + board.y}, 0.0f, scaleX,
//                   WHITE);
// }

// void draw_pieces()
// {
//     // for (int i = 0; i < MAX_PIECES; i++)
//     // {
//     //     PieceMapEntry *entry = &map.entries[i];
//     //     if (entry->is_alive)
//     //     {
//     //         draw_image(entry->texture, entry->x_pos, entry->y_pos);
//     //     }
//     // }
// }

// void draw_open_routes() {}

// void draw_selection()
// {
//     // if (current_piece && current_piece->is_alive && game_state.status == SELECTING)
//     // {
//     //     draw_piece_selection(current_piece->x_pos, current_piece->y_pos);
//     // }
// }

// bool draw_button(Rectangle bounds, const char *text, int font_size)
// {
//     Vector2 mouse = GetMousePosition();

//     bool hovered = CheckCollisionPointRec(mouse, bounds);
//     bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

//     Color background = hovered ? GRAY : LIGHTGRAY;

//     DrawRectangleRec(bounds, background);
//     DrawRectangleLinesEx(bounds, 2, BLACK);

//     int text_width = MeasureText(text, font_size);

//     DrawText(text, bounds.x + (bounds.width - text_width) / 2,
//              bounds.y + (bounds.height - font_size) / 2, font_size, BLACK);

//     return clicked;
// }

// bool draw_image_button(Rectangle bounds, Texture2D texture)
// {
//     Vector2 mouse = GetMousePosition();

//     bool hovered = CheckCollisionPointRec(mouse, bounds);
//     bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

//     Color background = hovered ? GRAY : LIGHTGRAY;

//     DrawRectangleRec(bounds, background);
//     DrawRectangleLinesEx(bounds, 2, BLACK);

//     draw_screen_image(texture, bounds.x + 5, bounds.y + 5);

//     return clicked;
// }

// void draw_king_attacked()
// {
//     // PieceMapEntry *kings[2] = {&map.entries[28], &map.entries[4]};
//     // for (int i = 0; i < 2; i++)
//     // {
//     //     if (kings[i] != NULL &&
//     //         calculate_square_attacked((XYPosition){kings[i]->x_pos, kings[i]->y_pos},
//     //                                   kings[i]->color))
//     //     {
//     //         draw_king_danger(kings[i]->x_pos, kings[i]->y_pos);
//     //     }
//     // }
// }