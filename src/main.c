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
// test.cpp : Defines the entry point for the application.
//
#include "resources\resource.h"
#include "header.h"
#include "applications.h"
#include "common.h"
#include <commdlg.h>
#include "ui.h"
#include <stdio.h>
#include "app_settings.h"
#include "update_dialog.h"
#include "about_dialog.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                     // current instance
WCHAR szTitle[MAX_LOADSTRING];       // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name
WINDOW_VIEW_CONTROLS windowControls = {0};
SETTINGS settings = {0}; // application settings
ARGS args = {0};
BOOL running = FALSE;
BOOL applicationRunning = TRUE;
VERSION gVersion = {0};
BOOL initalUpdate = FALSE;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MainWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK windowViewProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK settingsViewProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK applicationsViewProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK about(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK updateProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK appSettingsProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TEST);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
                             CW_USEDEFAULT, 0, 300, 240, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static NOTIFYICONDATA sysTray = {0};

    switch (message)
    {

    case WM_CREATE:
        if (settings.checkUpdateStartup)
        {
            getVersion(&gVersion);
            initalUpdate = !compareVersion(&gVersion);
        }

        CreateDialog(NULL, MAKEINTRESOURCE(IDD_MAIN_VIEW), hWnd, MainWindow);
        invokeReadSettings(&settings);
        notifyInit(hWnd, &sysTray);
        Shell_NotifyIcon(NIM_ADD, &sysTray);
        Shell_NotifyIcon(NIM_SETVERSION, &sysTray);

        HANDLE mutex = CreateMutex(NULL, FALSE, APPLICATION_MUTEX_NAME);
        ReleaseMutex(mutex);
        CloseHandle(mutex);

        break;

    case NOTIFY_MSG:
        switch (lParam)
        {
        case WM_LBUTTONDBLCLK:
            showMainWindow(hWnd, &sysTray);
            break;
        case WM_RBUTTONDOWN:
            showContext(hWnd, &windowControls.windows, settings, running);
            break;
        default:
            break;
        }
        break;

    case WM_SIZE:
        switch (wParam)
        {
        case SIZE_MINIMIZED:
            ShowWindow(hWnd, FALSE);
            break;
        default:
            break;
        }
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        // item from submenu in notifications
        if (wParam >= SUBMENU_ITEM_BASE && wParam <= (SUBMENU_ITEM_BASE + 100))
        {
            settings.foreground = TRUE;
            windowsButtonStart(&windowControls, &args, &running, ((int)wParam - SUBMENU_ITEM_BASE));
            settings.foreground = FALSE;
        }

        switch (wmId)
        {
        case ID_CONTEXTMENU_STOP:
            notifyChildWindows(hWnd, IDC_BUTTON_WINDOWS_STOP);
            break;
        case ID_CONTEXTMENU_SHOWWINDOW:
            ShowWindow(hWnd, TRUE);
            break;
        case ID_CONTEXTMENU_EXIT:
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        case ID_CONTEXTMENU_SETTINGS_MINIMIZE:
            settings.minimize = !settings.minimize;
            notifyChildWindows(hWnd, NOTIFY_SETTINGS_CHANGED);
            break;
        case ID_CONTEXTMENU_SETTINGS_FOREGROUND:
            settings.foreground = !settings.foreground;
            notifyChildWindows(hWnd, NOTIFY_SETTINGS_CHANGED);
            break;
        case ID_CONTEXTMENU_SETTINGS_BORDERLESS:
            settings.borderless = !settings.borderless;
            notifyChildWindows(hWnd, NOTIFY_SETTINGS_CHANGED);
            break;
        case ID_CONTEXTMENU_SETTINGS_FULLSCREEN:
            settings.fullScreen = !settings.fullScreen;
            notifyChildWindows(hWnd, NOTIFY_SETTINGS_CHANGED);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, about);
            break;
        case ID_HELP_CHECKFORUPDATES:
        {
            if (gVersion.verArr[0] == 0)
                getVersion(&gVersion);

            if (!compareVersion(&gVersion))
                DialogBox(hInst, MAKEINTRESOURCE(IDD_UPDATE), hWnd, updateProc);

            break;
        }
        case IDM_EXIT:
        {
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);

        if (initalUpdate)
        {
            SendMessage(hWnd, WM_COMMAND, LOWORD(ID_HELP_CHECKFORUPDATES), 0);
            initalUpdate = FALSE;
        }
    }
    break;
    case WM_CLOSE:
        Shell_NotifyIcon(NIM_DELETE, &sysTray);
        shutDown(settings);
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK MainWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static MAIN_WINDOW_CONTROLS mainWindowControls;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        mainWindowInit(hDlg, &mainWindowControls);

        // initalize the tab views
        mainWindowControls.windowView = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_WINDOWS_VIEW), mainWindowControls.tabCtrl, windowViewProc);
        mainWindowControls.settingsView = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SETTINGS_VIEW), mainWindowControls.tabCtrl, settingsViewProc);
        mainWindowControls.applicationView = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_APPLICATIONS_VIEW), mainWindowControls.tabCtrl, applicationsViewProc);

        // show the default window
        ShowWindow(mainWindowControls.windowView, SW_SHOW);
        ShowWindow(mainWindowControls.settingsView, SW_HIDE);
        ShowWindow(mainWindowControls.applicationView, SW_HIDE);
        args.controls = mainWindowControls;

        return (INT_PTR)TRUE;

    case WM_NOTIFY:

        switch (((LPNMHDR)lParam)->code)
        {
        // Handles tab switching
        case TCN_SELCHANGE:
            switch (TabCtrl_GetCurSel(((LPNMHDR)lParam)->hwndFrom))
            {
            case WINDOW_VIEW: // change this to a const or define
                ShowWindow(mainWindowControls.windowView, SW_SHOW);
                ShowWindow(mainWindowControls.settingsView, SW_HIDE);
                ShowWindow(mainWindowControls.applicationView, SW_HIDE);
                break;
            case SETTINGS_VIEW:
                ShowWindow(mainWindowControls.windowView, SW_HIDE);
                ShowWindow(mainWindowControls.settingsView, SW_SHOW);
                ShowWindow(mainWindowControls.applicationView, SW_HIDE);
                break;
            case APPLICATION_VIEW:
                ShowWindow(mainWindowControls.windowView, SW_HIDE);
                ShowWindow(mainWindowControls.settingsView, SW_HIDE);
                ShowWindow(mainWindowControls.applicationView, SW_SHOW);
                break;
            }
            break;
        }
        break;

    case WM_COMMAND:
        break;
    default:
        return DefWindowProc(hDlg, message, wParam, lParam);
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK windowViewProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // struct that contains controls
    static MENU menu;
    static HWND parent;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        initalizeWindowView(hDlg, &menu, &settings, &running, &windowControls, &args);
        parent = GetParent(GetParent(GetParent(hDlg)));
        args.hWnd = parent;

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_COMBO_WINDOW:
            switch (HIWORD(wParam))
            {
            case CBN_DROPDOWN:
                // when the dropdown is pressed, populate the field
                menu.updateComboBox(windowControls.comboBox, &windowControls.windows, openWindows);
                break;
            default:
                break;
            }
            break;

        case IDC_BUTTON_WINDOWS_STOP:
            windowsButtonStop(menu, &windowControls);
            break;

        case IDC_BUTTON_WINDOWS_START:
        {
            int windowSelection = (int)SendMessage(windowControls.comboBox, CB_GETCURSEL, 0, 0);
            windowsButtonStart(&windowControls, &args, &running, windowSelection);
            break;
        }
        default:
            break;
        }
        break;
    case WM_DESTROY:
        menu.closeThread(windowControls.clipThread, &running);
        break;
    default:
        return DefWindowProc(hDlg, message, wParam, lParam);
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK settingsViewProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static SETTINGS_VIEW_CONTROLS settingsControls;
    static SETTINGS previousSettings;
    static HWND parent;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_SHOWWINDOW:
        EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), FALSE);
        settingsShowWindow(settingsControls, &settings, &previousSettings, running);

        // when the window is being hidden, reset settings
        // when the window is being shown, copy the previous settings
        if ((BOOL)wParam == FALSE)
            settings = previousSettings;
        else if ((BOOL)wParam == TRUE)
            previousSettings = settings;

        break;

    case WM_INITDIALOG:
        initalizeSettings(hDlg, &settingsControls);
        parent = GetParent(GetParent(GetParent(hDlg)));
        return (INT_PTR)TRUE;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case NOTIFY_SETTINGS_CHANGED:
            previousSettings = settings;
            settingsControls.settingsChanged = TRUE;
            settingsShowWindow(settingsControls, &settings, &previousSettings, running);
            break;
        case IDC_CHECK_SETTINGS_BORDERLESS:
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), TRUE);
            settingsControls.settingsChanged = TRUE;
            settings.borderless = (BOOL)SendMessage(settingsControls.borderless, BM_GETCHECK, 0, 0);
            break;
        case IDC_CHECK_SETTINGS_FOREGROUND:
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), TRUE);
            settingsControls.settingsChanged = TRUE;
            settings.foreground = (BOOL)SendMessage(settingsControls.foreground, BM_GETCHECK, 0, 0);
            break;
        case IDC_CHECK_SETTINGS_FULL_SCREEN:
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), TRUE);
            settingsControls.settingsChanged = TRUE;
            settings.fullScreen = (BOOL)SendMessage(settingsControls.fullScreen, BM_GETCHECK, 0, 0);
            break;
        case IDC_CHECK_SETTINGS_MINIMIZE:
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), TRUE);
            settingsControls.settingsChanged = TRUE;
            settings.minimize = (BOOL)SendMessage(settingsControls.minimize, BM_GETCHECK, 0, 0);
            break;

        case IDC_CHECK_STARTUP_UPDATES:
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), TRUE);
            settingsControls.settingsChanged = TRUE;
            settings.checkUpdateStartup = (BOOL)SendMessage(settingsControls.checkForUpdatesStartup, BM_GETCHECK, 0, 0);
            break;

        case IDC_HOTKEY:
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), TRUE);
            settingsControls.settingsChanged = TRUE;
            break;

        case IDC_BUTTON_SETTINGS_SAVE:
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), FALSE);
            settingsSave(parent, settingsControls, settings, &previousSettings);
            break;

        case IDC_BUTTON_SETTINGS_CANCEL:
            settingsCancel(settingsControls, &settings, previousSettings);
            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SETTINGS_SAVE), FALSE);
            break;
        default:
            break;
        }
        break;
    }

    default:
        return DefWindowProc(hDlg, message, wParam, lParam);
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK applicationsViewProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND parent;
    static APPLICATION_VIEW_CONTROLS controls;
    static APPLICATION_SETTINGS settings;
    static APPLICATION_LIST applicationList;
    static APPLICATION_ARGS args;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_SHOWWINDOW:
        break;

    case WM_INITDIALOG:
        parent = GetParent(GetParent(GetParent(hDlg)));
        initApplicationView(hDlg, &controls);
        controls.runningClip = &applicationRunning;
        applicationRunning = TRUE;
        initApplicationList(&applicationList);

        args.applicationList = &applicationList;
        args.clipRunning = &applicationRunning;

        startApplicationThread(&controls.clipThread, cursorLockApplications, (void *)&args);

        EnableWindow(controls.settingsButton, FALSE);
        EnableWindow(controls.removeButton, FALSE);

        for (int i = 0; i < applicationList.count; i++)
            SendMessage(controls.listView, LB_ADDSTRING, 0, (LPARAM)applicationList.applications[i].application_name);

        return (INT_PTR)TRUE;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_LIST_PROGRAMS:
            int result = SendMessage(controls.listView, LB_GETCURSEL, 0, 0);
            if (result != LB_ERR)
            {
                EnableWindow(controls.settingsButton, TRUE);
                EnableWindow(controls.removeButton, TRUE);
            }
            else
            {
                EnableWindow(controls.settingsButton, FALSE);
                EnableWindow(controls.removeButton, FALSE);
            }
            break;
        case IDC_BTN_APP_ADD:
            wchar_t filename[MAX_PATH];

            OPENFILENAMEW ofn;
            ZeroMemory(&filename, sizeof(filename));
            ZeroMemory(&ofn, sizeof(ofn));

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = L"Executable\0*.exe\0";
            ofn.lpstrFile = filename;
            ofn.nMaxFile = sizeof(filename);
            ofn.lpstrTitle = L"Select an application";
            ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn))
            {

                HANDLE mutex = CreateMutex(NULL, FALSE, APPLICATION_MUTEX_NAME);
                DWORD result = WaitForSingleObject(mutex, INFINITE);

                if ((result == 0) || (result == WAIT_ABANDONED))
                {

                    APPLICATION_SETTINGS application;
                    ZeroMemory(&application, sizeof(application));
                    createApplicationSettings(ofn.lpstrFile, &application);

                    if (addApplication(&applicationList, application))
                        SendMessage(controls.listView, LB_ADDSTRING, 0, (LPARAM)application.application_name);
                }
                ReleaseMutex(mutex);
                CloseHandle(mutex);
            }

            break;
        case IDC_BTN_APP_RMV:
        {

            int current_selected = 0;
            int result = SendMessage(controls.listView, LB_GETCURSEL, (WPARAM)&current_selected, 0);
            if (result != LB_ERR)
            {

                HANDLE mutex = CreateMutex(NULL, FALSE, APPLICATION_MUTEX_NAME);
                DWORD result = WaitForSingleObject(mutex, INFINITE);

                if ((result == 0) || (result == WAIT_ABANDONED))
                {
                    SendMessage(controls.listView, LB_DELETESTRING, current_selected, 0);
                    removeApplication(&applicationList, current_selected);
                }
                ReleaseMutex(mutex);
                CloseHandle(mutex);
            }

            result = SendMessage(controls.listView, LB_GETCURSEL, 0, 0);
            if (result != LB_ERR)
            {
                EnableWindow(controls.settingsButton, TRUE);
                EnableWindow(controls.removeButton, TRUE);
            }
            else
            {
                EnableWindow(controls.settingsButton, FALSE);
                EnableWindow(controls.removeButton, FALSE);
            }
        }
        break;
        case IDC_BTN_APP_SETTINGS:
        {
            int current_selected = 0;
            int result = SendMessage(controls.listView, LB_GETCURSEL, (WPARAM)&current_selected, 0);

            if (result != LB_ERR)
            {

                HANDLE mutex = CreateMutex(NULL, FALSE, APPLICATION_MUTEX_NAME);
                DWORD result = WaitForSingleObject(mutex, INFINITE);

                if ((result == 0) || (result == WAIT_ABANDONED))
                {
                    APPLICATION_SETTINGS *settings = &applicationList.applications[current_selected];
                    DialogBoxParam(hInst, MAKEINTRESOURCE(IDC_APP_SETTINGS), hDlg, appSettingsProc, (LPARAM)settings);
                }
                ReleaseMutex(mutex);
                CloseHandle(mutex);
            }
        }
        break;
        default:
            break;
        }
        break;
    }
    case WM_DESTROY:
    {
        closeApplicationThread(controls.clipThread, args.clipRunning);
        closeApplicationList(&applicationList);

        HANDLE mutex = CreateMutex(NULL, FALSE, APPLICATION_MUTEX_NAME);
        if (mutex != NULL)
        {
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    }
    default:
        return DefWindowProc(hDlg, message, wParam, lParam);
    }
    return (INT_PTR)FALSE;
}
