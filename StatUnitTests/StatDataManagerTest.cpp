#include "stdafx.h"
#include "CppUnitTest.h"
#include "GenHeader.h"
/////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StatUnitTests
{
	TEST_CLASS(StatDataManagerTest)
	{
	private:
		static std::unique_ptr<PredStatDataManagerType> m_man;
	public:
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			Logger::WriteMessage("In Class Initialize");
			std::string databasename(TEST_DATABASE_FILENAME);
			m_man.reset(new PredStatDataManagerType());
			PredStatDataManagerType *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			bool bRet = pMan->open(databasename);
			Assert::IsTrue(bRet);
			Assert::IsTrue(pMan->is_valid());
			Logger::WriteMessage("");
		}
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			Logger::WriteMessage("In Class Cleanup");
			m_man.reset();
			Logger::WriteMessage("");
		}
		TEST_METHOD(TestGetAllDatasets)
		{
			PredStatDataManagerType *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			Assert::IsTrue(pMan->is_valid());
			std::vector<DatasetType> oVec;
			bool bRet = pMan->get_all_datasets(oVec);
			Assert::IsTrue(bRet);
			const size_t n = oVec.size();
			for (size_t i = 0; i < n; ++i){
				DatasetType &v = oVec[i];
				int nId = v.id();
				Assert::IsTrue(nId > 0);
				String sigle = v.sigle();
				Assert::IsFalse(sigle.empty());
			}// i
		}// TestGetAllDatasets
		TEST_METHOD(TestGetDatasetVariables)
		{
			PredStatDataManagerType *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			Assert::IsTrue(pMan->is_valid());
			DatasetType oSet;
			String sigle(TEST_DATABASE_SIGLE);
			bool bRet = pMan->get_dataset_by_sigle(sigle,oSet);
			Assert::IsTrue(bRet);
			int nDatasetId = oSet.id();
			Assert::IsTrue(nDatasetId > 0);
			std::vector<VariableType> oVec;
			bRet = pMan->get_dataset_variables(nDatasetId,oVec);
			Assert::IsTrue(bRet);
			const size_t n = oVec.size();
			for (size_t i = 0; i < n; ++i){
				VariableType &v = oVec[i];
				int nId = v.id();
				Assert::IsTrue(nId > 0);
				String varsigle = v.sigle();
				Assert::IsFalse(varsigle.empty());
				Assert::AreEqual(nDatasetId,v.dataset_id());
				VariableType v1;
				bRet = pMan->get_variable_by_id(nId,v1);
				Assert::IsTrue(bRet);
				Assert::AreEqual(nId,v1.id());
				Assert::AreEqual(varsigle,v1.sigle());
				VariableType v2;
				bRet = pMan->get_variable_by_dataset_and_sigle(nDatasetId,varsigle,v2);
				Assert::IsTrue(bRet);
				Assert::AreEqual(nId,v2.id());
				Assert::AreEqual(varsigle,v2.sigle());
			}// i

		}// TestGetDatasetVariables
		TEST_METHOD(TestGetDatasetIndivs)
		{
			PredStatDataManagerType *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			Assert::IsTrue(pMan->is_valid());
			DatasetType oSet;
			String sigle(TEST_DATABASE_SIGLE);
			bool bRet = pMan->get_dataset_by_sigle(sigle,oSet);
			Assert::IsTrue(bRet);
			int nDatasetId = oSet.id();
			Assert::IsTrue(nDatasetId > 0);
			int nCount = 0;
			bRet = pMan->get_dataset_indivs_count(nDatasetId,nCount);
			Assert::IsTrue(bRet);
			Assert::IsTrue(nCount > 0);
			const int taken = 100;
			int skip = 0;
			while (skip < nCount){
				std::vector<IndivType> oVec;
				bRet = pMan->get_dataset_indivs(nDatasetId,oVec,skip,taken);
				Assert::IsTrue(bRet);
				const size_t n = oVec.size();
				if (n < 1){
					break;
				}
				for (size_t i = 0; i < n; ++i){
					IndivType &v = oVec[i];
					int nId = v.id();
					Assert::IsTrue(nId > 0);
					String indsigle = v.sigle();
					Assert::IsFalse(indsigle.empty());
					Assert::AreEqual(nDatasetId,v.dataset_id());
					IndivType v1;
					bRet = pMan->get_indiv_by_id(nId,v1);
					Assert::IsTrue(bRet);
					Assert::AreEqual(nId,v1.id());
					Assert::AreEqual(indsigle,v1.sigle());
					IndivType v2;
					bRet = pMan->get_indiv_by_dataset_sigle(nDatasetId,indsigle,v2);
					Assert::IsTrue(bRet);
					Assert::AreEqual(nId,v2.id());
					Assert::AreEqual(indsigle,v2.sigle());
				}// i
				skip += taken;
			}// while
			std::vector<IndivType> oVec;
			bRet = pMan->get_dataset_indivs(nDatasetId, oVec);
			Assert::IsTrue(bRet);
			Assert::AreEqual(nCount,(int)oVec.size());
			std::vector<int> oIds;
			bRet = pMan->get_dataset_indivs_ids(nDatasetId,oIds);
			Assert::IsTrue(bRet);
			Assert::AreEqual(nCount,(int)oIds.size());
			for (int i = 0; i < nCount; ++i){
				int nId = oIds[i];
				Assert::IsTrue(nId > 0);
			}// i
		}// TestGetDatasetIndivs
		TEST_METHOD(TestGetDatasetVariablesValues)
		{
			PredStatDataManagerType *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			Assert::IsTrue(pMan->is_valid());
			DatasetType oSet;
			String sigle(TEST_DATABASE_SIGLE);
			bool bRet = pMan->get_dataset_by_sigle(sigle,oSet);
			Assert::IsTrue(bRet);
			int nDatasetId = oSet.id();
			Assert::IsTrue(nDatasetId > 0);
			std::vector<VariableType> oVec;
			bRet = pMan->get_dataset_variables(nDatasetId,oVec);
			Assert::IsTrue(bRet);
			const size_t n = oVec.size();
			const int taken = 100;
			for (size_t i = 0; i < n; ++i){
				VariableType &v = oVec[i];
				int nVarId = v.id();
				Assert::IsTrue(nVarId > 0);
				int nCount = 0;
				bRet = pMan->get_variable_values_pair_not_null_count(nVarId,nCount);
				Assert::IsTrue(bRet);
				int skip = 0;
				while (skip < nCount){
					std::map<int,boost::any> oMap;
					bRet = pMan->get_variable_values_pair_not_null(nVarId,oMap,skip,taken);
					Assert::IsTrue(bRet);
					if (!oMap.empty()){
						auto iend = oMap.end();
						for (auto it = oMap.begin(); it != iend; ++it){
							int nIndivId = (*it).first;
							boost::any v = (*it).second;
							Assert::IsTrue(nIndivId > 0);
							Assert::IsFalse(v.empty());
						}// it
					}// oMap;
					skip += taken;
				}// skip
			}// i

		}// TestGetDatasetVariablesValues
		TEST_METHOD(TestGetDatasetValues)
		{
			PredStatDataManagerType *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			Assert::IsTrue(pMan->is_valid());
			DatasetType oSet;
			String sigle(TEST_DATABASE_SIGLE);
			bool bRet = pMan->get_dataset_by_sigle(sigle,oSet);
			Assert::IsTrue(bRet);
			int nDatasetId = oSet.id();
			Assert::IsTrue(nDatasetId > 0);
			int nCount = 0;
			bRet = pMan->get_dataset_values_count(nDatasetId, nCount);
			Assert::IsTrue(bRet);
			const int taken = 100;
			int skip = 0;
			while (skip < nCount){
				std::vector<ValueType> oVec;
				bRet = pMan->get_dataset_values(nDatasetId,oVec,skip,taken);
				Assert::IsTrue(bRet);
				const size_t n = oVec.size();
				for (size_t i = 0; i < n; ++i){
					ValueType &v = oVec[i];
					Assert::IsTrue(v.id() > 0);
					Assert::IsTrue(v.variable_id() > 0);
					Assert::IsTrue(v.indiv_id() > 0);
				}// i
				skip += taken;
			}// skip
		}// TestGetDatasetValues
	};// classStatDataManagerTest
	std::unique_ptr<PredStatDataManagerType> StatDataManagerTest::m_man;
}// namespace
