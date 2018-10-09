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

void drawMenuSettings(HMENU menu, SETTINGS settings)
{
    CheckMenuItem(menu, ID_CONTEXTMENU_SETTINGS_MINIMIZE, (settings.minimize * 8));
    CheckMenuItem(menu, ID_CONTEXTMENU_SETTINGS_FOREGROUND, (settings.foreground * 8));
    CheckMenuItem(menu, ID_CONTEXTMENU_SETTINGS_BORDERLESS, (settings.borderless * 8));
    CheckMenuItem(menu, ID_CONTEXTMENU_SETTINGS_FULLSCREEN, (settings.fullScreen * 8));
}

void showContext(HWND hWnd, WINDOWLIST *windows, SETTINGS settings, BOOL running)
{
    HMENU menu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_NOTIFY_MENU));
    HMENU contextMenu = GetSubMenu(menu, 0);
    EnableMenuItem(menu, ID_CONTEXTMENU_STOP, !running);
    // create new windowmenu
    if (!running)
    {
        HMENU windowMenu = CreatePopupMenu();
        // populate window list
        updateContextMenu(&windowMenu, windows);
        // modify the popup with the new window list
        ModifyMenu(contextMenu, ID_CONTEXTMENU_WINDOWS, MF_BYCOMMAND | MF_POPUP, (UINT_PTR)windowMenu, TEXT("Windows"));
    }
    else
    {
        //HMENU foo = GetSubMenu(contextMenu, 3);
        EnableMenuItem(menu, ID_CONTEXTMENU_WINDOWS, MF_BYCOMMAND | MF_GRAYED);
    }

    drawMenuSettings(menu, settings);

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

void updateContextMenu(HMENU *submenu, WINDOWLIST *windows)
{
    openWindows(windows);

    for (int i = 0; i < windows->count; i++)
        AppendMenuA(*submenu, MF_STRING, (SUBMENU_ITEM_BASE + i), windows->windows[i].title);

}

BOOL EnumChildProc(HWND hWnd, LPARAM lParam)
{
    UINT msg = (UINT)lParam;
    SendMessage(hWnd, WM_COMMAND, msg, 0);

    return TRUE;
}
