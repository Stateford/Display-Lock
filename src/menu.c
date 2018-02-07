#include "menu.h"
#include <stdio.h>
#include <Windows.h>
#include <process.h>

void start()
{
    Menu menu;
    // initalize menu
    initMenu(&menu);


    openWindows(&menu.windows);
    display(&menu);
}

void initMenu(Menu *menu)
{
    menu->currentMenu = MAIN;
    menu->windows.count = 0;
    menu->currentSelection = 0;
    menu->active = FALSE;

    menu->mainMenu.maxSelections = 3;

    strcpy_s(menu->mainMenu.options[0], 30, "Select Window");
    strcpy_s(menu->mainMenu.options[1], 30, "Start");
    strcpy_s(menu->mainMenu.options[2], 30, "Quit");

    menu->hstdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(menu->hstdin, &menu->mode);
    SetConsoleMode(menu->hstdin, 0);
    menu->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void redraw(Menu *menu)
{
    switch(menu->currentMenu)
    {
    case MAIN:
        drawMain(menu);
        break;
    case WINDOWS:
        drawWindows(menu);
        break;
    default:
        menu->currentMenu = MAIN;
        break;
    }
}

void drawMain(Menu *menu)
{
    if (menu->currentSelection > menu->mainMenu.maxSelections)
        menu->currentSelection = menu->mainMenu.maxSelections - 1;

    system("CLS");  // clear the screen
    // default color
    SetConsoleTextAttribute(menu->hConsole, 7);

    for(int i = 0; i < menu->mainMenu.maxSelections; i++)
    {
        if(menu->currentSelection == i)
            SetConsoleTextAttribute(menu->hConsole, 23);
        else
            SetConsoleTextAttribute(menu->hConsole, 7);

        if(i == 1 && menu->active)
            printf("Stop\n");
        else
            printf("%s\n", menu->mainMenu.options[i]);

        SetConsoleTextAttribute(menu->hConsole, 7);
    }
}

void drawWindows(Menu *menu)
{
    system("CLS");  // clear the screen
    // default color
    SetConsoleTextAttribute(menu->hConsole, 7);

    for (int i = 0; i < menu->windows.count; i++)
    {
        if(menu->currentSelection == i)
            SetConsoleTextAttribute(menu->hConsole, 23);
        else
            SetConsoleTextAttribute(menu->hConsole, 7);

        printf("%s\n", menu->windows.windows[i].title);
        SetConsoleTextAttribute(menu->hConsole, 7);
    }
}

void display(Menu *menu)
{
    redraw(menu);

    while(menu->live)
    {
        menu->consoleWindowIsFocused = (GetConsoleWindow() == GetForegroundWindow());


        if(GetAsyncKeyState(VK_ESCAPE) & 0x1 && menu->consoleWindowIsFocused)
        {
            menu->live = FALSE;
        }

        if (GetAsyncKeyState(VK_UP) & 0x1 && menu->consoleWindowIsFocused)
        {
            if (menu->currentSelection > 0)
            {
                menu->currentSelection--;
                redraw(menu);
            }
        }

        if (GetAsyncKeyState(VK_DOWN) & 0x1 && menu->consoleWindowIsFocused)
        {
            if (menu->currentMenu == WINDOWS && menu->currentSelection < menu->windows.count - 1)
            {
                menu->currentSelection++;
                redraw(menu);
            }
            else if (menu->currentMenu == MAIN && menu->currentSelection < menu->mainMenu.maxSelections - 1)
            {
                menu->currentSelection++;
                redraw(menu);
            }
        }

        if(GetAsyncKeyState(VK_RETURN) & 0x1 && menu->consoleWindowIsFocused)
        {
            switch(menu->currentMenu)
            {
            case MAIN:
                if (menu->currentSelection == 0)
                {
                    openWindows(&menu->windows);
                    menu->currentMenu = WINDOWS;
                    redraw(menu);
                }
                else if (menu->currentSelection == 1)
                {
                    menu->active = !menu->active;
                    _beginthread(cursorLock, 0, menu);
                    redraw(menu);
                }
                else if (menu->currentSelection == 2)
                    exit(0);
                break;
            case WINDOWS:
                menu->selectedWindow = menu->windows.windows[menu->currentSelection];
                menu->currentSelection = 0;
                menu->currentMenu = MAIN;
                redraw(menu);
                break;
            default:
                break;
            }
        }

        Sleep(1);
    }
}

