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
        protected statdatastoreEntities getContext()
        {
            return new statdatastoreEntities();
        }
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
                pRet = ctx.DbDatasets.Find(nId);
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
                String ss = sigle.Trim().ToUpper();
                statdatastoreEntities ctx = getContext();
                var q = from x in ctx.DbDatasets where (x.Sigle.Trim().ToUpper() == ss) select x;
                if (q.Count() > 0)
                {
                    pRet = q.First();
                }
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
                    pRet = ctx.DbDatasets.Find(nId);
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
                String ss = s.Trim().ToUpper();
                var q = from x in ctx.DbDatasets where (x.Sigle.Trim().ToUpper() == ss) select x;
                if (q.Count() > 0)
                {
                    pRet = q.First();
                }
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
                    pRet = ctx.DbDatasets.Find(nId);
                }
                if (pRet == null)
                {
                    String ss = sSigle.Trim().ToUpper();
                    var q = from x in ctx.DbDatasets where (x.Sigle.Trim().ToUpper() == ss) select x;
                    if (q.Count() > 0)
                    {
                        pRet = q.First();
                    }
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
                    pRet = ctx.DbDatasets.Find(nId);
                }
                String s = pSet.Sigle;
                if (pRet == null)
                {
                   if (String.IsNullOrEmpty(s))
                    {
                        return new Tuple<bool, Exception>(bRet, new ArgumentException());
                    }
                   String ss = s.Trim().ToUpper();
                   var q = from x in ctx.DbDatasets where (x.Sigle.Trim().ToUpper() == ss) select x;
                   if (q.Count() > 0)
                   {
                       pRet = q.First();
                   }
                }
                if (pRet == null)
                {
                    return new Tuple<bool, Exception>(bRet, new ArgumentException());
                }
                ctx.DbDatasets.Remove(pRet);
                ctx.SaveChanges();
                bRet = true;
            }
            catch (Exception ex)
            {
                err = ex;
            }
            return new Tuple<bool, Exception>(bRet, err);
        }// FindDatasetBySigle
    }// class StatDataStore
}
