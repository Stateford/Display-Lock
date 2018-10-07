#include "notify.h"
#include "win.h"


void notifyInit(HWND hWnd, NOTIFYICONDATA * notify)
{
    notify->hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
    notify->uVersion = 5;
    notify->cbSize = sizeof(notify);
    notify->hWnd = hWnd;
    notify->uID = 1;
    notify->uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    notify->uCallbackMessage = NOTIFY_MSG;

}

void showMainWindow(HWND hWnd, NOTIFYICONDATA *notify)
{
    ShowWindow(hWnd, SW_RESTORE);
    SetForegroundWindow(hWnd);
}

void showContext(HWND hWnd, HMENU menu)
{
    HMENU contextMenu = GetSubMenu(menu, 0);
    // create new windowmenu
    HMENU windowMenu = CreatePopupMenu();
    // populate window list
    updateContextMenu(&windowMenu);
    // modify the popup with the new window list
    ModifyMenu(contextMenu, 3, MF_BYPOSITION | MF_POPUP, (UINT)windowMenu, TEXT("Windows"));

    POINT cursor;
    GetCursorPos(&cursor);
    SetForegroundWindow(hWnd);
    // open the popup menu
    TrackPopupMenu(contextMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, cursor.x, cursor.y, 0, hWnd, 0);
}

void notifyChildWindows(HWND hWnd, UINT msg)
{
    EnumChildWindows(hWnd, EnumChildProc, (LPARAM)msg);
}

void updateContextMenu(HMENU *submenu)
{
    WINDOWLIST windows;
    openWindows(&windows);

    for (int i = 0; i < windows.count; i++)
    {
        AppendMenuA(*submenu, MF_STRING, (100 + i), windows.windows[i].title);
    }

}

BOOL EnumChildProc(HWND hWnd, LPARAM lParam)
{
    UINT msg = (UINT)lParam;
    SendMessage(hWnd, WM_COMMAND, msg, 0);

    return TRUE;
}

