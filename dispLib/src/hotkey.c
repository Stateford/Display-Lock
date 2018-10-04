#include "hotkey.h"

void initHotkey(HWND hWnd, SETTINGS *settings)
{
    for (int i = 0; i < settings->numOfHotkeys; i++)
    {
        RegisterHotKey(hWnd, settings->hotkeys[i].id, settings->hotkeys[i].modifiers, settings->hotkeys[i].key);
    }
}


BOOL addHotkey(HWND hWnd, SETTINGS *settings, int msgID, int modifier, unsigned int key)
{
    /*
    if (settings->numOfHotkeys > 1)
        return FALSE;
     */
    UnregisterHotKey(hWnd, msgID);
    BOOL result = RegisterHotKey(hWnd, msgID, modifier, key);
    
    // store in setting
    if (result)
    {
        settings->hotkeys[0] = (HOTKEY) { msgID, modifier, key };
        //settings->numOfHotkeys++;
    }

    return result;
}


BOOL removeHotkey(HWND hWnd, SETTINGS *settings, int index)
{
    if (index > settings->numOfHotkeys)
        return FALSE;

    // unregister the hotkey
    BOOL result = UnregisterHotKey(hWnd, settings->hotkeys[index].id);

    // remove hotkeys from settings
    if (result)
    {
        settings->hotkeys[index] = (HOTKEY) { 0 };
        //settings->numOfHotkeys--;
    }

    return result;
}

void cleanupHotkeys(HWND hWnd, SETTINGS *settings)
{
    for (unsigned int i = 0; i < settings->numOfHotkeys; i++)
    {
        UnregisterHotKey(hWnd, settings->hotkeys[i].id);
    }
}