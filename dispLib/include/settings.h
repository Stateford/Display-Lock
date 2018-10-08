#pragma once

#include "common.h"
#include <Windows.h>

void initalizeSettings(HWND hDlg, SETTINGS_VIEW_CONTROLS *settingsControls);
void setSettingsDlg(HWND hDlg, SETTINGS settings);
void defaultSettings(SETTINGS *settings, wchar_t *versionStr);
BOOL checkVersion(SETTINGS *settings, wchar_t *versionStr);
BOOL findPath(wchar_t *path);
BOOL createDirectory(wchar_t *outPath);
BOOL readSettings(SETTINGS *settings, wchar_t *versionStr, wchar_t *path);
BOOL writeSettings(SETTINGS settings, wchar_t *path);