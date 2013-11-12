using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatDataset
{
    [Serializable]
    public class StatDataset : NamedElement
    {
        #region Constructors
        public StatDataset()
        {
        }
        #endregion // Constructors
        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }
            if (!(obj is StatDataset))
            {
                return false;
            }
            StatDataset p = obj as StatDataset;
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
    }// class StatDataset
}
