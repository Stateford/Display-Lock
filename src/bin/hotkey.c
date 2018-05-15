#include "hotkey.h"


int __stdcall hotkeyListener(void* arguments)
{
    SETTINGS *settings = (SETTINGS*)arguments;



    _endthreadex(1);	// end the thread
    return 1;
}
