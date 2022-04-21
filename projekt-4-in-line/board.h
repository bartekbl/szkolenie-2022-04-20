#ifndef BOARD_H
#define BOARD_H

#include "ret.h"

#include <stdbool.h>

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6
#define WIN_CONDITION 4

typedef enum
{
    Tile_EMPTY,
    Tile_PLAYER_X,
    Tile_PLAYER_O,
} Tile;

typedef struct
{
    Tile tiles[BOARD_HEIGHT][BOARD_WIDTH];
} Board;

int Tile_print(Tile tile);

int Board_init(Board* board);

#define RET_BOARD_MAKE_MOVE_WRONG_COLUMN 1001
#define RET_BOARD_MAKE_MOVE_COLUMN_FULL 1002
int Board_makeMove(Board* board, int column, Tile move);

int Board_checkWinCondition(const Board* board, Tile* winner);

#endif // BOARD_H
