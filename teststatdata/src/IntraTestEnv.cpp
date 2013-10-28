/*
 * IntraTestEnv.cpp
 *
 *  Created on: 28 oct. 2013
 *      Author: boubad
 */
#include "IntraTestEnv.h"
//////////////////////////////
const char *IntraTestEnv::TEST_FILENAME = "./testdata/data_2013.txt";
const char *IntraTestEnv::DATABASE_FILENAME = "./testdata/testdb.db";
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
	std::ifstream in(filename.c_str());
	ASSERT_TRUE(in.is_open());
	pImport->open(in, delim, na);
	ASSERT_TRUE(pImport->cols() > 0);
	ASSERT_TRUE(pImport->rows() > 0);
	//
	m_manager.reset(new StatDataManager(databasename));
	StatDataManager *pMan = m_manager.get();
	ASSERT_TRUE(pMan != nullptr);
	ASSERT_EQ(true, pMan->is_valid());
} // SetUp
void IntraTestEnv::TearDown() {
	m_manager.reset();
	m_import.reset();
} // TearDown

