using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using DbStatStore;
namespace UnitTestDbStatStore
{
    [TestClass]
    public class StatDataStoreTest
    {
        private static readonly String TEST_DATABASE_SIGLE = "TestSet";
        private static readonly String TEST_VARIABLE_SIGLE = "TestVar";
        private static readonly String TEST_VARIABLE_TYPE = "float";
        private static readonly String TEST_VARIABLE_GENRE = "Input";
        private static readonly String TEST_INDIV_SIGLE = "TestIndiv";
        //
        private TestContext testContextInstance;
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        private StatDataStore m_store;
        private DbDataset m_set;
        private DbVariable m_var;
        private DbIndiv m_indiv;
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
            //
            var rx = m_store.FindVariableByDatasetAndSigle(m_set.Id, TEST_VARIABLE_SIGLE);
            Assert.IsNotNull(rx);
            Assert.IsNull(rx.Item2);
            m_var = rx.Item1;
            if (m_var == null)
            {
                DbVariable pVar = new DbVariable();
                pVar.DatasetId = m_set.Id;
                pVar.Sigle = TEST_VARIABLE_SIGLE;
                pVar.VarType = TEST_VARIABLE_TYPE;
                pVar.Genre = TEST_VARIABLE_GENRE;
                pVar.Name = TEST_VARIABLE_SIGLE;
                pVar.Description = TEST_VARIABLE_SIGLE;
                rx = m_store.MaintainsVariable(pVar);
                Assert.IsNotNull(rx);
                Assert.IsNull(rx.Item2);
                m_var = rx.Item1;
                Assert.IsNotNull(m_var);
            }
            Assert.IsTrue(m_var.Id != 0);
            //
            DbIndiv pInd = new DbIndiv();
            pInd.DatasetId = m_set.Id;
            pInd.Sigle = TEST_INDIV_SIGLE;
            var ry = m_store.FindIndiv(pInd);
            Assert.IsNotNull(ry);
            Assert.IsNull(ry.Item2);
            m_indiv = ry.Item1;
            if (m_indiv == null)
            {
                pInd.Name = TEST_INDIV_SIGLE;
                pInd.Description = TEST_INDIV_SIGLE;
                ry = m_store.MaintainsIndiv(pInd);
                Assert.IsNotNull(ry);
                Assert.IsNull(ry.Item2);
                m_indiv = ry.Item1;
                Assert.IsNotNull(m_indiv);
            }
            Assert.IsTrue(m_indiv.Id != 0);
        } // Initialize

        [TestCleanup()]
        public void Cleanup()
        {
            m_indiv = null;
            m_var = null;
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
            //
            r = oMan.FindDataset(null);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            //
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
            r = oMan.MaintainsDataset(null);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            DbDataset pw = new DbDataset();
            r = oMan.MaintainsDataset(pw);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            //
            DbDataset pt = m_set;
            pt.Sigle = new String('S', 75);
            pt.Name = new String('N', 98);
            pt.Description = new String('D', 300);
            r = oMan.MaintainsDataset(pt);
            Assert.IsNotNull(r);
            Assert.IsNotNull(r.Item1);
          //  Assert.IsNull(r.Item2);
            //
            pt.Id = 0;
            pt.Sigle = null;
            var rx = oMan.RemoveDataset(pt);
            Assert.IsNotNull(rx);
            Assert.IsFalse(rx.Item1);
            Assert.IsNotNull(rx.Item2);
            //
            var rr = oMan.RemoveDataset(m_set);
            Assert.IsNotNull(rr);
            //Assert.IsTrue(rr.Item1);
            //Assert.IsNull(rr.Item2);
            //

        }// TestDbFindDatasetBySigle
        [TestMethod]
        public void TestDbRemoveDataset()
        {
            StatDataStore oMan = m_store;
            Assert.IsNotNull(oMan);
            Assert.IsNotNull(m_set);
            DbDataset pSet = new DbDataset();
            pSet.Sigle = m_set.Sigle;
            var rr = oMan.RemoveDataset(m_set);
            Assert.IsNotNull(rr);
            Assert.IsTrue(rr.Item1);
            Assert.IsNull(rr.Item2);
            //
        }// TestDbFindDatasetBySigle
         [TestMethod]
        public void TestDbGetDatasetVariables()
        {
            StatDataStore oMan = m_store;
            Assert.IsNotNull(oMan);
            Assert.IsNotNull(m_set);
            Assert.IsTrue(m_set.Id != 0);
            //
            var rx = oMan.GetDatasetVariables(m_set);
            Assert.IsNotNull(rx);
            Assert.IsNull(rx.Item2);
            Assert.IsNotNull(rx.Item1);
        }// TestDbFindDatasetBySigle
         [TestMethod]
        public void TestDbFindVariable()
        {
            StatDataStore oMan = m_store;
            Assert.IsNotNull(oMan);
            Assert.IsNotNull(m_set);
            Assert.IsTrue(m_set.Id != 0);
            //
            int nId = 0;
            var r = oMan.FindVariableById(nId);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            //
            nId = 123456;
            r = oMan.FindVariableById(nId);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNull(r.Item2);
            //
            nId = m_var.Id;
            r = oMan.FindVariableById(nId);
            Assert.IsNotNull(r);
            Assert.IsNotNull(r.Item1);
            Assert.IsNull(r.Item2);
            //
            int nDatasetId = 0;
            String sigle = null;
            r = oMan.FindVariableByDatasetAndSigle(nDatasetId, sigle);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            //
            sigle = m_var.Sigle;
            nDatasetId = 0;
            r = oMan.FindVariableByDatasetAndSigle(nDatasetId, sigle);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            //
            sigle = null;
            nDatasetId = m_set.Id;
            r = oMan.FindVariableByDatasetAndSigle(nDatasetId, sigle);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNotNull(r.Item2);
            //
            sigle = m_var.Sigle;
            nDatasetId = m_set.Id;
            r = oMan.FindVariableByDatasetAndSigle(nDatasetId, sigle);
            Assert.IsNotNull(r);
            Assert.IsNotNull(r.Item1);
            Assert.IsNull(r.Item2);
            //
        }// TestDbFindDatasetBySigle
        [TestMethod]
         public void TestDbMaintainsVariables()
         {
             StatDataStore oMan = m_store;
             Assert.IsNotNull(oMan);
             Assert.IsNotNull(m_set);
             Assert.IsTrue(m_set.Id != 0);
             Assert.IsNotNull(m_var);
             Assert.IsTrue(m_var.Id != 0);
             m_var.Name = "TestName2";
             List<DbVariable> oList = new List<DbVariable>() { m_var };
             var rx = oMan.MaintainsVariables(oList);
             Assert.IsNotNull(rx);
             Assert.IsNull(rx.Item2);
             Assert.IsTrue(rx.Item1);
             DbVariable v = new DbVariable();
             v.DatasetId = m_set.Id;
             v.Sigle = m_var.Sigle;
             v.Name = "TestName3";
             List<DbVariable> oList2 = new List<DbVariable>() { v };
             rx = oMan.MaintainsVariables(oList);
             Assert.IsNotNull(rx);
             Assert.IsNull(rx.Item2);
             Assert.IsTrue(rx.Item1);
         }// TestDbMaintainsVariables
        [TestMethod]
         public void TestDbRemoveVariables()
         {
             StatDataStore oMan = m_store;
             Assert.IsNotNull(oMan);
             Assert.IsNotNull(m_set);
             Assert.IsTrue(m_set.Id != 0);
             Assert.IsNotNull(m_var);
             Assert.IsTrue(m_var.Id != 0);
             List<DbVariable> oList = new List<DbVariable>() { m_var };
             var rx = oMan.RemoveVariables(oList);
             Assert.IsNotNull(rx);
             Assert.IsNull(rx.Item2);
             Assert.IsTrue(rx.Item1);
         }// TestDbMaintainsVariables
        [TestMethod]
        public void TestDbRemoveVariable()
        {
            StatDataStore oMan = m_store;
            Assert.IsNotNull(oMan);
            Assert.IsNotNull(m_set);
            Assert.IsTrue(m_set.Id != 0);
            Assert.IsNotNull(m_var);
            Assert.IsTrue(m_var.Id != 0);
            var rx = oMan.RemoveVariable(m_var);
            Assert.IsNotNull(rx);
            Assert.IsNull(rx.Item2);
            Assert.IsTrue(rx.Item1);
        }// TestDbMaintainsVariable
        [TestMethod]
        public void TestDbGetDatasetIndivs()
        {
            StatDataStore oMan = m_store;
            Assert.IsNotNull(oMan);
            Assert.IsNotNull(m_set);
            Assert.IsTrue(m_set.Id != 0);
            //
            int nCount = 0;
            var r = oMan.GetDatasetIndivsCount(m_set);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item2);
            nCount = r.Item1;
            const int taken = 64;
            int skip = 0;
            while (skip < taken)
            {
                var z = oMan.GetDatasetIndivs(m_set, skip, taken);
                Assert.IsNotNull(z);
                Assert.IsNull(z.Item2);
                Assert.IsNotNull(z.Item1);
                IEnumerable<DbIndiv> oInds = z.Item1;
                List<DbIndiv> oList = new List<DbIndiv>(oInds);
                if (oList.Count < 1)
                {
                    break;
                }
                skip += taken;
            }// while
        }// TestDbGetDatasetIndivs
        [TestMethod]
        public void TestDbFindIndiv()
        {
            StatDataStore oMan = m_store;
            Assert.IsNotNull(oMan);
            Assert.IsNotNull(m_set);
            Assert.IsTrue(m_set.Id != 0);
            DbIndiv pInd = new DbIndiv();
            //
            pInd.Id = 0;
            pInd.DatasetId = 0;
            pInd.Sigle = null;
            var r = oMan.FindIndiv(pInd);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNull(r.Item2);
            //
            pInd.Id = 123456;
            r = oMan.FindIndiv(pInd);
            Assert.IsNotNull(r);
            Assert.IsNull(r.Item1);
            Assert.IsNull(r.Item2);
            //
            pInd.Id = m_indiv.Id;
            r = oMan.FindIndiv(pInd);
            Assert.IsNotNull(r);
            Assert.IsNotNull(r.Item1);
            Assert.IsNull(r.Item2);
            //
        }// TestDbFindIndiv
        [TestMethod]
         public void TestDbMaintainsIndivs()
         {
             StatDataStore oMan = m_store;
             Assert.IsNotNull(oMan);
             Assert.IsNotNull(m_set);
             Assert.IsTrue(m_set.Id != 0);
             Assert.IsNotNull(m_indiv);
             Assert.IsTrue(m_var.Id != 0);
             m_indiv.Name = "TestName2";
             List<DbIndiv> oList = new List<DbIndiv>() { m_indiv };
             var rx = oMan.MaintainsIndivs(oList);
             Assert.IsNotNull(rx);
             Assert.IsNull(rx.Item2);
             Assert.IsTrue(rx.Item1);
             DbIndiv v = new DbIndiv();
             v.DatasetId = m_set.Id;
             v.Sigle = m_indiv.Sigle;
             v.Name = "TestName3";
             List<DbIndiv> oList2 = new List<DbIndiv>() { v };
             rx = oMan.MaintainsIndivs(oList);
             Assert.IsNotNull(rx);
             Assert.IsNull(rx.Item2);
             Assert.IsTrue(rx.Item1);
         }// TestDbMaintainsIndivs
        [TestMethod]
         public void TestDbRemoveIndivs()
         {
             StatDataStore oMan = m_store;
             Assert.IsNotNull(oMan);
             Assert.IsNotNull(m_set);
             Assert.IsTrue(m_set.Id != 0);
             Assert.IsNotNull(m_indiv);
             Assert.IsTrue(m_indiv.Id != 0);
             List<DbIndiv> oList = new List<DbIndiv>() { m_indiv };
             var rx = oMan.RemoveIndivs(oList);
             Assert.IsNotNull(rx);
             Assert.IsNull(rx.Item2);
             Assert.IsTrue(rx.Item1);
         }// TestDbRemoveIndivs
    }
}
