#ifndef WIN_H
#define WIN_H

#include <Windows.h>
//#include "menu.h"



typedef struct WINDOW
{
    char title[120];
    int x;
    int y;
    RECT size;
    BOOL selected;
    HWND hWnd;
} WINDOW;

typedef struct WINDOWLIST
{
    int count;
    WINDOW windows[25];
} WINDOWLIST;


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

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

void openWindows(WINDOWLIST *windows);

void getCurrentMousePos(POINT*);

void lockFocused(WINDOW*);

void cursorLock(Menu*);

#endif