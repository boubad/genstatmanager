using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatDataset
{
    [Serializable]
    public class PredStatDataset : StatDataset
    {
        public PredStatDataset()
        {

        }
        public PredStatDataset(StatDataset p)
        {
            if (p != null)
            {
                this.Id = p.Id;
                this.Version = p.Version;
                this.Sigle = p.Sigle;
                this.Name = p.Name;
                this.Description = p.Description;
                this.IsModified = false;
            }
        }
        public override bool Equals(object obj)
        {
            return base.Equals(obj);
        }
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
    }// class PredStatDataset
}
