#include "about_dialog.h"
#include "resource.h"


INT_PTR CALLBACK about(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        wchar_t version[40];
        if (getVersionString(version, sizeof(version) / sizeof(wchar_t)))
            SetDlgItemText(hDlg, IDC_STATIC_VERSION, version);
    }
        return (INT_PTR)TRUE;

    case WM_NOTIFY:
        // if (((LPNMHDR)lParam)->code == NM_CLICK)
        // ShellExecuteW(NULL, TEXT("open"), TEXT("https://github.com/idietmoran/Display-Lock"), NULL, NULL, SW_SHOWNORMAL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        break;
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

