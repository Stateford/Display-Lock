#pragma once

#include "common.h"
#include <Windows.h>

void initalizeSettings(HWND hDlg, SETTINGS_VIEW_CONTROLS *settingsControls);
void setSettingsDlg(HWND hDlg, SETTINGS settings);
void defaultSettings(SETTINGS *settings, wchar_t *versionStr);
BOOL checkVersion(SETTINGS *settings, wchar_t *versionStr);
BOOL writeSettings(SETTINGS settings);
BOOL readSettings(SETTINGS *settings, wchar_t *versionStr);

BOOL findPath(wchar_t *path);