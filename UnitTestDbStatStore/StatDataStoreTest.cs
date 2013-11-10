using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using DbStatStore;
namespace UnitTestDbStatStore
{
    [TestClass]
    public class StatDataStoreTest
    {
        private static String TEST_DATABASE_SIGLE = "TestSet";
        private StatDataStore m_store;
        private DbDataset m_set;
        //
        [TestInitialize()]
        public void Initialize()
        {
            m_store = new StatDataStore();
            Assert.IsNotNull(m_store);
            var r = m_store.FindDatasetBySigle(TEST_DATABASE_SIGLE);
            Assert.IsNotNull(r);
            Exception err = r.Item2;
            Assert.IsNull(err);
            m_set = r.Item1;
            if (m_set == null)
            {
                DbDataset pSet = new DbDataset();
                pSet.Sigle = TEST_DATABASE_SIGLE;
                pSet.Name = TEST_DATABASE_SIGLE;
                pSet.Description = TEST_DATABASE_SIGLE;
                r = m_store.MaintainsDataset(pSet);
                Assert.IsNotNull(r);
                Assert.IsNull(r.Item2);
                m_set = r.Item1;
                Assert.IsNotNull(m_set);
            }
            Assert.IsTrue(m_set.Id != 0);
        }

        [TestCleanup()]
        public void Cleanup()
        {
            m_set = null;
            m_store = null;
        }

        //
        [TestMethod]
        public void TestGetStoreDatasets()
        {
            StatDataStore oMan = new StatDataStore();
            Assert.IsNotNull(oMan);
            var r = oMan.GetAllDataSets();
            Assert.IsNotNull(oMan);
            Exception err = r.Item2;
            Assert.IsNull(err);
            IEnumerable<DbDataset> oList = r.Item1;
            Assert.IsNotNull(oList);
        }//TestGetStoreDatasets
        [TestMethod]
        public void TestDbFindDataset()
        {
            StatDataStore oMan = m_store;
            Assert.IsNotNull(oMan);
            int nId = 0;
            var r = oMan.FindDatasetById(nId);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            String empty = null;
            r = oMan.FindDatasetBySigle(empty);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            Assert.IsNotNull(m_set);
            nId = m_set.Id;
            Assert.IsTrue(nId != 0);
            r = oMan.FindDatasetById(nId);
            Assert.IsNotNull(r);
            Assert.IsNotNull(r.Item1);
            Assert.IsNull(r.Item2);
            String sigle = m_set.Sigle;
            Assert.IsFalse(String.IsNullOrEmpty(sigle));
            r = oMan.FindDatasetBySigle(sigle);
            Assert.IsNotNull(r);
            Assert.IsNotNull(r.Item1);
            Assert.IsNull(r.Item2);
            DbDataset px = new DbDataset();
            r = oMan.FindDataset(px);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            //
            px.Id = m_set.Id;
            r = oMan.FindDataset(px);
            Assert.IsNotNull(r);
            Assert.IsNotNull(r.Item1);
            Assert.IsNull(r.Item2);
            //
            DbDataset py = new DbDataset();
            py.Sigle = TEST_DATABASE_SIGLE;
            r = oMan.FindDataset(py);
            Assert.IsNotNull(r);
            DbDataset pz = r.Item1;
            Assert.IsNotNull(pz);
            Assert.IsNull(r.Item2);
            //
            var rr = oMan.RemoveDataset(m_set);
            Assert.IsNotNull(rr);
            Assert.IsTrue(rr.Item1);
            Assert.IsNull(rr.Item2);
        }// TestDbFindDatasetBySigle
    }
}
