#pragma once

#include "common.h"
#include <Windows.h>

void initalizeSettings(HWND hDlg, SETTINGS_VIEW_CONTROLS *settingsControls, SETTINGS *settings);
void setSettingsDlg(HWND hDlg, SETTINGS settings);
void defaultSettings(SETTINGS *settings);
BOOL checkVersion(SETTINGS *settings);
BOOL writeSettings(SETTINGS *settings);
BOOL readSettings(SETTINGS *settings);