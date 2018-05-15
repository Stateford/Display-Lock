#pragma once
#include "header.h"
#include "bin/hotkey.h"



// pack the struct to easily read from file
// TODO: unpack struct for compatibility
#pragma pack(push, 1)
typedef struct SETTINGS
{
    char header[6];
	BOOL minimize;
    BOOL borderlessWindow;
    BOOL fullScreen;
    BOOL foreground;
    HOTKEY hotkeys[20]; // this is probably too much
                        // TODO: find out how many options will be available
} SETTINGS;
#pragma pack(pop)


// TODO: create listener for hotkeys

void initalizeSettings(HWND hDlg, SETTINGS* settings);
void updateSettings(HWND hDlg, SETTINGS *settings);

void writeSettings(SETTINGS settings);
void readSettings(SETTINGS *settings);

BOOL checkHeader(SETTINGS *settings);

void defaultSettings(SETTINGS *settings);