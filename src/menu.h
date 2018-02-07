#ifndef MENU_H
#define MENU_H

#include "win.h"

typedef enum MAINMENU
{
    MAIN,
    WINDOWS,
    DEFAULT,
    ACTIVE
} MAINMENU;
/*
typedef struct Main
{
    int maxSelections;
    char options[5][30];
} Main;

typedef struct Menu
{
    Main mainMenu;
    WINDOWLIST windows;
    WINDOW selectedWindow;
    BOOL active;
    int currentMenu;
    int currentSelection;
    BOOL live;
    BOOL consoleWindowIsFocused;
    DWORD mode;
    INPUT_RECORD event;
    HANDLE hstdin;
    HANDLE hConsole;
} Menu;
*/

void start();
void initMenu(Menu*);

void redraw(Menu*);
void drawWindows(Menu*);
void drawMain(Menu*);
void display(Menu*);

#endif