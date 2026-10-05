#include "defs.h"
#include "game.h"
#include "test_fen.h"
#include "test_game.h"
#include "test_moves.h"
#include <stdio.h>
#include <time.h>

typedef void (*TestFunction)(void);

typedef struct
{
    TestFunction function;
    char *name;
} TestFunctionWName;

double run_test(TestFunction test, const char *name)
{
    printf("%-50s ----->  ", name);
    struct timespec start, end;

    timespec_get(&start, TIME_UTC);
    test();
    timespec_get(&end, TIME_UTC);

    long long start_us = (long long)start.tv_sec * 1000000LL + (start.tv_nsec / 1000);
    long long end_us = (long long)end.tv_sec * 1000000LL + (end.tv_nsec / 1000);

    double elapsed_ms = (double)(end_us - start_us) / 1000.0;

    printf("PASSED   (%8.4f ms)\n", elapsed_ms);
    return elapsed_ms;
}

TestFunctionWName tests[] = {
    // ----------------- BOARD --------------------
    {test_initial_board, "Test inital board"},
    {test_handle_board_click_status_none, "Test handle board click status NONE"},

    // ----------------- FEN --------------------
    {test_load_fen, "Test load fen"},
    {test_generate_fen, "Test generate fen"},

    // ----------------- MOVES --------------------
    {test_white_pawn_normal_move, "Test white pawn normal move"},
    {test_white_pawn_double_move, "Test white pawn double move"},
    {test_white_pawn_capture_move, "Test white pawn capture move"},
    {test_white_pawn_en_passant_move, "Test white pawn en passant move"},
    {test_white_pawn_promotion_move, "Test white pawn promotion move"},
    {test_white_knight_normal_move, "Test white knight normal move"},
    {test_white_bishop_normal_move, "Test white bishop normal move"},
    {test_white_rook_normal_move, "Test white rook normal move"},
    {test_white_queen_normal_move, "Test white queen normal move"},
    {test_white_king_normal_move, "Test white king normal move"},
    {test_white_king_side_castle_move, "Test white king side castle move"},
    {test_white_queen_side_castle_move, "Test white queen side castle move"},
    {test_white_piece_normal_move, "Test white piece normal move"},
    {test_white_king_normal_move_restricted_by_check,
     "Test white king normal move restricted by check"},
    {test_white_piece_normal_move_restricted_by_check,
     "Test white piece normal move restricted by check"},
};

int main(void)
{
    double total_execution_ms = 0.0;
    size_t total_test_count = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < total_test_count; i++)
    {
        total_execution_ms += run_test(tests[i].function, tests[i].name);
    }
    printf("%-60s        (%8.4f ms)\n", "All tests passed!", total_execution_ms);

    return 0;
}