#include "settings.h"
#include <stdio.h>

void initalizeSettings(HWND hDlg, SETTINGS_VIEW_CONTROLS *settingsControls, SETTINGS *settings)
{
    settingsControls->minimize = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_MINIMIZE);
    settingsControls->foreground = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_FOREGROUND);
    settingsControls->borderless = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_BORDERLESS);
    settingsControls->fullScreen = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_FULL_SCREEN);
    settingsControls->settingsChanged = FALSE;

    settings->borderless = SendMessage(settingsControls->borderless, BM_GETCHECK, 0, 0);
    settings->foreground = SendMessage(settingsControls->foreground, BM_GETCHECK, 0, 0);
    settings->fullScreen = SendMessage(settingsControls->fullScreen, BM_GETCHECK, 0, 0);
    settings->minimize = SendMessage(settingsControls->minimize, BM_GETCHECK, 0, 0);
}

void setSettingsDlg(HWND hDlg, SETTINGS settings)
{

    return;
}


void defaultSettings(SETTINGS *settings)
{
    wchar_t buff[4];
    LoadString(GetModuleHandle(NULL), IDS_BUILD, buff, 4);

    settings->version = 0;
    for (unsigned int i = 0; i < wcslen(buff); i++)
    {
        if(buff[i] >= '0' && buff[i] <= '9')
            settings->version += (int)buff[i] - '0';
    }
    
    settings->borderless = FALSE;
    settings->foreground = FALSE;
    settings->fullScreen = FALSE;
    settings->minimize = TRUE;
}

// checks the version of the config file
BOOL checkVersion(SETTINGS *settings)
{
    wchar_t buff[4];
    LoadString(GetModuleHandle(NULL), IDS_BUILD, buff, 4);

    int version = 0;
    for (unsigned int i = 0; i < wcslen(buff); i++)
    {
        if (buff[i] >= '0' && buff[i] <= '9')
            version += (int)buff[i] - '0';
    }

    return settings->version == version;
}

BOOL readSettings(SETTINGS *settings)
{
    return TRUE;
}

BOOL writeSettings(SETTINGS *settings)
{
    return TRUE;
}