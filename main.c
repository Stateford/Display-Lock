#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <Windows.h>

#include "src\win.h"


int main()
{
    WINDOWLIST windows;
    windows.count = 0;

    openWindows(&windows);

    for (int i = 0; i < windows.count; i++)
    {
        printf("%s\n", windows.windows[i].title);
    }

    return 0;
}