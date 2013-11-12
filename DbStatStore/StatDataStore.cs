using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DbStatStore
{
    public class StatDataStore
    {
        public StatDataStore()
        {

        }
        #region Protected Methods
        protected statdatastoreEntities getContext()
        {
            return new statdatastoreEntities();
        }
        protected Object convertValue(statdatastoreEntities ctx, Dictionary<int, String> oTypes, int nVarId, String sval)
        {
            Object pRet = null;
            if ((nVarId != 0) && (!String.IsNullOrEmpty(sval)) || (oTypes == null))
            {
                String s = sval.Trim();
                if (!String.IsNullOrEmpty(s))
                {
                    String stype = null;
                    if (oTypes.ContainsKey(nVarId))
                    {
                        stype = oTypes[nVarId];
                    }
                    else
                    {
                        var q = from x in ctx.DbVariables where (x.Id == nVarId) select x.VarType;
                        if (q.Count() < 1)
                        {
                            return pRet;
                        }
                        stype = q.First().Trim().ToLower();
                        oTypes[nVarId] = stype;
                    }
                    if (stype.Contains("int"))
                    {
                        int nVal = 0;
                        if (int.TryParse(s, out nVal))
                        {
                            pRet = nVal;
                        }
                    }
                    else if (stype.Contains("float"))
                    {
                        float fval = 0.0f;
                        if (float.TryParse(s, out fval))
                        {
                            pRet = fval;
                        }
                    }
                    else if (stype.Contains("double"))
                    {
                        double d = 0.0;
                        if (double.TryParse(s, out d))
                        {
                            pRet = d;
                        }
                    }
                    else
                    {
                        pRet = s;
                    }
                }// not empty
            }// sval
            return pRet;
        }// convertValue
        protected IEnumerable<Tuple<int, Object>> convertValues(statdatastoreEntities ctx, IEnumerable<DbValue> oVals)
        {
            List<Tuple<int, Object>> oRet = null;
            if (oVals != null)
            {
                oRet = new List<Tuple<int, object>>();
                Dictionary<int, String> oTypes = new Dictionary<int, string>();
                foreach (var p in oVals)
                {
                    if (p != null)
                    {
                        String sval = p.Value;
                        if (!String.IsNullOrEmpty(sval))
                        {
                            sval = sval.Trim();
                            if (!String.IsNullOrEmpty(sval))
                            {
                                int nId = p.IndivId;
                                int nVarId = p.VariableId;
                                if ((nId != 0) && (nVarId != 0))
                                {
                                    Object pRet = this.convertValue(ctx, oTypes, nVarId, sval);
                                    if (pRet != null)
                                    {
                                        oRet.Add(new Tuple<int, Object>(nId, pRet));
                                    }
                                }// ids
                            }
                        }

                    }// p
                }// p
            }// oVals
            return oRet;
        }// convertValues
        protected DbDataset findDatasetById(statdatastoreEntities ctx, int nId)
        {
            return ctx.DbDatasets.Find(nId);
        }// FindDatasetById
        protected DbDataset findDatasetBySigle(statdatastoreEntities ctx, String sigle)
        {
            DbDataset pRet = null;
            if (String.IsNullOrEmpty(sigle))
            {
                return null;
            }
            String ss = sigle.Trim().ToUpper();
            var q = from x in ctx.DbDatasets where (x.Sigle.Trim().ToUpper() == ss) select x;
            if (q.Count() > 0)
            {
                pRet = q.First();
            }
            return pRet;
        }// FindDatasetBySigle
        protected DbVariable findVariableById(statdatastoreEntities ctx, int nVarId)
        {
            return ctx.DbVariables.Find(nVarId);
        }// FindVariableById
        protected DbVariable findVariableByDatasetAndSigle(statdatastoreEntities ctx, int nDatasetId, String sigle)
        {
            DbVariable pRet = null;
            if ((nDatasetId != 0) && (!String.IsNullOrEmpty(sigle)))
            {
                String ss = sigle.Trim().ToUpper();
                var q = from x in ctx.DbVariables where (x.DatasetId == nDatasetId) && (x.Sigle.Trim().ToUpper() == ss) select x;
                if (q.Count() > 0)
                {
                    pRet = q.First();
                }
            }
            return pRet;
        }// FindVariableById
        protected DbVariable maintainsVariable(statdatastoreEntities ctx, DbVariable pVar)
        {
            if (pVar == null)
            {
                return null;
            }
            String sigle = pVar.Sigle;
            if (String.IsNullOrEmpty(sigle))
            {
                return null;
            }
            else
            {
                sigle = sigle.Trim();
                if (sigle.Length > 31)
                {
                    sigle = sigle.Substring(0, 31).Trim();
                }
            }
            String stype = pVar.VarType;
            if (String.IsNullOrEmpty(stype))
            {
                stype = "float";
            }
            else
            {
                stype = stype.Trim().ToLower();
                if (stype.Length > 15)
                {
                    stype = stype.Substring(0, 15).Trim();
                }
            }
            bool bCateg = pVar.IsCateg;
            if ((!stype.Contains("int")) && (!stype.Contains("double")) && (!stype.Contains("float")))
            {
                bCateg = true;
            }
            String name = pVar.Name;
            if (!String.IsNullOrEmpty(name))
            {
                if (name.Length > 63)
                {
                    name = name.Substring(0, 63).Trim();
                }
            }
            String desc = pVar.Description;
            if (!String.IsNullOrEmpty(desc))
            {
                if (desc.Length > 255)
                {
                    desc = desc.Substring(0, 255).Trim();
                }
            }
            String genre = pVar.Genre;
            if (!String.IsNullOrEmpty(genre))
            {
                genre = genre.Trim().ToLower();
                if (genre.Length > 15)
                {
                    genre = genre.Substring(0, 15).Trim();
                }
            }
            int nId = pVar.Id;
            if (nId != 0)
            {
                DbVariable pOld = this.findVariableById(ctx, nId);
                if (pOld == null)
                {
                    nId = 0;
                }
                else
                {
                    DbVariable pCheck = this.findVariableByDatasetAndSigle(ctx, pOld.DatasetId, sigle);
                    if ((pCheck != null) && (pCheck.Id != pOld.Id))
                    {
                        return null;
                    }
                    pOld.Version = pOld.Version + 1;
                    pOld.Sigle = sigle;
                    pOld.VarType = stype;
                    pOld.Genre = genre;
                    pOld.Name = name;
                    pOld.Description = desc;
                    pOld.IsCateg = bCateg;
                    return pOld;
                }
            }// nId
            int nDatasetId = pVar.DatasetId;
            if (nDatasetId == 0)
            {
                return null;
            }
            DbDataset pSet = this.findDatasetById(ctx, nDatasetId);
            if (pSet == null)
            {
                return null;
            }
            DbVariable pp = this.findVariableByDatasetAndSigle(ctx, nDatasetId, sigle);
            if (pp != null)
            {
                pp.Version = pp.Version + 1;
                pp.VarType = stype;
                pp.Genre = genre;
                pp.Name = name;
                pp.Description = desc;
                pp.IsCateg = bCateg;
                return pp;
            }
            DbVariable pRet = new DbVariable();
            pRet.Dataset = pSet;
            pRet.Version = 1;
            pRet.Sigle = sigle;
            pRet.VarType = stype;
            pRet.IsCateg = bCateg;
            pRet.Name = name;
            pRet.Description = desc;
            ctx.DbVariables.Add(pRet);
            return pRet;
        }// maintainsVariable
        protected void removeValue(statdatastoreEntities ctx, DbValue p)
        {
            int nId = p.Id;
            if (nId != 0)
            {
                DbValue v = ctx.DbValues.Find(nId);
                if (v != null)
                {
                    ctx.DbValues.Remove(v);
                }
            }
        }// removeValue
        protected void removeVariable(statdatastoreEntities ctx, DbVariable p)
        {
            int nId = p.Id;
            if (nId != 0)
            {
                DbVariable v = ctx.DbVariables.Find(nId);
                if (v != null)
                {
                    while (v.Values.Count() > 0)
                    {
                        DbValue vx = v.Values.First();
                        ctx.DbValues.Remove(vx);
                        v.Values.Remove(vx);
                    }
                    ctx.DbVariables.Remove(v);
                }
            }
        }// removeVariable
        protected void removeIndiv(statdatastoreEntities ctx, DbIndiv p)
        {
            int nId = p.Id;
            if (nId != 0)
            {
                DbIndiv v = ctx.DbIndivs.Find(nId);
                if (v != null)
                {
                    while (v.Values.Count() > 0)
                    {
                        DbValue vx = v.Values.First();
                        ctx.DbValues.Remove(vx);
                        v.Values.Remove(vx);
                    }
                    ctx.DbIndivs.Remove(v);
                }
            }
        }// removeIndiv
        protected void removeDataset(statdatastoreEntities ctx, DbDataset p)
        {
            int nId = p.Id;
            if (nId != 0)
            {
                DbDataset v = ctx.DbDatasets.Find(nId);
                if (v != null)
                {
                    while (v.Variables.Count() > 0)
                    {
                        DbVariable vx = v.Variables.First();
                        this.removeVariable(ctx, vx);
                        v.Variables.Remove(vx);
                    }
                    while (v.Indivs.Count() > 0)
                    {
                        DbIndiv vx = v.Indivs.First();
                        this.removeIndiv(ctx, vx);
                        v.Indivs.Remove(vx);
                    }
                    ctx.DbDatasets.Remove(v);
                }// v
            }
        }// removeDataset
        protected int getDatasetIndivsCount(statdatastoreEntities ctx, int nDatasetId)
        {
            var q = from x in ctx.DbIndivs where (x.DatasetId == nDatasetId) select x;
            int nRet = q.Count();
            return nRet;
        }
        protected Tuple<IEnumerable<DbIndiv>, Exception> getDatasetIndivs(statdatastoreEntities ctx, int nDatasetId, int skip, int taken)
        {
            List<DbIndiv> oRet = null;
            Exception err = null;
            try
            {
                var q = (from x in ctx.DbIndivs where (x.DatasetId == nDatasetId) orderby x.Sigle ascending select x).Skip(skip).Take(taken);
                oRet = new List<DbIndiv>();
                foreach (var p in q)
                {
                    oRet.Add(p);
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<IEnumerable<DbIndiv>, Exception>(oRet, err);
        }
        protected DbIndiv findIndivById(statdatastoreEntities ctx, int nIndivId)
        {
            return ctx.DbIndivs.Find(nIndivId);
        }// findIndivById
        protected DbIndiv findIndivByDatasetAndSigle(statdatastoreEntities ctx, int nDatasetId, String sigle)
        {
            DbIndiv pRet = null;
            if ((nDatasetId != 0) && (!String.IsNullOrEmpty(sigle)))
            {
                String ss = sigle.Trim().ToUpper();
                var q = from x in ctx.DbIndivs where (x.DatasetId == nDatasetId) && (x.Sigle.Trim().ToUpper() == ss) select x;
                if (q.Count() > 0)
                {
                    pRet = q.First();
                }
            }
            return pRet;
        }// findIndivByDatasetAndSigle
        protected DbIndiv findIndiv(statdatastoreEntities ctx, DbIndiv p)
        {
            DbIndiv pRet = null;
            if (p != null)
            {
                int nId = p.Id;
                if (nId != 0)
                {
                    pRet = this.findIndivById(ctx, nId);
                }
                if (pRet == null)
                {
                    pRet = this.findIndivByDatasetAndSigle(ctx, p.DatasetId, p.Sigle);
                }
            }// p
            return pRet;
        }// findIndiv
        protected DbVariable findVariable(statdatastoreEntities ctx, DbVariable p)
        {
            DbVariable pRet = null;
            if (p != null)
            {
                int nId = p.Id;
                if (nId != 0)
                {
                    pRet = this.findVariableById(ctx, nId);
                }
                if (pRet == null)
                {
                    pRet = this.findVariableByDatasetAndSigle(ctx, p.DatasetId, p.Sigle);
                }
            }// p
            return pRet;
        }// findVariable
        protected DbIndiv maintainsIndiv(statdatastoreEntities ctx, DbIndiv pInd)
        {
            if (pInd == null)
            {
                return null;
            }
            String sigle = pInd.Sigle;
            if (String.IsNullOrEmpty(sigle))
            {
                return null;
            }
            else
            {
                sigle = sigle.Trim();
                if (sigle.Length > 31)
                {
                    sigle = sigle.Substring(0, 31).Trim();
                }
            }
            String status = pInd.Status;
            if (!String.IsNullOrEmpty(status))
            {
                status = status.Trim().ToLower();
                if (status.Length > 15)
                {
                    status = status.Substring(0, 15).Trim();
                }
            }
            String name = pInd.Name;
            if (!String.IsNullOrEmpty(name))
            {
                if (name.Length > 63)
                {
                    name = name.Substring(0, 63).Trim();
                }
            }
            String desc = pInd.Description;
            if (!String.IsNullOrEmpty(desc))
            {
                if (desc.Length > 255)
                {
                    desc = desc.Substring(0, 255).Trim();
                }
            }
            byte[] dataBytes = pInd.DataBytes;
            if (dataBytes != null)
            {
                if (dataBytes.Length < 2)
                {
                    dataBytes = null;
                }
            }
            int nId = pInd.Id;
            if (nId != 0)
            {
                DbIndiv pOld = this.findIndivById(ctx, nId);
                if (pOld == null)
                {
                    nId = 0;
                }
                else
                {
                    DbIndiv pCheck = this.findIndivByDatasetAndSigle(ctx, pOld.DatasetId, sigle);
                    if ((pCheck != null) && (pCheck.Id != pOld.Id))
                    {
                        return null;
                    }
                    pOld.Version = pOld.Version + 1;
                    pOld.Sigle = sigle;
                    pOld.Status = status;
                    pOld.Name = name;
                    pOld.Description = desc;
                    pOld.DataBytes = dataBytes;
                    return pOld;
                }
            }// nId
            int nDatasetId = pInd.DatasetId;
            if (nDatasetId == 0)
            {
                return null;
            }
            DbDataset pSet = this.findDatasetById(ctx, nDatasetId);
            if (pSet == null)
            {
                return null;
            }
            DbIndiv pp = this.findIndivByDatasetAndSigle(ctx, nDatasetId, sigle);
            if (pp != null)
            {
                pp.Version = pp.Version + 1;
                pp.Status = status;
                pp.DataBytes = dataBytes;
                pp.Name = name;
                pp.Description = desc;
                return pp;
            }
            DbIndiv pRet = new DbIndiv();
            pRet.Dataset = pSet;
            pRet.Version = 1;
            pRet.Sigle = sigle;
            pRet.Status = status;
            pRet.DataBytes = dataBytes;
            pRet.Name = name;
            pRet.Description = desc;
            ctx.DbIndivs.Add(pRet);
            return pRet;
        }// maintainsIndiv
        protected DbValue findValue(statdatastoreEntities ctx, DbValue pVal)
        {
            DbValue pRet = null;
            if (pVal != null)
            {
                int nId = pVal.Id;
                if (nId != 0)
                {
                    if ((pRet = ctx.DbValues.Find(nId)) != null)
                    {
                        return pRet;
                    }
                }
                int nVariableId = pVal.VariableId;
                int nIndivId = pVal.IndivId;
                if ((nVariableId != 0) && (nIndivId != 0))
                {
                    var q = from x in ctx.DbValues where (x.VariableId == nVariableId) && (x.IndivId == nIndivId) select x;
                    if (q.Count() > 0)
                    {
                        pRet = q.First();
                    }
                }
            }// pVal
            return pRet;
        }// findValue
        protected DbValue maintainsValue(statdatastoreEntities ctx, DbValue pVal)
        {
            if (pVal == null)
            {
                return null;
            }
            DbValue pRet = null;
            String sval = pVal.Value;
            if (!String.IsNullOrEmpty(sval))
            {
                sval = sval.Trim();
                if (sval.Length > 31)
                {
                    sval = sval.Substring(0, 31).Trim();
                }
            }
            else
            {
                sval = String.Empty;
            }
            String status = pVal.Status;
            if (!String.IsNullOrEmpty(status))
            {
                if (status.Length > 15)
                {
                    status = status.Substring(0, 15).Trim();
                }
            }
            else
            {
                status = String.Empty;
            }
            int nId = pVal.Id;
            if (nId != 0)
            {
                if ((pRet = ctx.DbValues.Find(nId)) != null)
                {
                    return pRet;
                }
            }
            if (pRet != null)
            {
                String oldStatus = pRet.Status;
                if (!String.IsNullOrEmpty(oldStatus))
                {
                    oldStatus = oldStatus.Trim();
                }
                else
                {
                    oldStatus = String.Empty;
                }
                String sOld = pRet.Value;
                if (!String.IsNullOrEmpty(sOld))
                {
                    sOld = sOld.Trim();
                }
                else
                {
                    sOld = String.Empty;
                }
                if ((sOld.ToLower() != sval.ToLower()) || (status.ToLower() != oldStatus.ToLower()))
                {
                    pRet.Version = pRet.Version + 1;
                    if (String.IsNullOrEmpty(sval))
                    {
                        pRet.Value = null;
                    }
                    else
                    {
                        pRet.Value = sval;
                    }
                    pRet.Value = sval;
                    if (String.IsNullOrEmpty(status))
                    {
                        pRet.Status = null;
                    }
                    else
                    {
                        pRet.Status = status;
                    }
                }
            }
            else
            {
                int nVariableId = pVal.VariableId;
                int nIndivId = pVal.IndivId;
                if ((nVariableId == 0) || (nIndivId == 0))
                {
                    return null;
                }
                var q = from x in ctx.DbValues where (x.VariableId == nVariableId) && (x.IndivId == nIndivId) select x;
                if (q.Count() > 0)
                {
                    pRet = q.First();
                }
                if (pRet == null)
                {
                    DbVariable pVar = ctx.DbVariables.Find(nVariableId);
                    DbIndiv pInd = ctx.DbIndivs.Find(nIndivId);
                    if ((pVar == null) || (pInd == null))
                    {
                        return null;
                    }
                    pRet = new DbValue();
                    pRet.Variable = pVar;
                    pRet.Indiv = pInd;
                    pRet.Version = 1;
                    if (!String.IsNullOrEmpty(sval))
                    {
                        pRet.Value = sval;
                    }
                    if (!String.IsNullOrEmpty(status))
                    {
                        pRet.Status = status;
                    }
                    ctx.DbValues.Add(pRet);
                }
                else
                {
                    String oldStatus = pRet.Status;
                    if (!String.IsNullOrEmpty(oldStatus))
                    {
                        oldStatus = oldStatus.Trim();
                    }
                    else
                    {
                        oldStatus = String.Empty;
                    }
                    String sOld = pRet.Value;
                    if (!String.IsNullOrEmpty(sOld))
                    {
                        sOld = sOld.Trim();
                    }
                    else
                    {
                        sOld = String.Empty;
                    }
                    if ((sOld.ToLower() != sval.ToLower()) || (status.ToLower() != oldStatus.ToLower()))
                    {
                        pRet.Version = pRet.Version + 1;
                        if (String.IsNullOrEmpty(sval))
                        {
                            pRet.Value = null;
                        }
                        else
                        {
                            pRet.Value = sval;
                        }
                        pRet.Value = sval;
                        if (String.IsNullOrEmpty(status))
                        {
                            pRet.Status = null;
                        }
                        else
                        {
                            pRet.Status = status;
                        }
                    }
                }
            }// pRet
            return pRet;
        }// findValue
        #endregion // Protected Methods
       
        public Tuple<IEnumerable<DbDataset>, Exception> GetAllDataSets()
        {
            List<DbDataset> oRet = null;
            Exception err = null;
            try
            {
                statdatastoreEntities ctx = getContext();
                var q = from x in ctx.DbDatasets orderby x.Sigle ascending select x;
                oRet = new List<DbDataset>();
                foreach (var p in q)
                {
                    oRet.Add(p);
                }// p
            }
            catch (Exception ex)
            {
                ex = err;
            }
            return new Tuple<IEnumerable<DbDataset>, Exception>(oRet, err);
        }// getAllDataSets
        public Tuple<DbDataset, Exception> FindDatasetById(int nId)
        {
            DbDataset pRet = null;
            if (nId == 0)
            {
                return new Tuple<DbDataset, Exception>(pRet, new ArgumentException());
            }
            Exception err = null;
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.findDatasetById(ctx, nId);
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbDataset, Exception>(pRet, err);
        }// FindDatasetById
        public Tuple<DbDataset, Exception> FindDatasetBySigle(String sigle)
        {
            DbDataset pRet = null;
            if (String.IsNullOrEmpty(sigle))
            {
                return new Tuple<DbDataset, Exception>(pRet, new ArgumentException());
            }
            Exception err = null;
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.findDatasetBySigle(ctx, sigle);
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbDataset, Exception>(pRet, err);
        }// FindDatasetBySigle
        public Tuple<DbDataset, Exception> FindDataset(DbDataset pSet)
        {
            DbDataset pRet = null;
            Exception err = null;
            if (pSet == null)
            {
                return new Tuple<DbDataset, Exception>(pRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                int nId = pSet.Id;
                if (nId != 0)
                {
                    pRet = this.findDatasetById(ctx, nId);
                    if (pRet != null)
                    {
                        return new Tuple<DbDataset, Exception>(pRet, err);
                    }
                }
                String s = pSet.Sigle;
                if (pRet == null)
                {
                    if (String.IsNullOrEmpty(s))
                    {
                        return new Tuple<DbDataset, Exception>(pRet, new ArgumentException());
                    }
                }
                pRet = this.findDatasetBySigle(ctx, s);
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbDataset, Exception>(pRet, err);
        }// FindDatasetBySigle
        public Tuple<DbDataset, Exception> MaintainsDataset(DbDataset pSet)
        {
            DbDataset pRet = null;
            if (pSet == null)
            {
                return new Tuple<DbDataset, Exception>(pRet, new ArgumentNullException());
            }
            String sigle = pSet.Sigle;
            if (String.IsNullOrEmpty(sigle))
            {
                return new Tuple<DbDataset, Exception>(pRet, new ArgumentException());
            }
            String sSigle = sigle.Trim();
            if (sSigle.Length > 31)
            {
                sSigle = sSigle.Substring(0, 31).Trim();
            }
            Exception err = null;
            String name = pSet.Name;
            if (!String.IsNullOrEmpty(name))
            {
                if (name.Length > 63)
                {
                    name = name.Substring(0, 63).Trim();
                }
            }
            String desc = pSet.Description;
            if (!String.IsNullOrEmpty(desc))
            {
                if (desc.Length > 255)
                {
                    desc = desc.Substring(0, 255).Trim();
                }
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                int nId = pSet.Id;
                if (nId != 0)
                {
                    pRet = this.findDatasetById(ctx, nId);
                }
                if (pRet == null)
                {
                    pRet = this.findDatasetBySigle(ctx, sSigle);
                }
                if (pRet != null)
                {
                    pRet.Sigle = sSigle;
                    pRet.Version = pRet.Version + 1;
                    pRet.Name = name;
                    pRet.Description = desc;
                    ctx.SaveChanges();
                }
                else
                {
                    pRet = new DbDataset();
                    pRet.Version = 1;
                    pRet.Sigle = sSigle;
                    pRet.Name = name;
                    pRet.Description = desc;
                    ctx.DbDatasets.Add(pRet);
                    ctx.SaveChanges();
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbDataset, Exception>(pRet, err);
        }// MaintainsDataset
        public Tuple<bool, Exception> RemoveDataset(DbDataset pSet)
        {
            bool bRet = false;
            Exception err = null;
            if (pSet == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                DbDataset pRet = null;
                int nId = pSet.Id;
                if (nId != 0)
                {
                    pRet = this.findDatasetById(ctx, nId);
                }
                String s = pSet.Sigle;
                if (pRet == null)
                {
                    String sigle = pSet.Sigle;
                    pRet = this.findDatasetBySigle(ctx, sigle);
                }
                if (pRet == null)
                {
                    return new Tuple<bool, Exception>(bRet, new ArgumentException());
                }
                this.removeDataset(ctx, pRet);
                ctx.SaveChanges();
                bRet = true;
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// FindDatasetBySigle
        public Tuple<IEnumerable<DbVariable>, Exception> GetDatasetVariables(DbDataset pSet)
        {
            List<DbVariable> oRet = null;
            if (pSet == null)
            {
                return new Tuple<IEnumerable<DbVariable>, Exception>(oRet, new ArgumentNullException());
            }
            Exception err = null;
            var rr = this.FindDataset(pSet);
            DbDataset xSet = rr.Item1;
            if (xSet == null)
            {
                return new Tuple<IEnumerable<DbVariable>, Exception>(oRet, new ArgumentException());
            }
            int nId = xSet.Id;
            if (nId == 0)
            {
                return new Tuple<IEnumerable<DbVariable>, Exception>(oRet, new ArgumentException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                var q = from x in ctx.DbVariables where (x.DatasetId == nId)
                        orderby x.IsCateg descending, x.Genre ascending, x.Sigle ascending select x;
                oRet = new List<DbVariable>();
                foreach (var p in q)
                {
                    oRet.Add(p);
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<IEnumerable<DbVariable>, Exception>(oRet, err);
        }//GetDatasetVariables 
        public Tuple<DbVariable, Exception> FindVariableById(int nVarId)
        {
            DbVariable pRet = null;
            if (nVarId == 0)
            {
                return new Tuple<DbVariable, Exception>(pRet, new ArgumentException());
            }
            Exception err = null;
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.findVariableById(ctx, nVarId);
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbVariable, Exception>(pRet, err);
        }// FindVariableById
        public Tuple<DbVariable, Exception> FindVariableByDatasetAndSigle(int nDatasetId, String sigle)
        {
            DbVariable pRet = null;
            if ((nDatasetId == 0) || String.IsNullOrEmpty(sigle))
            {
                return new Tuple<DbVariable, Exception>(pRet, new ArgumentException());
            }
            Exception err = null;
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.findVariableByDatasetAndSigle(ctx, nDatasetId, sigle);
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbVariable, Exception>(pRet, err);
        }// FindVariableByDatasetAndSigle
        public Tuple<DbVariable, Exception> FindVariable(DbVariable pVar)
        {
            Exception err = null;
            DbVariable pRet = null;
            if (pVar == null)
            {
                return new Tuple<DbVariable, Exception>(pRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.findVariable(ctx, pVar);
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbVariable, Exception>(pRet, err);
        }// FindVariable
        public Tuple<DbVariable, Exception> MaintainsVariable(DbVariable pVar)
        {
            DbVariable pRet = null;
            if (pVar == null)
            {
                return new Tuple<DbVariable, Exception>(pRet, new ArgumentException());
            }
            Exception err = null;
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.maintainsVariable(ctx, pVar);
                if (pRet != null)
                {
                    ctx.SaveChanges();
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbVariable, Exception>(pRet, err);
        }// MaintainsVariable
        public Tuple<bool, Exception> MaintainsVariables(IEnumerable<DbVariable> oVars)
        {
            bool bRet = false;
            if (oVars == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentException());
            }
            Exception err = null;
            try
            {
                bRet = true;
                statdatastoreEntities ctx = getContext();
                foreach (var p in oVars)
                {
                    if (this.maintainsVariable(ctx, p) == null)
                    {
                        bRet = false;
                        break;
                    }
                }// p
                if (bRet)
                {
                    bRet = false;
                    ctx.SaveChanges();
                    bRet = true;
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// MaintainsVariable
        public Tuple<bool, Exception> RemoveVariable(DbVariable pVar)
        {
            bool bRet = false;
            Exception err = null;
            if (pVar == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                DbVariable pp = null;
                int nId = pVar.Id;
                if (nId != 0)
                {
                    pp = this.findVariableById(ctx, nId);
                }
                if (pp == null)
                {
                    String sigle = pVar.Sigle;
                    int nDatasetId = pVar.DatasetId;
                    pp = this.findVariableByDatasetAndSigle(ctx, nDatasetId, sigle);
                }
                if (pp == null)
                {
                    return new Tuple<bool, Exception>(bRet, new ArgumentException());
                }
                this.removeVariable(ctx, pp);
                ctx.SaveChanges();
                bRet = true;
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveVariable
        public Tuple<bool, Exception> RemoveVariables(IEnumerable<DbVariable> oVars)
        {
            bool bRet = false;
            Exception err = null;
            if (oVars == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                foreach (var p in oVars)
                {
                    this.removeVariable(ctx, p);
                }
                ctx.SaveChanges();
                bRet = true;
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveVariable
        public Tuple<int, Exception> GetDatasetIndivsCount(DbDataset pSet)
        {
            int nRet = 0;
            Exception err = null;
            if (pSet == null)
            {
                return new Tuple<int, Exception>(nRet, new ArgumentException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                DbDataset xSet = null;
                int nId = pSet.Id;
                if (nId != 0)
                {
                    xSet = this.findDatasetById(ctx, nId);
                }
                if (xSet == null)
                {
                    xSet = this.findDatasetBySigle(ctx, pSet.Sigle);
                }
                if (xSet != null)
                {
                    nRet = this.getDatasetIndivsCount(ctx, xSet.Id);
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<int, Exception>(nRet, err);
        }//GetDatasetIndivsCount
        public Tuple<IEnumerable<DbIndiv>, Exception> GetDatasetIndivs(DbDataset pSet, int skip, int taken)
        {
            List<DbIndiv> oRet = null;
            Exception err = null;
            if ((pSet == null) || (skip < 0) || (taken < 1))
            {
                return new Tuple<IEnumerable<DbIndiv>, Exception>(oRet, new ArgumentException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                DbDataset xSet = null;
                int nId = pSet.Id;
                if (nId != 0)
                {
                    xSet = this.findDatasetById(ctx, nId);
                }
                if (xSet == null)
                {
                    xSet = this.findDatasetBySigle(ctx, pSet.Sigle);
                }
                if (xSet != null)
                {
                    return this.getDatasetIndivs(ctx, xSet.Id, skip, taken);
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<IEnumerable<DbIndiv>, Exception>(oRet, err);
        }//GetDatasetIndivsCount
        public Tuple<DbIndiv, Exception> FindIndiv(DbIndiv pIndiv)
        {
            Exception err = null;
            DbIndiv pRet = null;
            if (pIndiv == null)
            {
                return new Tuple<DbIndiv, Exception>(pRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.findIndiv(ctx, pIndiv);
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbIndiv, Exception>(pRet, err);
        }// FindDatasetById
        public Tuple<DbIndiv, Exception> MaintainsIndiv(DbIndiv pInd)
        {
            DbIndiv pRet = null;
            if (pInd == null)
            {
                return new Tuple<DbIndiv, Exception>(pRet, new ArgumentException());
            }
            Exception err = null;
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.maintainsIndiv(ctx, pInd);
                if (pRet != null)
                {
                    ctx.SaveChanges();
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbIndiv, Exception>(pRet, err);
        }// MaintainsIndiv
        public Tuple<bool, Exception> MaintainsIndivs(IEnumerable<DbIndiv> oInds)
        {
            bool bRet = false;
            if (oInds == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentException());
            }
            Exception err = null;
            try
            {
                bRet = true;
                statdatastoreEntities ctx = getContext();
                foreach (var p in oInds)
                {
                    if (this.maintainsIndiv(ctx, p) == null)
                    {
                        bRet = false;
                        break;
                    }
                }// p
                if (bRet)
                {
                    bRet = false;
                    ctx.SaveChanges();
                    bRet = true;
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// MaintainsIndivs
        public Tuple<bool, Exception> RemoveIndiv(DbIndiv pInd)
        {
            bool bRet = false;
            Exception err = null;
            if (pInd == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                DbIndiv pp = findIndiv(ctx, pInd);
                if (pp == null)
                {
                    return new Tuple<bool, Exception>(bRet, new ArgumentException());
                }
                this.removeIndiv(ctx, pp);
                ctx.SaveChanges();
                bRet = true;
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveIndiv
        public Tuple<bool, Exception> RemoveIndivs(IEnumerable<DbIndiv> oInds)
        {
            bool bRet = false;
            Exception err = null;
            if (oInds == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                foreach (var p in oInds)
                {
                    this.removeIndiv(ctx, p);
                }
                ctx.SaveChanges();
                bRet = true;
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// RemoveIndivs
        public Tuple<bool, Exception> MaintainsValues(IEnumerable<DbValue> oVals)
        {
            bool bRet = false;
            Exception err = null;
            if (oVals == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                bRet = true;
                foreach (var p in oVals)
                {
                    if (p != null)
                    {
                        if (maintainsValue(ctx, p) == null)
                        {
                            bRet = false;
                            break;
                        }
                    }
                }// p
                if (bRet)
                {
                    bRet = false;
                    ctx.SaveChanges();
                    bRet = true;
                }
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// MaintainsValues 
        public Tuple<bool, Exception> RemoveValues(IEnumerable<DbValue> oVals)
        {
            bool bRet = false;
            Exception err = null;
            if (oVals == null)
            {
                return new Tuple<bool, Exception>(bRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                bRet = true;
                foreach (var p in oVals)
                {
                    if (p != null)
                    {
                        this.removeValue(ctx, p);
                    }
                }// p
                ctx.SaveChanges();
                bRet = true;
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// MaintainsValues 
        public Tuple<int, Exception> GetVariableValuesCount(DbVariable pVar)
        {
            int nRet = 0;
            Exception err = null;
            if (pVar == null)
            {
                return new Tuple<int, Exception>(nRet, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                DbVariable xVar = this.findVariable(ctx, pVar);
                if (xVar != null)
                {
                    var q = from x in ctx.DbValues where (x.VariableId == xVar.Id) select x;
                    nRet = q.Count();
                }// pVar
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<int, Exception>(nRet, err);
        }//GetVariableValuesCount 
        public Tuple<IEnumerable<DbValue>, Exception> GetVariableValues(DbVariable pVar, int skip, int taken)
        {
            List<DbValue> oRet = null;
            Exception err = null;
            if ((pVar == null) || (skip < 0) || (taken < 1))
            {
                return new Tuple<IEnumerable<DbValue>, Exception>(null, new ArgumentException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                DbVariable xVar = this.findVariable(ctx, pVar);
                oRet = new List<DbValue>();
                if (xVar != null)
                {
                    var q = (from x in ctx.DbValues where (x.VariableId == xVar.Id) orderby x.IndivId ascending select x).Skip(0).Take(taken);
                    foreach (var p in q){
                        oRet.Add(p);
                    }// p
                }// pVar
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<IEnumerable<DbValue>, Exception>(oRet, err);
        }//GetVariableValues
        public Tuple<IEnumerable<DbValue>, Exception> GetIndivValues(DbIndiv pInd)
        {
            List<DbValue> oRet = null;
            Exception err = null;
            if (pInd == null)
            {
                return new Tuple<IEnumerable<DbValue>, Exception>(null, new ArgumentException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                DbIndiv xInd = this.findIndiv(ctx, pInd);
                oRet = new List<DbValue>();
                if (xInd != null)
                {
                    var q = from x in ctx.DbValues where (x.IndivId == xInd.Id) orderby x.VariableId ascending select x;
                    foreach (var p in q)
                    {
                        oRet.Add(p);
                    }// p
                }// pVar
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<IEnumerable<DbValue>, Exception>(oRet, err);
        }//GetVariableValues
        public Tuple<DbValue, Exception> FindValue(DbValue pVal)
        {
            DbValue pRet = null;
            Exception err = null;
            if (pVal == null)
            {
                return new Tuple<DbValue, Exception>(null, new ArgumentNullException());
            }
            try
            {
                statdatastoreEntities ctx = getContext();
                pRet = this.findValue(ctx, pVal);
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<DbValue, Exception>(pRet, err);
        }// MaintainsValues 
        public Tuple<IEnumerable<Tuple<int, Object>>, Exception> ConvertValues(IEnumerable<DbValue> oVals)
        {
            IEnumerable<Tuple<int, Object>> oRet = null;
            Exception err = null;
            try
            {
                statdatastoreEntities ctx = getContext();
                oRet = this.convertValues(ctx, oVals);
            }
            catch (Exception ex)
            {
                ex = err;
            }
            return new Tuple<IEnumerable<Tuple<int, object>>, Exception>(oRet, err);
        }// ConvertValues
        public Tuple<Dictionary<int, Dictionary<int, Object>>,Exception> GetCommonValues(IEnumerable<DbVariable> oVars)
        {
            Dictionary<int, Dictionary<int, Object>> oRet = new Dictionary<int, Dictionary<int, object>>();
            if (oVars == null)
            {
                return new Tuple<Dictionary<int, Dictionary<int, object>>, Exception>(null, new ArgumentNullException());
            }
            Exception err = null;
            try {
                statdatastoreEntities ctx = getContext();
                foreach (var pVar in oVars)
                {
                    DbVariable xvar = this.findVariable(ctx, pVar);
                    if (xvar != null)
                    {
                        int nVarId = xvar.Id;
                        var q = from x in ctx.DbValues where (x.VariableId == nVarId) select x;
                        List<DbValue> oList = new List<DbValue>();
                        foreach (var v in q)
                        {
                            oList.Add(v);
                        }// v
                        IEnumerable<Tuple<int, Object>> oSet = this.convertValues(ctx, oList);
                        if (oSet == null)
                        {
                            oRet.Clear();
                            break;
                        }
                        List<Tuple<int, Object>> xList = new List<Tuple<int, object>>(oSet);
                        if (xList.Count < 1)
                        {
                            oRet.Clear();
                            break;
                        }
                        if (oRet.Count < 1)
                        {
                            foreach (var vv in xList)
                            {
                                if (vv == null)
                                {
                                    continue;
                                }
                                int nIndivId = vv.Item1;
                                Object val = vv.Item2;
                                if ((nIndivId != 0) && (val != null))
                                {
                                    Dictionary<int, Object> oDict = new Dictionary<int, object>();
                                    oDict[nVarId] = val;
                                    oRet[nIndivId] = oDict;
                                }
                            }// vv
                        }
                        else
                        {
                            List<int> oDelSet = new List<int>();
                            Dictionary<int, Object> oTemp = new Dictionary<int, object>();
                            foreach (var vv in xList)
                            {
                                if (vv == null)
                                {
                                    continue;
                                }
                                int nId = vv.Item1;
                                Object val = vv.Item2;
                                if ((nId != 0) && (val != null))
                                {
                                    oTemp[nId] = val;
                                }
                            }// vv
                            //
                            var keys = oTemp.Keys;
                            foreach (var nId in keys)
                            {
                                if (!oTemp.ContainsKey(nId))
                                {
                                    oDelSet.Add(nId);
                                }
                                else
                                {
                                    Object val = oTemp[nId];
                                    IDictionary<int, Object> oDict = oRet[nId];
                                    oDict[nVarId] = val;
                                }
                            }// nId
                            //
                            if (oDelSet.Count > 0)
                            {
                                foreach (var nId in oDelSet)
                                {
                                    if (oRet.ContainsKey(nId))
                                    {
                                        oRet.Remove(nId);
                                    }
                                }// nId
                            }// oDelSet
                        }
                    }// xvar

                }// pVal
            }// try
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<Dictionary<int, Dictionary<int, object>>,Exception>(oRet, err);
        }// GetCommonValues
    }// class StatDataStore
}
