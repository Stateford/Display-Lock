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
    settingsControls->hotkey = GetDlgItem(hDlg, IDC_HOTKEY);
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
    settings->numOfHotkeys = 0;
    settings->hotkeys[0] = (HOTKEY){ 0 };
    settings->hotkeys[1] = (HOTKEY){ 0 };
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

BOOL createDirectory(wchar_t *outPath)
{
    PWSTR path;

    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path)))
    {
        wcscpy(outPath, path);

        // create directory
        PathAppend(outPath, TEXT("DisplayLock"));
        // TODO: check if directory was created
        CreateDirectory(outPath, NULL);
        // create file
        PathAppend(outPath, TEXT("\\settings.DLOCK"));
    }
    else
        return FALSE;

    CoTaskMemFree(path);
    return TRUE;
}

BOOL readSettings(SETTINGS *settings, wchar_t *versionStr, wchar_t *path)
{
    FILE *file = _wfopen(path, TEXT("rb"));

    // if if opening file is succcessful read into struct
    // otherwise use default settings
    if (file == NULL)
    {
        defaultSettings(settings, versionStr);
        return FALSE;
    }

    fread(settings, sizeof(SETTINGS), 1, file);


    fclose(file);
    _fcloseall();

    if (!checkVersion(settings, versionStr))
        defaultSettings(settings, versionStr);

    return TRUE;
}

BOOL writeSettings(SETTINGS settings, wchar_t *path)
{
    // if loadstring could not be read, do not write the file
    if (settings.version <= 0 || strcmp(settings.header, "DLOCK") != 0)
        return FALSE;


    FILE *file = _wfopen(path, TEXT("wb"));

    if (file == NULL)
        return FALSE;

    fwrite(&settings, sizeof(settings), 1, file);
    fclose(file);
    _fcloseall();

    return TRUE;
}
