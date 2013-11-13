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
        protected virtual IEnumerable<TableDef> getTablesDefs()
        {
            return new List<TableDef>();
        }// getTablesDesf
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

        public virtual Tuple<StatDataset.StatDataset, Exception> FindDataset(StatDataset.StatDataset pSet)
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
                    pRet = new StatDataset.StatDataset();
                    this.convertDataset(p, pRet);
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
            if ((!pSet.IsRemoveable) && (!pSet.IsWriteable))
            {
                return new Tuple<bool, Exception>(false, new ArgumentException());
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
        public virtual Tuple<StatDataset.StatDataset, Exception> MaintainsDataset(StatDataset.StatDataset pSet)
        {
            if (pSet == null)
            {
                return new Tuple<StatDataset.StatDataset, Exception>(null, new ArgumentNullException());
            }
            var pMan = getStore();
            DbDataset pOld = new DbDataset();
            pOld.Id = pSet.Id;
            pOld.Sigle = pSet.Sigle;
            pOld.Name = pSet.Name;
            pOld.Description = pSet.Description;
            var rr = pMan.MaintainsDataset(pOld);
            DbDataset p = rr.Item1;
            Exception err = rr.Item2;
            StatDataset.StatDataset pRet = null;
            if (p != null)
            {
                IEnumerable<TableDef> oTables = this.getTablesDefs();
                if (oTables != null)
                {
                    pMan.MaintainsDataset(p, oTables);
                }
                pRet = new StatDataset.StatDataset();
                this.convertDataset(p, pRet);
            }
            return new Tuple<StatDataset.StatDataset, Exception>(pRet, err);
        }// MaintainsDataset
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
            if (pVar == null)
            {
                return new Tuple<StatVariable, Exception>(null, new ArgumentNullException());
            }
            StatVariable pRet = null;
            var pMan = getStore();
            DbVariable pOld = new DbVariable();
            pOld.Id = pVar.Id;
            pOld.DatasetId = pVar.DatasetId;
            pOld.Sigle = pVar.Sigle;
            var rr = pMan.FindVariable(pOld);
            if (rr.Item2 == null)
            {
                DbVariable p = rr.Item1;
                if (p != null)
                {
                    pRet = new StatVariable();
                    this.convertVariable(p, pRet);
                }
            }// no err
            return new Tuple<StatVariable, Exception>(pRet, rr.Item2);
        }// FindVariable

        public Tuple<StatVariable, Exception> MaintainsVariable(StatVariable pVar)
        {
            if (pVar == null)
            {
                return new Tuple<StatVariable, Exception>(null, new ArgumentNullException());
            }
            if (!pVar.IsWriteable)
            {
                return new Tuple<StatVariable, Exception>(null, new ArgumentException());
            }
            var pMan = getStore();
            DbVariable pOld = new DbVariable();
            pOld.Id = pVar.Id;
            pOld.DatasetId = pVar.DatasetId;
            pOld.Sigle = pVar.Sigle;
            pOld.VarType = pVar.VarType;
            pOld.Genre = pVar.Genre;
            pOld.IsCateg = pVar.IsCateg;
            pOld.Name = pVar.Name;
            pOld.Description = pVar.Description;
            var rr = pMan.MaintainsVariable(pOld);
            DbVariable p = rr.Item1;
            Exception err = rr.Item2;
            StatVariable pRet = null;
            if (p != null)
            {
                pRet = new StatVariable();
                this.convertVariable(p, pRet);
            }
            return new Tuple<StatVariable, Exception>(pRet, err);
        }// MaintainsVariable

        public Tuple<bool, Exception> MaintainsVariables(IEnumerable<StatVariable> oVars)
        {
            if (oVars == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            bool bRet = false;
            Exception err = null;
            List<DbVariable> oList = new List<DbVariable>();
            foreach (var pVar in oVars)
            {
                if (pVar != null)
                {
                    if (pVar.IsWriteable)
                    {
                        DbVariable pOld = new DbVariable();
                        pOld.Id = pVar.Id;
                        pOld.DatasetId = pVar.DatasetId;
                        pOld.Sigle = pVar.Sigle;
                        pOld.VarType = pVar.VarType;
                        pOld.Genre = pVar.Genre;
                        pOld.IsCateg = pVar.IsCateg;
                        pOld.Name = pVar.Name;
                        pOld.Description = pVar.Description;
                        oList.Add(pOld);
                    }// pVar
                }// // pVar
            }// x
            if (oList.Count > 0)
            {
                var pMan = getStore();
                var rr = pMan.MaintainsVariables(oList);
                err = rr.Item2;
                bRet = rr.Item1;
            }
            else
            {
                bRet = true;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// MaintainsVariables

        public Tuple<bool, Exception> RemoveVariable(StatVariable pVar)
        {
            if (pVar == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            if ((!pVar.IsRemoveable) && (!pVar.IsWriteable))
            {
                return new Tuple<bool, Exception>(false, new ArgumentException());
            }
            var pMan = getStore();
            DbVariable pOld = new DbVariable();
            pOld.Id = pVar.Id;
            pOld.DatasetId = pVar.DatasetId;
            pOld.Sigle = pVar.Sigle;
            var rr = pMan.RemoveVariable(pOld);
            bool bRet = rr.Item1;
            Exception err = rr.Item2;
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveVariable

        public Tuple<bool, Exception> RemoveVariables(IEnumerable<StatVariable> oVars)
        {
            if (oVars == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            bool bRet = false;
            Exception err = null;
            List<DbVariable> oList = new List<DbVariable>();
            foreach (var pVar in oVars)
            {
                if (pVar != null)
                {
                    if (pVar.IsRemoveable || pVar.IsWriteable)
                    {
                        DbVariable pOld = new DbVariable();
                        pOld.Id = pVar.Id;
                        pOld.DatasetId = pVar.DatasetId;
                        pOld.Sigle = pVar.Sigle;
                        oList.Add(pOld);
                    }// pVar
                }// // pVar
            }// x
            if (oList.Count > 0)
            {
                var pMan = getStore();
                var rr = pMan.RemoveVariables(oList);
                err = rr.Item2;
                bRet = rr.Item1;
            }
            else
            {
                bRet = true;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveVariables

        public Tuple<int, Exception> GetDatasetIndivsCount(StatDataset.StatDataset pSet)
        {
            if (pSet == null)
            {
                return new Tuple<int, Exception>(0, new ArgumentNullException());
            }
            if ((!pSet.IsRemoveable) && (!pSet.IsWriteable))
            {
                return new Tuple<int, Exception>(0, new ArgumentException());
            }
            var pMan = getStore();
            DbDataset pOld = new DbDataset();
            pOld.Id = pSet.Id;
            pOld.Sigle = pSet.Sigle;
            var rr = pMan.GetDatasetIndivsCount(pOld);
            int nRet = rr.Item1;
            Exception err = rr.Item2;
            return new Tuple<int, Exception>(nRet, err);
        }// GetDatasetIndivsCount

        public Tuple<IEnumerable<StatIndiv>, Exception> GetDatasetIndivs(StatDataset.StatDataset pSet, int skip, int taken)
        {
            if (pSet == null)
            {
                return new Tuple<IEnumerable<StatIndiv>, Exception>(null, new ArgumentNullException());
            }
            if ((skip < 0) || (taken < 1))
            {
                return new Tuple<IEnumerable<StatIndiv>, Exception>(null, new ArgumentException());
            }
            var pMan = getStore();
            DbDataset pOld = new DbDataset();
            pOld.Id = pSet.Id;
            pOld.Sigle = pSet.Sigle;
            var rr = pMan.GetDatasetIndivs(pOld, skip, taken);
            Exception err = rr.Item2;
            List<StatIndiv> oRet = new List<StatIndiv>();
            var col = rr.Item1;
            if ((err != null) && (col != null))
            {
                foreach (var p in col)
                {
                    if (p != null)
                    {
                        StatIndiv pp = new StatIndiv();
                        this.convertIndiv(p, pp);
                        oRet.Add(pp);
                    }// p
                }// p
            }
            return new Tuple<IEnumerable<StatIndiv>, Exception>(oRet, err);
        }//GetDatasetIndivs

        public Tuple<StatIndiv, Exception> FindIndiv(StatIndiv pIndiv)
        {
            if (pIndiv == null)
            {
                return new Tuple<StatIndiv, Exception>(null, new ArgumentNullException());
            }
            StatIndiv pRet = null;
            var pMan = getStore();
            DbIndiv pOld = new DbIndiv();
            pOld.Id = pIndiv.Id;
            pOld.DatasetId = pIndiv.DatasetId;
            pOld.Sigle = pIndiv.Sigle;
            var rr = pMan.FindIndiv(pOld);
            if (rr.Item2 == null)
            {
                DbIndiv p = rr.Item1;
                if (p != null)
                {
                    pRet = new StatIndiv();
                    this.convertIndiv(p, pRet);
                }
            }// no err
            return new Tuple<StatIndiv, Exception>(pRet, rr.Item2);
        }// FindIndiv

        public Tuple<StatIndiv, Exception> MaintainsIndiv(StatIndiv pInd)
        {
            if (pInd == null)
            {
                return new Tuple<StatIndiv, Exception>(null, new ArgumentNullException());
            }
            if (!pInd.IsWriteable)
            {
                return new Tuple<StatIndiv, Exception>(null, new ArgumentException());
            }
            var pMan = getStore();
            DbIndiv pOld = new DbIndiv();
            pOld.Id = pInd.Id;
            pOld.DatasetId = pInd.DatasetId;
            pOld.Sigle = pInd.Sigle;
            pOld.Name = pInd.Name;
            pOld.Description = pInd.Description;
            pOld.Status = pInd.Status;
            pOld.DataBytes = pInd.DataBytes;
            var rr = pMan.MaintainsIndiv(pOld);
            DbIndiv p = rr.Item1;
            Exception err = rr.Item2;
            StatIndiv pRet = null;
            if (p != null)
            {
                pRet = new StatIndiv();
                this.convertIndiv(p, pRet);
            }
            return new Tuple<StatIndiv, Exception>(pRet, err);
        }// MaintainsIndiv

        public Tuple<bool, Exception> MaintainsIndivs(IEnumerable<StatIndiv> oInds)
        {
            if (oInds == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            bool bRet = false;
            Exception err = null;
            List<DbIndiv> oList = new List<DbIndiv>();
            foreach (var pInd in oInds)
            {
                if (pInd != null)
                {
                    if (pInd.IsWriteable)
                    {
                        DbIndiv pOld = new DbIndiv();
                        pOld.Id = pInd.Id;
                        pOld.DatasetId = pInd.DatasetId;
                        pOld.Sigle = pInd.Sigle;
                        pOld.Name = pInd.Name;
                        pOld.Description = pInd.Description;
                        pOld.Status = pInd.Status;
                        pOld.DataBytes = pInd.DataBytes;
                        oList.Add(pOld);
                    }// pVar
                }// // pVar
            }// x
            if (oList.Count > 0)
            {
                var pMan = getStore();
                var rr = pMan.MaintainsIndivs(oList);
                err = rr.Item2;
                bRet = rr.Item1;
            }
            else
            {
                bRet = true;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// MaintainsIndivs

        public Tuple<bool, Exception> RemoveIndiv(StatIndiv pInd)
        {
            if (pInd == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            if ((!pInd.IsRemoveable) && (!pInd.IsWriteable))
            {
                return new Tuple<bool, Exception>(false, new ArgumentException());
            }
            var pMan = getStore();
            DbIndiv pOld = new DbIndiv();
            pOld.Id = pInd.Id;
            pOld.DatasetId = pInd.DatasetId;
            pOld.Sigle = pInd.Sigle;
            var rr = pMan.RemoveIndiv(pOld);
            bool bRet = rr.Item1;
            Exception err = rr.Item2;
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveIndiv

        public Tuple<bool, Exception> RemoveIndivs(IEnumerable<StatIndiv> oInds)
        {
            if (oInds == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            bool bRet = false;
            Exception err = null;
            List<DbIndiv> oList = new List<DbIndiv>();
            foreach (var pInd in oInds)
            {
                if (pInd != null)
                {
                    if (pInd.IsRemoveable || pInd.IsWriteable)
                    {
                        DbIndiv pOld = new DbIndiv();
                        pOld.Id = pInd.Id;
                        pOld.DatasetId = pInd.DatasetId;
                        pOld.Sigle = pInd.Sigle;
                        oList.Add(pOld);
                    }// pVar
                }// // pVar
            }// x
            if (oList.Count > 0)
            {
                var pMan = getStore();
                var rr = pMan.RemoveIndivs(oList);
                err = rr.Item2;
                bRet = rr.Item1;
            }
            else
            {
                bRet = true;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveIndivs

        public Tuple<bool, Exception> MaintainsValues(IEnumerable<StatValue> oVals)
        {
            if (oVals == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            List<DbValue> oList = new List<DbValue>();
            foreach (var oVal in oVals)
            {
                if (oVal != null)
                {
                    if (oVal.IsWriteable)
                    {
                        DbValue pOld = new DbValue();
                        pOld.Id = oVal.Id;
                        pOld.VariableId = oVal.VariableId;
                        pOld.IndivId = oVal.IndivId;
                        pOld.Status = oVal.Status;
                        pOld.Value = oVal.StringValue;
                        oList.Add(pOld);
                    }
                }// oVar
            }// oVal
            bool bRet = false;
            Exception err = null;
            if (oList.Count > 0)
            {
                var pMan = getStore();
                var rr = pMan.MaintainsValues(oList);
                bRet = rr.Item1;
                err = rr.Item2;
            }
            else
            {
                bRet = true;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// MaintainsValues

        public Tuple<bool, Exception> RemoveValues(IEnumerable<StatValue> oVals)
        {
            if (oVals == null)
            {
                return new Tuple<bool, Exception>(false, new ArgumentNullException());
            }
            List<DbValue> oList = new List<DbValue>();
            foreach (var oVal in oVals)
            {
                if (oVal != null)
                {
                    if (oVal.IsWriteable)
                    {
                        DbValue pOld = new DbValue();
                        pOld.Id = oVal.Id;
                        pOld.VariableId = oVal.VariableId;
                        pOld.IndivId = oVal.IndivId;
                        oList.Add(pOld);
                    }
                }// oVar
            }// oVal
            bool bRet = false;
            Exception err = null;
            if (oList.Count > 0)
            {
                var pMan = getStore();
                var rr = pMan.RemoveValues(oList);
                bRet = rr.Item1;
                err = rr.Item2;
            }
            else
            {
                bRet = true;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveValues

        public Tuple<int, Exception> GetVariableValuesCount(StatVariable pVar)
        {
            if (pVar == null)
            {
                return new Tuple<int, Exception>(0, new ArgumentNullException());
            }
            if ((!pVar.IsRemoveable) && (!pVar.IsWriteable))
            {
                return new Tuple<int, Exception>(0, new ArgumentException());
            }
            var pMan = getStore();
            DbVariable pOld = new DbVariable();
            pOld.Id = pVar.Id;
            pOld.DatasetId = pVar.DatasetId;
            pOld.Sigle = pVar.Sigle;
            var rr = pMan.GetVariableValuesCount(pOld);
            int nRet = rr.Item1;
            Exception err = rr.Item2;
            return new Tuple<int, Exception>(nRet, err);
        }// GetVariableValuesCount

        public Tuple<IEnumerable<StatValue>, Exception> GetVariableValues(StatVariable pVar, int skip, int taken)
        {
            if (pVar == null)
            {
                return new Tuple<IEnumerable<StatValue>, Exception>(null, new ArgumentNullException());
            }
            if ((!pVar.IsRemoveable) && (!pVar.IsWriteable))
            {
                return new Tuple<IEnumerable<StatValue>, Exception>(null, new ArgumentException());
            }
            if ((skip < 0) || (taken < 1))
            {
                return new Tuple<IEnumerable<StatValue>, Exception>(null, new ArgumentException());
            }
            var pMan = getStore();
            DbVariable pOld = new DbVariable();
            pOld.Id = pVar.Id;
            pOld.DatasetId = pVar.DatasetId;
            pOld.Sigle = pVar.Sigle;
            var rr = pMan.GetVariableValues(pOld, skip, taken);
            Exception err = rr.Item2;
            List<StatValue> oRet = new List<StatValue>();
            var col = rr.Item1;
            if ((err != null) && (col != null))
            {
                foreach (var p in col)
                {
                    if (p != null)
                    {
                        StatValue pp = new StatValue();
                        this.convertValue(p, pp);
                        oRet.Add(pp);
                    }// p
                }// p
            }
            return new Tuple<IEnumerable<StatValue>, Exception>(oRet, err);
        }//GetVariableValues

        public Tuple<IEnumerable<StatValue>, Exception> GetIndivValues(StatIndiv pInd)
        {
            if (pInd == null)
            {
                return new Tuple<IEnumerable<StatValue>, Exception>(null, new ArgumentNullException());
            }
            if ((!pInd.IsRemoveable) && (!pInd.IsWriteable))
            {
                return new Tuple<IEnumerable<StatValue>, Exception>(null, new ArgumentException());
            }
            var pMan = getStore();
            DbIndiv pOld = new DbIndiv();
            pOld.Id = pInd.Id;
            pOld.DatasetId = pInd.DatasetId;
            pOld.Sigle = pInd.Sigle;
            var rr = pMan.GetIndivValues(pOld);
            Exception err = rr.Item2;
            List<StatValue> oRet = new List<StatValue>();
            var col = rr.Item1;
            if ((err != null) && (col != null))
            {
                foreach (var p in col)
                {
                    if (p != null)
                    {
                        StatValue pp = new StatValue();
                        this.convertValue(p, pp);
                        oRet.Add(pp);
                    }// p
                }// p
            }
            return new Tuple<IEnumerable<StatValue>, Exception>(oRet, err);
        }//GetIndivValues 

        public Tuple<StatValue, Exception> FindValue(StatValue pVal)
        {
            if (pVal == null)
            {
                return new Tuple<StatValue, Exception>(null, new ArgumentNullException());
            }
            StatValue pRet = null;
            var pMan = getStore();
            DbValue pOld = new DbValue();
            pOld.Id = pVal.Id;
            pOld.VariableId = pVal.VariableId;
            pOld.IndivId = pVal.IndivId;
            var rr = pMan.FindValue(pOld);
            if (rr.Item2 == null)
            {
                DbValue p = rr.Item1;
                if (p != null)
                {
                    pRet = new StatValue();
                    this.convertValue(p, pRet);
                }
            }// no err
            return new Tuple<StatValue, Exception>(pRet, rr.Item2);
        }// FindValue

        public Tuple<IEnumerable<Tuple<int, object>>, Exception> ConvertValues(IEnumerable<StatValue> oVals)
        {
            if (oVals == null)
            {
                return new Tuple<IEnumerable<Tuple<int, object>>, Exception>(null, new ArgumentNullException());
            }
            List<DbValue> oList = new List<DbValue>();
            foreach (var oVal in oVals)
            {
                if (oVal != null)
                {
                    if (oVal.IsWriteable)
                    {
                        DbValue pOld = new DbValue();
                        pOld.Id = oVal.Id;
                        pOld.VariableId = oVal.VariableId;
                        pOld.IndivId = oVal.IndivId;
                        pOld.Status = oVal.Status;
                        pOld.Value = oVal.StringValue;
                        oList.Add(pOld);
                    }
                }// oVar
            }// oVal
            var pMan = getStore();
            var rr = pMan.ConvertValues(oList);
            return rr;
        }// ConvertValues

        public Tuple<Dictionary<int, Dictionary<int, object>>, Exception> GetCommonValues(IEnumerable<StatVariable> oVars)
        {
            if (oVars == null)
            {
                return new Tuple<Dictionary<int, Dictionary<int, object>>, Exception>(null, new ArgumentNullException());
            }
            List<DbVariable> oList = new List<DbVariable>();
            foreach (var pVar in oVars)
            {
                if (pVar != null)
                {
                    if (pVar.IsRemoveable || pVar.IsWriteable)
                    {
                        DbVariable pOld = new DbVariable();
                        pOld.Id = pVar.Id;
                        pOld.DatasetId = pVar.DatasetId;
                        pOld.Sigle = pVar.Sigle;
                        oList.Add(pOld);
                    }// pVar
                }// // pVar
            }// x
            Exception err = null;
            Dictionary<int, Dictionary<int, object>> oRet = null;
            if (oList.Count > 0)
            {
                var pMan = getStore();
                var rr = pMan.GetCommonValues(oList);
                err = rr.Item2;
                oRet = rr.Item1;
            }
            else
            {
                oRet = new Dictionary<int, Dictionary<int, object>>();
            }
            return new Tuple<Dictionary<int, Dictionary<int, object>>, Exception>(oRet, err);
        }// GetCommonValues
    }// class DbStatStoreManager
}
