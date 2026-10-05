#include "moves.h"

bool move_to_exists(Move pseudo[MAX_MOVES], int counter, int x, int y)
{
    for (int i = 0; i < counter; i++)
    {
        if (pseudo[i].to_x == x && pseudo[i].to_y == y)
        {
            return true;
        }
    }
    return false;
}

void test_white_pawn_normal_move(void)
{
    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){PAWN, PIECE_WHITE};

    Move moves[MAX_MOVES];

    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);

    TEST(counter == 1);
    TEST(moves[0].to_x == 4);
    TEST(moves[0].to_y == 3);

    game.board.squares[3][4] = (Piece){KNIGHT, PIECE_WHITE};
    counter = 0;
    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);
    TEST(counter == 0);
}

void test_white_pawn_double_move(void)
{
    GameState game = {0, .en_passant_x = -1, .en_passant_y = -1};

    game.turn = PIECE_WHITE;
    game.board.squares[6][4] = (Piece){PAWN, PIECE_WHITE};

    Move moves[MAX_MOVES];

    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[6][4], &moves, &counter, 4, 6);

    TEST(counter == 2);

    TEST(moves[0].to_x == 4);
    TEST(moves[0].to_y == 5);

    TEST(moves[1].to_x == 4);
    TEST(moves[1].to_y == 4);

    game.board.squares[4][4] = (Piece){KNIGHT, PIECE_WHITE};
    counter = 0;
    generate_legal_moves_by_piece(&game, game.board.squares[6][4], &moves, &counter, 4, 6);
    TEST(counter == 1);
}

void test_white_pawn_capture_move(void)
{
    GameState game = {0, .en_passant_x = -1, .en_passant_y = -1};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){PAWN, PIECE_WHITE};
    game.board.squares[3][5] = (Piece){PAWN, PIECE_BLACK};

    Move moves[MAX_MOVES];

    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);

    TEST(counter == 2);

    TEST(moves[0].to_x == 4);
    TEST(moves[0].to_y == 3);

    TEST(moves[1].to_x == 5);
    TEST(moves[1].to_y == 3);
}

void test_white_pawn_en_passant_move(void)
{
    GameState game = {0, .en_passant_x = -1, .en_passant_y = -1};

    game.turn = PIECE_WHITE;
    game.board.squares[3][3] = (Piece){PAWN, PIECE_WHITE};
    game.board.squares[3][4] = (Piece){PAWN, PIECE_BLACK};
    game.en_passant_x = 4;
    game.en_passant_y = 2;

    Move moves[MAX_MOVES];

    int counter = 0;
    generate_legal_moves_by_piece(&game, game.board.squares[3][3], &moves, &counter, 3, 3);
    TEST(counter == 2);

    TEST(moves[0].to_x == 3);
    TEST(moves[0].to_y == 2);

    TEST(moves[1].to_x == 4);
    TEST(moves[1].to_y == 2);
}

void test_white_pawn_promotion_move(void)
{
    GameState game = {0, .en_passant_x = -1, .en_passant_y = -1};

    game.turn = PIECE_WHITE;
    game.board.squares[1][3] = (Piece){PAWN, PIECE_WHITE};
    Move moves[MAX_MOVES];

    int counter = 0;
    generate_legal_moves_by_piece(&game, game.board.squares[1][3], &moves, &counter, 3, 1);
    TEST(counter == 1);

    TEST(moves[0].to_x == 3);
    TEST(moves[0].to_y == 0);
    TEST(moves[0].type == MOVE_PROMOTION);
}

void test_white_knight_normal_move(void)
{
    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){KNIGHT, PIECE_WHITE};

    Move moves[MAX_MOVES];

    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);

    TEST(counter == 8);

    TEST(move_to_exists(moves, counter, 6, 5));
    TEST(move_to_exists(moves, counter, 6, 3));

    TEST(move_to_exists(moves, counter, 5, 6));
    TEST(move_to_exists(moves, counter, 5, 2));

    TEST(move_to_exists(moves, counter, 3, 6));
    TEST(move_to_exists(moves, counter, 3, 2));

    TEST(move_to_exists(moves, counter, 2, 5));
    TEST(move_to_exists(moves, counter, 2, 3));
}

void test_white_bishop_normal_move(void)
{

    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){BISHOP, PIECE_WHITE};

    Move moves[MAX_MOVES];

    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);
    TEST(counter == 13);

    TEST(move_to_exists(moves, counter, 5, 5));
    TEST(move_to_exists(moves, counter, 6, 6));
    TEST(move_to_exists(moves, counter, 7, 7));

    TEST(move_to_exists(moves, counter, 5, 3));
    TEST(move_to_exists(moves, counter, 6, 2));
    TEST(move_to_exists(moves, counter, 7, 1));

    TEST(move_to_exists(moves, counter, 3, 5));
    TEST(move_to_exists(moves, counter, 2, 6));
    TEST(move_to_exists(moves, counter, 1, 7));

    TEST(move_to_exists(moves, counter, 3, 3));
    TEST(move_to_exists(moves, counter, 2, 2));
    TEST(move_to_exists(moves, counter, 1, 1));
    TEST(move_to_exists(moves, counter, 0, 0));
}

void test_white_rook_normal_move(void)
{

    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){ROOK, PIECE_WHITE};

    Move moves[MAX_MOVES];
    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);

    TEST(counter == 14);

    TEST(move_to_exists(moves, counter, 5, 4));
    TEST(move_to_exists(moves, counter, 6, 4));
    TEST(move_to_exists(moves, counter, 7, 4));

    TEST(move_to_exists(moves, counter, 3, 4));
    TEST(move_to_exists(moves, counter, 2, 4));
    TEST(move_to_exists(moves, counter, 1, 4));
    TEST(move_to_exists(moves, counter, 0, 4));

    TEST(move_to_exists(moves, counter, 4, 5));
    TEST(move_to_exists(moves, counter, 4, 6));
    TEST(move_to_exists(moves, counter, 4, 7));

    TEST(move_to_exists(moves, counter, 4, 3));
    TEST(move_to_exists(moves, counter, 4, 2));
    TEST(move_to_exists(moves, counter, 4, 1));
    TEST(move_to_exists(moves, counter, 4, 0));
}

void test_white_queen_normal_move(void)
{

    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){QUEEN, PIECE_WHITE};

    Move moves[MAX_MOVES];
    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);

    TEST(counter == 27);

    TEST(move_to_exists(moves, counter, 5, 4));
    TEST(move_to_exists(moves, counter, 6, 4));
    TEST(move_to_exists(moves, counter, 7, 4));

    TEST(move_to_exists(moves, counter, 3, 4));
    TEST(move_to_exists(moves, counter, 2, 4));
    TEST(move_to_exists(moves, counter, 1, 4));
    TEST(move_to_exists(moves, counter, 0, 4));

    TEST(move_to_exists(moves, counter, 4, 5));
    TEST(move_to_exists(moves, counter, 4, 6));
    TEST(move_to_exists(moves, counter, 4, 7));

    TEST(move_to_exists(moves, counter, 4, 3));
    TEST(move_to_exists(moves, counter, 4, 2));
    TEST(move_to_exists(moves, counter, 4, 1));
    TEST(move_to_exists(moves, counter, 4, 0));

    TEST(move_to_exists(moves, counter, 5, 5));
    TEST(move_to_exists(moves, counter, 6, 6));
    TEST(move_to_exists(moves, counter, 7, 7));

    TEST(move_to_exists(moves, counter, 5, 3));
    TEST(move_to_exists(moves, counter, 6, 2));
    TEST(move_to_exists(moves, counter, 7, 1));

    TEST(move_to_exists(moves, counter, 3, 5));
    TEST(move_to_exists(moves, counter, 2, 6));
    TEST(move_to_exists(moves, counter, 1, 7));

    TEST(move_to_exists(moves, counter, 3, 3));
    TEST(move_to_exists(moves, counter, 2, 2));
    TEST(move_to_exists(moves, counter, 1, 1));
    TEST(move_to_exists(moves, counter, 0, 0));
}

void test_white_king_normal_move(void)
{

    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){KING, PIECE_WHITE};

    Move moves[MAX_MOVES];
    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);

    TEST(counter == 8);
    TEST(move_to_exists(moves, counter, 3, 3));
    TEST(move_to_exists(moves, counter, 4, 3));
    TEST(move_to_exists(moves, counter, 5, 3));
    TEST(move_to_exists(moves, counter, 3, 4));
    TEST(move_to_exists(moves, counter, 5, 4));
    TEST(move_to_exists(moves, counter, 3, 5));
    TEST(move_to_exists(moves, counter, 4, 5));
    TEST(move_to_exists(moves, counter, 5, 5));
}

void test_white_king_normal_move_restricted_by_check(void)
{

    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){KING, PIECE_WHITE};
    game.board.squares[3][0] = (Piece){ROOK, PIECE_BLACK};

    Move moves[MAX_MOVES];
    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][4], &moves, &counter, 4, 4);

    TEST(counter == 5);
    TEST(move_to_exists(moves, counter, 3, 4));
    TEST(move_to_exists(moves, counter, 5, 4));
    TEST(move_to_exists(moves, counter, 3, 5));
    TEST(move_to_exists(moves, counter, 4, 5));
    TEST(move_to_exists(moves, counter, 5, 5));
}

void test_white_piece_normal_move_restricted_by_check(void)
{
    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[4][4] = (Piece){KING, PIECE_WHITE};
    game.board.squares[4][3] = (Piece){ROOK, PIECE_WHITE};
    game.board.squares[4][0] = (Piece){ROOK, PIECE_BLACK};

    Move moves[MAX_MOVES];
    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[4][3], &moves, &counter, 3, 4);

    TEST(counter == 3);
    TEST(move_to_exists(moves, counter, 2, 4));
    TEST(move_to_exists(moves, counter, 1, 4));
    TEST(move_to_exists(moves, counter, 0, 4));
}

void test_white_queen_side_castle_move(void)
{

    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[7][4] = (Piece){KING, PIECE_WHITE};
    game.board.squares[7][0] = (Piece){ROOK, PIECE_WHITE};
    game.white_can_castle_queenside = true;
    Move moves[MAX_MOVES];
    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[7][4], &moves, &counter, 4, 7);

    TEST(counter == 6);
    TEST(move_to_exists(moves, counter, 2, 7));
    TEST(move_to_exists(moves, counter, 3, 7));
}

void test_white_king_side_castle_move(void)
{
    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[7][4] = (Piece){KING, PIECE_WHITE};
    game.board.squares[7][7] = (Piece){ROOK, PIECE_WHITE};
    game.white_can_castle_kingside = true;
    Move moves[MAX_MOVES];
    int counter = 0;

    generate_legal_moves_by_piece(&game, game.board.squares[7][4], &moves, &counter, 4, 7);

    TEST(counter == 6);
    TEST(move_to_exists(moves, counter, 5, 7));
    TEST(move_to_exists(moves, counter, 6, 7));
}

void test_white_piece_normal_move(void)
{

    GameState game = {0};

    game.turn = PIECE_WHITE;
    game.board.squares[7][4] = (Piece){KING, PIECE_WHITE};

    make_move(&game, (Move){.type = MOVE_NORMAL, .from_x = 4, .from_y = 7, .to_x = 4, .to_y = 6});

    TEST(game.board.squares[6][4].type == KING);
    TEST(game.board.squares[7][4].type == EMPTY);
}