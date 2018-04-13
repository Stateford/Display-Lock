#include "settings.h"
#include <stdio.h>
#include <ShlObj.h>
#include <shtypes.h>
#include "Shlwapi.h"


// initalizes settings window on startup
void initalizeSettings(HWND hDlg, SETTINGS* settings)
{
    // get the checkbox
	HWND checkbox = GetDlgItem(hDlg, IDC_SETTINGS_MINIMIZE);

    // check the current setting
	int checkState;
	if (!settings->minimize)
		checkState = BST_UNCHECKED;
	else
		checkState = BST_CHECKED;
    
    // update the dialog window
	SendMessage(checkbox, BM_SETCHECK, checkState, 0);
}

// update settings
void updateSettings(HWND hDlg, SETTINGS* settings)
{
    // check the checkbox
	HWND checkbox = GetDlgItem(hDlg, IDC_SETTINGS_MINIMIZE);
	settings->minimize = SendMessage(checkbox, BM_GETCHECK, 0, 0);
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
        fread(settings, sizeof(settings), 1, file);
        fclose(file);
    } 
    else
    {
        strcpy(settings->header, "DLOCK");
        settings->minimize = 1;
    }
    // free memory
    CoTaskMemFree(path);
}