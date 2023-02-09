#include "CppUnitTest.h"

extern "C" {
#include "settings.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
    // settings.h
	TEST_CLASS(Settings)
	{
	public:
        // default settings
        TEST_METHOD(DefaultSettings)
        {
            Logger::WriteMessage("defaultSettings()...");
            SETTINGS settings;
            defaultSettings(&settings, L"5");

            // It's impossible to test the default version, as it's determined at run time
            Assert::AreEqual(settings.header, "DLOCK");
            Assert::AreEqual(settings.version, 5);
            Assert::AreEqual(settings.borderless, FALSE);
            Assert::AreEqual(settings.foreground, FALSE);
            Assert::AreEqual(settings.fullScreen, FALSE);
            Assert::AreEqual(settings.minimize, TRUE);
        }

        TEST_METHOD(CheckVersion)
        {
            Logger::WriteMessage("CheckVersion()...");

            SETTINGS settings;
            defaultSettings(&settings, L"5");
            Assert::AreEqual(checkVersion(&settings, L"5"), TRUE);
            Assert::AreEqual(checkVersion(&settings, L"0005"), TRUE);
            Assert::AreEqual(checkVersion(&settings, L"15"), FALSE);
            Assert::AreEqual(checkVersion(&settings, L"A"), FALSE);
            Assert::AreEqual(checkVersion(&settings, L"FOO"), FALSE);
            Assert::AreEqual(checkVersion(&settings, L"F5"), FALSE);
            
            strcpy_s(settings.header, 6, "5A!");
            Assert::AreEqual(checkVersion(&settings, L"5"), FALSE);
            strcpy_s(settings.header, 6, "FOO");
            Assert::AreEqual(checkVersion(&settings, L"5"), FALSE);
            strcpy_s(settings.header, 6, "");
            Assert::AreEqual(checkVersion(&settings, L"5"), FALSE);
        }

        TEST_METHOD(FindPath)
        {
            Logger::WriteMessage("findPath()...");

            wchar_t path[MAX_PATH];
            BOOL res = findPath(path, sizeof(path));

            Assert::AreEqual(res, TRUE);
            Logger::WriteMessage(path);
        }

		TEST_METHOD(ReadSettings)
		{
            Logger::WriteMessage("readSettings()...");

            SETTINGS settings = { 0 };
            BOOL result;
            result = readSettings(&settings, L"5", L"..\\..\\tests\\readTests\\test1.DLOCK");
            Assert::AreEqual(result, TRUE, L"readSettings boolean, expected TRUE");
            Assert::AreEqual(strcmp(settings.header, "DLOCK"), 0, L"settings header, expected DLOCK");
            Assert::AreEqual(settings.version, 5, L"settings version, expected 5");
            Assert::AreEqual(settings.minimize, FALSE, L"settings minimize, expected FALSE");
            Assert::AreEqual(settings.foreground, FALSE, L"settings foreground, expected FALSE");
            Assert::AreEqual(settings.borderless, FALSE, L"settings borderless, expected FALSE");
            Assert::AreEqual(settings.fullScreen, FALSE, L"settings fullScreen, expected FALSE");
		}

        TEST_METHOD(WriteSettings)
        {
            Logger::WriteMessage("writeSettings()...");
            
            BOOL result;

            SETTINGS settings = {"DLOCK", 5, TRUE, TRUE, FALSE, FALSE};

            result = writeSettings(settings, L"output.DLOCK");
            Assert::AreEqual(result, TRUE, L"first test...");

            settings.version = 0;

            result = writeSettings(settings, L"output.DLOCK");
            Assert::AreEqual(result, FALSE, L"second test...");

            settings.version = 5;

            strcpy_s(settings.header, 6, "FOOBA");

            result = writeSettings(settings, L".\\output.DLOCK");
            Assert::AreEqual(result, FALSE, L"third test...");

            DeleteFile(L"output.DLOCK");
        }
	};
}