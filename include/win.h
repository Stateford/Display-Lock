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
#pragma once

#include <Windows.h>
#include "common.h"

typedef struct PREVIOUSRECT PREVIOUSRECT;

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