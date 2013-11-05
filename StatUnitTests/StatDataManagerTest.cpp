#include "stdafx.h"
#include "CppUnitTest.h"
#include "GenHeader.h"
/////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StatUnitTests
{
	TEST_CLASS(StatDataManagerTest)
	{
	private:
		static std::unique_ptr<PredStatDataManagerType> m_man;
	public:
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("In Class Initialize");
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			Logger::WriteMessage("In Class Cleanup");
		}
		TEST_METHOD(TestMethod1)
		{
			// TODO: Ici, votre code de test
		}

	};// classStatDataManagerTest
	std::unique_ptr<PredStatDataManagerType> StatDataManagerTest::m_man;
}// namespace
