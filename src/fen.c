#include "fen.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void set_board_empty(Board *board)
{
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            board->squares[y][x] = (Piece){EMPTY};
        }
    }
}

void set_board_pieces(Board *board, char piece_str[128])
{
    int row = 0;
    int col = 0;
    for (int i = 0; piece_str[i] != '\0'; i++)
    {
        char c = piece_str[i];

        if (c == '/')
        {
            row++;
            col = 0;
        }
        else if (isdigit(c))
        {
            int empty_squares = c - '0';
            col += empty_squares;
        }
        else
        {
            Piece piece = {EMPTY};
            piece.color = isupper(c) ? PIECE_WHITE : PIECE_BLACK;
            switch (tolower(c))
            {
            case 'p':
                piece.type = PAWN;
                break;
            case 'n':
                piece.type = KNIGHT;
                break;
            case 'b':
                piece.type = BISHOP;
                break;
            case 'r':
                piece.type = ROOK;
                break;
            case 'k':
                piece.type = KING;
                break;
            case 'q':
                piece.type = QUEEN;
                break;
            default:
                piece.type = EMPTY;
                break;
            }
            board->squares[row][col] = piece;
            piece.type = col++;
        }
    }
}

void load_fen(GameState *game_state, char *fen)
{
    char piece_str[128];
    char color_str[2];
    char castle_str[16];
    char ep_str[4];
    int parsed = sscanf(fen, "%s %s %s %s %d %d", piece_str, color_str, castle_str, ep_str,
                        &game_state->halfmove_clock, &game_state->fullmove_number);

    if (parsed != 6)
    {
        return;
    }

    game_state->turn = color_str[0] == 'w' ? PIECE_WHITE : PIECE_BLACK;
    game_state->white_can_castle_kingside = strchr(castle_str, 'K') != NULL;
    game_state->white_can_castle_queenside = strchr(castle_str, 'Q') != NULL;
    game_state->black_can_castle_kingside = strchr(castle_str, 'k') != NULL;
    game_state->black_can_castle_queenside = strchr(castle_str, 'q') != NULL;

    if (strcmp(ep_str, "-") == 0)
    {
        game_state->en_passant_x = -1;
        game_state->en_passant_y = -1;
    }
    else
    {
        game_state->en_passant_x = ep_str[0] - 'a';
        game_state->en_passant_y = 8 - (ep_str[1] - '0');
    }

    set_board_empty(&game_state->board);
    set_board_pieces(&game_state->board, piece_str);
}

void generate_fen(GameState *game_state, char *fen)
{
    int pos = 0;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        int empty_count = 0;
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            char c;
            Piece piece = game_state->board.squares[y][x];

            if (piece.type == EMPTY)
            {
                empty_count++;
                continue;
            }
            if (empty_count > 0)
            {
                pos += sprintf(&fen[pos], "%d", empty_count);
                empty_count = 0;
            }
            switch (piece.type)
            {
            case PAWN:
                c = 'p';
                break;
            case KNIGHT:
                c = 'n';
                break;
            case BISHOP:
                c = 'b';
                break;
            case ROOK:
                c = 'r';
                break;
            case QUEEN:
                c = 'q';
                break;
            case KING:
                c = 'k';
                break;
            case EMPTY:
                break;
            }
            pos += sprintf(&fen[pos], "%c", piece.color == PIECE_WHITE ? toupper(c) : c);
        }
        if (empty_count > 0)
            pos += sprintf(&fen[pos], "%d", empty_count);

        if (y < BOARD_SIZE - 1)
            fen[pos++] = '/';
    }
    fen[pos++] = ' ';
    fen[pos++] = game_state->turn == PIECE_WHITE ? 'w' : 'b';
    fen[pos++] = ' ';

    int has_castling = 0;

    if (game_state->white_can_castle_kingside)
    {
        fen[pos++] = 'K';
        has_castling = 1;
    }
    if (game_state->white_can_castle_queenside)
    {
        fen[pos++] = 'Q';
        has_castling = 1;
    }
    if (game_state->black_can_castle_kingside)
    {
        fen[pos++] = 'k';
        has_castling = 1;
    }
    if (game_state->black_can_castle_queenside)
    {
        fen[pos++] = 'q';
        has_castling = 1;
    }
    if (!has_castling)
        fen[pos++] = '-';

    fen[pos++] = ' ';

    if (game_state->en_passant_x == -1)
    {
        fen[pos++] = '-';
    }
    else
    {
        fen[pos++] = 'a' + game_state->en_passant_x;
        fen[pos++] = '8' - game_state->en_passant_y;
    }

    pos += sprintf(&fen[pos], " %d", game_state->halfmove_clock);

    pos += sprintf(&fen[pos], " %d", game_state->fullmove_number);

    fen[pos] = '\0';
}