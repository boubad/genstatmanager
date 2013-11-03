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
/////////////////////////////////////
#ifdef INTRA_USE_WSTRING
typedef std::wstring String;
#else
typedef std::string String;
#endif // INTRA_USE_WSTRING
///////////////////////////////////
template <class TSTRING = std::string>
class IntraTestEnv: public ::testing::Environment {
public:
	typedef TSTRING StringType;
	typedef typename StringType::value_type CharType;
	typedef intrasqlite::PredStatDataManager<StringType> PredStatDataManagerType;
public:
	IntraTestEnv() {
	}
	virtual ~IntraTestEnv() {
	}
	virtual void SetUp() {
		StringType filename(TEST_FILENAME);
		StringType datasetSigle(TEST_DATASET_SIGLE);
		StringType databasename(DATABASE_FILENAME);
		//
		PredStatDataManagerType oMan;
		bool bRet = oMan.open(databasename);
		ASSERT_TRUE(bRet);
		//
#ifdef INTRA_USE_WSTRING
#else
		std::ifstream inFile(filename.c_str());
		bRet = inFile.is_open();
		ASSERT_TRUE(bRet);
		std::string na("na");
		char delim('\t');
		bRet = oMan.process_data(inFile, datasetSigle,delim,na);
#endif // INTRA_USE_WSTRING
		ASSERT_EQ(true, bRet);
	}
	virtual void TearDown() {
	}
public:
	void get_database_name(StringType &s) const {
		s = DATABASE_FILENAME;
	}
	void get_dataset_sigle(StringType &s) const {
		s = TEST_DATASET_SIGLE;
	}
private:
	static const CharType *TEST_FILENAME;
	static const CharType *DATABASE_FILENAME;
	static const CharType *TEST_DATASET_SIGLE;
public:
	static IntraTestEnv<TSTRING> *global_intraenv;
};
//////////////////////////////
#ifndef INTRA_USE_WSTRING
#ifdef __GNUC__
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::TEST_FILENAME = "./testdata/data_2013.txt";
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::DATABASE_FILENAME = ":memory";
//const char *IntraTestEnv::DATABASE_FILENAME = "./testdata/testdb.db";
#else
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::TEST_FILENAME = "..\\teststatdata\\testdata\\data_2013.txt";
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::DATABASE_FILENAME = "..\\teststatdata\\testdata\\testdb.db";
#endif
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::TEST_DATASET_SIGLE = "TESTSET";
#else
////////////////////////////////////////////////////
#ifdef __GNUC__
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::TEST_FILENAME = L"./testdata/data_2013.txt";
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::DATABASE_FILENAME = L":memory";
//const wchar_t *IntraTestEnv::DATABASE_FILENAME = L"./testdata/testdb.db";
#else
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::TEST_FILENAME = L"..\\teststatdata\\testdata\\data_2013.txt";
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::DATABASE_FILENAME = L"..\\teststatdata\\testdata\\testdb.db";
#endif
	template <class TSTRING>
	const typename IntraTestEnv<TSTRING>::CharType  *IntraTestEnv<TSTRING>::TEST_DATASET_SIGLE = L"TESTSET";
#endif // INTRA_USE_WSTRING////////////////////////////////
	template<class TSTRING>
	IntraTestEnv<TSTRING> *IntraTestEnv<TSTRING>::global_intraenv = nullptr;
/////////////////////////////#endif /* INTRATESTENV_H_ */
#endif
