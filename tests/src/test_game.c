#include "test_game.h"
#include "game.h"
#include "test_main.h"

void test_initial_board(void)
{
    GameState game;

    init_game_state(&game);

    TEST(game.board.squares[0][0].type == ROOK);
    TEST(game.board.squares[0][0].color == BLACK);

    TEST(game.board.squares[0][4].type == KING);
    TEST(game.board.squares[0][4].color == BLACK);

    TEST(game.board.squares[7][4].type == KING);
    TEST(game.board.squares[7][4].color == WHITE);

    TEST(game.board.squares[7][7].type == ROOK);
    TEST(game.board.squares[7][7].color == WHITE);

    TEST(game.board.squares[4][4].type == EMPTY);
    TEST(game.board.squares[3][3].type == EMPTY);
}
