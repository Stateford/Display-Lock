#ifndef MENU_H
#define MENU_H

#include "win.h"

typedef struct Args
{
    Menu *menu;
    HWND *hWnd;
    HANDLE *mutex;
    int *currentSel;
} Args;

typedef struct winArgs
{
    HANDLE *mutex;
    WINDOW *window;
    BOOL *active;
} winArgs;

void initMenu(Menu*);

void updateComboBox(void*);
int __stdcall updateComboBoxEx(void *arguments);
WINDOW *sortWindow(Args*, char*, int);

#endif