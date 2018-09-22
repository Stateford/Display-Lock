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
            
            strcpy_s(settings.header, 4, "5A!");
            Assert::AreEqual(checkVersion(&settings, L"5"), FALSE);
            strcpy_s(settings.header, 4, "FOO");
            Assert::AreEqual(checkVersion(&settings, L"5"), FALSE);
            strcpy_s(settings.header, 4, "");
            Assert::AreEqual(checkVersion(&settings, L"5"), FALSE);


        }

        TEST_METHOD(FindPath)
        {
            Logger::WriteMessage("findPath()...");

            wchar_t path[MAX_PATH];
            BOOL res = findPath(path);

            Assert::AreEqual(res, TRUE);
            Logger::WriteMessage(path);
        }

		TEST_METHOD(ReadSettings)
		{
            Logger::WriteMessage("readSettings()...");
			// TODO: Your test code here
		}

        TEST_METHOD(WriteSettings)
        {
            Logger::WriteMessage("writeSettings()...");
            // TODO: test code here
        }

	};
}