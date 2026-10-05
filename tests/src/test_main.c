#include "defs.h"
#include "game.h"
#include "test_fen.h"
#include "test_game.h"
#include "test_moves.h"
#include <stdio.h>
#include <time.h>

typedef void (*TestFunction)(void);

void run_test(TestFunction test, const char *name)
{
    printf("%-50s ----->  ", name);
    clock_t start = clock();
    test();
    clock_t end = clock();

    double elapsed_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

    printf("PASSED   (%8.3f ms)\n", elapsed_ms);
}

int main(void)
{
    clock_t start = clock();

    // ----------------- BOARD TESTS --------------------
    run_test(test_initial_board, "Test inital board");

    // ----------------- FEN --------------------
    run_test(test_load_fen, "Test load fen");
    run_test(test_generate_fen, "Test generate fen");

    // ----------------- MOVES --------------------
    run_test(test_white_pawn_normal_move, "Test white pawn normal move");
    run_test(test_white_pawn_double_move, "Test white pawn double move");
    run_test(test_white_pawn_capture_move, "Test white pawn capture move");
    run_test(test_white_pawn_en_passant_move, "Test white pawn en passant move");
    run_test(test_white_pawn_promotion_move, "Test white pawn promotion move");
    run_test(test_white_knight_normal_move, "Test white knight normal move");
    run_test(test_white_bishop_normal_move, "Test white bishop normal move");
    run_test(test_white_rook_normal_move, "Test white rook normal move");
    run_test(test_white_queen_normal_move, "Test white queen normal move");
    run_test(test_white_king_normal_move, "Test white king normal move");
    run_test(test_white_king_side_castle_move, "Test white king side castle move");
    run_test(test_white_queen_side_castle_move, "Test white queen side castle move");
    run_test(test_white_piece_normal_move, "Test white piece normal move");
    run_test(test_white_king_normal_move_restricted_by_check,
             "Test white king normal move restricted by check");
    run_test(test_white_piece_normal_move_restricted_by_check,
             "Test white piece normal move restricted by check");
    clock_t end = clock();
    double elapsed_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
    printf("%-60s        (%8.3f ms)\n", "All tests passed!", elapsed_ms);

    return 0;
}