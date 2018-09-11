#include "settings.h"

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

void resetSettings(HWND hDlg)
{
    return;
}
