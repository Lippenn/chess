
#include "../raylib/include/raylib.h"
#include <stdbool.h>

#ifndef DEFS_H
#define DEFS_H

#define TILE_SIZE 75

#define HEADER_HEIGHT 100
#define HEADER_WIDTH 600

#define FOOTER_HEIGHT 100
#define FOOTER_WIDTH 600

#define BOARD_HEIGHT 600
#define BOARD_WIDTH 600

#define MAX_PIECES 32
#define MAX_MOVES 32
#define MAX_POSITION_MOVES 1024

typedef struct
{
    int index;
    int capture_index;
    int from_x;
    int from_y;
    int to_x;
    int to_y;
    int sub_x;
    int sub_y;
    bool is_promotion;
    bool is_castle;
} Move;

typedef struct
{
    Move moves[MAX_POSITION_MOVES];
    int count;
} MoveHistory;

typedef enum
{
    PLAYER_WHITE,
    PLAYER_BLACK,
    NIL
} PlayerEnum;

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
    bool promotion_active;
    bool game_over;
    bool stalemate;
    bool flip;
    int promotion_x;
    int promotion_y;
    PlayerEnum winner;
} GameState;

typedef struct
{
    PieceTypeEnum type;
    char color;
    int x;
    int y;
    bool is_alive;
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
    int double_move_at;
} PieceMapEntry;

typedef struct
{
    PieceMapEntry entries[MAX_PIECES];
    int count;
} PieceMap;

typedef struct
{
    PieceTypeEnum type;
    int value;
} PieceValueMapEntry;

typedef struct
{
    int value;
    PieceTypeEnum type;
} PieceMapValue;

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

typedef struct
{
    Texture2D texture;
    PieceTypeEnum type;
} PromotionPiece;

extern OpenRoutes open_routes;

extern int pawn_directions[2];
extern int knight_directions[8][2];
extern int bishop_directions[4][2];
extern int rook_directions[4][2];
extern int king_directions[8][2];
extern int king_castle_directions[2][2];

#endif