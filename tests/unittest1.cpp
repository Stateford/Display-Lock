#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{		
	TEST_CLASS(Settings)
	{
	public:
		
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