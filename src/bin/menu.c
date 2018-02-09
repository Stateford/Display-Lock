#include "menu.h"
#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include "../resource.h"


void initMenu(Menu *menu)
{
    menu->currentMenu = MAIN;
    menu->windows.count = 0;
    menu->currentSelection = 0;
    menu->active = FALSE;
    menu->live = TRUE;

    menu->mainMenu.maxSelections = 3;

    strcpy_s(menu->mainMenu.options[0], 30, "Select Window");
    strcpy_s(menu->mainMenu.options[1], 30, "Start");
    strcpy_s(menu->mainMenu.options[2], 30, "Quit");

    menu->hstdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(menu->hstdin, &menu->mode);
    SetConsoleMode(menu->hstdin, 0);
    menu->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void updateComboBox(void *arguments)
{
    Args *args = (Args*)arguments;  // cast arguments into proper data type
    int prevCount = 0;              // keep count of our previous count to reduce flickering
    BOOL init = FALSE;
    int prevValue = 0;
    int currentSel = 0;
    while(args->menu->live)
    {
        WaitForSingleObject(&args->mutex, INFINITE);    // wait for mutex


        args->mutex = CreateMutex(NULL, FALSE, NULL);   // lock the mutex

        openWindows(&args->menu->windows);    // open open windows
        

        // if the count changes
        if(prevCount != args->menu->windows.count)
        {
            prevCount = args->menu->windows.count;  // reset count
            SendMessage(*args->hWnd, CB_RESETCONTENT, (WPARAM)NULL, 0); // clear combo box

            // write to combo box
            for (int i = 0; i < args->menu->windows.count; i++)
            {
                char foo[500];
                strcpy_s(foo, 500, args->menu->windows.windows[i].title);
                SendMessageA(*args->hWnd, CB_ADDSTRING, (WPARAM)NULL, (LPARAM)foo);
            }
            
            
            
        }
        if (!init)
        {
            init = TRUE;
            currentSel = (int)SendMessage(*args->hWnd, CB_GETCURSEL, 0, 0);

            if (currentSel == -1)
            {
                SendMessage(*args->hWnd, CB_SETCURSEL, 0, 0);
            }
        }
        else if (init)
        {
            currentSel = (int)SendMessage(*args->hWnd, CB_GETCURSEL, 0, 0);


            if (prevValue != currentSel && currentSel != -1)
            {
                prevValue = currentSel;
            }
            else
            {
                SendMessage(*args->hWnd, CB_SETCURSEL, prevValue, 0);
            }
        }
 
        ReleaseMutex(args->mutex);  // unlock mutex
        Sleep(100);
    }
    free(args);
    _endthread();
}

WINDOW *sortWindow(Args *args, char* str, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(args->menu->windows.windows[i].title, str) == 0)
        {
            args->menu->selectedWindow = args->menu->windows.windows[i];
            return &args->menu->windows.windows[i];
        }
    }
}
