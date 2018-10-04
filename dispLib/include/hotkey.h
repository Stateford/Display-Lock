#pragma once
#include "common.h"

void initHotkey(HWND hWnd, SETTINGS *settings);
BOOL addHotkey(HWND hWnd, SETTINGS *settings, int msgID, int modifier, unsigned int key);
BOOL removeHotkey(HWND hWnd, SETTINGS *settings, int index);
void cleanupHotkeys(HWND hWnd, SETTINGS *settings);