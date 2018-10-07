#pragma once

#include <Windows.h>
#include "common.h"

BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);
BOOL CALLBACK EnumChildProcUWP(HWND, LPARAM);

BOOL checkWindowTaskbar(HWND);

void openWindows(WINDOWLIST*);
void clearWindows(WINDOWLIST*);
void lockFocused(WINDOW*);
BOOL checkClientArea(POINT*, RECT*);
BOOL checkResizeStyle(HWND);
void toggleBorderlessWindow(HWND);
void resizeBorderless(WINDOW, PREVIOUSRECT*);
void enableFullScreen(WINDOW, PREVIOUSRECT*);
void disableFullScreen(WINDOW, PREVIOUSRECT*);

BOOL checkProcess(WINDOW);

int CALLBACK cursorLock(void* arguments);