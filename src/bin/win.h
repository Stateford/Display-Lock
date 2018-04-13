#ifndef WIN_H
#define WIN_H

#include <Windows.h>
//#include "menu.h"



typedef struct WINDOW
{
    char title[500];
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


typedef struct Menu
{
    WINDOWLIST windows;
    WINDOW selectedWindow;
    BOOL active;
    int currentMenu;
    int currentSelection;
    BOOL live;
    DWORD mode;
    INPUT_RECORD event;
} Menu;

typedef enum MAINMENU
{
    MAIN,
    WINDOWS,
    DEFAULT,
    ACTIVE
} MAINMENU;

BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);

void openWindows(WINDOWLIST*);
void clearWindows(WINDOWLIST*);
void getCurrentMousePos(POINT*);

void lockFocused(WINDOW*);

void cursorLock(void*);

BOOL checkClientArea(POINT*, RECT*);

int __stdcall cursorLockEx(void* arguments);

#endif