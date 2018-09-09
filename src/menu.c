#include "menu.h"

// updates a combobox
// takes a callback to an enumeration function
void updateComboBox(HWND control, WINDOWLIST *windows, void(*callback)(WINDOWLIST*))
{
    SendMessage(control, CB_RESETCONTENT, 0, 0);

    callback(windows);

    for (int i = 0; i < windows->count; i++)
        SendMessageA(control, CB_ADDSTRING, (WPARAM)NULL, (LPARAM)windows->windows[i].title);

    SendMessage(control, CB_SETCURSEL, 0, 0);
}

// safely closes the thread
void closeThread(HANDLE thread, volatile BOOL *status)
{
    // check to see if thread is running
    if (thread != NULL)
    {
        *status = FALSE;
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
        thread = NULL;
    }
}