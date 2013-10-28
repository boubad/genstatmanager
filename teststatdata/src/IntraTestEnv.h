/*
 * IntraTestEnv.h
 *
 *  Created on: 28 oct. 2013
 *      Author: boubad
 */

#ifndef INTRATESTENV_H_
#define INTRATESTENV_H_
/////////////////////////////////
#include "importdata.h"
#include "datamanager.h"
///////////////////////////////
#include <gtest/gtest.h>
///////////////////////////////////
typedef std::string String;
typedef intra::ImportData<String> ImportDataType;
typedef intrasqlite::StatDataManager StatDataManager;
typedef intra::Dataset Dataset;
/////////////////////////////////
class IntraTestEnv: public ::testing::Environment {
public:
	IntraTestEnv();
	virtual ~IntraTestEnv();
	virtual void SetUp();
	virtual void TearDown();
public:
	StatDataManager *data_manager(void) const {
		StatDataManager *p = m_manager.get();
		EXPECT_TRUE(p != nullptr);
		return (p);
	} // data_manager
	ImportDataType *import_data(void) const {
		ImportDataType *p = m_import.get();
		EXPECT_TRUE(p != nullptr);
		return (p);
	}
private:
	std::unique_ptr<ImportDataType> m_import;
	std::unique_ptr<StatDataManager> m_manager;
private:
	static const char *TEST_FILENAME;
	static const char *DATABASE_FILENAME;
	static const char *TEST_DATASET_SIGLE;
};
// class IntraTestEnv
/////////////////////////////////////////
extern IntraTestEnv *global_intraenv;
//////////////////////////////////////////
#endif /* INTRATESTENV_H_ */
