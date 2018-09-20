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
            defaultSettings(&settings);

            // It's impossible to test the default version, as it's determined at run time
            Assert::AreEqual(settings.header, "DLOCK");
            Assert::AreEqual(settings.borderless, FALSE);
            Assert::AreEqual(settings.foreground, FALSE);
            Assert::AreEqual(settings.fullScreen, FALSE);
            Assert::AreEqual(settings.minimize, TRUE);
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