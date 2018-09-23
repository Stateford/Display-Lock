#pragma once

#include <Windows.h>
#include <shellapi.h>
#include "common.h"

typedef struct msgData msgData;

struct msgData
{
    UINT msg;
    MENU *menu;
};

void notifyInit(HWND hWnd, NOTIFYICONDATA *notify);

void showMainWindow(HWND hWnd, NOTIFYICONDATA *notify);

void showContext(HWND hWnd, HMENU menu);

void notifyChildWindows(HWND hWnd, UINT msg);

BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam);