using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatDataset
{
    [Serializable]
    public class NamedElement : ARootElement
    {
        #region Instance Variables
        private String m_sigle;
        private String m_name;
        private String m_desc;
        #endregion // Instance Variables
        #region Constructors
        public NamedElement()
        {
        }
        #endregion // Constructors
        #region Properties
        public String Sigle
        {
            get
            {
                return String.IsNullOrEmpty(m_sigle) ? String.Empty : m_sigle;
            }
            set
            {
                String old = String.IsNullOrEmpty(m_sigle) ? String.Empty : m_sigle.Trim();
                String s = String.IsNullOrEmpty(value) ? String.Empty : value.Trim();
                if (s.Length > 31)
                {
                    s = s.Substring(0, 31).Trim();
                }
                if (s.ToLower() != old.ToLower())
                {
                    m_sigle = s;
                    NotifyPropertyChanged("Sigle");
                    this.IsModified = true;
                }
            }
        }// Sigle
        public String Description
        {
            get
            {
                return String.IsNullOrEmpty(m_desc) ? String.Empty : m_desc;
            }
            set
            {
                String old = String.IsNullOrEmpty(m_desc) ? String.Empty : m_desc.Trim();
                String s = String.IsNullOrEmpty(value) ? String.Empty : value.Trim();
                if (s.Length > 255)
                {
                    s = s.Substring(0, 255).Trim();
                }
                if (s.ToLower() != old.ToLower())
                {
                    m_desc = s;
                    NotifyPropertyChanged("Description");
                    this.IsModified = true;
                }
            }
        }// Description
        public String Name
        {
            get
            {
                return String.IsNullOrEmpty(m_name) ? String.Empty : m_name;
            }
            set
            {
                String old = String.IsNullOrEmpty(m_name) ? String.Empty : m_name.Trim();
                String s = String.IsNullOrEmpty(value) ? String.Empty : value.Trim();
                if (s.Length > 63)
                {
                    s = s.Substring(0, 63).Trim();
                }
                if (s.ToLower() != old.ToLower())
                {
                    m_name = s;
                    NotifyPropertyChanged("Name");
                    this.IsModified = true;
                }
            }
        }// Name
        #endregion // Properties
        #region Overrides
        protected override bool canBeRefreshed()
        {
            if (base.canBeRefreshed())
            {
                return true;
            }
            return (!String.IsNullOrEmpty(this.Sigle));
        }
        protected override List<string> getValidatedProperties()
        {
            List<string> oRet = new List<string>() { "Sigle" };
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
            return sRet;
        }
        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }
            if (!(obj is NamedElement))
            {
                return false;
            }
            NamedElement p = obj as NamedElement;
            if ((this.Id != 0) && (this.Id == p.Id))
            {
                return true;
            }
            return (this.Sigle.ToLower() == p.Sigle.ToLower());
        }
        public override int CompareTo(object obj)
        {
            if (obj == null)
            {
                return -1;
            }
            if (!(obj is NamedElement))
            {
                return -1;
            }
            NamedElement p = obj as NamedElement;
            return this.Sigle.ToLower().CompareTo(p.Sigle.ToLower());
        }
        public override int GetHashCode()
        {
            return this.Sigle.ToLower().GetHashCode();
        }
        public override string ToString()
        {
            String s = this.Name;
            if (String.IsNullOrEmpty(s))
            {
                s = this.Sigle;
            }
            return this.Name;
        }
        #endregion // Overrides
    }// class NamedElement
}
