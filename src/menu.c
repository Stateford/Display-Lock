#include "menu.h"
#include <stdio.h>
#include <Windows.h>

void start()
{
    Menu menu;
    menu.currentMenu = DEFAULT;
    menu.windows.count = 0;
    menu.currentSelection = 0;

    menu.hstdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(menu.hstdin, &menu.mode);
    SetConsoleMode(menu.hstdin, 0);
    menu.hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    openWindows(&menu.windows);
    display(&menu);
}

void draw(Menu *menu)
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

    draw(menu);


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
                draw(menu);
            }
        }

        if (GetAsyncKeyState(VK_DOWN) & 0x1 && menu->consoleWindowIsFocused)
        {
            if (menu->currentSelection < menu->windows.count - 1)
            {
                menu->currentSelection++;
                draw(menu);
            }
        }

        Sleep(1);
    }
}

