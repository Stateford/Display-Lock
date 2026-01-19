#pragma once

#include <Windows.h>
#include "common.h"

typedef struct EnumWindowsProcPIDArgs
{
    DWORD pid;
    HWND hwnd;
} EnumWindowsProcPIDArgs;

BOOL initApplicationView(HWND hDlg, APPLICATION_VIEW_CONTROLS *applicationControls);
BOOL readApplicationList(APPLICATION_LIST *applicationList, const wchar_t *path);
BOOL writeApplicationList(APPLICATION_LIST *applicationList, const wchar_t *path);
BOOL addApplication(APPLICATION_LIST *applicationList, APPLICATION_SETTINGS application);
BOOL removeApplication(APPLICATION_LIST *applicationList, int index);
BOOL initApplicationList(APPLICATION_LIST *applicationList);
BOOL closeApplicationList(APPLICATION_LIST *applicationList);
BOOL createApplicationDirectory(wchar_t *outPath);
BOOL createApplicationSettings(const wchar_t *appPath, APPLICATION_SETTINGS *application);

BOOL startApplicationThread(HANDLE *thread, int (*callback)(void *parameters), void *args);
void closeApplicationThread(HANDLE *thread, volatile BOOL *status);
DWORD getPidFromName(const wchar_t *name);

BOOL CALLBACK EnumWindowsProcPID(HWND hwnd, LPARAM lParam);

int CALLBACK cursorLockApplications(void *parameters);