#include "stdafx.h"
#include "CppUnitTest.h"
#include "GenHeader.h"
/////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StatUnitTests
{		
#ifdef WIN64
	const char *TEST_DATABASE_FILENAME= "..\\..\\StatunitTests\\testdata\\testdb.db";
	const char *TEST_IMPORT_FILENAME = "..\\..\\StatunitTests\\testdata\\data_2013.txt";
#else
	const char *TEST_DATABASE_FILENAME= "..\\StatunitTests\\testdata\\testdb.db";
	const char *TEST_IMPORT_FILENAME = "..\\StatunitTests\\testdata\\data_2013.txt";
#endif // WIN64
#ifndef INTRA_USE_WSTRING
	const char *TEST_DATABASE_SIGLE = "TESTSIGLE";
	const char *TEST_NA = "na";
	const char TEST_DELIM = '\t';
#else
	const wchar_t *StatDataManagerTest::TEST_DATABASE_SIGLE = L"TESTSIGLE";
	const wchar_t *StatDataManagerTest::TEST_NA = L"na";
	const wchar_t StatDataManagerTest::TEST_DELIM = L'\t';
#endif
	TEST_MODULE_INITIALIZE(ModuleInitialize)
	{
		Logger::WriteMessage("In Module Initialize");
		bool bRet = false;
		{
			std::ifstream in(TEST_DATABASE_FILENAME);
			bRet = in.is_open();
		}
		if (!bRet){
			std::string databasename(TEST_DATABASE_FILENAME);
			PredStatDataManagerType oMan;
#ifdef INTRA_USE_WSTRING
			bRet = oMan.open(databasename);
			Assert::IsTrue(bRet);
			std::string s(TEST_IMPORT_FILENAME);
			std::wstring sk(s.length(),L' ');
			std::copy(s.begin(),s.end(),sk.begin());
			std::wstring na(TEST_NA);
			wchar_t delim(TEST_DELIM);
			bRet = oMan.process_data(inFile, datasetSigle,delim,na);
#else
			
			bRet = oMan.open(databasename);
			Assert::IsTrue(bRet);
			std::ifstream inFile(TEST_IMPORT_FILENAME);
			Assert::IsTrue(inFile.is_open());
			std::string na(TEST_NA);
			std::string sigle(TEST_DATABASE_SIGLE);
			char delim(TEST_DELIM);
			bRet = oMan.process_data(inFile, sigle,delim,na);
#endif // INTRA_USE_WSTRING
			Assert::IsTrue(bRet);
		}// not exists
		Logger::WriteMessage("\n");
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		Logger::WriteMessage("In Module Cleanup");
		Logger::WriteMessage("\n");
	}

}