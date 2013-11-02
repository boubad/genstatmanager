/*
 * IntraTestEnv.h
 *
 *  Created on: 28 oct. 2013
 *      Author: boubad
 */

#ifndef INTRATESTENV_H_
#define INTRATESTENV_H_
/////////////////////////////////
#include "preddataset.h"
///////////////////////////////
#include <gtest/gtest.h>
///////////////////////////////////
typedef std::string String;
typedef intra::Char CharType;
typedef intra::String StringType;
typedef intra::IFStream IFStream;
typedef intra::ImportData<StringType> ImportDataType;
typedef intra::Dataset DatasetType;
typedef intra::Variable Variable;
typedef intra::Indiv Indiv;
typedef intra::Value Value;
typedef intra::PVariable PVariable;
typedef intra::PIndiv PIndiv;
typedef intra::PValue PValue;
typedef intrasqlite::PredStatDataManager PredStatDataManager;
typedef intra::Dataset Dataset;
/////////////////////////////////
class IntraTestEnv: public ::testing::Environment {
public:
	IntraTestEnv();
	virtual ~IntraTestEnv();
	virtual void SetUp();
	virtual void TearDown();
public:
	void get_database_name(StringType &s) const {
		s = DATABASE_FILENAME;
	}
	void get_dataset_sigle(StringType &s) const {
		s = TEST_DATASET_SIGLE;
	}
private:
#ifndef INTRA_USE_WSTRING
	static const char *TEST_FILENAME;
	static const char *DATABASE_FILENAME;
	static const char *TEST_DATASET_SIGLE;
#else
	static const wchar_t *TEST_FILENAME;
	static const wchar_t *DATABASE_FILENAME;
	static const wchar_t *TEST_DATASET_SIGLE;
#endif // INTRA_USE_WSTRING
};
// class IntraTestEnv
/////////////////////////////////////////
extern IntraTestEnv *global_intraenv;
//////////////////////////////////////////
#endif /* INTRATESTENV_H_ */
