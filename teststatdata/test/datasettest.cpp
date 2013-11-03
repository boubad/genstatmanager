/*
 * datasettest.cpp
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */
////////////////////////////////
#include "dataset.h"
//////////////////////////
#include "IntraTestEnv.h"
////////////////////////////
namespace {
/////////////////////////////
class DatasetTest: public ::testing::Test {
public:
	typedef intra::Dataset<String> DatasetType;
protected:
	std::unique_ptr<DatasetType> m_set;
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
		m_set.reset(new DatasetType());
		DatasetType *pSet = m_set.get();
		ASSERT_TRUE(pSet != nullptr);
	}
	virtual void TearDown() {
		m_set.reset();
	}
};
///////////////////////////////////////
TEST_F(DatasetTest,testDummy) {

}// TestDummy
////////////////////////////////////////////////
} // namespace

