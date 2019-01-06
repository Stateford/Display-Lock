#include "menu.h"
#include <process.h>

// initalizes the menu object
// contains a list of pointers to functions
void initMenuObj(MENU *menu)
{
    menu->startThread = startThread;
    menu->closeThread = closeThread;
    menu->updateComboBox = updateComboBox;
}

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

BOOL startThread(HANDLE *thread, int(*callback)(void* parameters), void *args)
{
    // TODO: check better error checking
    *thread = (HANDLE)_beginthreadex(NULL, 0, callback, args, 0, NULL);
    
    if (*thread == NULL)
        return FALSE;

    return TRUE;
}

// safely closes the thread
void closeThread(HANDLE thread, BOOL *status)
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


