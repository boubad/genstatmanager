using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatDataset
{
    [Serializable]
    public class StatIndiv : DatasetChild
    {
        private String m_status;
        private byte[] m_databytes;
        public StatIndiv()
        {
        }
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
        public byte[] DataBytes
        {
            get
            {
                return this.m_databytes;
            }
            set
            {
                if (this.m_databytes != value)
                {
                    this.m_databytes = value;
                    this.NotifyPropertyChanged("DataBytes");
                    this.IsModified = true;
                }
            }
        }// DataBytes
        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }
            if (!(obj is StatIndiv))
            {
                return false;
            }
            StatIndiv p = obj as StatIndiv;
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
    }// class StatIndiv
}
