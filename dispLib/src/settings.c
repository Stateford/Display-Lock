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


void defaultSettings(SETTINGS *settings, wchar_t *versionStr)
{
    strcpy(settings->header, "DLOCK");
    settings->version = 0;
    for (unsigned int i = 0; i < wcslen(versionStr); i++)
    {
        if(versionStr[i] >= '0' && versionStr[i] <= '9')
            settings->version += (int)versionStr[i] - '0';
    }
    
    settings->borderless = FALSE;
    settings->foreground = FALSE;
    settings->fullScreen = FALSE;
    settings->minimize = TRUE;
}

BOOL checkVersion(SETTINGS *settings, wchar_t *versionStr)
{
    if (strcmp(settings->header, "DLOCK") != 0 || wcslen(versionStr) == 0)
        return FALSE;

    int version = 0;
    for (unsigned int i = 0; i < wcslen(versionStr); i++)
    {
        if (versionStr[i] >= '0' && versionStr[i] <= '9')
            version += (int)versionStr[i] - '0';
        else
            return FALSE;
    }

    return settings->version == version;
}

BOOL findPath(wchar_t *outPath)
{
    PWSTR path;

    HRESULT hr = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path);

    if (SUCCEEDED(hr))
    {
        wcscpy(outPath, path);
        LPCWSTR x = L"DisplayLock\\settings.DLOCK";
        PathAppend(outPath, x);
    }
    else
        return FALSE;

    CoTaskMemFree(path);
    return TRUE;
}

BOOL readSettings(SETTINGS *settings, wchar_t *versionStr)
{
    PWSTR path;

    HRESULT hr = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path);

    if (SUCCEEDED(hr))
    {
        wchar_t fullPath[MAX_PATH];
        wcscpy(fullPath, path);

        LPCWSTR x = L"DisplayLock\\settings.DLOCK";
        PathAppend(fullPath, x);

        FILE *file = _wfopen(fullPath, TEXT("rb"));

        // if if opening file is succcessful read into struct
        // otherwise use default settings
        if (file == NULL)
        {
            defaultSettings(settings, versionStr);
            CoTaskMemFree(path);
            return FALSE;
        }

        fread(settings, sizeof(*settings), 1, file);
        

        fclose(file);
        _fcloseall();
    }
    else
    {
        defaultSettings(settings, versionStr);
        CoTaskMemFree(path);
        return FALSE;
    }

    if (!checkVersion(settings, versionStr))
        defaultSettings(settings, versionStr);

    // free memory allocated by SHGetKnownFolderPath
    CoTaskMemFree(path);
    return TRUE;
}

BOOL writeSettings(SETTINGS settings)
{
    // if loadstring could not be read, do not write the file
    if (settings.version <= 0)
        return FALSE;

    PWSTR path;

    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path)))
    {
        wchar_t fullPath[MAX_PATH];
        wcscpy(fullPath, path);

        // create directory
        PathAppend(fullPath, TEXT("DisplayLock"));
        // TODO: check if directory was created
        CreateDirectory(fullPath, NULL);
        // create file
        PathAppend(fullPath, TEXT("\\settings.DLOCK"));
        FILE *file = _wfopen(fullPath, TEXT("wb"));

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