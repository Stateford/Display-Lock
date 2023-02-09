/*
    Display Lock
    Copyright (C) 2018 Dimitrios Apostal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "common.h"
#include <Windows.h>

void initalizeSettings(HWND hDlg, SETTINGS_VIEW_CONTROLS *settingsControls);
void defaultSettings(SETTINGS *settings, wchar_t *versionStr);
BOOL checkVersion(SETTINGS *settings, wchar_t *versionStr);
BOOL findPath(wchar_t *path, const size_t size);
BOOL createDirectory(wchar_t *outPath, const size_t size);
BOOL readSettings(SETTINGS *settings, wchar_t *versionStr, wchar_t *path);
BOOL writeSettings(SETTINGS settings, wchar_t *path);