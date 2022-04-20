#include "board.h"

#include "ret.h"

int main()
{
    Board board;
    Board_init(&board);
    Board_print(&board);
    Tile current_player = Tile_PLAYER_O;
    Tile winner = Tile_EMPTY;
    while (1)
    {
        char input = getchar();
        if (input < '1' || input > '7')
        {
            printf("Please press a number 1÷7\n");
            continue;
        }
        int ret = Board_makeMove(&board, input - '0', current_player);
        if (ret != RET_SUCCESS)
        {
            if (ret == RET_BOARD_MAKE_MOVE_COLUMN_FULL) printf("Column full\n");
            else printf("unknown error: %d\n", ret);
            
            continue;
        }
        
        ret = Board_checkWinCondition(&board, &winner);
        if (ret != RET_SUCCESS)
        {
            printf("unknown error: %d\n", ret);
            continue;
        }
        
        Board_print(&board);
        if (winner != Tile_EMPTY) break;
        if (current_player == Tile_PLAYER_X) current_player = Tile_PLAYER_O;
        else current_player = Tile_PLAYER_X;
    }
    switch (winner)
    {
    case Tile_PLAYER_O:
        printf("Player O wins\n");
        break;
    case Tile_PLAYER_X:
        printf("Player X wins\n");
        break;
    default:
        printf("Internal error\n");
        break;
    }
    return 0;
}
