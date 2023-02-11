#include "update_dialog.h"
#include "header.h"
#include "resource.h"
#include "common.h"
#include <commdlg.h>


INT_PTR CALLBACK updateProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        break;
    }
        return (INT_PTR)TRUE;

    case WM_NOTIFY:

        switch (((NMHDR *)lParam)->code)
        {
        case NM_CLICK:
        {
            if (wParam == IDC_UPDATE_LINK)
            {
                NMLINK *pNMLink = (NMLINK *)lParam;
                LITEM iItem = pNMLink->item;
                ShellExecuteW(
                    NULL,
                    TEXT("open"),
                    iItem.szUrl,
                    NULL,
                    NULL,
                    SW_HIDE);
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        default:
            break;
        }
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