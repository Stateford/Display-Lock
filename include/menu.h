#pragma once

#include <Windows.h>
#include "common.h"

void updateComboBox(HWND control, WINDOWLIST *windows, void(*callback)(WINDOWLIST*));
void closeThread(HANDLE thread, volatile BOOL *status);