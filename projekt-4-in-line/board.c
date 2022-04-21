#include "board.h"
#include <stddef.h>

typedef struct
{
    int row_step;
    int column_step;
} Direction;

#define DIRECTIONS_NUMBER 4
static const Direction directions[DIRECTIONS_NUMBER] =
{
    { +1,  0 }, // check vertical
    {  0, +1 }, // check horizontal
    { +1, +1 }, // check diagonal down
    { -1, +1 },// check diagonal up
};

int Tile_print(Tile tile)
{
    int ret = RET_SUCCESS;
    switch (tile)
    {
    case Tile_EMPTY:
        printf(" ");
        break;
    case Tile_PLAYER_X:
        printf("X");
        break;
    case Tile_PLAYER_O:
        printf("O");
        break;
    default:
        assert(0);
        ret = RET_INTERNAL_ERROR;
        goto CLEANUP;
    }
CLEANUP:
    return ret;
}

int Board_init(Board* board)
{
    int ret = RET_SUCCESS;
    assert(board != NULL);
    for (int row = 0; row < BOARD_HEIGHT; row++)
    {
        for (int column = 0; column < BOARD_WIDTH; column++)
        {
            board->tiles[row][column] = Tile_EMPTY;
        }
    }
//CLEANUP:
    return ret;
}

#define RET_BOARD_MAKE_MOVE_WRONG_COLUMN 1001
#define RET_BOARD_MAKE_MOVE_COLUMN_FULL 1002

int Board_makeMove(Board* board, int column, Tile move)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(move != Tile_EMPTY);
    CHECK_ASSERT(board != NULL);
    CHECK(column >= 1 || column <= BOARD_WIDTH, RET_BOARD_MAKE_MOVE_WRONG_COLUMN);
    
    column -= 1; // column is 1-based but array is 0-based
    
    int row = BOARD_HEIGHT - 1;
    while (row >= 0)
    {
        if (board->tiles[row][column] == Tile_EMPTY)
        {
            board->tiles[row][column] = move;
            goto CLEANUP;
        }
        row--;
    }
    ret = RET_BOARD_MAKE_MOVE_COLUMN_FULL;
    goto CLEANUP;
    
CLEANUP:
    return ret;
}

// this is an internal function only called by Board_checkWinCondition so it should always be called with correct parameters
static int Board_checkWinCondition_singleLine(const Board* board, int row, int column, int row_step, int column_step, Tile* winner)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(board != NULL);
    CHECK_ASSERT(row >= 0 && row < BOARD_HEIGHT);
    CHECK_ASSERT(column >= 0 && row < BOARD_WIDTH);
    CHECK_ASSERT(row_step >= -1 && row_step <= 1);
    CHECK_ASSERT(column_step >= 0 && column_step <= 1);
    CHECK_ASSERT(winner != NULL);
    
    Tile current_tile = board->tiles[row][column];
    if (current_tile == Tile_EMPTY) goto CLEANUP;
    
    CHECK_ASSERT(current_tile == Tile_PLAYER_O || current_tile == Tile_PLAYER_X);
    
    if (row                  >= WIN_CONDITION * -row_step &&
        BOARD_HEIGHT - row   >= WIN_CONDITION * row_step &&
        BOARD_WIDTH - column >= WIN_CONDITION * column_step)
    {
        for (int i = 0; i < WIN_CONDITION; i++)
        {
            Tile checked_tile = board->tiles[row + i * row_step][column + i * column_step];
            if (checked_tile != current_tile) goto CLEANUP;
        }
        *winner = current_tile;
        goto CLEANUP;
    }
    
    CLEANUP:
    return ret;
}

int Board_checkWinCondition(const Board* board, Tile* winner)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(board != NULL);
    CHECK_ASSERT(winner != NULL);
    
    for (int row = 0; row < BOARD_HEIGHT; row++)
    {
        for (int column = 0; column < BOARD_WIDTH; column++)
        {
            Tile current_tile = board->tiles[row][column];
            if (current_tile == Tile_EMPTY) continue;
            
            for (int i = 0; i < DIRECTIONS_NUMBER; i++)
            {
                Board_checkWinCondition_singleLine(board, row, column, directions[i].row_step, directions[i].column_step, winner);
                if (*winner != Tile_EMPTY) goto CLEANUP;
            }
        }
    }
    
    *winner = Tile_EMPTY;
    
CLEANUP:
    return ret;
}
