#include "settings.h"
#include <stdio.h>

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

// TODO: This should probably save to a temp folder instead of a local folder to make the program more portable
void writeSettings(SETTINGS settings)
{
    FILE *file = fopen("settings.DLOCK", "w");
    fwrite(&settings, sizeof(settings), 1, file);
    fclose(file);
}

void readSettings(SETTINGS *settings)
{
    FILE *file = fopen("settings.DLOCK", "r");
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
}