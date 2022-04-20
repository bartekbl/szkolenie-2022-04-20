#include "board.h"

#include <stdio.h>

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
    CHECK_ASSERT(move != Tile_EMPTY, RET_INTERNAL_ERROR);
    CHECK_ASSERT(board != NULL, RET_INTERNAL_ERROR);
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

int Board_checkWinCondition(const Board* board, Tile* winner)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(board != NULL, RET_INTERNAL_ERROR);
    CHECK_ASSERT(winner != NULL, RET_INTERNAL_ERROR);
    
    for (int row = 0; row < BOARD_HEIGHT; row++)
    {
        for (int column = 0; column < BOARD_WIDTH; column++)
        {
            Tile current_tile = board->tiles[row][column];
            if (current_tile == Tile_EMPTY) continue;
            
            // check vertical
            if (BOARD_HEIGHT - row >= WIN_CONDITION)
            {
                bool win = true;
                for (int i = 0; i < WIN_CONDITION; i++)
                {
                    if (board->tiles[row + i][column] != current_tile)
                    {
                        win = false;
                        continue;
                    }
                }
                if (win)
                {
                    *winner = current_tile;
                    goto CLEANUP;
                }
            }
            
            // check horizontal
            if (BOARD_WIDTH - column >= WIN_CONDITION)
            {
                bool win = true;
                for (int i = 0; i < WIN_CONDITION; i++)
                {
                    if (board->tiles[row][column + i] != current_tile)
                    {
                        win = false;
                        continue;
                    }
                }
                if (win)
                {
                    *winner = current_tile;
                    goto CLEANUP;
                }
            }
            
            // check diagonal down
            if (BOARD_HEIGHT - row >= WIN_CONDITION && BOARD_WIDTH - column >= WIN_CONDITION)
            {
                bool win = true;
                for (int i = 0; i < WIN_CONDITION; i++)
                {
                    if (board->tiles[row + i][column + i] != current_tile)
                    {
                        win = false;
                        continue;
                    }
                }
                if (win)
                {
                    *winner = current_tile;
                    goto CLEANUP;
                }
            }
            
            // check diagonal up
            if (row >= WIN_CONDITION && BOARD_WIDTH - column >= WIN_CONDITION)
            {
                bool win = true;
                for (int i = 0; i < WIN_CONDITION; i++)
                {
                    if (board->tiles[row - i][column + i] != current_tile)
                    {
                        win = false;
                        continue;
                    }
                }
                if (win)
                {
                    *winner = current_tile;
                    goto CLEANUP;
                }
            }
        }
    }
    
    *winner = Tile_EMPTY;
    
CLEANUP:
    return ret;
}

int Board_print(const Board* board)
{
    int ret = 0;
    assert(board != NULL);
    for (int column = 0; column < BOARD_WIDTH; column++)
    {
        printf(" %d", column + 1);
    }
    printf("\n");
    for (int row = 0; row < BOARD_HEIGHT; row++)
    {
        for (int column = 0; column < BOARD_WIDTH; column++)
        {
            printf("+-");
        }
        printf("+\n");
        for (int column = 0; column < BOARD_WIDTH; column++)
        {
            printf("|");
            CALL(Tile_print(board->tiles[row][column]));
        }
        printf("|\n");
    }
    for (int column = 0; column < BOARD_WIDTH; column++)
    {
        printf("+-");
    }
    printf("+\n");
    for (int column = 0; column < BOARD_WIDTH; column++)
    {
        printf(" %d", column + 1);
    }
    printf("\n");
CLEANUP:
    return ret;
}
