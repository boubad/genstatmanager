////////////////////////////////
#include "preddataset.h"
//////////////////////////
#include "IntraTestEnv.h"
////////////////////////////
namespace {
	/////////////////////////////
	class PredStatDataManagerTest : public ::testing::Test {
	public:
		typedef intra::Dataset<String> DatasetType;
		typedef intrasqlite::PredStatDataManager<String> PredStatDataManagerType;
	protected:
		static std::unique_ptr<PredStatDataManagerType> m_man;
		static String m_sigle;
		//
		PredStatDataManagerType *m_pman;
		std::unique_ptr<DatasetType> m_set;
	protected:
		static void SetUpTestCase() {
			m_man.reset(new PredStatDataManagerType());
			PredStatDataManagerType *pMan = m_man.get();
			ASSERT_TRUE(pMan != nullptr);
			String databasename;
			IntraTestEnv<String>::get_database_name(databasename);
			ASSERT_FALSE(databasename.empty());
#ifdef INTRA_USE_WSTRING
#else
			bool bRet = pMan->open(databasename);
#endif // INTRA_USE_WSTRING
			ASSERT_TRUE(bRet);
			ASSERT_TRUE(pMan->is_valid());
			IntraTestEnv<String>::get_dataset_sigle(m_sigle);
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
			m_set.reset(new DatasetType());
			DatasetType *pSet = m_set.get();
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
	std::unique_ptr<PredStatDataManagerTest::PredStatDataManagerType> PredStatDataManagerTest::m_man;
	String PredStatDataManagerTest::m_sigle;
	///////////////////////////////////
	TEST_F(PredStatDataManagerTest, datasets)
	{
		PredStatDataManagerType *pMan = m_pman;
		std::vector<DatasetType> oVec;
		bool bRet = pMan->get_all_datasets(oVec);
		EXPECT_TRUE(bRet);
		size_t n = oVec.size();
		bRet = (n > 0);
		EXPECT_TRUE(bRet);
		const DatasetType &oSet = *(oVec.begin());
		int nId = oSet.id();
		ASSERT_TRUE(nId != 0);
	}// datasets
	/////////////////
}// namespace

