/*
 * datasettest.cpp
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */
////////////////////////////////
#include "preddataset.h"
//////////////////////////
#include "IntraTestEnv.h"
////////////////////////////
namespace {
/////////////////////////////
class DatasetTest: public ::testing::Test {
	
protected:
	std::unique_ptr<Dataset> m_set;
	typedef Dataset::VariableMap VariableMap;
	typedef Dataset::IndivMap IndivMap;
	typedef Dataset::ValueVector ValueVector;
protected:
	static void SetUpTestCase() {
	} // SetUpTestCase
	static void TearSownTestCase() {
	} // TearDownTestCase
	DatasetTest() {
	}
	virtual ~DatasetTest() {
	}
	virtual void SetUp() {
		m_set.reset(new Dataset());
		Dataset *pSet = m_set.get();
		ASSERT_TRUE(pSet != nullptr);
	}
	virtual void TearDown() {
		m_set.reset();
	}
};
///////////////////////////////////////
TEST_F(DatasetTest,createVariableBySigle) {
	String sigle = "testSigle";
	String name = "testName";
	String desc = "testDesc";
	int nId = 50;
	int nVersion = 15;
	Dataset *pSet = m_set.get();
	ASSERT_TRUE(pSet != nullptr);
	pSet->id(nId);
	pSet->version(nVersion);
	pSet->sigle(sigle);
	pSet->name(name);
	pSet->description(desc);
	//
	String varSigle = "testVar";
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
	String sigle = "testSigle";
	String name = "testName";
	String desc = "testDesc";
	int nId = 50;
	int nVersion = 15;
	//
	Dataset *pSet = m_set.get();
	ASSERT_TRUE(pSet != nullptr);
	//
	pSet->id(nId);
	pSet->version(nVersion);
	pSet->sigle(sigle);
	pSet->name(name);
	pSet->description(desc);
	//
	Dataset oSet(*pSet);
	EXPECT_EQ(nId, oSet.id());
	EXPECT_EQ(nVersion, oSet.version());
	EXPECT_EQ(sigle, oSet.sigle());
	EXPECT_EQ(name, oSet.name());
	EXPECT_EQ(desc, oSet.description());
	//
	Dataset bSet = *pSet;
	EXPECT_EQ(nId, bSet.id());
	EXPECT_EQ(nVersion, bSet.version());
	EXPECT_EQ(sigle, bSet.sigle());
	EXPECT_EQ(name, bSet.name());
	EXPECT_EQ(desc, bSet.description());
} //copyConstructorTest
TEST_F(DatasetTest,constructorTest1) {
	Dataset *pSet = m_set.get();
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

