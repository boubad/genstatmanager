/*
 * datasettest.cpp
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */
////////////////////////////////
#include "statdata.h"
#include "statdatamanager.h"
//////////////////////////
#include <gtest/gtest.h>
#include "IntraTestEnv.h"
////////////////////////////
namespace {
/////////////////////////////
using namespace intra;
////////////////////////////////////////////
class DatasetTest: public ::testing::Test {
protected:
	static std::unique_ptr<intra::Dataset> m_globalset;
	std::unique_ptr<intra::Dataset> m_set;
	typedef intra::Dataset::VariableMap VariableMap;
	typedef intra::Dataset::IndivMap IndivMap;
	typedef intra::Dataset::ValueVector ValueVector;
protected:
	static void SetUpTestCase() {
		m_globalset.reset(new intra::Dataset());
		intra::Dataset *pSet = m_globalset.get();
		ASSERT_TRUE(pSet != nullptr);
		intra::String databasename;
		intra::String datasetSigle;
		global_intraenv->get_database_name(databasename);
		global_intraenv->get_dataset_sigle(datasetSigle);
		pSet->sigle(datasetSigle);
		intrasqlite::StatDataManager oMan(databasename);
		ASSERT_TRUE(oMan.is_valid());
		bool bRet = oMan.load_dataset(*pSet);
		ASSERT_TRUE(bRet);
	} // SetUpTestCase
	static void TearSownTestCase() {
		m_globalset.reset();
	} // TearDownTestCase
	DatasetTest() {
	}
	virtual ~DatasetTest() {
	}
	virtual void SetUp() {
		m_set.reset(new intra::Dataset());
		intra::Dataset *pSet = m_set.get();
		ASSERT_TRUE(pSet != nullptr);
	}
	virtual void TearDown() {
		m_set.reset();
	}
};
///////////////////////////////////////
std::unique_ptr<intra::Dataset> DatasetTest::m_globalset;
// class DatasetTest
//
//////////////////////////////////////////////////////
TEST_F(DatasetTest,testDataset) {
	intra::Dataset *pSet = m_globalset.get();
	ASSERT_TRUE(pSet != nullptr);
	size_t nCols = pSet->cols();
	size_t nRows = pSet->rows();
	std::vector<PVariable> oVars;
	pSet->get_variables(oVars);
	size_t nx = oVars.size();
	ASSERT_EQ(nCols, nx);
	std::vector<PIndiv> oInds;
	pSet->get_indivs(oInds);
	size_t ny = oInds.size();
	ASSERT_EQ(nRows, ny);
} // testDataset
//////////////////////////////////////////////////
TEST_F(DatasetTest,createVariableBySigle) {
	intra::String sigle = "testSigle";
	intra::String name = "testName";
	intra::String desc = "testDesc";
	int nId = 50;
	int nVersion = 15;
	intra::Dataset *pSet = m_set.get();
	ASSERT_TRUE(pSet != nullptr);
	pSet->id(nId);
	pSet->version(nVersion);
	pSet->sigle(sigle);
	pSet->name(name);
	pSet->description(desc);
	//
	intra::String varSigle = "testVar";
	//int nVarId = 46;
	Variable *pVar = pSet->create_variable(varSigle);
	EXPECT_TRUE(pVar != nullptr);
	EXPECT_EQ(varSigle, pVar->sigle());
	VariableMap &oVars = pSet->variables();
	size_t nSize = 1;
	size_t nActual = oVars.size();
	EXPECT_EQ(nSize, nActual);
}
TEST_F(DatasetTest,copyConstructorTest) {
	intra::String sigle = "testSigle";
	intra::String name = "testName";
	intra::String desc = "testDesc";
	int nId = 50;
	int nVersion = 15;
	//
	intra::Dataset *pSet = m_set.get();
	ASSERT_TRUE(pSet != nullptr);
	//
	pSet->id(nId);
	pSet->version(nVersion);
	pSet->sigle(sigle);
	pSet->name(name);
	pSet->description(desc);
	//
	intra::Dataset oSet(*pSet);
	EXPECT_EQ(nId, oSet.id());
	EXPECT_EQ(nVersion, oSet.version());
	EXPECT_EQ(sigle, oSet.sigle());
	EXPECT_EQ(name, oSet.name());
	EXPECT_EQ(desc, oSet.description());
	//
	intra::Dataset bSet = *pSet;
	EXPECT_EQ(nId, bSet.id());
	EXPECT_EQ(nVersion, bSet.version());
	EXPECT_EQ(sigle, bSet.sigle());
	EXPECT_EQ(name, bSet.name());
	EXPECT_EQ(desc, bSet.description());
} //copyConstructorTest
TEST_F(DatasetTest,constructorTest1) {
	intra::Dataset *pSet = m_set.get();
	ASSERT_TRUE(pSet != nullptr);
	EXPECT_EQ(0, pSet->id());
	EXPECT_EQ(1, pSet->version());
	bool bRet = pSet->is_changed();
	EXPECT_FALSE(bRet);
	bRet = pSet->is_removeable();
	EXPECT_FALSE(bRet);
	bRet = pSet->is_updateable();
	EXPECT_FALSE(bRet);
	bRet = pSet->is_writeable();
	EXPECT_FALSE(bRet);
	//
	pSet->sigle("testSigle");
	EXPECT_EQ(true, pSet->is_changed());
	EXPECT_EQ(true, pSet->is_writeable());
	//
	pSet->id(34);
	pSet->version(50);
	pSet->name("testname");
	pSet->description("testdesc");
	//
	EXPECT_EQ(34, pSet->id());
	EXPECT_EQ(50, pSet->version());
	EXPECT_EQ(true, pSet->is_removeable());
	EXPECT_EQ(true, pSet->is_updateable());
	//
//	pSet->id(0);
//	bRet = pSet->is_updateable();
//	EXPECT_TRUE(bRet);
//	EXPECT_EQ(true, pSet->is_writeable());
	//
	VariableMap &oVars = pSet->variables();
	EXPECT_TRUE(oVars.empty());
	IndivMap &oInds = pSet->indivs();
	EXPECT_TRUE(oInds.empty());
	ValueVector &oVals = pSet->values();
	EXPECT_TRUE(oVals.empty());
} // constructorTest1
////////////////////////////////////////////////
} // namespace

