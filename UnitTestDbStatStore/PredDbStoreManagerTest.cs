using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using DbStatStore;
using StatDataset;

namespace UnitTestDbStatStore
{
    [TestClass]
    public class PredDbStoreManagerTest
    {
        private static readonly String TEST_DATABASE_SIGLE = "PredTestSet";
        private IStoreDataManager m_man;
        private PredStatDataset m_set;
        [TestInitialize]
        public void Initialize()
        {
            m_man = new PredDbStoreManager();
            Assert.IsNotNull(m_man);
            StatDataset.StatDataset pSet = new StatDataset.StatDataset();
            pSet.Sigle = TEST_DATABASE_SIGLE;
            var r = m_man.FindDataset(pSet);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item2);
            StatDataset.StatDataset p = r.Item1;
            if (p == null)
            {
                pSet.Name = TEST_DATABASE_SIGLE;
                pSet.Description = TEST_DATABASE_SIGLE;
                r = m_man.MaintainsDataset(pSet);
                Assert.IsNotNull(r);
                Assert.IsNull(r.Item2);
                p = r.Item1;
                Assert.IsNotNull(p);
                bool bRet = p is PredStatDataset;
                Assert.IsTrue(bRet);
            }
            m_set = p as PredStatDataset;
            Assert.IsTrue(m_set.Id != 0);
        }// Initialize
        [TestCleanup]
        public void Cleanup()
        {
            m_set = null;
            m_man = null;
        }// Cleanup
        [TestMethod]
        public void TestAFindPredDataset()
        {
            Assert.IsNotNull(m_set);
            Assert.IsNotNull(m_man);
            var r = m_man.FindDataset(m_set);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item2);
            StatDataset.StatDataset p = r.Item1;
            Assert.IsNotNull(p);
            bool bRet = p is PredStatDataset;
            Assert.IsTrue(bRet);
            Assert.IsTrue(m_set.Equals(p));
        }// TestAFindPredDataset
    }// class PredDbStoreManagerTest
}
