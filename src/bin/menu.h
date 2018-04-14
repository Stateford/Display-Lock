#ifndef MENU_H
#define MENU_H

#include "win.h"

typedef struct SETTINGS SETTINGS;

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
    SETTINGS *settings;
    BOOL *active;
} winArgs;

void initMenu(Menu*);
void updateComboBox(Args*);
WINDOW *sortWindow(Args*, char*, int);

#endif