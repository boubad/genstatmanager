/*
 * IntraTestEnv.cpp
 *
 *  Created on: 28 oct. 2013
 *      Author: boubad
 */
#include "IntraTestEnv.h"
//////////////////////////////
#ifndef INTRA_USE_WSTRING
#ifdef __GNUC__
const char *IntraTestEnv::TEST_FILENAME = "./testdata/data_2013.txt";
const char *IntraTestEnv::DATABASE_FILENAME = ":memory";
//const char *IntraTestEnv::DATABASE_FILENAME = "./testdata/testdb.db";
#else
const char *IntraTestEnv::TEST_FILENAME = "..\\teststatdata\\testdata\\data_2013.txt";
const char *IntraTestEnv::DATABASE_FILENAME = "..\\teststatdata\\testdata\\testdb.db";
#endif
const char *IntraTestEnv::TEST_DATASET_SIGLE = "TESTSET";
#else
////////////////////////////////////////////////////
#ifdef __GNUC__
const wchar_t *IntraTestEnv::TEST_FILENAME = L"./testdata/data_2013.txt";
const wchar_t *IntraTestEnv::DATABASE_FILENAME = L":memory";
//const wchar_t *IntraTestEnv::DATABASE_FILENAME = L"./testdata/testdb.db";
#else
const wchar_t *IntraTestEnv::TEST_FILENAME = L"..\\teststatdata\\testdata\\data_2013.txt";
const wchar_t *IntraTestEnv::DATABASE_FILENAME = L"..\\teststatdata\\testdata\\testdb.db";
#endif
const wchar_t *IntraTestEnv::TEST_DATASET_SIGLE = L"TESTSET";
#endif // INTRA_USE_WSTRING
////////////////////////////////
IntraTestEnv *global_intraenv = nullptr;
/////////////////////////////
IntraTestEnv::IntraTestEnv() {
}
IntraTestEnv::~IntraTestEnv() {

}
void IntraTestEnv::SetUp() {
	StringType filename(TEST_FILENAME);
	StringType datasetSigle(TEST_DATASET_SIGLE);
	StringType databasename(DATABASE_FILENAME);
	//
	PredStatDataManager oMan;
	bool bRet = oMan.open(databasename);
	ASSERT_TRUE(bRet);
	//
	bRet = oMan.process_data(filename, datasetSigle);
	ASSERT_EQ(true, bRet);
} // SetUp
void IntraTestEnv::TearDown() {
} // TearDown

