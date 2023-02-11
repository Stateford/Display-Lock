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

BOOL getVersionString(wchar_t* buffer, int bufferSize);
BOOL getVersion(VERSION *version);
void settingsShowWindow(SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS *settings, SETTINGS *previousSettings, BOOL running);
void settingsSave(HWND hWnd, SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS settings, SETTINGS *previousSettings);
void settingsCancel(SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS *settings, SETTINGS previousSettings);
void initalizeWindowView(HWND hDlg, MENU *menu, SETTINGS *settings, BOOL *running, WINDOW_VIEW_CONTROLS *windowControls, ARGS *args);
void windowsButtonStart(WINDOW_VIEW_CONTROLS *windowControls, ARGS *args, BOOL *running, int windowSelection);
void windowsButtonStop(MENU menu, WINDOW_VIEW_CONTROLS *windowControls);
void mainWindowInit(HWND hDlg, MAIN_WINDOW_CONTROLS *mainWindowControls);
void invokeReadSettings(SETTINGS *settings);
void shutDown(SETTINGS settings);