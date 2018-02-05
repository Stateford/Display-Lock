#include "win.h"

void openWindows(WINDOWLIST *windows)
{
    EnumWindows(&EnumWindowsProc, (LPARAM)windows);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    WINDOWLIST* win = (WINDOWLIST*)lParam;
    char title[120];
    if (IsWindowVisible(hwnd))
    {
        GetWindowText(hwnd, title, sizeof(title));
        if (strlen(title) != 0)
        {
            strcpy_s(win->windows[win->count].title, sizeof(title), title);
            ++win->count;
        }
    }
    //GetClassName(hwnd, class_name, sizeof(class_name));

    return TRUE;
}