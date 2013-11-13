using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StatDataset;

namespace DbStatStore
{

    public class PredDbStoreManager : DbStatStoreManager
    {
        public PredDbStoreManager()
        {

        }
        public override Tuple<StatDataset.StatDataset, Exception> MaintainsDataset(StatDataset.StatDataset pSet)
        {
            if (pSet == null)
            {
                return new Tuple<StatDataset.StatDataset, Exception>(null, new ArgumentNullException());
            }
            var rr = base.MaintainsDataset(pSet);
            StatDataset.StatDataset p = rr.Item1;
            Exception err = rr.Item2;
            if ((err != null) || (p == null))
            {
                return new Tuple<StatDataset.StatDataset, Exception>(null, err);
            }
            PredStatDataset pRet = new PredStatDataset(p);
            return new Tuple<StatDataset.StatDataset, Exception>(pRet, null);
        }// MaintainsDataset
        public override Tuple<StatDataset.StatDataset, Exception> FindDataset(StatDataset.StatDataset pSet)
        {
            var rr = base.FindDataset(pSet);
            if ((rr.Item2 != null) || (rr.Item1 == null))
            {
                return rr;
            }
            PredStatDataset pRet = new PredStatDataset(rr.Item1);
            return new Tuple<StatDataset.StatDataset, Exception>(pRet, null);
        }
        protected override IEnumerable<TableDef> getTablesDefs()
        {
            List<TableDef> oList = new List<TableDef>() {
                 new TableDef(){IsCateg=true,Sigle="AVISCE",VarType="string",Genre="input"},
                 new TableDef(){IsCateg=true,Sigle="CLASST",VarType="int",Genre="input"},
                 new TableDef(){IsCateg=true,Sigle="ETABL",VarType="string",Genre="input"},
                 new TableDef(){IsCateg=true,Sigle="NOM",VarType="string",Genre="info"},
                 new TableDef(){IsCateg=true,Sigle="PRENOM",VarType="string",Genre="info"},
                 new TableDef(){IsCateg=true,Sigle="ANNEE",VarType="string",Genre="info"},
                 new TableDef(){IsCateg=true,Sigle="NAISSANCE",VarType="string",Genre="info"},
                new TableDef(){IsCateg=true,Sigle="DOSSIER",VarType="int",Genre="info"},
                new TableDef(){IsCateg=true,Sigle="SEXE",VarType="string",Genre="input"},
                new TableDef(){IsCateg=true,Sigle="SERIEBAC",VarType="string",Genre="input"},
                new TableDef(){IsCateg=true,Sigle="OPTIONBAC",VarType="string",Genre="input"},
                new TableDef(){IsCateg=true,Sigle="MENTIONBAC",VarType="string",Genre="input"},
                new TableDef(){IsCateg=true,Sigle="LV1",VarType="string",Genre="input"},
                new TableDef(){IsCateg=true,Sigle="LV2",VarType="string",Genre="input"},
                new TableDef(){IsCateg=false,Sigle="MATHS_BAC",VarType="float",Genre="input"},
                 new TableDef(){IsCateg=false,Sigle="PHYS_BAC",VarType="float",Genre="input"},
                  new TableDef(){IsCateg=false,Sigle="MATHS_T",VarType="float",Genre="input"},
                   new TableDef(){IsCateg=false,Sigle="PHYS_T",VarType="float",Genre="input"},
                    new TableDef(){IsCateg=false,Sigle="HISTGEO_T",VarType="float",Genre="input"},
                     new TableDef(){IsCateg=false,Sigle="LV1_T",VarType="float",Genre="input"},
                      new TableDef(){IsCateg=false,Sigle="LV2_T",VarType="float",Genre="input"},
                       new TableDef(){IsCateg=false,Sigle="MATHS_P",VarType="float",Genre="input"},
                        new TableDef(){IsCateg=false,Sigle="PHYS_P",VarType="float",Genre="input"},
                         new TableDef(){IsCateg=false,Sigle="HISTGEO_P",VarType="float",Genre="input"},
                          new TableDef(){IsCateg=false,Sigle="LV1_P",VarType="float",Genre="input"},
                           new TableDef(){IsCateg=false,Sigle="LV2_P",VarType="float",Genre="input"},
                            new TableDef(){IsCateg=false,Sigle="MOY_S1",VarType="float",Genre="target"},
                             new TableDef(){IsCateg=false,Sigle="REG_MOY_S1",VarType="float",Genre="pred"}
            };
            return oList;
        }

    }// class PredDbStoreManager
}
