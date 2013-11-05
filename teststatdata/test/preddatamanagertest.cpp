////////////////////////////////
#include "preddataset.h"
//////////////////////////
#include "IntraTestEnv.h"
////////////////////////////
namespace {
/////////////////////////////
class PredStatDataManagerTest: public ::testing::Test {
public:
	typedef intra::Dataset<String> DatasetType;
	typedef intrasqlite::PredStatDataManager<String> PredStatDataManagerType;
	typedef intra::Variable<String> VariableType;
	typedef intra::Indiv<String> IndivType;
	typedef intra::Value ValueType;
protected:
	int m_datasetid;
	std::unique_ptr<PredStatDataManagerType> m_man;
	String m_sigle;
protected:
	PredStatDataManagerTest():m_datasetid(0) {
	}
	virtual ~PredStatDataManagerTest() {
	}
	virtual void SetUp() {
		m_man.reset(new PredStatDataManagerType());
		PredStatDataManagerType *pMan = m_man.get();
		ASSERT_TRUE(pMan != nullptr);
		String databasename;
		IntraTestEnv < String > ::get_database_name(databasename);
		ASSERT_FALSE(databasename.empty());
		bool bRet = pMan->open(databasename);
		ASSERT_TRUE(bRet);
		ASSERT_TRUE(pMan->is_valid());
		pMan->reset();
		IntraTestEnv<String>::get_dataset_sigle (m_sigle);
		ASSERT_FALSE(m_sigle.empty());
		DatasetType oSet;
		bRet = pMan->get_dataset_by_sigle(m_sigle,oSet);
		ASSERT_TRUE(bRet);
		this->m_datasetid = oSet.id();
		ASSERT_TRUE(this->m_datasetid != 0);
	}
	virtual void TearDown() {
		m_man.reset();
	}
};
///////////////////////////////////////
TEST_F(PredStatDataManagerTest, get_all_datasets) {
	PredStatDataManagerType *pMan = m_man.get();
	ASSERT_TRUE(pMan != nullptr);
	std::vector<DatasetType> oVec;
	bool bRet = pMan->get_all_datasets(oVec);
	EXPECT_TRUE(bRet);
	size_t n = oVec.size();
	bRet = (n > 0);
	EXPECT_TRUE(bRet);
	for (auto it = oVec.begin(); it != oVec.end(); ++it){
		DatasetType &oSet = *it;
		int nId = oSet.id();
		EXPECT_TRUE(nId != 0);
	}// id
} // datasets
TEST_F(PredStatDataManagerTest, get_dataset_by_id) {
	PredStatDataManagerType *pMan = m_man.get();
	ASSERT_TRUE(pMan != nullptr);
	DatasetType oSet;
	bool bRet = pMan->get_dataset_by_id(this->m_datasetid,oSet);
	EXPECT_TRUE(bRet);
	int nId = oSet.id();
	EXPECT_EQ(this->m_datasetid,nId);
} // get_dataset_by_id
TEST_F(PredStatDataManagerTest, get_dataset_variables) {
	PredStatDataManagerType *pMan = m_man.get();
	ASSERT_TRUE(pMan != nullptr);
	std::vector<VariableType> oVec;
	bool bRet = pMan->get_dataset_variables(this->m_datasetid,oVec);
	EXPECT_TRUE(bRet);
	size_t n = oVec.size();
	EXPECT_TRUE(n > 0);
	for (size_t i = 0; i < n; ++i){
		VariableType &v = oVec[i];
		int nVarId = v.id();
		EXPECT_TRUE(nVarId > 0);
		int nDatasetId = v.dataset_id();
		EXPECT_EQ(this->m_datasetid, nDatasetId);
		String sigle = v.sigle();
		EXPECT_FALSE(sigle.empty());
	}// i
} // get_dataset_variables
TEST_F(PredStatDataManagerTest, get_dataset_indivs_ids) {
	PredStatDataManagerType *pMan = m_man.get();
	ASSERT_TRUE(pMan != nullptr);
	std::vector<int> oVec;
	bool bRet = pMan->get_dataset_indivs_ids(this->m_datasetid,oVec);
	EXPECT_TRUE(bRet);
	size_t n = oVec.size();
	EXPECT_TRUE(n > 0);
	for (size_t i = 0; i < n; ++i){
		int nId = oVec[i];
		EXPECT_TRUE(nId > 0);
	}// i
} // get_dataset_indivs_ids
TEST_F(PredStatDataManagerTest, get_all_dataset_indivs) {
	PredStatDataManagerType *pMan = m_man.get();
	ASSERT_TRUE(pMan != nullptr);
	std::vector<IndivType> oVec;
	bool bRet = pMan->get_dataset_indivs(this->m_datasetid,oVec);
	EXPECT_TRUE(bRet);
	size_t n = oVec.size();
	EXPECT_TRUE(n > 0);
	for (size_t i = 0; i < n; ++i){
		IndivType &v = oVec[i];
		int nIndId = v.id();
		EXPECT_TRUE(nIndId > 0);
		int nDatasetId = v.dataset_id();
		EXPECT_EQ(this->m_datasetid, nDatasetId);
		String sigle = v.sigle();
		EXPECT_FALSE(sigle.empty());
	}// i
} // get_dataset_indivs
TEST_F(PredStatDataManagerTest, get_dataset_by_status_indivs) {
#ifndef INTRA_USE_WSTRING
	String status("T");
#else
	String status(L"T");
#endif
	PredStatDataManagerType *pMan = m_man.get();
	ASSERT_TRUE(pMan != nullptr);
	int nCount = 0;
	bool bRet = pMan->get_dataset_indivs_by_status_count(this->m_datasetid,status,nCount);
	ASSERT_TRUE(bRet);
	ASSERT_TRUE(nCount > 0);
	const int taken = 100;
	int skip = 0;
	while (skip < nCount){
		std::vector<IndivType> oVec;
		bRet = pMan->get_dataset_indivs_by_status(this->m_datasetid,oVec,status,skip,taken);
		EXPECT_TRUE(bRet);
		const size_t n = oVec.size();
		for (size_t i = 0; i < n; ++i){
				IndivType &v = oVec[i];
				int nIndId = v.id();
				EXPECT_TRUE(nIndId > 0);
				int nDatasetId = v.dataset_id();
				EXPECT_EQ(this->m_datasetid, nDatasetId);
				String sigle = v.sigle();
				EXPECT_FALSE(sigle.empty());
			}// i
		skip += taken;
	}// skip
} //et_dataset_by_status_indivs
/////////////////
}	// namespace

