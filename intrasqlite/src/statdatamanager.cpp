/*
* statdatamanager.cpp
*
*  Created on: 17 sept. 2013
*      Author: Boubacar
*/

#include "statdatamanager.h"
///////////////////////////////////////
using namespace sqlite;
//////////////////////////////
namespace intrasqlite {
	//////////////////////////////////////////////////
#ifdef INTRA_USE_WSTRING
	bool StatDataManager::open(const std::wstring &filename) {
		this->m_database.reset(new Database());
		Database *pBase = this->m_database.get();
		assert(pBase != nullptr);
		if (!pBase->open(filename)) {
			this->m_database.reset();
			return (false);
		}
		if (!this->check_schema()) {
			this->m_database.reset();
			return (false);
		}
		return (true);
	} // open
	bool StatDataManager::process_data(const std::wstring &srcfilename,
		const std::wstring &datasetSigle,
		DatasetType &oSet, const wchar_t &delim, const std::wstring &na){
			if (!this->is_valid()){
				return (false);
			}
			bool bRet = false;
			try {
				intra::ImportData<std::wstring>  oData;
				std::string ss(srcfilename.length(), ' ');
				std::copy(srcfilename.begin(), srcfilename.end(), ss.begin());
				std::wifstream in(ss.c_str());
				if (!in.is_open()){
					return (false);
				}
				oData.open(in, delim, na);
				size_t ncols = oData.cols();
				if (ncols < 1) {
					return (false);
				}
				//
				oSet.sigle(datasetSigle);
				bRet = this->fill_data(oData, oSet);
			}
			catch (...) {
				return (false);
			}
			return (bRet);
	}// process_data
	bool StatDataManager::process_data(const std::wstring &srcfilename,
		const std::wstring &datasetSigle,
		const wchar_t &delim, const std::wstring &na){
			DatasetType oSet;
			return this->process_data(srcfilename, datasetSigle, oSet, delim, na);
	}// process_data
#else
	bool StatDataManager::process_data(const std::string &srcfilename,
		const std::string &datasetSigle,
		DatasetType &oSet, const char &delim, const std::string &na){
			if (!this->is_valid()){
				return (false);
			}
			bool bRet = false;
			try {
				intra::ImportData<std::string>  oData;
				std::ifstream in(srcfilename.c_str());
				if (!in.is_open()){
					return (false);
				}
				oData.open(in, delim, na);
				size_t ncols = oData.cols();
				if (ncols < 1) {
					return (false);
				}
				//
				oSet.sigle(datasetSigle);
				bRet = this->fill_data(oData, oSet);
			}
			catch (...) {
				return (false);
			}
			return (bRet);
	}// process_data
	bool StatDataManager::process_data(const std::string &srcfilename,
		const std::string &datasetSigle,
		const char &delim, const StringType &na){
			DatasetType oSet;
			return this->process_data(srcfilename, datasetSigle, oSet, delim, na);
	}// process_data
#endif // INTRA_USE_WSTRING
	bool StatDataManager::fill_data(const ImportDataType &oData, DatasetType &oSet){
		if (!this->is_valid()){
			return (false);
		}
		bool bRet = false;
		int nId = oSet.id();
		if (nId != 0){
			bRet = this->get_dataset_by_id(nId, oSet);
			nId = oSet.id();
			if (!bRet){
				nId = 0;
			}
		}
		StringType sigle = oSet.sigle();
		if (nId == 0){
			bRet = this->get_dataset_by_sigle(sigle, oSet);
			nId = oSet.id();
			if (!bRet){
				nId = 0;
			}
		}// nId
		if (nId == 0){
			oSet.sigle(sigle);
			oSet.name(sigle);
			oSet.description(sigle);
			bRet = this->insert_dataset(oSet);
			if (!bRet) {
				return false;
			}
			bRet = this->get_dataset_by_sigle(sigle, oSet);
			if (!bRet) {
				return (false);
			}
		}// nId
		nId = oSet.id();
		if (nId == 0) {
			return (false);
		}
		std::vector<Variable> oVecv;
		if (!this->get_dataset_variables(nId, oVecv)){
			return (false);
		}
		for (auto it = oVecv.begin(); it != oVecv.end(); ++it){
			const Variable & vv = *it;
			Variable *pv = oSet.create_variable(vv.sigle());
			if (pv != nullptr){
				pv->id(vv.id());
			}
		}
		std::map<size_t, int> oVarsMap;
		int nCols = oData.cols();
		int nIndex = -1;
		int nStatus = -1;
		StringType strIndex, strStatus;
		this->get_indiv_sigle_name(strIndex);
		this->get_indiv_status_name(strStatus);
		for (int i = 0; i < nCols; ++i) {
			StringType sigle = oData.colname(i);
			if (sigle == strIndex) {
				nIndex = (int) i;
			}
			if (sigle == strStatus) {
				nStatus = (int) i;
			}
			const Variable *pVar = oSet.find_variable_by_sigle(sigle);
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
			sigle.clear();
			if (oData.get_string_value(i, nIndex, sigle)) {
				if (!sigle.empty()) {
					auto pInd = oSet.find_indiv_by_sigle(sigle);
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
			if (!this->maintains_indivs(oInds)) {
				return (false);
			}
		}
		int nIndivs = 0;
		if (!this->get_dataset_indivs_count(nId, nIndivs)) {
			return (false);
		}
		if (nIndivs < 1) {
			return (true);
		}
		std::vector<Indiv> oVecx;
		if (!this->get_dataset_indivs(nId, oVecx)) {
			return (false);
		}
		std::map<size_t, int> oIndsMap;
		for (auto it = oVecx.begin(); it != oVecx.end(); ++it) {
			const Indiv &oInd = *it;
			sigle = oInd.sigle();
			for (size_t i = 0; i < nrows; ++i) {
				StringType ss;
				oData.get_string_value(i, nIndex, ss);
				if (ss == sigle) {
					int xid = oInd.id();
					oIndsMap[i] = xid;
					break;
				}
			} // i
		} // it
		size_t ncols = oData.cols();
		for (size_t icol = 0; icol < ncols; ++icol) {
			StringType siglecol = oData.colname(icol);
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
			if (!this->maintains_values(oVec)) {
				return (false);
			}
		} // icol
		bRet = this->check_values(nId);
		if (!bRet){
			return (false);
		}
		bRet = this->load_dataset(oSet);
		return bRet;
	}// fill_data
	void StatDataManager::get_indiv_sigle_name(StringType &s) const{
#ifndef INTRA_USE_WSTRING
		s = "NOM";
#else
		s = L"NOM";
#endif
	}
	void StatDataManager::get_indiv_status_name(StringType &s) const{
#ifndef INTRA_USE_WSTRING
		s = "STATUS";
#else
		s = L"STATUS";
#endif
	}
	/////////////////////////////////////////
	bool StatDataManager::load_dataset(intra::Dataset &cur) {
		//
		typedef intra::Variable Variable;
		typedef intra::Indiv Indiv;
		typedef intra::Value Value;
		//
		cur.values().clear();
		cur.indivs().clear();
		cur.variables().clear();
		//
		int nId = cur.id();
		bool bOk = false;
		if (nId != 0) {
			bOk = this->get_dataset_by_id(nId, cur);
		}
		if (!bOk) {
			const intra::String sigle = cur.sigle();
			if (!this->get_dataset_by_sigle(sigle, cur)) {
				return (false);
			}
			nId = cur.id();
		}
		std::vector<Variable> vecVars;
		if (!this->get_dataset_variables(nId, vecVars)) {
			return (false);
		}
		std::vector<Indiv> vecInds;
		if (!this->get_dataset_indivs(nId, vecInds)) {
			return (false);
		}
		std::for_each(vecInds.begin(), vecInds.end(), [&](const Indiv &v) {
			Indiv *p = cur.create_indiv(v.sigle());
			if (p != nullptr) {
				p->id(v.id());
				p->version(v.version());
				p->name(v.name());
				p->description(v.description());
				p->is_changed(false);
			}
		});
		std::for_each(vecVars.begin(), vecVars.end(), [&](const Variable &v) {
			Variable *p = cur.create_variable(v.sigle());
			if (p != nullptr) {
				p->id(v.id());
				p->version(v.version());
				p->name(v.name());
				p->description(v.description());
				p->vartype(v.vartype());
				p->genre(v.genre());
				p->is_categvar(v.is_categvar());
				p->is_changed(false);
			}
		});
		int nCount = 0;
		if (!this->get_dataset_values_count(nId, nCount)) {
			return (false);
		}
		if (nCount < 1) {
			return (true);
		}
		int skip = 0;
		int taken = 100;
		while (skip < nCount) {
			std::vector<Value> vecVals;
			if (!this->get_dataset_values(nId, vecVals, skip, taken)) {
				return (false);
			}
			if (vecVals.empty()) {
				break;
			}
			std::for_each(vecVals.begin(), vecVals.end(), [&](const Value &val) {
				if (!val.value().empty()) {
					int nVarId = val.variable_id();
					int nIndId = val.indiv_id();
					Value *p = cur.create_value(nVarId, nIndId);
					if (p != nullptr) {
						p->id(val.id());
						p->version(val.version());
						if (val.is_valid()) {
							p->value(val.value());
						}
						p->is_changed(false);
					}
				} // not_empty
			});
			skip += taken;
		} // while skip
		return (true);
	} // load_dataset
	bool StatDataManager::remove_dataset(const intra::Dataset &cur) {
		assert(this->is_valid());
		if (!cur.is_removeable()) {
			return (false);
		}
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_REMOVE_DATASET);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(1, cur.id())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.exec()) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // update_dataset
	bool StatDataManager::update_dataset(const intra::Dataset &cur) {
		assert(this->is_valid());
		if (!cur.is_updateable()) {
			return (false);
		}
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_UPDATE_DATASET);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(1, cur.sigle())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(2, cur.name())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(3, cur.description())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(4, cur.id())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.exec()) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // update_dataset
	bool StatDataManager::insert_dataset(const intra::Dataset &cur) {
		assert(this->is_valid());
		if (!cur.is_writeable()) {
			return (false);
		}
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_INSERT_DATASET);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		intra::String sigle = cur.sigle();
		if (!stmt.set_parameter(1, cur.sigle())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(2, cur.name())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(3, cur.description())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.exec()) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		intra::Dataset xcur;
		if (!this->get_dataset_by_sigle(sigle, xcur)) {
			return (false);
		}
		int nId = xcur.id();
		assert(nId != 0);
		bool bRet = this->check_pred_dataset(nId);
		return (bRet);
	} // insert_dataset
	bool StatDataManager::get_dataset_by_sigle(const intra::String &xSigle,
		intra::Dataset &cur) {
			assert(this->is_valid());
			sqlite::Database *pBase = this->m_database.get();
			sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_SIGLE);
			if (!stmt.is_valid()) {
				return (false);
			}
			intra::String sx = boost::to_upper_copy(boost::trim_copy(xSigle));
			if (!stmt.set_parameter(1, sx.c_str())) {
				return (false);
			}
			if (!stmt.exec()) {
				return (false);
			}
			if (!stmt.has_values()) {
				return (false);
			}
			int n = stmt.cols();
			assert(n >= 5);
			{
				sqlite::DbValue v;
				if (stmt.col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(2, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			return (true);
	} // get_dataset_by_sigle
	bool StatDataManager::get_dataset_by_id(int xId, intra::Dataset &cur) {
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_ID);
		if (!stmt.is_valid()) {
			return (false);
		}
		if (!stmt.set_parameter(1, xId)) {
			return (false);
		}
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		int n = stmt.cols();
		assert(n >= 5);
		{
			sqlite::DbValue v;
			if (stmt.col_value(0, v)) {
				int nId = v.int_value();
				cur.id(nId);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(1, v)) {
				int nVersion = v.int_value();
				cur.version(nVersion);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(2, v)) {
				intra::String s;
				if (v.string_value(s)) {
					cur.sigle(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(3, v)) {
				intra::String s;
				if (v.string_value(s)) {
					cur.name(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(4, v)) {
				intra::String s;
				if (v.string_value(s)) {
					cur.description(s);
				}
			}
		}
		return (true);
	} // get_dataset_by_id
	bool StatDataManager::get_variable_by_dataset_and_sigle(int nDatasetId,
		const intra::String &xSigle, intra::Variable &cur) {
			assert(this->is_valid());
			sqlite::Database *pBase = this->m_database.get();
			sqlite::Statement stmt(pBase, SQL_VARIABLE_BY_DATASET_AND_SIGLE);
			if (!stmt.is_valid()) {
				return (false);
			}
			intra::String sx = boost::to_upper_copy(boost::trim_copy(xSigle));
			stmt.set_parameter(1, nDatasetId);
			stmt.set_parameter(2, sx);
			if (!stmt.exec()) {
				return (false);
			}
			if (!stmt.has_values()) {
				return (false);
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(2, v)) {
					int nx = v.int_value();
					cur.dataset_id(nx);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.vartype(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(5, v)) {
					int nx = v.int_value();
					bool b = (nx != 0) ? true : false;
					cur.is_categvar(b);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(6, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(7, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(8, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.genre(s);
					}
				}
			}
			return (true);
	} // get_variable_by_dataset_and_sigle
	bool StatDataManager::check_variable(int nDatasetId, const intra::String &sigle,
		const intra::String &type, bool bCateg, const intra::String &genre) {
			assert(this->is_valid());
			sqlite::Database *pBase = this->m_database.get();
			{
				sqlite::Statement stmt(pBase, SQL_VARIABLE_BY_DATASET_AND_SIGLE);
				if (!stmt.is_valid()) {
					return (false);
				}
				intra::String sx = boost::to_upper_copy(boost::trim_copy(sigle));
				stmt.set_parameter(1, nDatasetId);
				stmt.set_parameter(2, sx);
				if (!stmt.exec()) {
					return (false);
				}
				if (stmt.has_values()) {
					return (true);
				}
			}
			if (!this->begin_transaction()) {
				return (false);
			}
			sqlite::Statement stmtInsert(pBase, SQL_INSERT_VARIABLE);
			if (!stmtInsert.is_valid()) {
				return (false);
			}
			int nz = (bCateg) ? 1 : 0;
			intra::String sSigle = boost::trim_copy(sigle);
			intra::String sType = boost::trim_copy(type);
			intra::String sGenre = boost::trim_copy(genre);
			stmtInsert.reset();
			stmtInsert.set_parameter(1, nDatasetId);
			stmtInsert.set_parameter(2, sSigle);
			stmtInsert.set_parameter(3, sType);
			stmtInsert.set_parameter(4, nz);
			stmtInsert.set_parameter(5, sSigle);
			stmtInsert.set_parameter(6, sSigle);
			stmtInsert.set_parameter(7, sGenre);
			if (!stmtInsert.exec()) {
				this->rollback_transaction();
				return (false);
			}
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			return (true);
	} // check_variable
	bool StatDataManager::get_dataset_indivs_by_status_count(int nId,
		const intra::String &status, int &nCount) {
			assert(this->is_valid());
			nCount = 0;
			sqlite::Database *pBase = this->m_database.get();
			sqlite::Statement stmt(pBase, SQL_FIND_INDIVS_BY_DATASET_AND_STATUS_COUNT);
			if (!stmt.is_valid()) {
				return false;
			}
			intra::String sStatus = boost::to_upper_copy(boost::trim_copy(status));
			stmt.set_parameter(1, nId);
			stmt.set_parameter(2, sStatus);
			if (!stmt.exec()) {
				return (false);
			}
			if (!stmt.has_values()) {
				return (false);
			}
			sqlite::DbValue v;
			if (stmt.col_value(0, v)) {
				nCount = v.int_value();
			}
			return (true);
	} // get_dataset_indivs_count
	bool StatDataManager::get_indiv_by_dataset_sigle(int nDatasetId,
		const intra::String &xSigle, intra::Indiv &cur) {
			assert(this->is_valid());
			sqlite::Database *pBase = this->m_database.get();
			sqlite::Statement stmt(pBase, SQL_INDIV_BY_DATASET_AND_SIGLE);
			if (!stmt.is_valid()) {
				return (false);
			}
			intra::String sigle = boost::to_upper_copy(boost::trim_copy(xSigle));
			stmt.set_parameter(1, nDatasetId);
			stmt.set_parameter(2, sigle);
			if (!stmt.exec()) {
				return (false);
			}
			if (!stmt.has_values()) {
				return (false);
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(2, v)) {
					int nx = v.int_value();
					cur.dataset_id(nx);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(5, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(6, v)) {
					intra::String s;
					if (v.string_value(s)) {
						cur.status(s);
					}
				}
			}
			return (true);
	} // get_indiv_by_dataset_sigle
	bool StatDataManager::get_dataset_by_status_values_count(int nId,
		const intra::String &status, int &nCount) {
			assert(this->is_valid());
			nCount = 0;
			sqlite::Database *pBase = this->m_database.get();
			sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_STATUS_VALUES_COUNT);
			if (!stmt.is_valid()) {
				return false;
			}
			intra::String sStatus = boost::to_upper_copy(boost::trim_copy(status));
			stmt.set_parameter(1, nId);
			stmt.set_parameter(2, sStatus);
			if (!stmt.exec()) {
				return (false);
			}
			if (!stmt.has_values()) {
				return (false);
			}
			sqlite::DbValue v;
			if (stmt.col_value(0, v)) {
				nCount = v.int_value();
			}
			return (true);
	} // get_dataset_by_status_values_count
	/////////////////////////////////////
	StatDataManager::StatDataManager() :
		m_intransaction(false) {
	} // StatDataManager
	bool StatDataManager::begin_transaction(void) {
		if (this->m_intransaction) {
			return (true);
		}
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_BEGIN_TRANSACTION);
		if (!stmt.is_valid()) {
			return (false);
		}
		if (stmt.exec()) {
			this->m_intransaction = true;
			return (true);
		}
		return (false);
	} // begin_transaction
	bool StatDataManager::commit_transaction(void) {
		if (!this->m_intransaction) {
			return (false);
		}
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_COMMIT_TRANSACTION);
		if (!stmt.is_valid()) {
			return (false);
		}
		if (stmt.exec()) {
			this->m_intransaction = false;
			return (true);
		}
		return (false);
	} // commit_transaction
	bool StatDataManager::rollback_transaction(void) {
		if (!this->m_intransaction) {
			return (false);
		}
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_ROLLBACK_TRANSACTION);
		if (!stmt.is_valid()) {
			return (false);
		}
		if (stmt.exec()) {
			this->m_intransaction = false;
			return (true);
		}
		return (false);
	} // rollback_transaction
	bool StatDataManager::close(void) {
		bool bRet = true;
		Database *pbase = this->m_database.get();
		if (pbase != nullptr) {
			if (pbase->close()) {
				this->m_database.reset();
			}
			else {
				bRet = false;
			}
		}
		return (bRet);
	} // close
	StatDataManager::~StatDataManager() {
	}
	bool StatDataManager::open(const std::string &filename) {
		this->m_database.reset(new Database());
		Database *pBase = this->m_database.get();
		assert(pBase != nullptr);
		if (!pBase->open(filename)) {
			this->m_database.reset();
			return (false);
		}
		if (!this->check_schema()) {
			this->m_database.reset();
			return (false);
		}
		return (true);
	} // open
	
	bool StatDataManager::is_valid(void) const {
		Database *pbase = this->m_database.get();
		return ((pbase != nullptr) && pbase->is_open());
	} // is_valid
	bool StatDataManager::check_schema(void) {
		Database *pbase = this->m_database.get();
		assert(pbase != nullptr);
		assert(pbase->is_open());
		int i = 0;
		while (StatDataManager::CREATE_SQL[i] != nullptr) {
			const char *pszSQL = CREATE_SQL[i];
			if (!pbase->exec_sql(pszSQL)) {
				return (false);
			}
			++i;
		} // ok
		return (true);
	} // check_schema
	void StatDataManager::init_tables(std::vector<TableDef> &oVec) const {
		oVec.clear();
	} // init_tables
	bool StatDataManager::check_variable_values(int nDatasetId, int nVarId){
		std::vector<int> oInds;
		if (!this->get_dataset_indivs_ids(nDatasetId, oInds)) {
			return (false);
		}
		if (oInds.empty()) {
			return (true);
		}
		sqlite::Database *pBase = this->m_database.get();
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Statement stmtFetch(pBase, SQL_VALUES_BY_VARIABLE_INDIV);
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_VALUE);
		if ((!stmtFetch.is_valid()) || (!stmtInsert.is_valid())) {
			return (false);
		}
		for (auto jt = oInds.begin(); jt != oInds.end(); ++jt) {
			int nIndivId = *jt;
			stmtFetch.reset();
			stmtFetch.set_parameter(1, nVarId);
			stmtFetch.set_parameter(2, nIndivId);
			if (!stmtFetch.exec()) {
				return (false);
			}
			if (stmtFetch.has_values()) {
				continue;
			}
			stmtInsert.reset();
			stmtInsert.set_parameter(1, nVarId);
			stmtInsert.set_parameter(2, nIndivId);
			stmtInsert.set_parameter_null(3);
			if (!stmtInsert.exec()) {
				this->rollback_transaction();
				return (false);
			}
		} // jt
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	}
	bool StatDataManager::check_indiv_values(int nDatasetId, int nIndivId){
		std::vector<int> oVars;
		if (!this->get_dataset_variables_ids(nDatasetId, oVars)) {
			return (false);
		}
		if (oVars.empty()) {
			return (true);
		}
		sqlite::Database *pBase = this->m_database.get();
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Statement stmtFetch(pBase, SQL_VALUES_BY_VARIABLE_INDIV);
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_VALUE);
		if ((!stmtFetch.is_valid()) || (!stmtInsert.is_valid())) {
			return (false);
		}
		for (auto jt = oVars.begin(); jt != oVars.end(); ++jt) {
			int nVarId = *jt;
			stmtFetch.reset();
			stmtFetch.set_parameter(1, nVarId);
			stmtFetch.set_parameter(2, nIndivId);
			if (!stmtFetch.exec()) {
				return (false);
			}
			if (stmtFetch.has_values()) {
				continue;
			}
			stmtInsert.reset();
			stmtInsert.set_parameter(1, nVarId);
			stmtInsert.set_parameter(2, nIndivId);
			stmtInsert.set_parameter_null(3);
			if (!stmtInsert.exec()) {
				this->rollback_transaction();
				return (false);
			}
		} // jt
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	}
	bool StatDataManager::check_values(int nDatasetId) {
		std::vector<int> oInds;
		if (!this->get_dataset_indivs_ids(nDatasetId, oInds)) {
			return (false);
		}
		if (oInds.empty()) {
			return (true);
		}
		sqlite::Database *pBase = this->m_database.get();
		std::vector<int> oVars;
		if (!this->get_dataset_variables_ids(nDatasetId, oVars)){
			return (false);
		}
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Statement stmtFetch(pBase, SQL_VALUES_BY_VARIABLE_INDIV);
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_VALUE);
		if ((!stmtFetch.is_valid()) || (!stmtInsert.is_valid())) {
			return (false);
		}
		for (auto it = oVars.begin(); it != oVars.end(); ++it) {
			int nVarId = *it;
			for (auto jt = oInds.begin(); jt != oInds.end(); ++jt) {
				int nIndivId = *jt;
				stmtFetch.reset();
				stmtFetch.set_parameter(1, nVarId);
				stmtFetch.set_parameter(2, nIndivId);
				if (!stmtFetch.exec()) {
					return (false);
				}
				if (stmtFetch.has_values()) {
					continue;
				}
				stmtInsert.reset();
				stmtInsert.set_parameter(1, nVarId);
				stmtInsert.set_parameter(2, nIndivId);
				stmtInsert.set_parameter_null(3);
				if (!stmtInsert.exec()) {
					this->rollback_transaction();
					return (false);
				}
			} // jt
		} // it
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // check_values
	bool StatDataManager::check_pred_dataset(int nDatasetId) {
		std::vector<TableDef> oDefs;
		this->init_tables(oDefs);
		if (oDefs.empty()){
			return (true);
		}
		for (auto it = oDefs.begin(); it != oDefs.end(); ++it) {
			const TableDef &oDef = *it;
			std::string sgenre = oDef.genre;
			std::string stype = oDef.type;
			std::string sname = oDef.name;
			bool bCateg = oDef.is_categ;
			if (!this->check_variable(nDatasetId, sname, stype, bCateg, sgenre)) {
				return (false);
			}
		} // it
		return (true);
	} // check_pred_dataset
	void StatDataManager::convert_value(const intra::Value &s,
		const std::string &vartype, boost::any &v) {
			std::string sx = boost::to_lower_copy(boost::trim_copy(vartype));
			if (sx == "bool") {
				v = boost::any(s.bool_value());
			}
			else if (sx == "short") {
				v = boost::any(s.short_value());
			}
			else if (sx == "integer") {
				v = boost::any(s.int_value());
			}
			else if (sx == "float") {
				v = boost::any(s.float_value());
			}
			else if (sx == "double") {
				v = boost::any(s.double_value());
			}
			else if (sx == "string") {
				std::string ss;
				s.string_value(ss);
				v = boost::any(ss);
			}
	} //convert_value
	bool StatDataManager::get_value_by_variable_indiv(int nVarId, int nIndivId,
		intra::Value &cur) {
			assert(this->is_valid());
			sqlite::Database *pBase = this->m_database.get();
			sqlite::Statement stmt(pBase, SQL_VALUES_BY_VARIABLE_INDIV);
			sqlite::Statement stmtVarType(pBase, SQL_FIND_VARIABLE_TYPE);
			if ((!stmt.is_valid()) || (!stmtVarType.is_valid())) {
				return (false);
			}
			stmt.set_parameter(1, nVarId);
			stmt.set_parameter(2, nIndivId);
			if (!stmt.exec()) {
				return (false);
			}
			if (!stmt.has_values()) {
				return (false);
			}
			std::string vartype;
			{
				sqlite::DbValue v;
				if (stmt.col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(2, v)) {
					int nx = v.int_value();
					cur.variable_id(nx);
					stmtVarType.set_parameter(1, nx);
					if (!stmtVarType.exec()) {
						return (false);
					}
					if (!stmtVarType.has_values()) {
						return (false);
					}
					sqlite::DbValue vv;
					stmtVarType.col_value(0, vv);
					vv.string_value(vartype);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					int nx = v.int_value();
					cur.indiv_id(nx);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					boost::any vx = v.value();
					intra::Value x;
					x.value(vx);
					boost::any vy;
					this->convert_value(x, vartype, vy);
					cur.value(vy);
				}
			}
			return (true);
	} // get_dataset_indivs
	bool StatDataManager::get_dataset_values_count(int nId, int &nCount) {
		assert(this->is_valid());
		nCount = 0;
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_VALUES_COUNT);
		if (!stmt.is_valid()) {
			return false;
		}
		stmt.set_parameter(1, nId);
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		sqlite::DbValue v;
		if (stmt.col_value(0, v)) {
			nCount = v.int_value();
		}
		return (true);
	} // get_dataset_values_count
	bool StatDataManager::get_variable_values_count(int nVarId, int &nCount){
		assert(this->is_valid());
		nCount = 0;
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_VALUES_BY_VARIABLE_NOT_NULL_COUNT);
		if (!stmt.is_valid()) {
			return false;
		}
		stmt.set_parameter(1, nVarId);
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		sqlite::DbValue v;
		if (stmt.col_value(0, v)) {
			nCount = v.int_value();
		}
		return (true);
	}
	bool StatDataManager::get_dataset_indivs_count(int nId, int &nCount) {
		assert(this->is_valid());
		nCount = 0;
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_INDIVS_COUNT);
		if (!stmt.is_valid()) {
			return false;
		}
		stmt.set_parameter(1, nId);
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		sqlite::DbValue v;
		if (stmt.col_value(0, v)) {
			nCount = v.int_value();
		}
		return (true);
	} // get_dataset_indivs_count
	///////////////////////////
} /* namespace intrasqlite */
