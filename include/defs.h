
#include "../raylib/include/raylib.h"
#include <stdbool.h>

#ifndef DEFS_H
#define DEFS_H

#define TILE_SIZE 75

#define HEADER_HEIGHT 100
#define HEADER_WIDTH 600

#define BOARD_HEIGHT 600
#define BOARD_WIDTH 600

#define MAX_PIECES 32
#define MAX_MOVES 32

typedef enum
{
    FRIENDLY,
    ENEMY,
    EMPTY,
    OUTSIDE
} SquareStatusEnum;

typedef enum
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceTypeEnum;

typedef enum
{
    SELECTING,
    APPLYING,
    NONE
} GameStateEnum;

typedef struct
{
    int move;
    char color;
    GameStateEnum status;
} GameState;

typedef struct
{
    PieceTypeEnum type;
    char color;
    int x;
    int y;
} StarterPiece;

extern StarterPiece starter_pieces[MAX_PIECES];

typedef struct
{
    int index;
    PieceTypeEnum type;
    char color;
    int x_pos;
    int y_pos;
    bool is_alive;
    Texture2D texture;
    int move_count;
} PieceMapEntry;

typedef struct
{
    PieceMapEntry entries[MAX_PIECES];
    int count;
} PieceMap;

typedef struct
{
    int x;
    int y;
    bool is_enemy;
} XYPosition;

typedef struct
{
    XYPosition routes[MAX_PIECES];
    int count;
} OpenRoutes;

extern OpenRoutes open_routes;

#endif