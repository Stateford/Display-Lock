#pragma once

#include "common.h"
#include <Windows.h>


typedef struct SETTINGS
{
    int version;
    BOOL minimize;
    BOOL foreground;
    BOOL borderless;
    BOOL fullScreen;
} SETTINGS;

void resetSettings(HWND hDlg);