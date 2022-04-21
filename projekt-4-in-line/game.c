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
    game->player1 = (Player) { "Adam", Tile_PLAYER_X };
    game->player2 = (Player) { "Eve", Tile_PLAYER_O };
    game->state = Game_PLAYING;
    game->error[0] = '\0';
CLEANUP:
    return ret;
}

static int translateTile(Tile tile, char* printable)
{
    int ret = RET_SUCCESS;
    switch (tile)
    {
    case Tile_EMPTY:
        *printable = ' ';
        break;
    case Tile_PLAYER_X:
        *printable = 'X';
        break;
    case Tile_PLAYER_O:
        *printable = 'O';
        break;
    default:
        assert(0);
        ret = RET_INTERNAL_ERROR;
        goto CLEANUP;
    }
CLEANUP:
    return ret;
}

static int printNumbersLine()
{
    for (int column = 0; column < BOARD_WIDTH; column++)
    {
        printf(" %d", column + 1);
    }
    printf("\n");
    return RET_SUCCESS;
}

static int printDividerLine()
{
    for (int column = 0; column < BOARD_WIDTH; column++)
    {
        printf("+-");
    }
    printf("+\n");
    return RET_SUCCESS;
}

static int printBoard(const Board* board)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(board != NULL);

    CALL(printNumbersLine());
    for (int row = 0; row < BOARD_HEIGHT; row++)
    {
        CALL(printDividerLine());
        for (int column = 0; column < BOARD_WIDTH; column++)
        {
            char printable = ' ';
            CALL(translateTile(board->tiles[row][column], &printable));
            printf("|%c", printable);
        }
        printf("|\n");
    }
    CALL(printDividerLine());
    CALL(printNumbersLine());
CLEANUP:
    return ret;
}

int Game_print(const Game* game)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(game != NULL);
    
    printf("\x1B[2J\x1B[1;1H"); // clear screen
    if (game->error[0] != '\0')
    {
        printf("\x1B[97;101mERROR: %s\x1B[0m", game->error);
    }
    if (game->state == Game_VICTORY)
    {
        printf("\x1B[30;102m%s wins\n\x1B[0m", game->current_player->name);
    }
    else if (game->state == Game_DRAW)
    {
        printf("\x1B[30;47mDraw.\n\x1B[0m");
    }
    else if (game->state == Game_PLAYING)
    {
        char printable = ' ';
        CALL(translateTile(game->current_player->token, &printable));
        printf("Move of %s - insert token %c\n", game->current_player->name, printable);
    }
    else
    {
        printf("\x1B[30;47mGame ended.\n\x1B[0m");
        ret = RET_GAME_PRINT_GAME_ENDED;
        goto CLEANUP;
    }
    CALL(printBoard(&game->board));
    
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
    Tile winner = Tile_EMPTY;
    ret = Board_checkWinCondition(&game->board, &winner);
    if      (ret == RET_CHECK_WIN_CONDITION_DRAW)
    {
        game->state = Game_DRAW;
        ret = RET_SUCCESS; // the error was serviced, don't propagate it up
    }
    else if (ret != RET_SUCCESS)                  goto CLEANUP;
    if (winner != Tile_EMPTY)
    {
        CHECK_ASSERT(winner == game->current_player->token);
        game->state = Game_VICTORY;
    }
CLEANUP:
    return ret;
}

int Game_step(Game *game, char input)
{
    game->error[0] = '\0';
    
    int column = input - '0';
    int ret = Game_makeMove(game, column);
    if (ret != RET_SUCCESS)
    {
        if (ret == RET_BOARD_MAKE_MOVE_COLUMN_FULL) sprintf(game->error, "Column %d full\n", column);
        else sprintf(game->error, "unknown error: %d\n", ret);
        
        goto CLEANUP;
    }
    
    ret = Game_checkWinCondition(game);
    if (ret != RET_SUCCESS)
    {
        sprintf(game->error, "unknown error: %d\n", ret);
        goto CLEANUP;
    }
CLEANUP:
    return ret;
}

int Game_run(Game *game)
{
    int ret;
    CHECK_ASSERT(game != NULL);
    while (true)
    {
        Game_print(game);
        printf("Please press a number 1÷7\n");
        char input = getchar();
        while (isspace(input)) input = getchar();
        if (input == 'q') break;
        if (input >= '1' && input <= '7') Game_step(game, input);
        
        if (game->state != Game_PLAYING) break;
        CALL(Game_nextPlayer(game));
    }
    Game_print(game);
CLEANUP:
    return ret;
}
