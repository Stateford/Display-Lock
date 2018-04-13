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
}

void updateComboBox(Args *arguments)
{
	openWindows(&arguments->menu->windows);    // open open windows
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
