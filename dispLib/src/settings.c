#include "settings.h"
#include <stdio.h>
#include <ShlObj.h>
#include <shtypes.h>
#include "Shlwapi.h"

void initalizeSettings(HWND hDlg, SETTINGS_VIEW_CONTROLS *settingsControls)
{
    settingsControls->minimize = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_MINIMIZE);
    settingsControls->foreground = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_FOREGROUND);
    settingsControls->borderless = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_BORDERLESS);
    settingsControls->fullScreen = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_FULL_SCREEN);
    settingsControls->settingsChanged = FALSE;
}

void setSettingsDlg(HWND hDlg, SETTINGS settings)
{

    return;
}


void defaultSettings(SETTINGS *settings)
{
    wchar_t buff[4];
    LoadString(GetModuleHandle(NULL), IDS_BUILD, buff, 3);
    strcpy(settings->header, "DLOCK");
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
    if (strcmp(settings->header, "DLOCK") != 0)
        return FALSE;

    wchar_t buff[4];
    LoadString(GetModuleHandle(NULL), IDS_BUILD, buff, 3);

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

    HRESULT hr = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path);

    if (SUCCEEDED(hr))
    {
        PathAppend(path, TEXT("DisplayLock\\settings.DLOCK"));
        FILE *file = _wfopen(path, TEXT("rb"));

        // if if opening file is succcessful read into struct
        // otherwise use default settings
        if (file == NULL)
        {
            defaultSettings(settings);
            CoTaskMemFree(path);
            return FALSE;
        }

        fread(settings, sizeof(*settings), 1, file);
        

        fclose(file);
        _fcloseall();
    }
    else
    {
        defaultSettings(settings);
        CoTaskMemFree(path);
        return FALSE;
    }

    if (!checkVersion(settings))
        defaultSettings(settings);

    // free memory allocated by SHGetKnownFolderPath
    CoTaskMemFree(path);
    return TRUE;
}

BOOL writeSettings(SETTINGS settings)
{
    PWSTR path;

    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path)))
    {
        // create directory
        PathAppend(path, TEXT("DisplayLock"));
        // TODO: check if directory was created
        CreateDirectory(path, NULL);
        // create file
        PathAppend(path, TEXT("\\settings.DLOCK"));
        FILE *file = _wfopen(path, TEXT("wb"));

        if (file == NULL)
        {
            CoTaskMemFree(path);
            return FALSE;
        }

        fwrite(&settings, sizeof(settings), 1, file);
        fclose(file);
        _fcloseall();
    }
    else
        return FALSE;

    // free memory allocated by SHGetKnownFolderPath
    CoTaskMemFree(path);
    return TRUE;
}