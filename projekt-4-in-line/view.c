#include "view.h"

#include "ret.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int View_init(View *view)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(view != NULL);
    view->error[0] = '\0';
CLEANUP:
    return ret;
}

static int translateTile(int tile, char* printable)
{
    int ret = RET_SUCCESS;
    switch (tile)
    {
    case 0:
        *printable = ' ';
        break;
    case 1:
        *printable = 'X';
        break;
    case 2:
        *printable = 'O';
        break;
    default:
        ret = RET_VIEW_VISIT_WRONG_DATA;
        goto CLEANUP;
    }
CLEANUP:
    return ret;
}

static int printNumbersLine(int board_width)
{
    for (int column = 0; column < board_width; column++)
    {
        printf(" %d", column + 1);
    }
    printf("\n");
    return RET_SUCCESS;
}

static int printDividerLine(int board_width)
{
    for (int column = 0; column < board_width; column++)
    {
        printf("+-");
    }
    printf("+\n");
    return RET_SUCCESS;
}

static int printBoard(int board_height, int board_width, int data[board_height][board_width])
{
    int ret = RET_SUCCESS;

    CALL(printNumbersLine(board_width));
    for (int row = 0; row < board_height; row++)
    {
        CALL(printDividerLine(board_width));
        for (int column = 0; column < board_width; column++)
        {
            char printable = ' ';
            CALL(translateTile(data[row][column], &printable));
            printf("|%c", printable);
        }
        printf("|\n");
    }
    CALL(printDividerLine(board_width));
    CALL(printNumbersLine(board_width));
CLEANUP:
    return ret;
}

int View_visit(const View* view, int board_height, int board_width, int data[board_height][board_width])
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(view != NULL);
    
    printf("\x1B[2J\x1B[1;1H"); // clear screen
    if (view->error[0] != '\0')
    {
        printf("\x1B[97;101mERROR: %s\x1B[0m", view->error);
    }
    if (view->state == View_VICTORY)
    {
        printf("\x1B[30;102m%s wins\n\x1B[0m", view->player_name);
    }
    else if (view->state == View_DRAW)
    {
        printf("\x1B[30;47mDraw.\n\x1B[0m");
    }
    else if (view->state == View_PLAYING)
    {
        char printable = ' ';
        CALL(translateTile(view->player_token, &printable));
        printf("Move of %s - insert token %c\n", view->player_name, printable);
    }
    else
    {
        printf("\x1B[30;47mGame ended.\n\x1B[0m");
    }
    CALL(printBoard(board_height, board_width, data));
    
CLEANUP:
    return ret;
}

int View_setError(View *view, const char *error)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(view != NULL);
    CHECK(strlen(error) < sizeof (view->error), RET_VIEW_SET_ERROR_TOO_LONG);
    
    strcpy(view->error, error);
    
CLEANUP:
    return ret;
}

int View_setState(View *view, View_State state)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(view != NULL);
    
    view->state = state;
    
CLEANUP:
    return ret;
}

int View_setPlayerName(View *view, const char *player_name)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(view != NULL);
    CHECK(strlen(player_name) < sizeof (view->player_name), RET_VIEW_SET_ERROR_TOO_LONG);
    
    strcpy(view->player_name, player_name);
    
    
CLEANUP:
    return ret;
}

int View_setPlayerToken(View *view, int token)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(view != NULL);
    
    view->player_token = token;
    
CLEANUP:
    return ret;
}

int View_getInput(View *view, char* action)
{
    int ret = RET_SUCCESS;
    CHECK_ASSERT(view != NULL);
    
    printf("Please press a number 1÷7\n");
    char input = getchar();
    while (isspace(input)) input = getchar();
    *action = input;
    
CLEANUP:
    return ret;
}
