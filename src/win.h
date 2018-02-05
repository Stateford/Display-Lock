#ifndef WIN_H
#define WIN_H

#include <Windows.h>
#include <string.h>

typedef struct WINDOW
{
    char title[120];
    int x;
    int y;
} WINDOW;

typedef struct WINDOWLIST
{
    int count;
    WINDOW windows[20];
} WINDOWLIST;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

void openWindows(WINDOWLIST *windows);


#endif