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
			std::string databasename(TEST_DATABASE_FILENAME);
			m_man.reset(new PredStatDataManagerType());
			PredStatDataManagerType *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			bool bRet = pMan->open(databasename);
			Assert::IsTrue(bRet);
			Assert::IsTrue(pMan->is_valid());
			Logger::WriteMessage("");
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			Logger::WriteMessage("In Class Cleanup");
			m_man.reset();
			Logger::WriteMessage("");
		}
		TEST_METHOD(TestGetAllDatasets)
		{
			PredStatDataManagerType *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			Assert::IsTrue(pMan->is_valid());
			std::vector<DatasetType> oVec;
			bool bRet = pMan->get_all_datasets(oVec);
			Assert::IsTrue(bRet);
			size_t n = oVec.size();
			bRet = (n > 0);
			Assert::IsTrue(bRet);
			const DatasetType &oSet = *(oVec.begin());
			int nId = oSet.id();
			Assert::IsTrue(nId != 0);
		}

	};// classStatDataManagerTest
	std::unique_ptr<PredStatDataManagerType> StatDataManagerTest::m_man;
}// namespace
