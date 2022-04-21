#include "game.h"

#include "ret.h"

#include <stdlib.h>
#include <stdio.h>

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
        if (ret == RET_BOARD_MAKE_MOVE_COLUMN_FULL)
        {
            sprintf(game->error, "Column %d full\n", column);
            ret = RET_SUCCESS; // error was handler
        }
        else sprintf(game->error, "unknown error: %d\n", ret);
        
        goto CLEANUP;
    }
    
    ret = Game_checkWinCondition(game);
    if (ret != RET_SUCCESS)
    {
        sprintf(game->error, "unknown error: %d\n", ret);
        goto CLEANUP;
    }
    
    if (game->state == Game_PLAYING) CALL(Game_nextPlayer(game));

CLEANUP:
    return ret;
}

int Game_accept(const Game* game, View* view)
{
    int ret = RET_SUCCESS;
    CALL(View_setError(view, game->error));
    CALL(View_setPlayerName(view, game->current_player->name));
    CALL(View_setPlayerToken(view, game->current_player->token));
    switch (game->state)
    {
    case Game_DRAW:
        CALL(View_setState(view, View_DRAW));
        break;
    case Game_PLAYING:
        CALL(View_setState(view, View_PLAYING));
        break;
    case Game_VICTORY:
        CALL(View_setState(view, View_VICTORY));
        break;
    case Game_INTERRUPTED:
        CALL(View_setState(view, View_GAME_ENDED));
        break;
    }

CLEANUP:
    return ret;
}

int Game_run(Game *game, View* view)
{
    int ret;
    CHECK_ASSERT(game != NULL);
    while (true)
    {
        Game_accept(game, view);
        Board_accept(&game->board, view);
        char input;
        CALL(View_getInput(view, &input));
        
        if (input == 'q') break;
        if (input >= '1' && input <= '7') CALL(Game_step(game, input));
        
        if (game->state != Game_PLAYING) break;
    }
    Game_accept(game, view);
    Board_accept(&game->board, view);
CLEANUP:
    return ret;
}
