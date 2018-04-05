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

void cursorLock(void* arguments)
{
    HANDLE hMessageStop = CreateEvent(NULL, FALSE, FALSE, _T("STOP"));
    HANDLE hMessageEmpty = CreateEvent(NULL, FALSE, TRUE, _T("EMPTY"));
    winArgs *args = (winArgs*)arguments;

	POINT cursorPos;

    while(args->active)
    {
        //DWORD waiting = WaitForSingleObject(hMessageStop, INFINITE);

        

        WaitForSingleObject(&args->mutex, INFINITE);    // wait for mutex


        //GetWindowRect(menu->selectedWindow.hWnd, &menu->selectedWindow.size);
        GetClientRect(args->window->hWnd, &args->window->size);
        ClientToScreen(args->window->hWnd, &args->window->size.left);
        ClientToScreen(args->window->hWnd, &args->window->size.right);
        
		getCurrentMousePos(&cursorPos);

        HWND active = GetForegroundWindow();

        if(args->window->hWnd == active)
        {
            ClipCursor(&args->window->size);
        }

        Sleep(1);
    }
    ClipCursor(NULL);
    _endthread();
}

BOOL checkClientArea(POINT* cursorPos, RECT* rect)
{
	return cursorPos->y <= rect->bottom && cursorPos->y >= rect->top;
}


int __stdcall cursorLockEx(void* arguments)
{
    HANDLE hMessageStop = CreateEvent(NULL, FALSE, FALSE, _T("STOP"));
    HANDLE hMessageEmpty = CreateEvent(NULL, FALSE, TRUE, _T("EMPTY"));
    winArgs *args = (winArgs*)arguments;
    WINDOW activeWindow = *args->window;

	POINT cursorPos;

    while (*args->active)
    {
        //DWORD waiting = WaitForSingleObject(hMessageStop, INFINITE);

        WaitForSingleObject(&args->mutex, INFINITE);    // wait for mutex

        //GetWindowRect(menu->selectedWindow.hWnd, &menu->selectedWindow.size);
        GetClientRect(activeWindow.hWnd, &activeWindow.size);
        ClientToScreen(activeWindow.hWnd, &activeWindow.size.left);
        ClientToScreen(activeWindow.hWnd, &activeWindow.size.right);

        HWND active = GetForegroundWindow();

		getCurrentMousePos(&cursorPos);

        if (activeWindow.hWnd == active && checkClientArea(&cursorPos, &activeWindow.size))
        {
            ClipCursor(&activeWindow.size);
        }

        Sleep(1);
    }
    ClipCursor(NULL);
    _endthreadex(1);
    return 1;
}