#ifndef WIN_H
#define WIN_H

#include <Windows.h>

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

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

void openWindows(WINDOWLIST *windows);

void getCurrentMousePos(POINT*);

void lockFocused(WINDOW*);

#endif