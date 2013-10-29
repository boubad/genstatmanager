/*
 * IntraTestEnv.cpp
 *
 *  Created on: 28 oct. 2013
 *      Author: boubad
 */
#include "IntraTestEnv.h"
//////////////////////////////
#ifdef __GNUC__
const char *IntraTestEnv::TEST_FILENAME = "./testdata/data_2013.txt";
//const char *IntraTestEnv::DATABASE_FILENAME = "./testdata/testdb.db";
const char *IntraTestEnv::DATABASE_FILENAME = ":memory";
#else
const char *IntraTestEnv::TEST_FILENAME = "..\\teststatdata\\testdata\\data_2013.txt";
const char *IntraTestEnv::DATABASE_FILENAME = "..\\teststatdata\\testdata\\testdb.db";
#endif
const char *IntraTestEnv::TEST_DATASET_SIGLE = "TESTSET";
////////////////////////////////
IntraTestEnv *global_intraenv = nullptr;
/////////////////////////////
IntraTestEnv::IntraTestEnv() {
}
IntraTestEnv::~IntraTestEnv() {

}
void IntraTestEnv::SetUp() {
	char delim('\t');
	String na("na");
	String filename(TEST_FILENAME);
	String datasetSigle(TEST_DATASET_SIGLE);
	String databasename(DATABASE_FILENAME);
	//
	bool bRet = intra::process_data(filename, databasename, datasetSigle, delim,
			na);
	ASSERT_EQ(true, bRet);
	//
	m_import.reset(new ImportDataType());
	ImportDataType *pImport = m_import.get();
	ASSERT_TRUE(pImport != nullptr);
} // SetUp
void IntraTestEnv::TearDown() {
	m_import.reset();
} // TearDown

