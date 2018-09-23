#include "notify.h"


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

void showContext(HWND hWnd)
{
    POINT cursor;
    GetCursorPos(&cursor);
    HMENU menu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_NOTIFY_MENU));
    SetForegroundWindow(hWnd);
    TrackPopupMenu(GetSubMenu(menu, 0), TPM_LEFTALIGN | TPM_LEFTBUTTON, cursor.x, cursor.y, 0, hWnd, 0);
}

