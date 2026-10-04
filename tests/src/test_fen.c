#include "test_fen.h"
#include "fen.h"
#include <string.h>

void test_load_fen(void)
{
    GameState game = {0};
    load_fen(&game, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    TEST(game.turn == WHITE);

    TEST(game.white_can_castle_kingside);
    TEST(game.white_can_castle_kingside);
    TEST(game.black_can_castle_kingside);
    TEST(game.black_can_castle_queenside);

    TEST(game.board.squares[0][0].color == BLACK);
    TEST(game.board.squares[0][0].type == ROOK);

    TEST(game.board.squares[7][7].color == WHITE);
    TEST(game.board.squares[7][7].type == ROOK);
}

void test_generate_fen(void)
{
    GameState game = {0};
    load_fen(&game, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    char fen[1024];
    generate_fen(&game, fen);
    TEST(strcmp(fen, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == 0);
}