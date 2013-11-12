using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatDataset
{
    [Serializable]
    public class StatVariable : DatasetChild
    {
        #region Instance variables
        private bool m_categ = false;
        private String m_type = "float";
        private String m_genre = "input";
        #endregion // instance variables
        #region Constructors
        public StatVariable()
        {
        }
        #endregion // Constructors
        #region Properties
        public String VarType
        {
            get
            {
                return String.IsNullOrEmpty(m_type) ? String.Empty : m_type;
            }
            set
            {
                String old = String.IsNullOrEmpty(m_type) ? String.Empty : m_type.Trim();
                String s = String.IsNullOrEmpty(value) ? String.Empty : value.Trim();
                if (s.Length > 15)
                {
                    s = s.Substring(0, 15).Trim();
                }
                if (s.ToLower() != old.ToLower())
                {
                    m_type = s.ToLower();
                    NotifyPropertyChanged("VarType");
                    if ((!m_type.Contains("int")) && (!m_type.Contains("double")) && (!m_type.Contains("float")))
                    {
                        this.m_categ = true;
                        this.NotifyPropertyChanged("IsCateg");
                    }
                    this.IsModified = true;
                }
            }
        }// VarType
        public String Genre
        {
            get
            {
                return String.IsNullOrEmpty(m_genre) ? String.Empty : m_genre;
            }
            set
            {
                String old = String.IsNullOrEmpty(m_genre) ? String.Empty : m_genre.Trim();
                String s = String.IsNullOrEmpty(value) ? String.Empty : value.Trim();
                if (s.Length > 15)
                {
                    s = s.Substring(0, 15).Trim();
                }
                if (s.ToLower() != old.ToLower())
                {
                    m_genre = s;
                    NotifyPropertyChanged("Genre");
                    this.IsModified = true;
                }
            }
        }// Genre
        public bool IsCateg
        {
            get
            {
                return m_categ;
            }
            set
            {
                if (m_categ != value)
                {
                    m_categ = value;
                    NotifyPropertyChanged("IsCateg");
                    this.IsModified = true;
                }
            }
        }// IsCateg
        #endregion Properties
        #region Overrides
        protected override List<string> getValidatedProperties()
        {
            List<string> oRet = new List<string>() { "DatasetId", "Sigle","VarType" };
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
            else if (propertyName == "VarType")
            {
                String s = this.VarType;
                if (!String.IsNullOrEmpty(s))
                {
                    s = s.Trim();
                }
                if (String.IsNullOrEmpty(s))
                {
                    sRet = "Le Type ne doit pas être vide.";
                }
                else if (s.Length > 15)
                {
                    sRet = "La longueur du Type ne doit pas dépasser 31 caractères.";
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
            if (!(obj is StatVariable))
            {
                return false;
            }
            StatVariable p = obj as StatVariable;
            if ((this.Id != 0) && (this.Id == p.Id))
            {
                return true;
            }
            return base.Equals(p);
        }
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
        #endregion // Overrides
    }// class StatVariable
}
