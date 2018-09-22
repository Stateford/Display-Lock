#include "ui.h"
#include <stdio.h>

BOOL getVersionString(wchar_t * buffer, int bufferSize)
{
    wchar_t version[2048];
    wchar_t fileName[MAX_PATH];
    GetModuleFileName(NULL, fileName, MAX_PATH);

    DWORD dwVersionBufferSize = GetFileVersionInfoSizeW(fileName, NULL);
    BOOL result = GetFileVersionInfoW(fileName, 0, dwVersionBufferSize, (LPVOID)version);

    if (!result)
        return FALSE;

 
    UINT size;
    VS_FIXEDFILEINFO *verInfo = NULL;
    VerQueryValue(version, L"\\", (LPVOID)&verInfo, &size);
    int major = HIWORD(verInfo->dwFileVersionMS);
    int minor = LOWORD(verInfo->dwFileVersionMS);
    int build = HIWORD(verInfo->dwFileVersionLS);
    int revision = LOWORD(verInfo->dwFileVersionLS);

    
    swprintf(buffer, bufferSize, L"Version: %d.%d.%d.%d", major, minor, build, revision);

    return TRUE;
}
