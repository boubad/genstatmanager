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
#include "dbvalue.h"
///////////////////////////////
#include <gtest/gtest.h>
/////////////////////////////////////
#ifdef INTRA_USE_WSTRING
typedef std::wstring String;
#else
typedef std::string String;
#endif // INTRA_USE_WSTRING/////////////////////////////////////////inline std::ostream & operator<<(std::ostream &os, const boost::any &v) {sqlite::DbValue vx(v);std::string s;
vx.string_value(s);
os << s;
return os;
}
inline std::wostream & operator<<(std::wostream &os, const boost::any &v) {
sqlite::DbValue vx(v);
std::wstring s;
vx.string_value(s);
os << s;
return os;
}
///////////////////////////////////
template<class TSTRING = std::string>
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
#ifndef INTRA_USE_STRING
	{
		std::ifstream in(DATABASE_FILENAME);
		if (in.is_open()) {
			return;
		}
	}
#else
	{
		std::wifstream in(DATABASE_FILENAME);
		if (in.is_open()) {
			return;
		}
	}
#endif
	StringType filename(TEST_FILENAME);
	StringType datasetSigle(TEST_DATASET_SIGLE);
	StringType databasename(DATABASE_FILENAME);
	//
	PredStatDataManagerType oMan;
	bool bRet = oMan.open(databasename);
	ASSERT_TRUE(bRet);
	//
#ifdef INTRA_USE_WSTRING
	std::string sk(filename.length(),' ');
	std::copy(filename.begin(),filename.end(),sk.begin());
	std::wifstream inFile(sk.c_str());
	bRet = inFile.is_open();
	ASSERT_TRUE(bRet);
	std::wstring na(L"na");
	wchar_t delim(L'\t');
	bRet = oMan.process_data(inFile, datasetSigle,delim,na);
#else
	std::ifstream inFile(filename.c_str());
	bRet = inFile.is_open();
	ASSERT_TRUE(bRet);
	std::string na("na");
	char delim('\t');
	bRet = oMan.process_data(inFile, datasetSigle, delim, na);
#endif // INTRA_USE_WSTRING		ASSERT_EQ(true, bRet);	}		virtual void TearDown() {}public:
	static void get_database_name(StringType &s) {
		s = DATABASE_FILENAME;
	}
	static void get_dataset_sigle(StringType &s) {
		s = TEST_DATASET_SIGLE;
	}
private:
	static const CharType *TEST_FILENAME;
	static const CharType *DATABASE_FILENAME;
	static const CharType *TEST_DATASET_SIGLE;
	static const char *TEST_RESULTS_FILENAME;
public:
static IntraTestEnv<TSTRING> *global_intraenv;
}
;
//////////////////////////////
#ifndef INTRA_USE_WSTRING
#ifdef __GNUC__
template<class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::TEST_FILENAME =
	"./testdata/data_2013.txt";
template<class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::DATABASE_FILENAME =
	"./testdata/testdb.db";
#else
template <class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::TEST_FILENAME = "..\\teststatdata\\testdata\\data_2013.txt";
template <class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::DATABASE_FILENAME = "..\\teststatdata\\testdata\\testdb.db";
template<class TSTRING>
const char *IntraTestEnv<TSTRING>::TEST_RESULTS_FILENAME = "..\\TestResults\\testresults.txt";
#endif
template<class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::TEST_DATASET_SIGLE =
	"TESTSET";
#else
////////////////////////////////////////////////////
#ifdef __GNUC__
template <class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::TEST_FILENAME = L"./testdata/data_2013.txt";
template <class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::DATABASE_FILENAME = "./testdata/testdb.db";
#else
template <class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::DATABASE_FILENAME = L"..\\teststatdata\\testdata\\testdb.db";
#endif
template <class TSTRING>
const typename IntraTestEnv<TSTRING>::CharType *IntraTestEnv<TSTRING>::TEST_DATASET_SIGLE = L"TESTSET";
#endif // INTRA_USE_WSTRING////////////////////////////////template<class TSTRING>IntraTestEnv<TSTRING> *IntraTestEnv<TSTRING>::global_intraenv = nullptr;/////////////////////////////#endif /* INTRATESTENV_H_ */
#endif
