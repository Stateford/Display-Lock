#include "app_settings.h"
#include "common.h"
#include "resource.h"


INT_PTR CALLBACK appSettingsProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    static HWND enabled = NULL;
    static HWND borderless = NULL;
    static HWND fullscreen = NULL;
    static APPLICATION_SETTINGS *settings = NULL;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        enabled = GetDlgItem(hDlg, IDC_CHK_APP_ENABLED);
        borderless = GetDlgItem(hDlg, IDC_CHK_APP_BORDERLESS);
        fullscreen = GetDlgItem(hDlg, IDC_CHK_APP_FULLSCREEN);
        settings = (APPLICATION_SETTINGS *)lParam;
        SendMessage(enabled, BM_SETCHECK, settings->enabled, 0);
        SendMessage(borderless, BM_SETCHECK, settings->borderless, 0);
        SendMessage(fullscreen, BM_SETCHECK, settings->fullscreen, 0);

        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BTN_APP_SETTINGS_OK:
        {
            if (settings != NULL)
            {
                int is_enabled = SendMessage(enabled, BM_GETCHECK, 0, 0);
                int is_borderless = SendMessage(borderless, BM_GETCHECK, 0, 0);
                int is_fullscreen = SendMessage(fullscreen, BM_GETCHECK, 0, 0);

                settings->enabled = (is_enabled == BST_CHECKED);
                settings->borderless = (is_borderless == BST_CHECKED);
                settings->fullscreen = (is_fullscreen == BST_CHECKED);
            }

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        case IDC_BTN_APP_SETTINGS_CANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        default:
            break;
        }
        break;
    }

    case WM_CLOSE:
    {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    }
    default:
        break;
    }
    return (INT_PTR)FALSE;
}
