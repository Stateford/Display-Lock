#pragma once

#include <Windows.h>
#include <shellapi.h>
#include "common.h"

void notifyInit(HWND hWnd, NOTIFYICONDATA *notify);
void showMainWindow(HWND hWnd, NOTIFYICONDATA *notify);
void showContext(HWND hWnd, HMENU menu, WINDOWLIST *windows);
void notifyChildWindows(HWND hWnd, UINT msg);
void updateContextMenu(HMENU *submenu, WINDOWLIST *windows);
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam);
