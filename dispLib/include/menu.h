#pragma once

#include <Windows.h>
#include "common.h"

void initMenuObj(MENU *menu);
void updateComboBox(HWND control, WINDOWLIST *windows, void(*callback)(WINDOWLIST*));
BOOL startThread(HANDLE *thread, int(*callback)(void* parameters), void *args);
void closeThread(HANDLE thread, volatile BOOL *status);