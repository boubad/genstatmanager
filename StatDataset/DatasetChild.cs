using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatDataset
{
    [Serializable]
    public class DatasetChild : NamedElement
    {
        private int m_datasetid;
        public DatasetChild()
        {
        }
        public int DatasetId
        {
            get
            {
                return this.m_datasetid;
            }
            set
            {
                if (this.m_datasetid != value)
                {
                    this.m_datasetid = value;
                    this.NotifyPropertyChanged("DatasetId");
                    this.IsModified = true;
                }
            }
        }// DatasetId
        protected override List<string> getValidatedProperties()
        {
            List<string> oRet = new List<string>() {"DatasetId", "Sigle" };
            return oRet;
        }
        protected override string getValidationError(string propertyName)
        {
            String sRet = null;
            if (propertyName == "Sigle")
            {
                String s = this.Sigle;
                if (!String.IsNullOrEmpty(s))
                {
                    s = s.Trim();
                }
                if (String.IsNullOrEmpty(s))
                {
                    sRet = "Le Sigle ne doit pas être vide.";
                }
                else if (s.Length > 31)
                {
                    sRet = "La longueur du Sigle ne doit pas dépasser 31 caractères.";
                }
            }
            else if (propertyName == "DatasetId")
            {
                if (this.DatasetId == 0)
                {
                    sRet = "L'identifiant du Dataset parent est incorrect.";
                }
            }
            return sRet;
        }
        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }
            if (!(obj is DatasetChild))
            {
                return false;
            }
            DatasetChild p = obj as DatasetChild;
            if ((this.Id != 0) && (this.Id == p.Id))
            {
                return true;
            }
            if (this.DatasetId != p.DatasetId)
            {
                return false;
            }
            return (this.Sigle.ToLower() == p.Sigle.ToLower());
        }
        public override int GetHashCode()
        {
            return this.DatasetId.GetHashCode() + this.Sigle.GetHashCode();
        }
    }// class DatasetChild
}
