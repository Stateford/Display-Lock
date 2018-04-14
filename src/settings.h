#pragma once
#include "header.h"

// pack the struct to easily read from file
#pragma pack(push, 1)
typedef struct SETTINGS
{
    char header[5];
	BOOL minimize;
    BOOL borderlessWindow;
    BOOL fullScreen;
    BYTE HOTKEY;
    UINT8 hotKeyCount;
    BYTE HOTKEY_MODIFIERS[3];
} SETTINGS;
#pragma pack(pop)

void initalizeSettings(HWND hDlg, SETTINGS* settings);
void updateSettings(HWND hDlg, SETTINGS *settings);

void writeSettings(SETTINGS settings);
void readSettings(SETTINGS *settings);