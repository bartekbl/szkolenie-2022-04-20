#ifndef VIEW_H
#define VIEW_H

typedef enum
{
    View_PLAYING,
    View_VICTORY,
    View_DRAW,
    View_GAME_ENDED,
} View_State;

typedef struct
{
    char error[100];
    char message[100];
    char player_name[100];
    int player_token;
    View_State state;
} View;

int View_init(View* view);

#define RET_VIEW_VISIT_WRONG_DATA 1005
int View_visit(const View* view, int board_height, int board_width, int data[6][7]);

#define RET_VIEW_SET_ERROR_TOO_LONG 1005
int View_setError(View* view, const char* error);

int View_setState(View* view, View_State state);

int View_setPlayerName(View* view, const char* player_name);

int View_setPlayerToken(View* view, int token);

int View_getInput(View *view, char* action);

#endif // VIEW_H
