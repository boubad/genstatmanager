using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
namespace StatDataset
{
    [Serializable]
    public class StatValue : ARootElement
    {
        private int m_variableid = 0;
        private int m_individ = 0;
        private String m_status = null;
        private String m_sval = null;
        public StatValue()
        {
        }
        public int VariableId
        {
            get
            {
                return m_variableid;
            }
            set
            {
                if (m_variableid != value)
                {
                    m_variableid = value;
                    NotifyPropertyChanged("VariableId");
                    this.IsModified = true;
                }
            }
        }// VariableId
        public int IndivId
        {
            get
            {
                return m_individ;
            }
            set
            {
                if (m_individ != value)
                {
                    m_individ = value;
                    NotifyPropertyChanged("IndivId");
                    this.IsModified = true;
                }
            }
        }// IndivId
        public String Status
        {
            get
            {
                return String.IsNullOrEmpty(m_status) ? String.Empty : m_status;
            }
            set
            {
                String old = String.IsNullOrEmpty(m_status) ? String.Empty : m_status.Trim();
                String s = String.IsNullOrEmpty(value) ? String.Empty : value.Trim();
                if (s.Length > 15)
                {
                    s = s.Substring(0, 15).Trim();
                }
                if (s.ToLower() != old.ToLower())
                {
                    m_status = s.ToLower();
                    NotifyPropertyChanged("Status");
                    this.IsModified = true;
                }
            }
        }// Status
        public String StringValue
        {
            get
            {
                return String.IsNullOrEmpty(m_sval) ? String.Empty : m_sval;
            }
            set
            {
                String old = String.IsNullOrEmpty(m_sval) ? String.Empty : m_sval.Trim();
                String s = String.IsNullOrEmpty(value) ? String.Empty : value.Trim();
                if (s.Length > 31)
                {
                    s = s.Substring(0, 31).Trim();
                }
                if (s.ToLower() != old.ToLower())
                {
                    m_sval = s.ToLower();
                    NotifyPropertyChanged("StringValue");
                    NotifyPropertyChanged("IntValue");
                    NotifyPropertyChanged("FloatValue");
                    NotifyPropertyChanged("DoubleValue");
                    this.IsModified = true;
                }
            }
        }// StringValue
        [XmlIgnore]
        public int IntValue
        {
            get
            {
                int nRet = -1;
                if (!String.IsNullOrEmpty(m_sval))
                {
                    int.TryParse(m_sval, out nRet);
                }
                return nRet;
            }
            set
            {
                int nOld = this.IntValue;
                if (nOld != value)
                {
                    String s = Convert.ToString(value);
                    this.StringValue = s;
                }
            }
        }// IntValue
        [XmlIgnore]
        public float FloatValue
        {
            get
            {
                float nRet = -1.0f;
                if (!String.IsNullOrEmpty(m_sval))
                {
                    float.TryParse(m_sval, out nRet);
                }
                return nRet;
            }
            set
            {
                float nOld = this.FloatValue;
                if (nOld != value)
                {
                    String s = Convert.ToString(value);
                    this.StringValue = s;
                }
            }
        }// FloatValue
        [XmlIgnore]
        public double DoubleValue
        {
            get
            {
                double nRet = -1.0;
                if (!String.IsNullOrEmpty(m_sval))
                {
                    double.TryParse(m_sval, out nRet);
                }
                return nRet;
            }
            set
            {
                double nOld = this.DoubleValue;
                if (nOld != value)
                {
                    String s = Convert.ToString(value);
                    this.StringValue = s;
                }
            }
        }// DoubleValue
        protected override List<string> getValidatedProperties()
        {
            List<string> oRet = new List<string>() { "VariableId", "IndivId"};
            return oRet;
        }
        protected override string getValidationError(string propertyName)
        {
            String sRet = null;
            if (propertyName == "VariableId")
            {
                if (this.VariableId == 0)
                {
                    sRet = "L'identifiant de Variable parent est incorrect.";
                }
            }
            else if (propertyName == "IndivId")
            {
                if (this.IndivId == 0)
                {
                    sRet = "L'identifiant d' individu parent est incorrect.";
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
            if (!(obj is StatValue))
            {
                return false;
            }
            StatValue p = obj as StatValue;
            if ((this.Id != 0) && (this.Id == p.Id))
            {
                return true;
            }
            return ((this.VariableId == p.VariableId) && (this.IndivId == p.IndivId));
        }
        public override int GetHashCode()
        {
            return this.VariableId.GetHashCode() + this.IndivId.GetHashCode();
        }
    }// class StatValue
}
