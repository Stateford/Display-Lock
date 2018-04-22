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

BOOL checkWindowTaskbar(HWND hwnd)
{
    return ((GetWindowLongPtr(hwnd, GWL_EXSTYLE)&WS_EX_WINDOWEDGE) > 0);
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

// checks if the cursor is within the client area of specified windows RECT object
inline BOOL checkClientArea(POINT* cursorPos, RECT* rect)
{
	return (cursorPos->y <= rect->bottom && cursorPos->y >= rect->top) && (cursorPos->x >= rect->left && cursorPos->x <= rect->right);
}

inline BOOL checkResizeStyle(HWND activeWindow)
{
    return (GetWindowLongPtr(activeWindow, GWL_STYLE)&WS_SIZEBOX);
}

inline void borderlessWindow(HWND activeWindow)
{
    SetWindowLongPtr(activeWindow, GWL_STYLE, GetWindowLongPtr(activeWindow, GWL_STYLE)^WS_OVERLAPPED^WS_THICKFRAME^WS_SYSMENU^WS_CAPTION);
    SetWindowLongPtr(activeWindow, GWL_EXSTYLE, GetWindowLongPtr(activeWindow, GWL_EXSTYLE)^WS_EX_WINDOWEDGE);
}

void resizeBorderless(WINDOW activeWindow, PREVIOUSRECT* prev)
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


void fullScreen(WINDOW activeWindow, PREVIOUSRECT *prev)
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

inline void disableFullScreen(WINDOW activeWindow, PREVIOUSRECT *prev)
{
    SetWindowPos(activeWindow.hWnd, NULL, prev->x, prev->y, prev->width, prev->height, 0);
}

inline BOOL checkProcess(WINDOW activeWindow)
{
    return (GetWindow(activeWindow.hWnd, 0) > 0);
}

// threaded function to lock the cursor to specified window
int __stdcall cursorLockEx(void* arguments)
{
    winArgs *args = (winArgs*)arguments;
    WINDOW activeWindow = *args->window;

    HWND currentWindow = activeWindow.hWnd;

	POINT cursorPos;
    
    // keeps track if style was changed
    BOOL styleChanged = FALSE;

    SETTINGS *settings = (SETTINGS*)args->settings;

    // this stores the previous X / Y position, as well as height and width of the window
    PREVIOUSRECT previousrect;

    // TODO: should borderless fullscreen be one setting??
    // borderless window w/o full screen breaks cursor placement
    // on most games...
    if(settings->borderlessWindow)
    {
        borderlessWindow(currentWindow);
        if (!settings->fullScreen)
            resizeBorderless(activeWindow, &previousrect);
    }

    if(settings->fullScreen)
        fullScreen(activeWindow, &previousrect);

    if(settings->foreground)
    {
        SetForegroundWindow(currentWindow);
        SetActiveWindow(currentWindow);
    }

    // TODO: bring to foreground
    // TODO: keep on top (WS_EX_TOP???)

    // if window style has WS_SIZEBOX, remove it with EXCLUSIVE OR (^)
    if(checkResizeStyle(activeWindow.hWnd))
    {
        // TODO: Might want to get error message here and check if elevated permissions are required, and display that to the user
        SetWindowLongPtr(activeWindow.hWnd, GWL_STYLE, GetWindowLongPtr(activeWindow.hWnd, GWL_STYLE)^WS_SIZEBOX);
        styleChanged = TRUE;
    }

    while (*args->active)
    {
        WaitForSingleObject(&args->mutex, INFINITE);    // wait for mutex

        args->mutex = CreateMutex(NULL, FALSE, NULL);

        GetClientRect(activeWindow.hWnd, &activeWindow.size);
        ClientToScreen(activeWindow.hWnd, (LPPOINT)&activeWindow.size.left);
        ClientToScreen(activeWindow.hWnd, (LPPOINT)&activeWindow.size.right);

        HWND active = GetForegroundWindow();

		GetCursorPos(&cursorPos);

        
        // see if process is running
        if(!checkProcess(activeWindow))
        {
            *args->active = FALSE;
            break;
        }
        

		// if the window is active and the cursor is in the client area clip the cursor to the window
		// check this first to make another check to see if user is clicking on the title bar to move the window around
        if (activeWindow.hWnd == active && checkClientArea(&cursorPos, &activeWindow.size))
            ClipCursor(&activeWindow.size);
		
		// if the window is active and the user is not clicking (on the title bar)
		// clip the cursor to the window automatically.
        // this will place the cursor in the window
		else if(activeWindow.hWnd == active && GetAsyncKeyState(VK_LBUTTON) == 0)
			ClipCursor(&activeWindow.size);
		
        ReleaseMutex(&args->mutex);

        Sleep(1);
    }

    // if window style was changed, change it back using the OR (|)
    if(styleChanged)
        SetWindowLongPtr(currentWindow, GWL_STYLE, GetWindowLongPtr(currentWindow, GWL_STYLE)|WS_SIZEBOX);

    if(settings->borderlessWindow)
    {
        borderlessWindow(currentWindow);
        if (!settings->fullScreen)
            resizeBorderless(activeWindow, &previousrect);
    }

    if (settings->fullScreen)
        disableFullScreen(activeWindow, &previousrect);

    ClipCursor(NULL);	// release the cursor clip
    _endthreadex(1);	// end thread_ex
    return 1;
}