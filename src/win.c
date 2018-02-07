#include "win.h"
#include <string.h>
#include <process.h>


void openWindows(WINDOWLIST *windows)
{
    EnumWindows(&EnumWindowsProc, (LPARAM)windows);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    // cast lParam to windowList
    WINDOWLIST* win = (WINDOWLIST*)lParam;

    // temp char title
    char title[120];

    // check if window is visible
    if (IsWindowVisible(hwnd))
    {
        // get text and store to title
        GetWindowText(hwnd, title, sizeof(title));

        // if title contains more than 1 character
        if (strlen(title) != 0)
        {
            // get rectangle
            GetWindowRect(hwnd, &win->windows[win->count].size);
            // get handle
            win->windows[win->count].hWnd = hwnd;
            // copy title
            strcpy_s(win->windows[win->count].title, sizeof(title), title);
            // add to count
            ++win->count;
        }
    }
    //GetClassName(hwnd, class_name, sizeof(class_name));

    return TRUE;
}

void getCurrentMousePos(POINT *cursor)
{
    GetCursorPos(cursor);
}

void lockFocused(WINDOW *win)
{
    while (1)
    {
        RECT foo;

        HWND active = GetForegroundWindow();

        GetWindowRect(active, &foo);
        ClipCursor(&foo);

        Sleep(500);
    }
}

void cursorLock(Menu *menu)
{
    while(menu->active)
    {
        GetWindowRect(menu->selectedWindow.hWnd, &menu->selectedWindow.size);
        HWND active = GetForegroundWindow();

        if(menu->selectedWindow.hWnd == active)
        {
            ClipCursor(&menu->selectedWindow.size);
        }
        Sleep(500);
    }
    _endthread();
}