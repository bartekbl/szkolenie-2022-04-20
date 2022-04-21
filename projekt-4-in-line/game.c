#include "game.h"

#include "ret.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int Game_init(Game* game)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(game != NULL);
    CALL(Board_init(&game->board));
    game->current_player = &game->player1;
    game->winner = Tile_EMPTY;
    game->player1 = (Player) { "Adam", Tile_PLAYER_X };
    game->player2 = (Player) { "Eve", Tile_PLAYER_O };
CLEANUP:
    return ret;
}

int Game_print(const Game* game)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(game != NULL);
    const Board* board = &game->board;
    CHECK_ASSERT(board != NULL);
    
    printf("\033[2J\033[1;1H");
    if (game->error[0] != '\0') printf("ERROR: %s", game->error);
    
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

int Game_nextPlayer(Game *game)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(game != NULL);
    if (game->current_player == &game->player1) game->current_player = &game->player2;
    else game->current_player = &game->player1;
CLEANUP:
    return ret;
}

int Game_makeMove(Game *game, int column)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(game != NULL);
    CALL(Board_makeMove(&game->board, column, game->current_player->token));
CLEANUP:
    return ret;
}

int Game_checkWinCondition(Game *game)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(game != NULL);
    CALL(Board_checkWinCondition(&game->board, &game->winner));
CLEANUP:
    return ret;
}

int Game_step(Game *game, char input)
{
    game->error[0] = '\0';
    
    int column = input - '0'
            ;
    int ret = Game_makeMove(game, column);
    if (ret != RET_SUCCESS)
    {
        if (ret == RET_BOARD_MAKE_MOVE_COLUMN_FULL) sprintf(game->error, "Column %d full\n", column);
        else sprintf(game->error, "unknown error: %d\n", ret);
        
        goto CLEANUP;
    }
    
    ret = Game_checkWinCondition(&game->board);
    if (ret != RET_SUCCESS)
    {
        sprintf(game->error, "unknown error: %d\n", ret);
        goto CLEANUP;
    }
    
    Game_nextPlayer(game);
CLEANUP:
    return ret;
}

int Game_run(Game *game)
{
    while (1)
    {
        Game_print(game);
        printf("Please press a number 1÷7\n");
        char input = getchar();
        while (isspace(input)) input = getchar();
        if (input < '1' || input > '7') continue;
        if (input == 'q') break;
        
        Game_step(game, input);
        
        if (game->winner != Tile_EMPTY) break;
    }
    Game_print(game);
    switch (game->winner)
    {
    case Tile_PLAYER_O:
        printf("Player O wins\n");
        break;
    case Tile_PLAYER_X:
        printf("Player X wins\n");
        break;
    default:
        printf("Game ended\n");
        break;
    }
}
