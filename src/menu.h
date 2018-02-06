#ifndef MENU_H
#define MENU_H

#include "win.h"

typedef enum MENU
{
    SPLASH,
    DEFAULT,
    ACTIVE
} MENU;

typedef struct Menu
{
    WINDOWLIST windows;
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


void start();
void draw(Menu*);
void display(Menu*);

#endif