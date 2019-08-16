/*
    Display Lock
    Copyright (C) 2018 Dimitrios Apostal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "update.h"
#include <stdlib.h>
#include <Windows.h>
#include <winhttp.h>


void resizeString(STRING* string, int size) {
    const int newSize = string->size + size;
    char* data = string->data;
    if (string->data != NULL) {
        string->data = (char*)realloc(string->data, ((size_t)newSize + 1));
        if (string->data == NULL)
            free(data);
        string->size = size;
    }
}

void parseVersionString(VERSION* version, STRING* response)
{
    int count = 0;
    char major[5] = "";
    char minor[5] = "";
    char patch[10] = "";

    memset(major, 0, sizeof(major));
    memset(minor, 0, sizeof(minor));
    memset(patch, 0, sizeof(patch));

    int position = 0;

    for (unsigned int i = 0; i <= response->size; i++) {
        if (count == 0)
        {
            if (response->data[i] == '.')
            {
                count++;
                major[position] = '\0';
                position = 0;
                continue;
            }
            major[position] = response->data[i];
            position++;
        }
        else if (count == 1)
        {
            if (response->data[i] == '.')
            {
                count++;
                minor[position] = '\0';
                position = 0;
                continue;
            }
            minor[position] = response->data[i];
            position++;
        }
        else
        {
            if (i == response->size)
            {
                patch[position] = '\0';
                break;
            }
            else
            {
                patch[position] = response->data[i];
                position++;
            }
        }
    }
    version->major = atoi(major);
    version->minor = atoi(minor);
    version->patch = atoi(patch);
}

void getLatestVersion(VERSION* version)
{
    HINTERNET hSession;
    HINTERNET hConnect;
    HINTERNET hRequest;
    BOOL bResults;
    DWORD dwSize;
    DWORD dwDownloaded;

    STRING response;

    int count = 0;
    response.size = 0;
    response.data = (char*)calloc(1024, sizeof(char));
    response.data[0] = '\0';

    memset(&hSession, 0, sizeof(HINTERNET));
    memset(&hConnect, 0, sizeof(HINTERNET));
    memset(&hRequest, 0, sizeof(HINTERNET));

    hSession = WinHttpOpen(
        TEXT("Example UserAgent/1.0"),
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );

    if (!hSession)
        goto CLEANUP;

    hConnect = WinHttpConnect(
        hSession,
        TEXT("stateford.github.io"),
        INTERNET_DEFAULT_HTTPS_PORT,
        0
    );

    if (!hConnect)
        goto CLEANUP;

    hRequest = WinHttpOpenRequest(
        hConnect,
        TEXT("GET"),
        TEXT("Display-Lock/version"),
        NULL,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE
    );

    if (!hRequest)
        goto CLEANUP;

    bResults = WinHttpSendRequest(
        hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS,
        0,
        0,
        0,
        0,
        0
    );

    if (bResults)
    {
        if (WinHttpReceiveResponse(hRequest, NULL))
        {
            do
            {
                // Check for available data.
                dwSize = 0;
                bResults = WinHttpQueryDataAvailable(hRequest, &dwSize);
                if (!bResults)
                {
                    break;
                }

                if (dwSize == 0)
                    break;

                do
                {
                    // Allocate space for the buffer.
                    DWORD dwOffset = response.size;
                    resizeString(&response, dwSize);

                    // Read the data.
                    bResults = WinHttpReadData(hRequest, &response.data[dwOffset], dwSize, &dwDownloaded);
                    if (!bResults)
                    {
                        dwDownloaded = 0;
                    }

                    resizeString(&response, dwOffset + dwDownloaded);

                    if (dwDownloaded == 0)
                        break;

                    dwSize -= dwDownloaded;
                } while (TRUE);
            } while (TRUE);
        }
        response.data[response.size] = '\0';
    }

    parseVersionString(version, &response);


CLEANUP:
    if (hConnect)
        WinHttpCloseHandle(hConnect);
    if (hSession)
        WinHttpCloseHandle(hSession);
    if (hRequest)
        WinHttpCloseHandle(hRequest);
    if (response.data != NULL)
        free(response.data);
}

BOOL compareVersion(const VERSION* current)
{
    VERSION latest;
    memset(&latest, 0, sizeof(latest));

    getLatestVersion(&latest);

    if (current->major < latest.major)
        return FALSE;
    else if (current->minor < latest.minor)
        return FALSE;
    else if (current->patch < latest.patch)
        return FALSE;

    return TRUE;
}