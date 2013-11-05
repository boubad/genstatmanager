//GenHeader.h
//////////////////
#ifndef __GENHEADER_H__
#define __GENHEADER_H__
//////////////////////////
#include "preddataset.h"
/////////////////////////
namespace StatUnitTests
{
	typedef std::string String;
	/////////////////////
	typedef String::value_type Char;
	typedef intrasqlite::PredStatDataManager<String> PredStatDataManagerType;
	typedef intra::Dataset<String> DatasetType;
	typedef intra::Variable<String> VariableType;
	typedef intra::Indiv<String> IndivType;
	typedef intra::Value ValueType;
	//////////////////////////////
	extern const char *TEST_DATABASE_FILENAME;
	extern const char *TEST_IMPORT_FILENAME;
	extern const Char *TEST_DATABASE_SIGLE;
	extern const Char *TEST_NA;
	extern const Char TEST_DELIM;
	///////////////////////////////
}//namespace StatUnitTests
//////////////////////////
#endif // __GENHEADER_H__
////////////////////
