#include "settings.h"
#include <stdio.h>
#include <ShlObj.h>
#include <shtypes.h>
#include "Shlwapi.h"


// initalizes settings window on startup
void initalizeSettings(HWND hDlg, SETTINGS* settings)
{
    // get the checkbox
	HWND minimize = GetDlgItem(hDlg, IDC_SETTINGS_MINIMIZE);
    HWND borderless = GetDlgItem(hDlg, IDC_SETTINGS_BORDERLESS);
    HWND fullScreen = GetDlgItem(hDlg, IDC_SETTINGS_FULLSCREEN);


    
    // update the dialog window
	SendMessage(minimize, BM_SETCHECK, settings->minimize, 0);
    SendMessage(borderless, BM_SETCHECK, settings->borderlessWindow, 0);
    SendMessage(fullScreen, BM_SETCHECK, settings->fullScreen, 0);
}

// update settings
void updateSettings(HWND hDlg, SETTINGS* settings)
{
    // check the checkbox
	HWND minimize = GetDlgItem(hDlg, IDC_SETTINGS_MINIMIZE);
    HWND borderless = GetDlgItem(hDlg, IDC_SETTINGS_BORDERLESS);
    HWND fullScreen = GetDlgItem(hDlg, IDC_SETTINGS_FULLSCREEN);

    
    //WORD wHotKey = LOWORD(SendMessage(GetDlgItem(hDlg, IDC_SETTINGS_HOTKEY), HKM_GETHOTKEY, 0, 0));
    //settings->hotKeyCount = 0;

	settings->minimize = SendMessage(minimize, BM_GETCHECK, 0, 0);
    settings->borderlessWindow = SendMessage(borderless, BM_GETCHECK, 0, 0);
    settings->fullScreen = SendMessage(fullScreen, BM_GETCHECK, 0, 0);
    //settings->HOTKEY = LOBYTE(wHotKey);
    //settings->HOTKEY_MODIFIERS[0] = HIBYTE(wHotKey);
    //settings->hotKeyCount;
}

void writeSettings(SETTINGS settings)
{
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, 0, &path);

    // create directory
    PathAppend(path, TEXT("DisplayLock"));
    CreateDirectory(path, NULL);
    // create file
    PathAppend(path, TEXT("\\settings.DLOCK"));
    FILE *file = _wfopen(path, TEXT("w"));

    fwrite(&settings, sizeof(settings), 1, file);
    fclose(file);

    // free the memory
    CoTaskMemFree(path);
}

void readSettings(SETTINGS *settings)
{
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, 0, &path);

    PathAppend(path, TEXT("DisplayLock\\settings.DLOCK"));

    // TODO: check header of file to ensure it's a valid config file using strcmp

    FILE *file = _wfopen(path, TEXT("r"));
    if(file != NULL)
    {
        fread(settings, sizeof(*settings), 1, file);
        fclose(file);
    } 
    else
    {
        strcpy(settings->header, "DLOCK");
        settings->minimize = 1;
        settings->borderlessWindow = 0;
        settings->fullScreen = 0;
    }
    // free memory
    CoTaskMemFree(path);
}