#include "gtest/gtest.h"

extern "C" {
#include "settings.h"
}

// #define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
// #include <Windows.h>
#include <iostream>
#include <filesystem>
#include <source_location>


namespace {

    // settings.h
    TEST(SettingsTest, DefaultSettings)
    {
        SETTINGS settings;
        defaultSettings(&settings, L"5");

        // It's impossible to test the default version, as it's determined at run time
        ASSERT_STREQ(settings.header, "DLOCK");
        ASSERT_EQ(settings.version, 5);
        ASSERT_EQ(settings.borderless, false);
        ASSERT_EQ(settings.foreground, false);
        ASSERT_EQ(settings.fullScreen, false);
        ASSERT_EQ(settings.minimize, true);
    }

    TEST(SettingsTest, CheckVersion)
    {
        SETTINGS settings;

        defaultSettings(&settings, L"5");
        ASSERT_EQ(checkVersion(&settings, L"5"), true);
        ASSERT_EQ(checkVersion(&settings, L"0005"), true);
        ASSERT_EQ(checkVersion(&settings, L"15"), false);
        ASSERT_EQ(checkVersion(&settings, L"A"), false);
        ASSERT_EQ(checkVersion(&settings, L"FOO"), false);
        ASSERT_EQ(checkVersion(&settings, L"F5"), false);
        
        strcpy_s(settings.header, 6, "5A!");
        ASSERT_EQ(checkVersion(&settings, L"5"), false);
        strcpy_s(settings.header, 6, "FOO");
        ASSERT_EQ(checkVersion(&settings, L"5"), false);
        strcpy_s(settings.header, 6, "");
        ASSERT_EQ(checkVersion(&settings, L"5"), false);
    }

    TEST(SettingsTest, FindPath)
    {
        wchar_t path[MAX_PATH];
        BOOL res = findPath(path);

        ASSERT_EQ(res, true);
    }

    TEST(SettingsTest, ReadSettings)
    {
        SETTINGS settings = { 0 };
        BOOL result;

        result = readSettings(&settings, L"5", L"test1.DLOCK");
        ASSERT_EQ(result, true);
        ASSERT_EQ(strcmp(settings.header, "DLOCK"), 0);
        ASSERT_EQ(settings.version, 5);
        ASSERT_EQ(settings.minimize, false);
        ASSERT_EQ(settings.foreground, false);
        ASSERT_EQ(settings.borderless, false);
        ASSERT_EQ(settings.fullScreen, false);
    }

    TEST(SettingsTest, WriteSettings)
    {
        BOOL result;

        SETTINGS settings = {"DLOCK", 5, true, true, false, false};

        result = writeSettings(settings, L"output.DLOCK");
        ASSERT_EQ(result, true);

        settings.version = 0;

        result = writeSettings(settings, L"output.DLOCK");
        ASSERT_EQ(result, false);

        settings.version = 5;

        strcpy_s(settings.header, 6, "FOOBA");

        result = writeSettings(settings, L".\\output.DLOCK");
        ASSERT_EQ(result, false);

        DeleteFileW(L"output.DLOCK");
    }
}
