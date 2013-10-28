/*
 * database_updater.cpp
 *
 *  Created on: 3 oct. 2013
 *      Author: Boubacar
 */
////////////////////////////////////////
#include "importdata.h"
/////////////////////////////
#include "datamanager.h"
#include "statdatamanager.h"
//////////////////////////////
using namespace std;
using namespace intrasqlite;
///////////////////////////////////////
namespace intra {
/////////////////////////////////////////
extern bool process_data(const std::string &srcfilename,
		const std::string &databaseFilename, const std::string &datasetSigle,char delim, const std::string na) {
	typedef std::string StringType;
//
	typedef ImportData<StringType> ImportDataType;
//
	try {
		ImportDataType oData;
		std::ifstream in(srcfilename.c_str());
		oData.open(in, delim, na);
		size_t ncols = oData.cols();
		if (ncols < 1) {
			return (false);
		}
		//
		StatDataManager oMan(databaseFilename);
		bool bRet = oMan.is_valid();
		if (!bRet) {
			return (false);
		}
		Dataset oDataset;
		bRet = oMan.get_dataset_by_sigle(datasetSigle, oDataset);
		if ((!bRet) || (oDataset.id() == 0)) {
			oDataset.sigle(datasetSigle);
			oDataset.name(datasetSigle);
			oDataset.description(datasetSigle);
			bRet = oMan.insert_dataset(oDataset);
			if (!bRet) {
				return false;
			}
			bRet = oMan.get_dataset_by_sigle(datasetSigle, oDataset);
			if (!bRet) {
				return (false);
			}
		}
		int nId = oDataset.id();
		if (nId == 0) {
			return (false);
		}
		bRet = oMan.check_pred_dataset(nId);
		if (!bRet) {
			return (false);
		}
		bRet = oMan.load_dataset(oDataset);
		if (!bRet) {
			return (false);
		}
		std::map<size_t, int> oVarsMap;
		int nCols = oData.cols();
		int nIndex = -1;
		int nStatus = -1;
		std::string strIndex("NOM");
		std::string strStatus("STATUS");
		for (int i = 0; i < nCols; ++i) {
			std::string sigle = oData.colname(i);
			if (sigle == strIndex) {
				nIndex = (int) i;
			}
			if (sigle == strStatus) {
				nStatus = (int) i;
			}
			const Variable *pVar = oDataset.find_variable_by_sigle(sigle);
			if (pVar != nullptr) {
				int xId = pVar->id();
				oVarsMap[i] = xId;
			}
		} // i
		if (nIndex < 0) {
			return (true);
		}
		size_t nrows = oData.rows();
		if (nrows < 1) {
			return (true);
		}
		std::vector<Indiv> oInds;
		for (size_t i = 0; i < nrows; ++i) {
			std::string sigle;
			if (oData.get_string_value(i, nIndex, sigle)) {
				if (!sigle.empty()) {
					auto pInd = oDataset.find_indiv_by_sigle(sigle);
					if (pInd == nullptr) {
						Indiv oInd;
						oInd.dataset_id(nId);
						oInd.sigle(sigle);
						oInd.name(sigle);
						oInd.description(sigle);
						if (nStatus >= 0) {
							std::string status;
							if (oData.get_string_value(i, nStatus, status)) {
								if (!status.empty()) {
									oInd.status(status);
								}
							}
						} // nStatus
						oInds.push_back(oInd);
					} // add
				} // not empty
			} // getVal
		} // i
		if (!oInds.empty()) {
			if (!oMan.maintains_indivs(oInds)) {
				return (false);
			}
		}
		int nIndivs = 0;
		if (!oMan.get_dataset_indivs_count(nId, nIndivs)) {
			return (false);
		}
		if (nIndivs < 1) {
			return (true);
		}
		std::vector<Indiv> oVecx;
		if (!oMan.get_dataset_indivs(nId, oVecx)) {
			return (false);
		}
		std::map<size_t, int> oIndsMap;
		for (auto it = oVecx.begin(); it != oVecx.end(); ++it) {
			const Indiv &oInd = *it;
			std::string sigle = oInd.sigle();
			for (size_t i = 0; i < nrows; ++i) {
				std::string ss;
				oData.get_string_value(i, nIndex, ss);
				if (ss == sigle) {
					int xid = oInd.id();
					oIndsMap[i] = xid;
					break;
				}
			} // i
		} // it
		for (size_t icol = 0; icol < ncols; ++icol) {
			std::string siglecol = oData.colname(icol);
			if ((siglecol == strIndex) || (siglecol == strStatus)) {
				continue;
			}
			if (oVarsMap.find(icol) == oVarsMap.end()) {
				continue;
			}
			int nVarId = oVarsMap[icol];
			std::vector<Value> oVec;
			std::vector<boost::any> vals;
			oData.get_col_values(icol, vals);
			for (size_t irow = 0; irow < nrows; ++irow) {
				boost::any v;
				if (oIndsMap.find(irow) == oIndsMap.end()) {
					continue;
				}
				int nIndivId = oIndsMap[irow];
				oData.get_value(irow, icol, v);
				Value cur;
				cur.value(v);
				cur.indiv_id(nIndivId);
				cur.variable_id(nVarId);
				oVec.push_back(cur);
			} // irow
			if (oVec.empty()) {
				continue;
			}
			if (!oMan.maintains_values(oVec)) {
				return (false);
			}
		} // icol
		bRet = oMan.check_values(nId);
	} catch (...) {
		return (false);
	}
	return (true);
} // process_data
/////////////////////////
} // namespace intra

