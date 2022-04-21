#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"

typedef enum
{
    Game_PLAYING,
    Game_VICTORY,
    Game_DRAW,
    Game_INTERRUPTED,
} Game_State;

typedef struct
{
    Board board;
    Player player1;
    Player player2;
    Player* current_player;
    Game_State state;
    char error[100];
} Game;

int Game_init(Game* game);

int Game_nextPlayer(Game* game);

int Game_makeMove(Game *game, int column);

int Game_checkWinCondition(Game *game);

int Game_step(Game *game, char input);

int Game_run(Game *game);

#endif // GAME_H
