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

#include <Windows.h>
#include "resource.h"

#define WINDOW_VIEW 0
#define SETTINGS_VIEW 1
#define APPLICATION_VIEW 2

#define APPLICATION_MUTEX_NAME L"DLockApplicationMutex"

// Magic number constants
#define MAX_WINDOW_COUNT 35
#define MAX_TITLE_LENGTH 500
#define MAX_CLASS_NAME 500

// custom messages

#define NOTIFY_MSG (WM_USER + 0x1)
#define APP_SETTINGS_INIT (WM_USER + 0x2)

#define NOTIFY_SETTINGS_CHANGED (NOTIFY_MSG + 0x1)

// RESERVED MESSAGES 2000-2100
#define SUBMENU_ITEM_BASE 2000

typedef struct MAIN_WINDOW_CONTROLS MAIN_WINDOW_CONTROLS;
typedef struct WINDOWLIST WINDOWLIST;
typedef struct WINDOW_VIEW_CONTROLS WINDOW_VIEW_CONTROLS;
typedef struct SETTINGS_VIEW_CONTROLS SETTINGS_VIEW_CONTROLS;
typedef struct MENU MENU;
typedef struct SETTINGS SETTINGS;
typedef struct ARGS ARGS;
typedef struct HOTKEY HOTKEY;
typedef struct WINDOW WINDOW;
typedef union VERSION VERSION;
typedef struct APPLICATION_VIEW_CONTROLS APPLICATION_VIEW_CONTROLS;
typedef struct APPLICATION_SETTINGS APPLICATION_SETTINGS;
typedef struct APPLICATION_LIST APPLICATION_LIST;
typedef struct APPLICATION_ARGS APPLICATION_ARGS;

struct APPLICATION_VIEW_CONTROLS
{
    HWND addButton;
    HWND removeButton;
    HWND settingsButton;
    HWND listView;

    BOOL *runningClip;
    HANDLE clipThread;
};

struct APPLICATION_SETTINGS
{
    wchar_t application_name[MAX_PATH];
    wchar_t application_path[MAX_PATH];
    BOOL fullscreen;
    BOOL borderless;
    BOOL enabled;
};

struct APPLICATION_LIST
{
    int count;
    APPLICATION_SETTINGS *applications;
};

struct MAIN_WINDOW_CONTROLS
{
    HWND tabCtrl;
    HWND windowView;
    HWND settingsView;
    HWND applicationView;
};

struct WINDOW
{
    char title[MAX_TITLE_LENGTH];
    int x;
    int y;
    RECT size;
    BOOL selected;
    HWND hWnd;
};

struct WINDOWLIST
{
    int count;
    WINDOW windows[MAX_WINDOW_COUNT];
};

struct WINDOW_VIEW_CONTROLS
{
    HWND comboBox;
    HWND startButton;
    HWND stopButton;
    WINDOWLIST windows;

    BOOL *runningClip;
    HANDLE clipThread;
};

struct SETTINGS_VIEW_CONTROLS
{
    BOOL settingsChanged;

    HWND minimize;
    HWND foreground;
    HWND borderless;
    HWND fullScreen;
    HWND hotkey;
    HWND checkForUpdatesStartup;
};

struct MENU
{
    void (*closeThread)(HANDLE *thread, volatile BOOL *status);
    void (*updateComboBox)(HWND control, WINDOWLIST *windows, void (*callback)(WINDOWLIST *));
    BOOL (*startThread)
    (HANDLE *thread, int (*callback)(void *parameters), void *args);
};

struct SETTINGS
{
    char header[6];
    int version;
    BOOL minimize;
    BOOL foreground;
    BOOL borderless;
    BOOL fullScreen;
    BOOL checkUpdateStartup;
};

struct ARGS
{
    SETTINGS *settings;
    volatile BOOL *clipRunning;
    WINDOW selectedWindow;
    HWND hWnd;
    MAIN_WINDOW_CONTROLS controls;
};

struct APPLICATION_ARGS
{
    APPLICATION_LIST *applicationList;
    volatile BOOL *clipRunning;
    HANDLE mutex;
};

union VERSION
{
    struct _version
    {
        unsigned int major;
        unsigned int minor;
        unsigned int patch;
    } version;

    unsigned int verArr[3];
};