#include "settings.h"
#include <stdio.h>
#include <ShlObj.h>
#include <shtypes.h>
#include "Shlwapi.h"

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
    PWSTR path;

    // TODO: check header of file to ensure it's a valid config file using strcmp

    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path)))
    {
        PathAppendW(path, TEXT("DisplayLock\\settings.DLOCK"));
        FILE *file = _wfopen(path, TEXT("r"));

        // if if opening file is succcessful read into struct
        // otherwise use default settings
        if (file != NULL && file == 0)
        {
            fread(settings, sizeof(*settings), 1, file);
            fclose(file);
        }
        else
            defaultSettings(settings);

    }
    else
        defaultSettings(settings);

    // free memory allocated by SHGetKnownFolderPath
    CoTaskMemFree(path);
    return TRUE;
}

BOOL writeSettings(SETTINGS *settings)
{
    PWSTR path;
    //const HRESULT hr = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path);

    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path)))
    {
        // create directory
        PathAppendW(path, TEXT("DisplayLock"));
        CreateDirectory(path, NULL);
        // create file
        PathAppendW(path, TEXT("\\settings.DLOCK"));
        FILE *file = _wfopen(path, TEXT("w"));
        if (file == NULL || file == 0)
        {
            CoTaskMemFree(path);
            return FALSE;
        }

        fwrite(&settings, sizeof(settings), 1, file);
        fclose(file);
    }

    // free memory allocated by SHGetKnownFolderPath
    CoTaskMemFree(path);
    return TRUE;
}