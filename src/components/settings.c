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
    settingsControls->checkForUpdatesStartup = GetDlgItem(hDlg, IDC_CHECK_STARTUP_UPDATES);
    settingsControls->minimizeToTray = GetDlgItem(hDlg, IDC_CHECK_SETTINGS_TRAY_CLOSE);
    settingsControls->settingsChanged = FALSE;
}

void setSettingsDlg(HWND hDlg, SETTINGS settings)
{
    return;
}

void defaultSettings(SETTINGS *settings, const wchar_t *versionStr)
{
    strcpy(settings->header, "DLOCK");
    settings->version = 0;
    for (unsigned int i = 0; i < wcslen(versionStr); i++)
    {
        if (versionStr[i] >= '0' && versionStr[i] <= '9')
            settings->version += (int)versionStr[i] - '0';
    }

    settings->borderless = FALSE;
    settings->foreground = FALSE;
    settings->fullScreen = FALSE;
    settings->minimize = TRUE;
    settings->checkUpdateStartup = TRUE;
    settings->minimizeToTray = FALSE;
}

BOOL checkVersion(SETTINGS *settings, const wchar_t *versionStr)
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

    if (!SUCCEEDED(hr))
        return FALSE;

    wcscpy_s(outPath, MAX_PATH, path);
    LPCWSTR x = L"DisplayLock\\settings.DLOCK";
    PathAppend(outPath, x);

    CoTaskMemFree(path);
    return TRUE;
}

BOOL createDirectory(wchar_t *outPath)
{
    PWSTR path;

    if (!SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &path)))
        return FALSE;

    wcscpy_s(outPath, MAX_PATH, path);

    // create directory
    PathAppend(outPath, TEXT("DisplayLock"));
    // TODO: check if directory was created
    CreateDirectory(outPath, NULL);
    // create file
    PathAppend(outPath, TEXT("\\settings.DLOCK"));

    CoTaskMemFree(path);
    return TRUE;
}

BOOL readSettings(SETTINGS *settings, const wchar_t *versionStr, const wchar_t *path)
{
    FILE *file = _wfopen(path, TEXT("rb"));

    // if if opening file is succcessful read into struct
    // otherwise use default settings
    if (file == NULL)
    {
        defaultSettings(settings, versionStr);
        return FALSE;
    }

    if (fread(settings, sizeof(SETTINGS), 1, file) != 1)
    {
        fclose(file);
        defaultSettings(settings, versionStr);
        return FALSE;
    }

    fclose(file);

    if (!checkVersion(settings, versionStr))
        defaultSettings(settings, versionStr);

    return TRUE;
}

BOOL writeSettings(SETTINGS settings, const wchar_t *path)
{
    // if loadstring could not be read, do not write the file
    if (settings.version <= 0 || strcmp(settings.header, "DLOCK") != 0)
        return FALSE;

    FILE *file = _wfopen(path, TEXT("wb"));

    if (file == NULL)
        return FALSE;

    fwrite(&settings, sizeof(settings), 1, file);
    fclose(file);

    return TRUE;
}
