////////////////////////////////
#include "preddataset.h"
//////////////////////////
#include "IntraTestEnv.h"
////////////////////////////
namespace {
	/////////////////////////////
	class PredStatDataManagerTest : public ::testing::Test {
	protected:
		static std::unique_ptr<PredStatDataManager> m_man;
		static StringType m_sigle;
		//
		PredStatDataManager *m_pman;
		std::unique_ptr<Dataset> m_set;
	protected:
		static void SetUpTestCase() {
			m_man.reset(new PredStatDataManager());
			PredStatDataManager *pMan = m_man.get();
			ASSERT_TRUE(pMan != nullptr);
			StringType databasename;
			IntraTestEnv *pEnv = global_intraenv;
			ASSERT_TRUE(pEnv != nullptr);
			pEnv->get_database_name(databasename);
			ASSERT_FALSE(databasename.empty());
			bool bRet = pMan->open(databasename);
			ASSERT_TRUE(bRet);
			ASSERT_TRUE(pMan->is_valid());
			pEnv->get_dataset_sigle(m_sigle);
			ASSERT_FALSE(m_sigle.empty());
		} // SetUpTestCase
		static void TearSownTestCase() {
		} // TearDownTestCase
		PredStatDataManagerTest() : m_pman(nullptr) {
		}
		virtual ~PredStatDataManagerTest() {
		}
		virtual void SetUp() {
			m_pman = m_man.get();
			ASSERT_TRUE(m_pman != nullptr);
			ASSERT_TRUE(m_pman->is_valid());
			m_set.reset(new Dataset());
			Dataset *pSet = m_set.get();
			EXPECT_TRUE(pSet != nullptr);
			bool bRet = m_pman->get_dataset_by_sigle(m_sigle, *pSet);
			EXPECT_TRUE(bRet);
			int nId = pSet->id();
			EXPECT_TRUE(nId != 0);
		}
		virtual void TearDown() {
			m_pman = nullptr;
			m_set.reset();
		}
	};
	///////////////////////////////////////
	std::unique_ptr<PredStatDataManager> PredStatDataManagerTest::m_man;
	StringType PredStatDataManagerTest::m_sigle;
	///////////////////////////////////
	TEST_F(PredStatDataManagerTest, datasets)
	{
		PredStatDataManager *pMan = m_pman;
		std::vector<Dataset> oVec;
		bool bRet = pMan->get_all_datasets(oVec);
		EXPECT_TRUE(bRet);
		size_t n = oVec.size();
		bRet = (n > 0);
		EXPECT_TRUE(bRet);
		const Dataset &oSet = *(oVec.begin());

	}// datasets
	/////////////////
}// namespace

