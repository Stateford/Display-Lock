/*
    Display Lock
    Copyright (C) 2018 Dimitrios Apostal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "win.h"
#include <process.h>

struct PREVIOUSRECT
{
    int width;
    int height;
    int x;
    int y;
    LONG_PTR previousStyle;
    LONG_PTR currentStyle;
};

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

// checks if window has borders and hides false-positives among UWP instances.
BOOL checkUWP(HWND hwnd)
{
    char className[500];
    GetClassNameA(hwnd, className, sizeof(className));

    WINDOWINFO wInfo;
    ZeroMemory(&wInfo, sizeof(WINDOWINFO));
    if (GetWindowInfo(hwnd, &wInfo))
    {
        if (strcmp(className, "Windows.UI.Core.CoreWindow") == 0 || strcmp(className, "ApplicationFrameWindow") == 0)
        {
            return FALSE;
        }
    }
    return TRUE;
}

// enumerate windows and get current window list
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    // cast lParam to windowList
    WINDOWLIST *win = (WINDOWLIST *)lParam;

    // temp char title
    char title[500];
    // check if window is visible
    if (IsWindowVisible(hwnd))
    {
        // get text and store to title
        GetWindowTextA(hwnd, title, sizeof(title));

        // if title contains more than 1 character
        if (strlen(title) != 0 && strcmp(title, "Display Lock") != 0 && checkWindowTaskbar(hwnd) && checkUWP(hwnd))
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
// checks if the cursor is within the client area of specified windows RECT object
inline BOOL checkClientArea(POINT *cursorPos, RECT *rect)
{
    return (cursorPos->y <= rect->bottom && cursorPos->y >= rect->top) && (cursorPos->x >= rect->left && cursorPos->x <= rect->right);
}

// checks if resize is allowed
inline BOOL checkResizeStyle(HWND activeWindow)
{
    return (GetWindowLongPtr(activeWindow, GWL_STYLE) & WS_SIZEBOX);
}

// toggles borderlessWindow
inline void toggleBorderlessWindow(HWND activeWindow)
{
    // XOR WS_OVERLAPPED, WS_THICKFRAME, WS_SYSMENU, WS_CAPTION
    SetWindowLongPtr(activeWindow, GWL_STYLE, GetWindowLongPtr(activeWindow, GWL_STYLE) ^ WS_OVERLAPPED ^ WS_THICKFRAME ^ WS_SYSMENU ^ WS_CAPTION);
    // XOR WS_EX_WINDOWEDGE
    SetWindowLongPtr(activeWindow, GWL_EXSTYLE, GetWindowLongPtr(activeWindow, GWL_EXSTYLE) ^ WS_EX_WINDOWEDGE);
}

// resizes borderless window
// This prevents a window that has windows styles removed from having incorrect cursor placements by resizing the window the current size
void resizeBorderless(WINDOW activeWindow, PREVIOUSRECT *prev)
{
    GetClientRect(activeWindow.hWnd, &activeWindow.size);
    ClientToScreen(activeWindow.hWnd, (LPPOINT)&activeWindow.size.left);
    ClientToScreen(activeWindow.hWnd, (LPPOINT)&activeWindow.size.right);

    prev->width = activeWindow.size.right - activeWindow.size.left;
    prev->height = activeWindow.size.bottom - activeWindow.size.top;
    prev->x = activeWindow.size.left;
    prev->y = activeWindow.size.top;

    SetWindowPos(activeWindow.hWnd, NULL, prev->x, prev->y, prev->width, prev->height, 0);
}

// enable fullscreen
void enableFullScreen(WINDOW activeWindow, PREVIOUSRECT *prev)
{
    GetClientRect(activeWindow.hWnd, &activeWindow.size);
    ClientToScreen(activeWindow.hWnd, (LPPOINT)&activeWindow.size.left);
    ClientToScreen(activeWindow.hWnd, (LPPOINT)&activeWindow.size.right);

    prev->width = activeWindow.size.right - activeWindow.size.left;
    prev->height = activeWindow.size.bottom - activeWindow.size.top;
    prev->x = activeWindow.size.left;
    prev->y = activeWindow.size.top;

    SetWindowPos(activeWindow.hWnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
}

// disable full screen
inline void disableFullScreen(WINDOW activeWindow, PREVIOUSRECT *prev)
{
    SetWindowPos(activeWindow.hWnd, NULL, prev->x, prev->y, prev->width, prev->height, 0);
}

// check if process is still running
inline BOOL checkProcess(WINDOW activeWindow)
{
    return (GetWindow(activeWindow.hWnd, 0) > 0);
}

// threaded function to lock the cursor to specified window
int CALLBACK cursorLock(void *arguments)
{
    // TODO: reimplement the cursor lock
    ARGS *args = (ARGS *)arguments;
    SETTINGS *settings = args->settings;
    WINDOW selectedWindow = args->selectedWindow;
    BOOL *isRunning = args->clipRunning;
    POINT cursorPos;
    BOOL styleChanged = FALSE;
    PREVIOUSRECT previousRect;

    previousRect.previousStyle = GetWindowLongPtr(selectedWindow.hWnd, GWL_STYLE);

    if (settings->borderless)
    {
        toggleBorderlessWindow(selectedWindow.hWnd);
        if (!settings->fullScreen)
            resizeBorderless(selectedWindow, &previousRect);
    }

    if (settings->fullScreen)
        enableFullScreen(selectedWindow, &previousRect);

    if (settings->minimize)
        ShowWindow(args->hWnd, SW_MINIMIZE);

    if (settings->foreground)
    {
        SetForegroundWindow(selectedWindow.hWnd);
        SetActiveWindow(selectedWindow.hWnd);
    }

    if (checkResizeStyle(selectedWindow.hWnd))
    {
        // TODO: might want to get error message here and check if elevated permissions are required
        SetWindowLongPtr(selectedWindow.hWnd, GWL_STYLE, GetWindowLongPtr(selectedWindow.hWnd, GWL_STYLE) ^ WS_SIZEBOX);
        styleChanged = TRUE;
    }

    previousRect.currentStyle = GetWindowLongPtr(selectedWindow.hWnd, GWL_STYLE);

    while (*isRunning)
    {
        // Get client area
        GetClientRect(selectedWindow.hWnd, &selectedWindow.size);
        ClientToScreen(selectedWindow.hWnd, (LPPOINT)&selectedWindow.size.left);
        ClientToScreen(selectedWindow.hWnd, (LPPOINT)&selectedWindow.size.right);

        // get the active window
        HWND active = GetForegroundWindow();

        GetCursorPos(&cursorPos);

        // Check if the window is closed
        if (!checkProcess(selectedWindow))
        {
            *args->clipRunning = FALSE;

            PostMessage(args->controls.windowView, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_WINDOWS_STOP, 0), 0);
            break;
        }

        // check fi the window style has changed
        if (previousRect.currentStyle != GetWindowLongPtr(selectedWindow.hWnd, GWL_STYLE))
        {
            SetWindowLongPtr(selectedWindow.hWnd, GWL_STYLE, previousRect.currentStyle);

            if (settings->borderless && !settings->fullScreen)
                resizeBorderless(selectedWindow, &previousRect);
            else if (settings->fullScreen)
                enableFullScreen(selectedWindow, &previousRect);
        }

        // if the window is active and the cursor is in the client area clip the cursor to the window
        // check this first to make another check to see if user is clicking on the title bar to move the window around
        if (selectedWindow.hWnd == active && checkClientArea(&cursorPos, &selectedWindow.size))
            ClipCursor(&selectedWindow.size);

        // if the window is active and the user is not clicking (on the title bar)
        // clip the cursor to the window automatically.
        // this will place the cursor in the window
        else if (selectedWindow.hWnd == active && GetAsyncKeyState(VK_LBUTTON) == 0)
            ClipCursor(&selectedWindow.size);

        Sleep(1);
    }

    // if window style was changed, change it back
    if (styleChanged)
        SetWindowLongPtr(selectedWindow.hWnd, GWL_STYLE, previousRect.previousStyle);

    if (settings->borderless)
    {
        toggleBorderlessWindow(selectedWindow.hWnd);
        if (!settings->fullScreen)
            resizeBorderless(selectedWindow, &previousRect);
    }

    if (settings->fullScreen)
        disableFullScreen(selectedWindow, &previousRect);

    ClipCursor(NULL); // release the cursor clip
    _endthreadex(1);  // end the thread
    return 1;
}
