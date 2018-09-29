#include "hotkey.h"

void initHotkey(SETTINGS *settings)
{

}

void createTable(MAIN_WINDOW_CONTROLS *mainControls, SETTINGS *settings)
{
    mainControls->userHotkeys = CreateAcceleratorTable(settings->hotkeys, settings->hotkeySize);
}

void addHotkey(SETTINGS *settings)
{

}

void removeHotkey(MAIN_WINDOW_CONTROLS *mainControls, SETTINGS *settings)
{
    // dest
    DestroyAcceleratorTable(mainControls->userHotkeys);
    

}