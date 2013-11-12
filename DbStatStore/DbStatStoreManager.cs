using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StatDataset;
namespace DbStatStore
{
    public class DbStatStoreManager : IStoreDataManager
    {
        public DbStatStoreManager()
        {

        }
        protected StatDataStore getStore()
        {
            return new StatDataStore();
        }
        protected void convertDataset(DbDataset p, StatDataset.StatDataset pp){
            pp.Id = p.Id;
            pp.Version = p.Version;
            pp.Sigle = p.Sigle;
            pp.Name = p.Name;
            pp.Description = p.Description;
            pp.IsModified = false;
        }// convertDataset
        protected void convertVariable(DbVariable p, StatVariable pp)
        {
            pp.Id = p.Id;
            pp.Version = p.Version;
            pp.DatasetId = p.DatasetId;
            pp.VarType = p.VarType;
            pp.Genre = p.Genre;
            pp.Sigle = p.Sigle;
            pp.Name = p.Name;
            pp.Description = p.Description;
            pp.IsModified = false;
        }// convertVariable
        protected void convertIndiv(DbIndiv p, StatIndiv pp)
        {
            pp.Id = p.Id;
            pp.Version = p.Version;
            pp.DatasetId = p.DatasetId;
            pp.Status = p.Status;
            pp.DataBytes = p.DataBytes;
            pp.Sigle = p.Sigle;
            pp.Name = p.Name;
            pp.Description = p.Description;
            pp.IsModified = false;
        }// ConvertIndiv
        protected void convertValue(DbValue p, StatValue pp)
        {
            pp.Id = p.Id;
            pp.Version = p.Version;
            pp.VariableId = p.VariableId;
            pp.IndivId = p.IndivId;
            pp.StringValue = p.Value;
            pp.Status = p.Status;
            pp.IsModified = false;
        }// convertValue
        public Tuple<IEnumerable<StatDataset.StatDataset>, Exception> GetAllDataSets()
        {
            var pMan = getStore();
            List<StatDataset.StatDataset> oRet = new List<StatDataset.StatDataset>();
            var rr = pMan.GetAllDataSets();
            var col = rr.Item1;
            if (col != null)
            {
                foreach (var p in col)
                {
                    StatDataset.StatDataset pp = new StatDataset.StatDataset();
                    this.convertDataset(p, pp);
                    oRet.Add(pp);
                }// p
            }// col
            return new Tuple<IEnumerable<StatDataset.StatDataset>, Exception>(oRet, rr.Item2);
        }// getAllDataSets

        public Tuple<StatDataset.StatDataset, Exception> FindDataset(StatDataset.StatDataset pSet)
        {
            if (pSet == null)
            {
                return new Tuple<StatDataset.StatDataset, Exception>(null, new ArgumentNullException());
            }
            StatDataset.StatDataset pRet = null;
            var pMan = getStore();
            DbDataset pOld = new DbDataset();
            pOld.Id = pSet.Id;
            pOld.Sigle = pSet.Sigle;
            var rr = pMan.FindDataset(pOld);
            if (rr.Item2 == null)
            {
                DbDataset p = rr.Item1;
                if (p != null)
                {
                    this.convertDataset(p, pSet);
                    pRet = pSet;
                }
            }// no err
            return new Tuple<StatDataset.StatDataset, Exception>(pRet, rr.Item2);
        }// FindDataset

        public Tuple<bool, Exception> RemoveDataset(StatDataset.StatDataset pSet)
        {
            if (pSet == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            var pMan = getStore();
            DbDataset pOld = new DbDataset();
            pOld.Id = pSet.Id;
            pOld.Sigle = pSet.Sigle;
            var rr = pMan.RemoveDataset(pOld);
            bool bRet = rr.Item1;
            Exception err = rr.Item2;
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveDataset

        public Tuple<IEnumerable<StatVariable>, Exception> GetDatasetVariables(StatDataset.StatDataset pSet)
        {
            if (pSet == null)
            {
                return new Tuple<IEnumerable<StatVariable>, Exception>(null, new ArgumentNullException());
            }
            List<StatVariable> oRet = new List<StatVariable>();
            Exception err = null;
            var pMan = getStore();
            DbDataset pOld = new DbDataset();
            pOld.Id = pSet.Id;
            pOld.Sigle = pSet.Sigle;
            var rr = pMan.GetDatasetVariables(pOld);
            err = rr.Item2;
            if (err == null)
            {
                var col = rr.Item1;
                if (col != null)
                {
                    foreach (var p in col)
                    {
                        StatVariable pp = new StatVariable();
                        this.convertVariable(p, pp);
                        oRet.Add(pp);
                    }// p
                }// col
            }
            return new Tuple<IEnumerable<StatVariable>, Exception>(oRet, err);
        }//GetDatasetVariables 

        public Tuple<StatVariable, Exception> FindVariable(StatVariable pVar)
        {
            throw new NotImplementedException();
        }

        public Tuple<StatVariable, Exception> MaintainsVariable(StatVariable pVar)
        {
            throw new NotImplementedException();
        }

        public Tuple<bool, Exception> MaintainsVariables(IEnumerable<StatVariable> oVars)
        {
            throw new NotImplementedException();
        }

        public Tuple<bool, Exception> RemoveVariable(StatVariable pVar)
        {
            throw new NotImplementedException();
        }

        public Tuple<bool, Exception> RemoveVariables(IEnumerable<StatVariable> oVars)
        {
            throw new NotImplementedException();
        }

        public Tuple<int, Exception> GetDatasetIndivsCount(StatDataset.StatDataset pSet)
        {
            throw new NotImplementedException();
        }

        public Tuple<IEnumerable<StatIndiv>, Exception> GetDatasetIndivs(StatDataset.StatDataset pSet, int skip, int taken)
        {
            throw new NotImplementedException();
        }

        public Tuple<StatIndiv, Exception> FindIndiv(StatIndiv pIndiv)
        {
            throw new NotImplementedException();
        }

        public Tuple<StatIndiv, Exception> MaintainsIndiv(StatIndiv pInd)
        {
            throw new NotImplementedException();
        }

        public Tuple<bool, Exception> MaintainsIndivs(IEnumerable<StatIndiv> oInds)
        {
            throw new NotImplementedException();
        }

        public Tuple<bool, Exception> RemoveIndiv(StatIndiv pInd)
        {
            throw new NotImplementedException();
        }

        public Tuple<bool, Exception> RemoveIndivs(IEnumerable<StatIndiv> oInds)
        {
            throw new NotImplementedException();
        }

        public Tuple<bool, Exception> MaintainsValues(IEnumerable<StatValue> oVals)
        {
            throw new NotImplementedException();
        }

        public Tuple<bool, Exception> RemoveValues(IEnumerable<StatValue> oVals)
        {
            throw new NotImplementedException();
        }

        public Tuple<int, Exception> GetVariableValuesCount(StatVariable pVar)
        {
            throw new NotImplementedException();
        }

        public Tuple<IEnumerable<StatValue>, Exception> GetVariableValues(StatVariable pVar, int skip, int taken)
        {
            throw new NotImplementedException();
        }

        public Tuple<IEnumerable<StatValue>, Exception> GetIndivValues(StatIndiv pInd)
        {
            throw new NotImplementedException();
        }

        public Tuple<StatValue, Exception> FindValue(StatValue pVal)
        {
            throw new NotImplementedException();
        }

        public Tuple<IEnumerable<Tuple<int, object>>, Exception> ConvertValues(IEnumerable<StatValue> oVals)
        {
            throw new NotImplementedException();
        }

        public Tuple<Dictionary<int, Dictionary<int, object>>, Exception> GetCommonValues(IEnumerable<StatVariable> oVars)
        {
            throw new NotImplementedException();
        }
    }// class DbStatStoreManager
}
