
#include <stdbool.h>
#include <stdio.h>  // IWYU pragma: export
#include <stdlib.h> // IWYU pragma: export

#ifndef DEFS_H
#define DEFS_H

#define TILE_SIZE 75

#define HEADER_HEIGHT 100
#define HEADER_WIDTH 600

#define FOOTER_HEIGHT 100
#define FOOTER_WIDTH 600

#define BOARD_HEIGHT 600
#define BOARD_WIDTH 600
#define BOARD_Y HEADER_HEIGHT

#define MAX_PIECES 32
#define BOARD_SIZE 8
#define MAX_MOVES 256

#define TEST(condition)                                                                            \
    do                                                                                             \
    {                                                                                              \
        if (!(condition))                                                                          \
        {                                                                                          \
            printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, #condition);                           \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

typedef enum
{
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceType;

typedef enum
{
    PIECE_WHITE,
    PIECE_BLACK
} PieceColor;

typedef struct
{
    PieceType type;
    PieceColor color;
} Piece;

typedef struct
{
    PieceType type;
    PieceColor color;
    int x;
    int y;
} SelectedPiece;

typedef struct
{
    Piece squares[BOARD_SIZE][BOARD_SIZE];
} Board;

typedef enum
{
    SELECTING,
    APPLYING,
    NONE
} BoardStatus;

typedef enum
{
    MOVE_NORMAL,
    MOVE_CASTLE,
    MOVE_EN_PASSANT,
    MOVE_PROMOTION
} MoveType;

typedef struct
{
    MoveType type;
    int from_x;
    int from_y;
    int to_x;
    int to_y;
} Move;

typedef struct
{
    Board board;

    SelectedPiece selected_piece;
    BoardStatus board_status;

    PieceColor turn;

    bool white_can_castle_kingside;
    bool white_can_castle_queenside;
    bool black_can_castle_kingside;
    bool black_can_castle_queenside;

    int en_passant_x;
    int en_passant_y;

    int halfmove_clock;
    int fullmove_number;
} GameState;

#endif