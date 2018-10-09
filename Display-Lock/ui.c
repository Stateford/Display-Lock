#include "ui.h"
#include <stdio.h>
#include <CommCtrl.h>
#include "win.h"
#include "menu.h"
#include "settings.h"

BOOL getVersionString(wchar_t * buffer, int bufferSize)
{
    wchar_t version[2048];
    wchar_t fileName[MAX_PATH];
    
    BOOL result;

    result = GetModuleFileName(NULL, fileName, MAX_PATH);

    if (!result)
        return FALSE;

    DWORD dwVersionBufferSize = GetFileVersionInfoSizeW(fileName, NULL);
    result = GetFileVersionInfo(fileName, 0, dwVersionBufferSize, (LPVOID)version);

    if (!result)
        return FALSE;

 
    UINT size;
    VS_FIXEDFILEINFO *verInfo = NULL;
    result = VerQueryValue(version, L"\\", (LPVOID)&verInfo, &size);

    if (!result)
        return FALSE;

    int major = HIWORD(verInfo->dwFileVersionMS);
    int minor = LOWORD(verInfo->dwFileVersionMS);
    int build = HIWORD(verInfo->dwFileVersionLS);
    int revision = LOWORD(verInfo->dwFileVersionLS);

    
    swprintf(buffer, bufferSize, L"Version: %d.%d.%d.%d", major, minor, build, revision);

    return TRUE;
}

void settingsShowWindow(SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS * settings, SETTINGS * previousSettings, BOOL running)
{
    SendMessage(settingsControls.borderless, BM_SETCHECK, settings->borderless, 0);
    SendMessage(settingsControls.foreground, BM_SETCHECK, settings->foreground, 0);
    SendMessage(settingsControls.fullScreen, BM_SETCHECK, settings->fullScreen, 0);
    SendMessage(settingsControls.minimize, BM_SETCHECK, settings->minimize, 0);

    EnableWindow(settingsControls.borderless, !running);
    EnableWindow(settingsControls.foreground, !running);
    EnableWindow(settingsControls.fullScreen, !running);
    EnableWindow(settingsControls.minimize, !running);
    EnableWindow(settingsControls.hotkey, !running);
}


void settingsSave(HWND hWnd,SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS settings, SETTINGS *previousSettings)
{
    settings.borderless = (BOOL)SendMessage(settingsControls.borderless, BM_GETCHECK, 0, 0);
    settings.foreground = (BOOL)SendMessage(settingsControls.foreground, BM_GETCHECK, 0, 0);
    settings.fullScreen = (BOOL)SendMessage(settingsControls.fullScreen, BM_GETCHECK, 0, 0);
    settings.minimize = (BOOL)SendMessage(settingsControls.minimize, BM_GETCHECK, 0, 0);
    
    *previousSettings = settings;
}

void settingsCancel(SETTINGS_VIEW_CONTROLS settingsControls, SETTINGS *settings, SETTINGS previousSettings)
{
    *settings = previousSettings;
    SendMessage(settingsControls.borderless, BM_SETCHECK, settings->borderless, 0);
    SendMessage(settingsControls.foreground, BM_SETCHECK, settings->foreground, 0);
    SendMessage(settingsControls.fullScreen, BM_SETCHECK, settings->fullScreen, 0);
    SendMessage(settingsControls.minimize, BM_SETCHECK, settings->minimize, 0);
    SendMessage(settingsControls.hotkey, HKM_SETHOTKEY, 0, 0);
}

void initalizeWindowView(HWND hDlg, MENU *menu, SETTINGS *settings, volatile BOOL *running, WINDOW_VIEW_CONTROLS *windowControls, ARGS *args)
{
    initMenuObj(menu);
    *running = FALSE;
    // initalize controls
    windowControls->runningClip = running;
    windowControls->clipThread = NULL;
    windowControls->comboBox = GetDlgItem(hDlg, IDC_COMBO_WINDOW);
    windowControls->stopButton = GetDlgItem(hDlg, IDC_BUTTON_WINDOWS_STOP);
    windowControls->startButton = GetDlgItem(hDlg, IDC_BUTTON_WINDOWS_START);

    args->settings = settings;
    args->clipRunning = running;

    // populate the combobox with fields
    menu->updateComboBox(windowControls->comboBox, &windowControls->windows, openWindows);
}

void mainWindowInit(HWND hDlg, MAIN_WINDOW_CONTROLS *mainWindowControls)
{
    // initalize the tab controls
    mainWindowControls->tabCtrl = GetDlgItem(hDlg, IDC_TAB_CONTROL);
    TCITEM tci = { 0 };
    tci.mask = TCIF_TEXT;
    tci.pszText = L"Window";
    TabCtrl_InsertItem(mainWindowControls->tabCtrl, WINDOW_VIEW, &tci);

    /*
    Currently disabled
    tci.pszText = L"Monitor";
    TabCtrl_InsertItem(tabCtrl, 1, &tci);
    */

    tci.pszText = L"Settings";
    TabCtrl_InsertItem(mainWindowControls->tabCtrl, SETTINGS_VIEW, &tci);


}

void windowsButtonStart(WINDOW_VIEW_CONTROLS *windowControls, ARGS *args, volatile BOOL *running, int windowSelection)
{
    *running = TRUE;
    args->selectedWindow = windowControls->windows.windows[windowSelection];
    startThread(&windowControls->clipThread, cursorLock, (void*)args);
    EnableWindow(windowControls->startButton, FALSE);
    EnableWindow(windowControls->stopButton, TRUE);
}

void windowsButtonStop(MENU menu, WINDOW_VIEW_CONTROLS *windowControls)
{
    *windowControls->runningClip = FALSE;
    EnableWindow(windowControls->startButton, TRUE);
    EnableWindow(windowControls->stopButton, FALSE);
    menu.closeThread(windowControls->clipThread, windowControls->runningClip);
}

void invokeReadSettings(SETTINGS *settings)
{
    // if this fails, an error stating config could not read/write
        // program will continue without saving a config
    wchar_t buff[4];
    int strSize = LoadString(GetModuleHandle(NULL), IDS_BUILD, buff, 4);

    if (strSize != 0)
    {
        wchar_t path[MAX_PATH];
        findPath(path);
        readSettings(settings, buff, path);
    }
    else
        defaultSettings(settings, L"0");
}

void shutDown(SETTINGS settings)
{
    wchar_t path[MAX_PATH];
    createDirectory(path);
    writeSettings(settings, path);
}