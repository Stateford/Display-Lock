#pragma once

#include "common.h"

BOOL getVersionString(wchar_t *buffer, int bufferSize);
void settingsShowWindow(SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS *settings, SETTINGS *previousSettings, BOOL running);
void settingsSave(HWND hWnd, SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS settings, SETTINGS *previousSettings);
void settingsCancel(SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS *settings, SETTINGS previousSettings);
void initalizeWindowView(HWND hDlg, MENU *menu, SETTINGS *settings, BOOL *running, WINDOW_VIEW_CONTROLS *windowControls, ARGS *args);
void windowsButtonStart(WINDOW_VIEW_CONTROLS *windowControls, ARGS *args, BOOL *running, int windowSelection);
void windowsButtonStop(MENU menu, WINDOW_VIEW_CONTROLS *windowControls);
void mainWindowInit(HWND hDlg, MAIN_WINDOW_CONTROLS *mainWindowControls);
void invokeReadSettings(SETTINGS *settings);
void shutDown(SETTINGS settings);