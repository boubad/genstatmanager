using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StatDataset
{
    public interface IStoreDataManager
    {
        Tuple<IEnumerable<StatDataset>, Exception> GetAllDataSets();
        Tuple<StatDataset, Exception> FindDataset(StatDataset pSet);
        Tuple<bool, Exception> RemoveDataset(StatDataset pSet);
        Tuple<IEnumerable<StatVariable>, Exception> GetDatasetVariables(StatDataset pSet);
        Tuple<StatVariable, Exception> FindVariable(StatVariable pVar);
        Tuple<StatVariable, Exception> MaintainsVariable(StatVariable pVar);
        Tuple<bool, Exception> MaintainsVariables(IEnumerable<StatVariable> oVars);
        Tuple<bool, Exception> RemoveVariable(StatVariable pVar);
        Tuple<bool, Exception> RemoveVariables(IEnumerable<StatVariable> oVars);
        Tuple<int, Exception> GetDatasetIndivsCount(StatDataset pSet);
        Tuple<IEnumerable<StatIndiv>, Exception> GetDatasetIndivs(StatDataset pSet, int skip, int taken);
        Tuple<StatIndiv, Exception> FindIndiv(StatIndiv pIndiv);
        Tuple<StatIndiv, Exception> MaintainsIndiv(StatIndiv pInd);
        Tuple<bool, Exception> MaintainsIndivs(IEnumerable<StatIndiv> oInds);
        Tuple<bool, Exception> RemoveIndiv(StatIndiv pInd);
        Tuple<bool, Exception> RemoveIndivs(IEnumerable<StatIndiv> oInds);
        Tuple<bool, Exception> MaintainsValues(IEnumerable<StatValue> oVals);
        Tuple<bool, Exception> RemoveValues(IEnumerable<StatValue> oVals);
        Tuple<int, Exception> GetVariableValuesCount(StatVariable pVar);
        Tuple<IEnumerable<StatValue>, Exception> GetVariableValues(StatVariable pVar, int skip, int taken);
        Tuple<IEnumerable<StatValue>, Exception> GetIndivValues(StatIndiv pInd);
        Tuple<StatValue, Exception> FindValue(StatValue pVal);
        Tuple<IEnumerable<Tuple<int, Object>>, Exception> ConvertValues(IEnumerable<StatValue> oVals);
        Tuple<Dictionary<int, Dictionary<int, Object>>, Exception> GetCommonValues(IEnumerable<StatVariable> oVars);
    }// interface IStoreDataManager
}
