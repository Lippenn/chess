#include "test_game.h"
#include "game.h"
#include "test_main.h"

void test_initial_board(void)
{
    GameState game;

    init_game_state(&game);

    TEST(game.board.squares[0][0].type == ROOK);
    TEST(game.board.squares[0][0].color == PIECE_BLACK);

    TEST(game.board.squares[0][4].type == KING);
    TEST(game.board.squares[0][4].color == PIECE_BLACK);

    TEST(game.board.squares[7][4].type == KING);
    TEST(game.board.squares[7][4].color == PIECE_WHITE);

    TEST(game.board.squares[7][7].type == ROOK);
    TEST(game.board.squares[7][7].color == PIECE_WHITE);

    TEST(game.board.squares[4][4].type == EMPTY);
    TEST(game.board.squares[3][3].type == EMPTY);
}

void test_handle_board_click_status_none()
{
    GameState game = {0};
    game.board.squares[0][0] = (Piece){ROOK, PIECE_BLACK};
    game.board_status = NONE;

    handle_board_click(&game, 0, 0);

    TEST(game.selected_piece.color == PIECE_BLACK);
    TEST(game.selected_piece.type == ROOK);
    TEST(game.selected_piece.x == 0);
    TEST(game.selected_piece.y == 0);
    TEST(game.board_status == SELECTING);
}