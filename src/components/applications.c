#include "applications.h"
#include <stdio.h>
#include <Windows.h>
#include <ShlObj.h>
#include <shtypes.h>
#include "Shlwapi.h"
#include <iphlpapi.h>
#include <TlHelp32.h>

BOOL initApplicationView(HWND hDlg, APPLICATION_VIEW_CONTROLS *applicationControls)
{
    applicationControls->listView = GetDlgItem(hDlg, IDC_LIST_PROGRAMS);
    applicationControls->addButton = GetDlgItem(hDlg, IDC_BTN_APP_ADD);
    applicationControls->removeButton = GetDlgItem(hDlg, IDC_BTN_APP_RMV);
    applicationControls->settingsButton = GetDlgItem(hDlg, IDC_BTN_APP_SETTINGS);

    applicationControls->clipThread = NULL;

    return TRUE;
}

BOOL readApplicationList(APPLICATION_LIST *applicationList, const wchar_t *path)
{
    FILE *file = _wfopen(path, TEXT("rb"));

    if (file == NULL)
    {
        applicationList->count = 0;
        applicationList->applications = NULL;
        return FALSE;
    }

    fread(&applicationList->count, sizeof(int), 1, file);
    applicationList->applications = (APPLICATION_SETTINGS *)malloc(sizeof(APPLICATION_SETTINGS) * applicationList->count);
    for (int i = 0; i < applicationList->count; i++)
    {
        fread(&applicationList->applications[i], sizeof(APPLICATION_SETTINGS), 1, file);
    }

    fclose(file);
    _fcloseall();

    return TRUE;
}

BOOL writeApplicationList(APPLICATION_LIST *applicationList, const wchar_t *path)
{
    FILE *file = _wfopen(path, TEXT("wb"));

    if (file == NULL)
        return FALSE;

    fwrite(&applicationList->count, sizeof(applicationList->count), 1, file);

    if (applicationList->count > 0)
    {
        APPLICATION_SETTINGS *applications = applicationList->applications;

        for (int i = 0; i < applicationList->count; i++)
        {
            APPLICATION_SETTINGS application = applications[i];
            fwrite(&application, sizeof(APPLICATION_SETTINGS), 1, file);
        }
    }

    fclose(file);
    _fcloseall();

    return TRUE;
}

BOOL addApplication(APPLICATION_LIST *applicationList, APPLICATION_SETTINGS application)
{

    if (applicationList->applications == NULL)
    {
        applicationList->count = 1;
        applicationList->applications = (APPLICATION_SETTINGS *)malloc(sizeof(APPLICATION_SETTINGS));
        applicationList->applications[0] = application;
    }
    else
    {
        // check if application is already in list
        for (int i = 0; i < applicationList->count; i++)
        {
            if (wcscmp(applicationList->applications[i].application_name, application.application_name) == 0)
                return FALSE;
        }

        applicationList->count++;
        applicationList->applications = (APPLICATION_SETTINGS *)realloc(applicationList->applications, sizeof(APPLICATION_SETTINGS) * applicationList->count);
        applicationList->applications[applicationList->count - 1] = application;
    }

    return TRUE;
}

BOOL removeApplication(APPLICATION_LIST *applicationList, int index)
{
    if (applicationList->applications == NULL)
        return TRUE;

    if (index < 0 || index >= applicationList->count)
        return FALSE;

    if (applicationList->count == 1)
    {
        free(applicationList->applications);
        applicationList->applications = NULL;
        applicationList->count = 0;
    }
    else
    {
        APPLICATION_SETTINGS *applications = applicationList->applications;

        for (int i = index; i < applicationList->count - 1; i++)
        {
            applications[i] = applications[i + 1];
        }

        applicationList->count--;
        applicationList->applications = (APPLICATION_SETTINGS *)realloc(applicationList->applications, sizeof(APPLICATION_SETTINGS) * applicationList->count);
    }

    return TRUE;
}

BOOL initApplicationList(APPLICATION_LIST *applicationList)
{
    wchar_t path[MAX_PATH];
    createApplicationDirectory(path);
    readApplicationList(applicationList, path);
    return TRUE;
}

BOOL closeApplicationList(APPLICATION_LIST *applicationList)
{
    wchar_t path[MAX_PATH];
    createApplicationDirectory(path);

    writeApplicationList(applicationList, path);

    if (applicationList->applications != NULL)
        free(applicationList->applications);

    return TRUE;
}

BOOL createApplicationDirectory(wchar_t *outPath)
{
    PWSTR path;

    if (!SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path)))
        return FALSE;

    wcscpy(outPath, path);

    // create directory
    PathAppend(outPath, TEXT("DisplayLock"));
    // TODO: check if directory was created
    CreateDirectory(outPath, NULL);
    // create file
    PathAppend(outPath, TEXT("\\applications.DLOCK"));

    CoTaskMemFree(path);
    return TRUE;
}

BOOL createApplicationSettings(const wchar_t *appPath, APPLICATION_SETTINGS *application)
{
    wchar_t *basename = PathFindFileNameW(appPath);

    if (basename == appPath)
        return FALSE;

    wcscpy(application->application_path, appPath);
    wcscpy(application->application_name, basename);
    application->borderless = FALSE;
    application->fullscreen = FALSE;
    application->enabled = TRUE;

    return TRUE;
}

BOOL startApplicationThread(HANDLE *thread, int (*callback)(void *parameters), void *args)
{
    // TODO: check better error checking
    *thread = (HANDLE)_beginthreadex(NULL, 0, callback, args, 0, NULL);

    if (*thread == NULL)
        return FALSE;

    return TRUE;
}

// safely closes the thread
void closeApplicationThread(HANDLE thread, BOOL *status)
{
    // check to see if thread is running
    if (thread != NULL)
    {
        *status = FALSE;
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
        thread = NULL;
    }
}

BOOL CALLBACK EnumWindowsProcPID(HWND hwnd, LPARAM lParam)
{
    DWORD pid;
    EnumWindowsProcPIDArgs *proc = (EnumWindowsProcPIDArgs *)lParam;
    GetWindowThreadProcessId(hwnd, &pid);

    if (pid == proc->pid)
    {
        proc->hwnd = hwnd;
        return FALSE;
    }

    return TRUE;
}

DWORD getPidFromName(const wchar_t *name)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (wcsicmp(entry.szExeFile, name) == 0)
            {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        }
    }

    CloseHandle(snapshot);
    return 0;
}

int CALLBACK cursorLockApplications(void *parameters)
{
    APPLICATION_ARGS *args = (APPLICATION_ARGS *)parameters;
    POINT cursorPosition;

    while (*(args->clipRunning))
    {
        HANDLE mutex = CreateMutex(NULL, FALSE, APPLICATION_MUTEX_NAME);
        for (int i = 0; i < args->applicationList->count; i++)
        {
            APPLICATION_SETTINGS application = args->applicationList->applications[i];

            if (application.enabled)
            {
                EnumWindowsProcPIDArgs args;
                args.pid = getPidFromName(application.application_name);
                args.hwnd = NULL;

                if (args.pid != 0)
                    EnumWindows(EnumWindowsProcPID, (LPARAM)&args);

                if (args.hwnd != NULL)
                {
                    RECT rect;
                    GetWindowRect(args.hwnd, &rect);

                    if (application.borderless)
                    {
                        const long long borderlessStyle = GetWindowLongPtr(args.hwnd, GWL_STYLE);
                        const long long borderlessStyleEx = GetWindowLongPtr(args.hwnd, GWL_EXSTYLE);

                        const long long mask = WS_OVERLAPPED | WS_THICKFRAME | WS_SYSMENU | WS_CAPTION;
                        const long long exMask = WS_EX_WINDOWEDGE;

                        if ((borderlessStyle & mask) != 0)
                            SetWindowLongPtr(args.hwnd, GWL_STYLE, borderlessStyle & ~mask);

                        if ((borderlessStyleEx & exMask) != 0)
                            SetWindowLongPtr(args.hwnd, GWL_EXSTYLE, borderlessStyleEx & ~exMask);
                    }
                    else if (application.fullscreen)
                    {
                        RECT rect = {0};
                        GetClientRect(args.hwnd, &rect);
                        ClientToScreen(args.hwnd, (LPPOINT)&rect.left);
                        ClientToScreen(args.hwnd, (LPPOINT)&rect.right);

                        if (rect.left != 0 || rect.top != 0 || rect.right != GetSystemMetrics(SM_CXSCREEN) || rect.bottom != GetSystemMetrics(SM_CYSCREEN))
                            SetWindowPos(args.hwnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
                    }

                    // TODO: lock cursor
                    HWND active = GetForegroundWindow();

                    if (args.hwnd == active)
                    {
                        GetCursorPos(&cursorPosition);
                        RECT windowRect = {0};
                        GetClientRect(args.hwnd, &windowRect);
                        ClientToScreen(args.hwnd, (LPPOINT)&windowRect.left);
                        ClientToScreen(args.hwnd, (LPPOINT)&windowRect.right);

                        if ((cursorPosition.y <= windowRect.bottom && cursorPosition.y >= windowRect.top) && (cursorPosition.x >= windowRect.left && cursorPosition.x <= windowRect.right))
                            ClipCursor(&windowRect);
                        else if (GetAsyncKeyState(VK_LBUTTON) == 0)
                            ClipCursor(&windowRect);
                    }
                }
            }
            Sleep(1);
        }

        ReleaseMutex(mutex);
        CloseHandle(mutex);
    }

    ClipCursor(NULL);
    _endthreadex(1);

    return 0;
}