#ifndef MENU_H
#define MENU_H

#include "win.h"

typedef struct Args
{
    Menu *menu;
    HWND *hWnd;
    HANDLE *mutex;
} Args;

typedef struct winArgs
{
    HANDLE *mutex;
    WINDOW *window;
    BOOL *active;
} winArgs;

void initMenu(Menu*);

void updateComboBox(Args*);

WINDOW *sortWindow(Args*, char*, int);

#endif