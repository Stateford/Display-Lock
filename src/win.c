#include "win.h"

// Get open windows
void openWindows(WINDOWLIST *windows)
{
    windows->count = 0;

    // enumerate through open windows
    EnumWindows(&EnumWindowsProc, (LPARAM)windows);
}

// checks if window is an open window or fullscreen application
BOOL checkWindowTaskbar(HWND hwnd)
{
    // WS_EX_WINDOWEDGE checks if the program is a window
    // WS_POPUP checks if the window is a fullscreen application
    return ((GetWindowLongPtr(hwnd, GWL_EXSTYLE) & WS_EX_WINDOWEDGE) > 0) || ((GetWindowLongPtr(hwnd, GWL_STYLE) & WS_POPUP) > 0);
}


// enumerate windows and get current window list
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    // cast lParam to windowList
    WINDOWLIST* win = (WINDOWLIST*)lParam;

    // temp char title
    char title[500];
    // check if window is visible
    if (IsWindowVisible(hwnd))
    {

        // get text and store to title
        GetWindowTextA(hwnd, title, sizeof(title));

        // if title contains more than 1 character
        if (strlen(title) != 0 && strcmp(title, "Display Lock") != 0 && checkWindowTaskbar(hwnd))
        {
            // get handle
            win->windows[win->count].hWnd = hwnd;
            // copy title
            strcpy_s(win->windows[win->count].title, sizeof(title), title);
            // add to count
            ++win->count;
        }
    }

    return TRUE;
}
