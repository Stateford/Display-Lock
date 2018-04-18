#include "settings.h"
#include <stdio.h>
#include <ShlObj.h>
#include <shtypes.h>
#include "Shlwapi.h"


// initalizes settings window on startup
void initalizeSettings(HWND hDlg, SETTINGS* settings)
{
    // get the checkbox
	const HWND minimize = GetDlgItem(hDlg, IDC_SETTINGS_MINIMIZE);
    const HWND borderless = GetDlgItem(hDlg, IDC_SETTINGS_BORDERLESS);
    const HWND fullScreen = GetDlgItem(hDlg, IDC_SETTINGS_FULLSCREEN);
    const HWND foreground = GetDlgItem(hDlg, IDC_SETTINGS_FOREGROUND);

    
    // update the dialog window
	SendMessage(minimize, BM_SETCHECK, settings->minimize, 0);
    SendMessage(borderless, BM_SETCHECK, settings->borderlessWindow, 0);
    SendMessage(fullScreen, BM_SETCHECK, settings->fullScreen, 0);
    SendMessage(foreground, BM_SETCHECK, settings->foreground, 0);
}

// update settings
void updateSettings(HWND hDlg, SETTINGS* settings)
{
    // check the checkbox
	const HWND minimize = GetDlgItem(hDlg, IDC_SETTINGS_MINIMIZE);
    const HWND borderless = GetDlgItem(hDlg, IDC_SETTINGS_BORDERLESS);
    const HWND fullScreen = GetDlgItem(hDlg, IDC_SETTINGS_FULLSCREEN);
    const HWND foreground = GetDlgItem(hDlg, IDC_SETTINGS_FOREGROUND);
    
    //WORD wHotKey = LOWORD(SendMessage(GetDlgItem(hDlg, IDC_SETTINGS_HOTKEY), HKM_GETHOTKEY, 0, 0));
    //settings->hotKeyCount = 0;

	settings->minimize = (BOOL)SendMessage(minimize, BM_GETCHECK, 0, 0);
    settings->borderlessWindow = (BOOL)SendMessage(borderless, BM_GETCHECK, 0, 0);
    settings->fullScreen = (BOOL)SendMessage(fullScreen, BM_GETCHECK, 0, 0);
    settings->foreground = (BOOL)SendMessage(foreground, BM_GETCHECK, 0, 0);
    //settings->HOTKEY = LOBYTE(wHotKey);
    //settings->HOTKEY_MODIFIERS[0] = HIBYTE(wHotKey);
    //settings->hotKeyCount;
}

// write settings to roaming directory
void writeSettings(SETTINGS settings)
{
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, 0, &path);

    if(hr == S_OK)
    {
        // create directory
        PathAppend(path, TEXT("DisplayLock"));
        CreateDirectory(path, NULL);
        // create file
        PathAppend(path, TEXT("\\settings.DLOCK"));
        FILE *file = _wfopen(path, TEXT("w"));

        if(file != NULL)
        {
            fwrite(&settings, sizeof(settings), 1, file);
        }

        fclose(file);
    }
    // free memory allocated by SHGetKnownFolderPath
    CoTaskMemFree(path);
}

// read settings from roaming directory
void readSettings(SETTINGS *settings)
{
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, 0, &path);


    // TODO: check header of file to ensure it's a valid config file using strcmp

    if(hr == S_OK)
    {
        PathAppend(path, TEXT("DisplayLock\\settings.DLOCK"));
        FILE *file = _wfopen(path, TEXT("r"));

        // if if opening file is succcessful read into struct
        // otherwise use default settings
        if (file != NULL)
            fread(settings, sizeof(*settings), 1, file);
        else
            defaultSettings(settings);

        fclose(file);
    } 
    else
        defaultSettings(settings);
    
    // free memory allocated by SHGetKnownFolderPath
    CoTaskMemFree(path);
}

void defaultSettings(SETTINGS* settings)
{
    strcpy(settings->header, "DLOCK");
    settings->minimize = 1;
    settings->borderlessWindow = 0;
    settings->fullScreen = 0;
    settings->foreground = 0;
}
