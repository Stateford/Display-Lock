#include "win.h"
#include <string.h>
#include <process.h>
#include "../header.h"


void openWindows(WINDOWLIST *windows)
{
    //mutex = CreateMutex(NULL, FALSE, NULL);

    windows->count = 0;

    EnumWindows(&EnumWindowsProc, (LPARAM)windows);

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
        if (strlen(title) != 0)
        {
            // get rectangle
            //GetWindowRect(hwnd, &win->windows[win->count].size);
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

// checks if the cursor is within the client area of specified windows RECT object
BOOL checkClientArea(POINT* cursorPos, RECT* rect)
{
	return (cursorPos->y <= rect->bottom && cursorPos->y >= rect->top) && (cursorPos->x >= rect->left && cursorPos->x <= rect->right);
}

// threaded function to lock the cursor to specified window
int __stdcall cursorLockEx(void* arguments)
{
    HANDLE hMessageStop = CreateEvent(NULL, FALSE, FALSE, _T("STOP"));
    HANDLE hMessageEmpty = CreateEvent(NULL, FALSE, TRUE, _T("EMPTY"));
    winArgs *args = (winArgs*)arguments;
    WINDOW activeWindow = *args->window;

	POINT cursorPos;

    while (*args->active)
    {
        WaitForSingleObject(&args->mutex, INFINITE);    // wait for mutex

        GetClientRect(activeWindow.hWnd, &activeWindow.size);
        ClientToScreen(activeWindow.hWnd, &activeWindow.size.left);
        ClientToScreen(activeWindow.hWnd, &activeWindow.size.right);

        HWND active = GetForegroundWindow();

		GetCursorPos(&cursorPos);

		// if the window is active and the cursor is in the client area clip the cursor to the window
		// check this first to make another check to see if user is clicking on the title bar to move the window around
        if (activeWindow.hWnd == active && checkClientArea(&cursorPos, &activeWindow.size))
            ClipCursor(&activeWindow.size);
		
		// if the window is active and the user is not clicking (on the title bar)
		// clip the cursor to the window
		else if(activeWindow.hWnd == active && GetAsyncKeyState(VK_LBUTTON) == 0)
			ClipCursor(&activeWindow.size);
		
        Sleep(1);
    }

    ClipCursor(NULL);	// release the cursor clip
    _endthreadex(1);	// end thread_ex
    return 1;
}