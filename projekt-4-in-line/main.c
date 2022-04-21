#include "game.h"
#include "view.h"

int main()
{
    Game game;
    Game_init(&game);
    
    View view;
    View_init(&view);
    
    Game_run(&game, &view);
    
    return 0;
}
