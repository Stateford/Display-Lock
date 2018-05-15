#pragma once
#include "../header.h"

// HOTKEY
// stores information on hotkeys
typedef struct HOTKEY
{
    BYTE HOTKEY;
    UINT8 MODIFIER_COUNT;
    BYTE HOTKEY_MODIFIERS[5];
} HOTKEY;

// TODO: create HOTKEY listeners

int __stdcall hotkeyListener(void* arguments);