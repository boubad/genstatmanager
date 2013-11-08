/*
* statdatamanager.h
*
*  Created on: 17 sept. 2013
*      Author: Boubacar
*/
#ifdef _MSC_VER
#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif
#endif // _MSC_VER
#ifndef STATDATAMANAGER_H_
#define STATDATAMANAGER_H_
/////////////////////////////////
#include <memory>
#include <string>
#include <sstream>
////////////////////////////////////////
#include "database.h"
#include "statement.h"
//////////////////////////////////
#include "statdata.h"
#include "importdata.h"
///////////////////////////////////
namespace intrasqlite {
	////////////////////////////////
	class TableDef {
	public:
		TableDef() :
			is_categ(false) {
		}
		TableDef(bool categ, const char *pname, const char *ptype,
			const char *pgenre) :
		is_categ(categ), name(pname), type(ptype), genre(pgenre) {
		}
		TableDef(const TableDef &other) :
			is_categ(other.is_categ), name(other.name), type(other.type), genre(
			other.genre) {
		}
		TableDef & operator=(const TableDef &other) {
			if (this != &other) {
				this->is_categ = other.is_categ;
				this->name = other.name;
				this->type = other.type;
				this->genre = other.genre;
			}
			return (*this);
		}
		~TableDef() {
		}
		bool is_categ;
		std::string name;
		std::string type;
		std::string genre;
	};
	/////////////////////////////////
	template<class TSTRING>
	class StatDataManager {
	public:
		typedef TSTRING StringType;
		typedef typename TSTRING::value_type Char;
		typedef intra::Value ValueType;
		typedef intra::Dataset<TSTRING> DatasetType;
		typedef intra::Variable<TSTRING> VariableType;
		typedef intra::Indiv<TSTRING> IndivType;
		typedef typename ValueType::PValue PValue;
		typedef typename VariableType::PVariable PVariable;
		typedef typename IndivType::PIndiv PIndiv;
		//
		typedef sqlite::Database Database;
		typedef sqlite::PStatement PStatement;
		typedef sqlite::DbValue DbValue;
	private:
		// no implementation
		StatDataManager(const StatDataManager<TSTRING> &other);
		StatDataManager<TSTRING> & operator=(const StatDataManager<TSTRING> &other);
	private:
		bool m_intransaction;
		std::unique_ptr<Database> m_database;
		std::map<int, StringType> m_vartypes;
	protected:
		virtual void init_tables(std::vector<TableDef> &oVec) const;
		virtual void get_indiv_sigle_name(StringType &s) const;
		virtual void get_indiv_status_name(StringType &s) const;
	protected:
		void read_dataset(PStatement stmt, DatasetType &cur){
			{
				DbValue v;
				if (stmt->col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				DbValue v;
				if (stmt->col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				DbValue v;
				if (stmt->col_value(2, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				DbValue v;
				if (stmt->col_value(3, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				DbValue v;
				if (stmt->col_value(4, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
		}// read_dataset
		void read_indiv(PStatement pStmtFetch, IndivType &cur){
			{
				DbValue v;
				if (pStmtFetch->col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(2, v)) {
					int nx = v.int_value();
					cur.dataset_id(nx);
				}
			}
			{
				sqlite::DbValue v;
				if (pStmtFetch->col_value(3, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(4, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(5, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(6, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.status(s);
					}
				}
			}
		}// read_indiv
		void read_variable(PStatement stmt, VariableType &cur){
			{
				DbValue v;
				if (stmt->col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				DbValue v;
				if (stmt->col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				DbValue v;
				if (stmt->col_value(2, v)) {
					int nx = v.int_value();
					cur.dataset_id(nx);
				}
			}
			{
				DbValue v;
				if (stmt->col_value(3, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				DbValue v;
				if (stmt->col_value(4, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.vartype(s);
					}
				}
			}
			{
				DbValue v;
				if (stmt->col_value(5, v)) {
					int nx = v.int_value();
					bool b = (nx != 0) ? true : false;
					cur.is_categvar(b);
				}
			}
			{
				DbValue v;
				if (stmt->col_value(6, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				DbValue v;
				if (stmt->col_value(7, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			{
				DbValue v;
				if (stmt->col_value(8, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.genre(s);
					}
				}
			}
		}// read_variable
		bool get_variable_type(int nVarId, StringType &vartype){
			vartype.clear();
			std::map<int, StringType> &oMap = this->m_vartypes;
			if (oMap.find(nVarId) != oMap.end()) {
				vartype = oMap[nVarId];
			} else {
				PStatement pStmtVar = this->find_statement(SQL_FIND_VARIABLE_TYPE);
				pStmtVar->reset();
				pStmtVar->set_parameter(1, nVarId);
				if (!pStmtVar->exec()) {
					return (false);
				}
				if (!pStmtVar->has_values()) {
					return (false);
				}
				DbValue vv;
				pStmtVar->col_value(0, vv);
				vv.string_value(vartype);
				if (vartype.empty()){
					return (false);
				}
				oMap[nVarId] = vartype;
			}
			return (true);
		}// get_variableType
		PStatement find_statement(const char *pszSQL){
			assert(this->is_valid());
			Database *pBase = this->m_database.get();
			assert(pBase != nullptr);
			PStatement pRet = pBase->find_statement(pszSQL);
			assert(pRet != nullptr);
			return pRet;
		}// find_statement
		bool read_value(PStatement pStmtFetch, ValueType &cur){
			StringType vartype;
			{
				DbValue v;
				if (pStmtFetch->col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(2, v)) {
					int nx = v.int_value();
					if (!this->get_variable_type(nx,vartype)){
						return (false);
					}
					cur.variable_id(nx);
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(3, v)) {
					int nx = v.int_value();
					cur.indiv_id(nx);
				}
			}
			{
				DbValue v;
				if (pStmtFetch->col_value(4, v)) {
					boost::any vx = v.value();
					ValueType x;
					x.value(vx);
					boost::any vy;
					this->convert_value(x, vartype, vy);
					cur.value(vy);
				}
			}
			return (true);
		}// readValue
		bool begin_transaction(void);
		bool commit_transaction(void);
		bool rollback_transaction(void);
		bool check_pred_dataset(int nDatasetId);
		void convert_value(const ValueType &s, const StringType &vartype,
			boost::any &v);
		void reset(void) {
		} // reset
	public:
		StatDataManager();
		virtual ~StatDataManager();
	public:
		template <class ALLOCINT, class COMPARE, class ALLOCPAIR, class ALLOCMAP>
		bool get_common_values_not_null(const std::set<int,COMPARE,ALLOCINT> &oVars,
			std::map<int,std::map<int,boost::any,COMPARE,ALLOCPAIR>,COMPARE,ALLOCMAP> &oRes){
				oRes.clear();
				auto iend = oVars.end();
				const int taken = 128;
				for (auto it = oVars.begin(); it != iend; ++it){
					int nVarId = *it;
					if (nVarId == 0){
						continue;
					}
					std::map<int,boost::any,COMPARE,ALLOCPAIR> oMap;
					int nCount = 0;
					bool bRet = this->get_variable_values_pair_not_null_count(nVarId,nCount);
					if (!bRet){
						return (false);
					}
					if (nCount < 1){
						continue;
					}
					int skip = 0;
					while (skip < nCount){
						std::map<int,boost::any,COMPARE,ALLOCPAIR> xMap;
						bRet = this->get_variable_values_pair_not_null(nVarId,xMap,skip,taken);
						if (!bRet){
							return (false);
						}
						auto jend = xMap.end();
						for (auto jt = xMap.begin(); jt != jend; ++jt){
							int nId = (*jt).first;
							boost::any v = (*jt).second;
							oMap[nId] = v;
						}// jt
						skip += taken;
					}// skip
					if (oMap.empty()){
						continue;
					}
					auto kend = oMap.end();
					if (oRes.empty()){
						for (auto kt = oMap.begin(); kt != kend; ++kt){
							int nIndivId = (*kt).first;
							boost::any v = (*kt).second;
							std::map<int,boost::any,COMPARE,ALLOCPAIR> m;
							m[nVarId] = v;
							oRes[nIndivId] = m;
						}// kt
					} else {
						std::set<int,COMPARE,ALLOCINT> oDelSet;
						auto mend = oRes.end();
						for (auto mt = oRes.begin(); mt != mend; ++mt){
							int nIndivId = (*mt).first;
							if (oMap.find(nIndivId) == kend){
								oDelSet.insert(nIndivId);
							} else {
								boost::any v = oMap[nIndivId];
								std::map<int,boost::any,COMPARE,ALLOCPAIR>  &m = oRes[nIndivId];
								m[nVarId] = v;
							}
						}// mt
						auto hend = oDelSet.end();
						for (auto ht = oDelSet.begin(); ht != hend; ++ht){
							int nIndivId = *ht;
							auto kx = oRes.find(nIndivId);
							if (kx != oRes.end()){
								oRes.erase(kx);
							}
						}// ht
					}
				}// it
				return (true);
		}// get_common_values
		template <class ALLOCINT,class ALLOCANY, class ALLOCANYVECTOR>
		bool get_common_values(const std::vector<int,ALLOCINT> &oVars, std::vector<int,ALLOCINT> &oCols,std::vector<int,ALLOCINT> &oInds,
			std::vector<std::vector<boost::any,ALLOCANY>,ALLOCANYVECTOR> &oVals) {
				typedef std::map<int,boost::any> AnyMap;
				typedef std::map<int,AnyMap> AnyArray;
				typedef std::set<int> IntSet;
				IntSet oSetVars;
				//
				oCols.clear();
				oInds.clear();
				oVals.clear();
				//
				for (auto it = oVars.begin(); it != oVars.end(); ++it){
					int nVarId = *it;
					if (nVarId > 0){
						oSetVars.insert(nVarId);
					}
				}// it
				if (oSetVars.empty()){
					return (false);
				}
				AnyArray oAr;
				if (!this->get_common_values_not_null(oSetVars,oAr)){
					return (false);
				}
				size_t nInds = oAr.size();
				if (nInds < 1){
					return (true);
				}
				oSetVars.clear();
				auto iFirst = oAr.begin();
				auto mfirst = (*iFirst).second;
				size_t nVars = 0;
				for (auto lt = mfirst.begin(); lt != mfirst.end(); ++lt){
					++nVars;
				}
				oCols.resize(nVars);
				size_t icur = 0;
				for (auto it = mfirst.begin(); it != mfirst.end(); ++it){
					int nVarId = (*it).first;
					oCols[icur++] = nVarId;
				}// it
				oInds.resize(nInds);
				oVals.resize(nInds);
				icur = 0;
				auto iend = oAr.end();
				for (auto it = oAr.begin(); it != iend; ++it){
					std::vector<boost::any,ALLOCANY> vcur(nVars);
					int nIndivId = (*it).first;
					oInds[icur] = nIndivId;
					AnyMap &oMap = (*it).second;
					auto kend = oMap.end();
					for (size_t i = 0; i < nVars; ++i){
						int nVarId = oCols[i];
						auto kt = oMap.find(nVarId);
						if (kt != kend){
							boost::any v = (*kt).second;
							vcur[i] = v;
						}
					}// i
					oVals[icur++] = vcur;
				}// it
				return (true);
		}// get_common_values
		template<class IFSTREAM>
		bool process_data(IFSTREAM &inFile, const StringType &datasetSigle,
			DatasetType &oSet, const Char & delim, const StringType & na);
		template<class IFSTREAM>
		bool process_data(IFSTREAM &inFile, const StringType &datasetSigle,
			const Char &delim, const StringType &na);
		bool fill_data(const intra::ImportData<TSTRING> &oData, DatasetType &oSet);
		bool open(const StringType &filename);
		bool get_value_by_variable_indiv(int nVarId, int nIndivId, ValueType &cur);
		bool get_dataset_values_count(int nId, int &nCount);
		bool get_variable_values_count(int nVarId, int &nCount);
		bool get_dataset_indivs_count(int nId, int &nCount);
		bool check_values(int nDatasetId);
		bool check_variable_values(int nDatasetId, int nVarId);
		bool check_indiv_values(int nDatasetId, int nIndId);
		bool get_dataset_by_status_values_count(int nId, const StringType &status,
			int &nCount);
		bool get_indiv_by_dataset_sigle(int nDatasetId, const StringType &xSigle,
			IndivType &cur);
		bool get_dataset_indivs_by_status_count(int nId, const StringType &status,
			int &nCount);
		bool get_variable_by_dataset_and_sigle(int nDatasetId, const StringType &xSigle,
			VariableType &cur);
		bool check_variable(int nDatasetId, const StringType &sigle,
			const StringType &type, bool bCateg, const StringType &genre);
		bool load_dataset(DatasetType &cur);
		bool remove_dataset(const DatasetType &cur);
		bool update_dataset(const DatasetType &cur);
		bool insert_dataset(const DatasetType &cur);
		bool get_dataset_by_sigle(const StringType &xSigle, DatasetType &cur);
		bool get_dataset_by_id(int xId, DatasetType &cur);
		bool is_valid(void) const;
		bool close(void);
		bool get_variable_by_id(int nVarId,VariableType &cur) {
			assert(this->is_valid());
			PStatement stmt = this->find_statement(SQL_FIND_VARIABLE_BY_ID);
			if (!stmt->is_valid()) {
				return (false);
			}
			stmt->reset();
			stmt->set_parameter(1, nVarId);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			this->read_variable(stmt, cur);
			return (true);
		} // get_variable_by_id
		bool get_indiv_by_id(int nIndId,IndivType &cur) {
			assert(this->is_valid());
			PStatement stmt = this->find_statement(SQL_FIND_INDIV_BY_ID);
			if (!stmt->is_valid()) {
				return (false);
			}
			stmt->reset();
			stmt->set_parameter(1, nIndId);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			this->read_indiv(stmt, cur);
			return (true);
		} // get_indiv_by_id
	public:
		template<class ALLOCVAL>
		bool remove_values(const std::vector<ValueType, ALLOCVAL> &oVec) {
			assert(this->is_valid());
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement pStmt = this->find_statement(SQL_REMOVE_VALUE);
			if (!pStmt->is_valid()) {
				this->rollback_transaction();
				return (false);
			}
			for (auto it = oVec.begin(); it != oVec.end(); ++it) {
				const ValueType &cur = *it;
				int nId = cur.id();
				if (nId != 0) {
					pStmt->reset();
					pStmt->set_parameter(1, nId);
					if (!pStmt->exec()) {
						this->rollback_transaction();
						return (false);
					}
				}
			} // it
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			return (true);
		} // remove_values
		template<class ALLOCVAL>
		bool maintains_values(const std::vector<ValueType, ALLOCVAL> &oVec) {
			assert(this->is_valid());
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement stmtFetch = find_statement(SQL_VALUES_BY_VARIABLE_INDIV);
			PStatement stmtInsert = find_statement(SQL_INSERT_VALUE);
			PStatement stmtUpdate = find_statement(SQL_UPDATE_VALUE);
			PStatement stmtFirst = find_statement(SQL_VALUE_BY_ID);
			if ((!stmtFetch->is_valid())
				|| (!stmtUpdate->is_valid() || (!stmtFirst->is_valid()))
				|| (!stmtInsert->is_valid())) {
					this->rollback_transaction();
					return (false);
			}
			for (auto it = oVec.begin(); it != oVec.end(); ++it) {
				const ValueType &cur = *it;
				bool bNull = !cur.is_valid();
				int nId = cur.id();
				std::string sval;
				cur.string_value(sval);
				if (nId != 0) {
					stmtFirst->reset();
					stmtFirst->set_parameter(1, nId);
					if (!stmtFirst->exec()) {
						this->rollback_transaction();
						return (false);
					}
					if (stmtFirst->has_values()) {
						stmtUpdate->reset();
						if (bNull) {
							stmtUpdate->set_parameter_null(1);
						} else {
							stmtUpdate->set_parameter(1, sval);
						}
						stmtUpdate->set_parameter(2, nId);
						if (!stmtUpdate->exec()) {
							this->rollback_transaction();
							return (false);
						}
						continue;
					}
				} // nId
				int nVarId = cur.variable_id();
				int nIndivId = cur.indiv_id();
				if ((nVarId == 0) || (nIndivId == 0)) {
					continue;
				}
				stmtFetch->reset();
				stmtFetch->set_parameter(1, nVarId);
				stmtFetch->set_parameter(2, nIndivId);
				if (!stmtFetch->exec()) {
					return (false);
				}
				if (stmtFetch->has_values()) {
					nId = 0;
					sqlite::DbValue v;
					if (stmtFetch->col_value(0, v)) {
						nId = v.int_value();
					}
					if (nId != 0) {
						stmtUpdate->reset();
						if (bNull) {
							stmtUpdate->set_parameter_null(1);
						} else {
							stmtUpdate->set_parameter(1, sval);
						}
						stmtUpdate->set_parameter(2, nId);
						if (!stmtUpdate->exec()) {
							this->rollback_transaction();
							return (false);
						}
					} // nId
					continue;
				} else {
					stmtInsert->reset();
					stmtInsert->set_parameter(1, nVarId);
					stmtInsert->set_parameter(2, nIndivId);
					if (bNull) {
						stmtInsert->set_parameter_null(3);
					} else {
						stmtInsert->set_parameter(3, sval);
					}
					if (!stmtInsert->exec()) {
						this->rollback_transaction();
						return (false);
					}
				}
			} // it
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			return (true);
		} // maintains_values
		template<class ALLOCVEC>
		bool get_variable_values(int nVarId, std::vector<ValueType, ALLOCVEC> &oVec,
			int skip, int taken) {
				oVec.clear();
				if ((skip < 0) || (taken < 1)) {
					return (false);
				}
				assert(this->is_valid());
				PStatement pStmtFetch = this->find_statement( SQL_VALUES_BY_VARIABLE_NOT_NULL);
				if (!pStmtFetch->is_valid()) {
					return (false);
				}
				pStmtFetch->reset();
				pStmtFetch->set_parameter(1, nVarId);
				pStmtFetch->set_parameter(2, taken);
				pStmtFetch->set_parameter(3, skip);
				if (!pStmtFetch->exec()) {
					return (false);
				}
				while (pStmtFetch->has_values()) {
					ValueType cur;
					if (!this->read_value(pStmtFetch,cur)){
						return (false);
					}
					oVec.push_back(cur);
					if (!pStmtFetch->next()) {
						break;
					}
				} // values
				return (true);
		} // get_variable_values
		template<class ALLOCVEC>
		bool get_dataset_values(int nDatasetId, std::vector<ValueType, ALLOCVEC> &oVec,
			int skip, int taken) {
				oVec.clear();
				if ((skip < 0) || (taken < 1)) {
					return (false);
				}
				assert(this->is_valid());
				PStatement pStmtFetch = this->find_statement(SQL_FIND_DATASET_VALUES);
				//
				pStmtFetch->reset();
				pStmtFetch->set_parameter(1, nDatasetId);
				pStmtFetch->set_parameter(2, taken);
				pStmtFetch->set_parameter(3, skip);
				if (!pStmtFetch->exec()) {
					return (false);
				}
				while (pStmtFetch->has_values()) {
					ValueType cur;
					if (!this->read_value(pStmtFetch, cur)){
						return (false);
					}
					oVec.push_back(cur);
					if (!pStmtFetch->next()) {
						break;
					}
				} // values
				return (true);
		} // get_dataset_values
		template<class ALLOCVEC>
		bool get_dataset_by_status_values(int nDatasetId, const StringType &status,
			std::vector<ValueType, ALLOCVEC> &oVec, int skip, int taken) {
				oVec.clear();
				if ((skip < 0) || (taken < 1)) {
					return (false);
				}
				assert(this->is_valid());
				PStatement pStmtFetch = this->find_statement(SQL_FIND_DATASET_BY_STATUS_VALUES);
				StringType sStatus = to_upper(trim(status));
				pStmtFetch->reset();
				pStmtFetch->set_parameter(1, nDatasetId);
				pStmtFetch->set_parameter(2, sStatus);
				pStmtFetch->set_parameter(3, taken);
				pStmtFetch->set_parameter(4, skip);
				if (!pStmtFetch->exec()) {
					return (false);
				}
				while (pStmtFetch->has_values()) {
					ValueType cur;
					if (!this->read_value(pStmtFetch,cur)){
						return (false);
					}
					oVec.push_back(cur);
					if (!pStmtFetch->next()) {
						break;
					}
				} // values
				return (true);
		} // get_dataset_by_status_values
		template<class ALLOCIND>
		bool remove_indivs(const std::vector<IndivType, ALLOCIND> &oVec, bool bTrans = true) {
			assert(this->is_valid());
			if (bTrans){
				if (!this->begin_transaction()) {
					return (false);
				}
			}
			PStatement stmt = this->find_statement(SQL_REMOVE_INDIV);
			PStatement stmtDelete = this->find_statement(SQL_REMOVE_INDIV_VALUES);
			if ( (!stmt->is_valid())  || (!stmtDelete->is_valid())) {
				if (bTrans){
					this->rollback_transaction();
				}
				return (false);
			}
			for (auto it = oVec.begin(); it != oVec.end(); ++it) {
				const IndivType &cur = *it;
				int nId = cur.id();
				if (nId != 0) {
					stmtDelete->reset();
					stmtDelete->set_parameter(1,nId);
					if (!stmtDelete->exec()){
						if (bTrans){
							this->rollback_transaction();
						}
						return (false);
					}
					stmt->reset();
					stmt->set_parameter(1, nId);
					if (!stmt->exec()) {
						if (bTrans){
							this->rollback_transaction();
						}
						return (false);
					}
				}
			} // it
			if (bTrans){
				if (!this->commit_transaction()) {
					this->rollback_transaction();
					return (false);
				}
			}
			return (true);
		} // remove_indivs
		template<class ALLOCIND>
		bool maintains_indivs(const std::vector<IndivType, ALLOCIND> &oVec) {
			assert(this->is_valid());
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement stmtFetch1 = this->find_statement(SQL_FIND_INDIV_BY_ID);
			PStatement stmtFetch2 = this->find_statement(SQL_INDIV_BY_DATASET_AND_SIGLE);
			if ((!stmtFetch1->is_valid()) || (!stmtFetch2->is_valid())) {
				this->rollback_transaction();
				return (false);
			}
			PStatement stmtInsert = this->find_statement(SQL_INSERT_INDIV);
			PStatement stmtUpdate = this->find_statement(SQL_UPDATE_INDIV);
			if ((!stmtUpdate->is_valid()) || (!stmtInsert->is_valid())) {
				this->rollback_transaction();
				return (false);
			}
			for (auto it = oVec.begin(); it != oVec.end(); ++it) {
				const IndivType &cur = *it;
				int nId = cur.id();
				int nDatasetId = cur.dataset_id();
				StringType sigle = cur.sigle();
				if (nId != 0) {
					stmtFetch1->reset();
					stmtFetch1->set_parameter(1, nId);
					if (!stmtFetch1->exec()) {
						this->rollback_transaction();
						return (false);
					}
					if (!stmtFetch1->has_values()) {
						nId = 0;
					}
				} // nId
				if ((nId == 0) && (nDatasetId != 0)) {
					StringType ss = to_upper(trim(sigle));
					stmtFetch2->reset();
					stmtFetch2->set_parameter(1, nDatasetId);
					stmtFetch2->set_parameter(2, ss);
					if (!stmtFetch2->exec()) {
						this->rollback_transaction();
						return (false);
					}
					if (stmtFetch2->has_values()) {
						DbValue v;
						if (stmtFetch2->col_value(0, v)) {
							nId = v.int_value();
						}
					}
				}
				if ((!sigle.empty()) && (nId != 0)) {
					stmtUpdate->reset();
					stmtUpdate->set_parameter(1, sigle);
					stmtUpdate->set_parameter(2, cur.name());
					stmtUpdate->set_parameter(3, cur.description());
					stmtUpdate->set_parameter(4, cur.status());
					stmtUpdate->set_parameter(5, nId);
					if (!stmtUpdate->exec()) {
						this->rollback_transaction();
						return (false);
					}
				} else if ((!sigle.empty()) && (nDatasetId != 0)) {
					stmtInsert->reset();
					stmtInsert->set_parameter(1, nDatasetId);
					stmtInsert->set_parameter(2, sigle);
					stmtInsert->set_parameter(3, cur.name());
					stmtInsert->set_parameter(4, cur.description());
					stmtInsert->set_parameter(5, cur.status());
					if (!stmtInsert->exec()) {
						this->rollback_transaction();
						return (false);
					}
				}
			} // it
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			return (true);
		} // maintains_indivs
		template<class ALLOCVEC>
		bool get_dataset_indivs_by_status(int nDatasetId,
			std::vector<IndivType, ALLOCVEC> &oVec, const StringType &status,
			int skip, int taken) {
				oVec.clear();
				if ((skip < 0) || (taken < 1)) {
					return (false);
				}
				assert(this->is_valid());
				PStatement pStmtFetch = this->find_statement(SQL_FIND_INDIVS_BY_DATASET_AND_STATUS);
				StringType sStatus = to_upper(trim(status));
				pStmtFetch->reset();
				pStmtFetch->set_parameter(1, nDatasetId);
				pStmtFetch->set_parameter(2, sStatus);
				pStmtFetch->set_parameter(3, taken);
				pStmtFetch->set_parameter(4, skip);
				if (!pStmtFetch->exec()) {
					return (false);
				}
				while (pStmtFetch->has_values()) {
					IndivType cur;
					this->read_indiv(pStmtFetch,cur);
					oVec.push_back(cur);
					if (!pStmtFetch->next()) {
						break;
					}
				} // values
				return (true);
		} // get_dataset_by_status_indivs
		template<class ALLOCVEC>
		bool get_dataset_indivs(int nDatasetId,
			std::vector<IndivType, ALLOCVEC> &oVec) {
				assert(this->is_valid());
				oVec.clear();
				PStatement stmt = this->find_statement(SQL_FIND_DATASET_INDIVS);
				if (!stmt->is_valid()) {
					return (false);
				}
				stmt->reset();
				stmt->set_parameter(1, nDatasetId);
				if (!stmt->exec()) {
					return (false);
				}
				while (stmt->has_values()) {
					IndivType cur;
					this->read_indiv(stmt,cur);
					oVec.push_back(cur);
					if (!stmt->next()) {
						break;
					}
				} // values
				return (true);
		} // get_dataset_indivs
		template<class ALLOCVEC>
		bool get_dataset_indivs(int nDatasetId,
			std::vector<IndivType, ALLOCVEC> &oVec,int skip,int taken) {
				assert(this->is_valid());
				oVec.clear();
				if ((skip < 0) || (taken < 1)){
					return (false);
				}
				PStatement stmt = this->find_statement(SQL_FIND_DATASET_INDIVS_PAGED);
				if (!stmt->is_valid()) {
					return (false);
				}
				stmt->reset();
				stmt->set_parameter(1, nDatasetId);
				stmt->set_parameter(2, taken);
				stmt->set_parameter(3, skip);
				if (!stmt->exec()) {
					return (false);
				}
				while (stmt->has_values()) {
					IndivType cur;
					this->read_indiv(stmt,cur);
					oVec.push_back(cur);
					if (!stmt->next()) {
						break;
					}
				} // values
				return (true);
		} // get_dataset_indivs
		template<class ALLOCINT>
		bool get_dataset_indivs_ids(int nDatasetId, std::vector<int, ALLOCINT> &oInds) {
			assert(this->is_valid());
			oInds.clear();
			PStatement stmt = this->find_statement(SQL_GET_DATASET_INDIV_IDS);
			if (!stmt->is_valid()) {
				return false;
			}
			stmt->reset();
			stmt->set_parameter(1, nDatasetId);
			if (!stmt->exec()) {
				return (false);
			}
			while (stmt->has_values()) {
				DbValue v;
				if (stmt->col_value(0, v)) {
					int nId = v.int_value();
					oInds.push_back(nId);
				}
				if (!stmt->next()) {
					break;
				}
			} // while
			return (true);
		} // get_dataset_indivs_ids
		template<class ALLOCVAR>
		bool remove_variables(const std::vector<VariableType, ALLOCVAR> &oVec,bool bTrans = true) {
			assert(this->is_valid());
			if (bTrans){
				if (!this->begin_transaction()) {
					return (false);
				}
			}
			PStatement stmt = this->find_statement(SQL_REMOVE_VARIABLE);
			PStatement stmtDelete = this->find_statement(SQL_REMOVE_VARIABLE_VALUES);
			if ((!stmt->is_valid()) || (!stmtDelete->is_valid())) {
				if (bTrans){
					this->rollback_transaction();
				}
				return (false);
			}
			for (auto it = oVec.begin(); it != oVec.end(); ++it) {
				const VariableType &cur = *it;
				int nId = cur.id();
				if (nId != 0) {
					stmtDelete->reset();
					stmtDelete->set_parameter(1,nId);
					if (!stmtDelete->exec()){
						if (bTrans){
							this->rollback_transaction();
						}
						return (false);
					}
					stmt->reset();
					stmt->set_parameter(1, nId);
					if (!stmt->exec()) {
						if (bTrans){
							this->rollback_transaction();
						}
						return (false);
					}
				}
			} // it
			if (bTrans){
				if (!this->commit_transaction()) {
					this->rollback_transaction();
					return (false);
				}
			}
			return (true);
		} // remove_variables
		template<class ALLOCVAR>
		bool maintains_variables(const std::vector<VariableType, ALLOCVAR> &oVec) {
			assert(this->is_valid());
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement stmtFetch1 = this->find_statement(SQL_FIND_VARIABLE_BY_ID);
			PStatement stmtFetch2 = this->find_statement(SQL_VARIABLE_BY_DATASET_AND_SIGLE);
			if ((!stmtFetch1->is_valid()) || (!stmtFetch2->is_valid())) {
				this->rollback_transaction();
				return (false);
			}
			PStatement stmtInsert = this->find_statement(SQL_INSERT_VARIABLE);
			PStatement stmtUpdate = this->find_statement(SQL_UPDATE_VARIABLE);
			if ((!stmtUpdate->is_valid()) || (!stmtInsert->is_valid())) {
				this->rollback_transaction();
				return (false);
			}
			for (auto it = oVec.begin(); it != oVec.end(); ++it) {
				const VariableType &cur = *it;
				int nId = cur.id();
				int nDatasetId = cur.dataset_id();
				StringType sigle = cur.sigle();
				int nz = (cur.is_categvar()) ? 1 : 0;
				if (nId != 0) {
					stmtFetch1->reset();
					stmtFetch1->set_parameter(1, nId);
					if (!stmtFetch1->exec()) {
						this->rollback_transaction();
						return (false);
					}
					if (!stmtFetch1->has_values()) {
						nId = 0;
					}
				} // nId
				if ((nId == 0) && (nDatasetId != 0)) {
					StringType ss = to_upper(trim(sigle));
					stmtFetch2->reset();
					stmtFetch2->set_parameter(1, nDatasetId);
					stmtFetch2->set_parameter(2, ss);
					if (!stmtFetch2->exec()) {
						this->rollback_transaction();
						return (false);
					}
					if (stmtFetch2->has_values()) {
						DbValue v;
						if (stmtFetch2->col_value(0, v)) {
							nId = v.int_value();
						}
					}
				}
				if ((nId != 0) && (!sigle.empty())) {
					stmtUpdate->reset();
					stmtUpdate->set_parameter(1, sigle);
					stmtUpdate->set_parameter(2, cur.vartype());
					stmtUpdate->set_parameter(3, nz);
					stmtUpdate->set_parameter(4, cur.name());
					stmtUpdate->set_parameter(5, cur.description());
					stmtUpdate->set_parameter(6, cur.genre());
					stmtUpdate->set_parameter(7, nId);
					if (!stmtUpdate->exec()) {
						this->rollback_transaction();
						return (false);
					}
				} else if ((nDatasetId != 0) && (!sigle.empty())) {
					stmtInsert->reset();
					stmtInsert->set_parameter(1, nDatasetId);
					stmtInsert->set_parameter(2, sigle);
					stmtInsert->set_parameter(3, cur.vartype());
					stmtInsert->set_parameter(4, nz);
					stmtInsert->set_parameter(5, cur.name());
					stmtInsert->set_parameter(6, cur.description());
					stmtInsert->set_parameter(7, cur.genre());
					if (!stmtInsert->exec()) {
						this->rollback_transaction();
						return (false);
					}
				}
			} // it
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			return (true);
		} // maintains_variables
		template<class ALLOCVEC>
		bool get_dataset_variables_ids(int nDatasetId,
			std::vector<int, ALLOCVEC> &oVec) {
				assert(this->is_valid());
				oVec.clear();
				PStatement stmt = this->find_statement(SQL_FIND_DATASET_VARIABLES_IDS);
				if (!stmt->is_valid()) {
					return (false);
				}
				stmt->reset();
				stmt->set_parameter(1, nDatasetId);
				if (!stmt->exec()) {
					return (false);
				}
				while (stmt->has_values()) {
					{
						DbValue v;
						if (stmt->col_value(0, v)) {
							int nId = v.int_value();
							oVec.push_back(nId);
						}
					}
					if (!stmt->next()) {
						break;
					}
				} // values
				return (true);
		} // get_dataset_variables_ids
		template<class ALLOCVAR>
		bool get_dataset_variables(int nDatasetId,
			std::vector<VariableType, ALLOCVAR> &oVec) {
				assert(this->is_valid());
				oVec.clear();
				PStatement stmt = this->find_statement(SQL_FIND_DATASET_VARIABLES);
				if (!stmt->is_valid()) {
					return (false);
				}
				stmt->reset();
				stmt->set_parameter(1, nDatasetId);
				if (!stmt->exec()) {
					return (false);
				}
				while (stmt->has_values()) {
					VariableType cur;
					this->read_variable(stmt,cur);
					oVec.push_back(cur);
					if (!stmt->next()) {
						break;
					}
				} // values
				return (true);
		} // get_dataset_variables
		template <class COMPARE, class ALLOCPAIR>
		bool get_variable_values_pair_not_null(int nVarId,std::map<int,boost::any,COMPARE,ALLOCPAIR> &oMap, int skip, int taken){
			oMap.clear();
			if ((skip < 0) || (taken < 1)) {
				return (false);
			}
			assert(this->is_valid());
			StringType vartype;
			if (!this->get_variable_type(nVarId,vartype)){
				return (false);
			}
			PStatement pStmtFetch = this->find_statement(SQL_VALUES_PAIR_BY_VARIABLE_NOT_NULL);
			//
			pStmtFetch->reset();
			pStmtFetch->set_parameter(1, nVarId);
			pStmtFetch->set_parameter(2, taken);
			pStmtFetch->set_parameter(3, skip);
			if (!pStmtFetch->exec()) {
				return (false);
			}
			while (pStmtFetch->has_values()) {
				int nId = 0;
				boost::any vcur;
				{
					DbValue v;
					if (pStmtFetch->col_value(0, v)) {
						nId = v.int_value();
					}
				}
				{
					DbValue v;
					if (pStmtFetch->col_value(1, v)) {
						boost::any vx = v.value();
						ValueType x;
						x.value(vx);
						this->convert_value(x, vartype, vcur);
					}
				}
				if ((nId != 0) && (!vcur.empty())){
					oMap[nId] = vcur;
				}
				if (!pStmtFetch->next()){
					break;
				}
			} // values
			return (true);
		}// get_variables_values
		bool get_variable_values_pair_not_null_count(int nVarId,int &nCount) {
			assert(this->is_valid());
			nCount = 0;
			PStatement stmt = this->find_statement(SQL_VALUES_BY_VARIABLE_NOT_NULL_COUNT);
			if (!stmt->is_valid()) {
				return false;
			}
			stmt->reset();
			stmt->set_parameter(1, nVarId);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			DbValue v;
			if (stmt->col_value(0, v)) {
				nCount = v.int_value();
			}
			return (true);
		} // get_variable_values_pair_not_null_count
		template<class ALLOCVEC>
		bool get_all_datasets(std::vector<DatasetType, ALLOCVEC> &oVec) {
			assert(this->is_valid());
			oVec.clear();
			PStatement stmt = this->find_statement(SQL_FIND_ALL_DATASETS);
			if (!stmt->is_valid()) {
				return (false);
			}
			stmt->reset();
			if (!stmt->exec()) {
				return (false);
			}
			while (stmt->has_values()) {
				DatasetType cur;
				this->read_dataset(stmt, cur);
				oVec.push_back(cur);
				if (!stmt->next()) {
					break;
				}
			} // values
			return (true);
		} // get_all_datasets
	private:
		bool check_schema(void);
	private:
		static const char *SQL_BEGIN_TRANSACTION;
		static const char *SQL_COMMIT_TRANSACTION;
		static const char *SQL_ROLLBACK_TRANSACTION;
		////////////////////////////////////////
		static const char *SQL_CREATE_DATASET;
		static const char *SQL_CREATE_VARIABLE;
		static const char *SQL_CREATE_INDIV;
		static const char *SQL_CREATE_VALUE;
		static const char *CREATE_SQL[];
		/////////////////////////////////////
		static const char *SQL_FIND_ALL_DATASETS;
		static const char *SQL_FIND_DATASET_BY_ID;
		static const char *SQL_FIND_DATASET_BY_SIGLE;
		static const char *SQL_INSERT_DATASET;
		static const char *SQL_UPDATE_DATASET;
		static const char *SQL_REMOVE_DATASET;
		/////////////////////////////////////
		static const char *SQL_FIND_DATASET_VARIABLES;
		static const char *SQL_FIND_DATASET_VARIABLES_IDS;
		static const char *SQL_VARIABLE_BY_DATASET_AND_SIGLE;
		static const char *SQL_FIND_VARIABLE_BY_ID;
		static const char *SQL_INSERT_VARIABLE;
		static const char *SQL_UPDATE_VARIABLE;
		static const char *SQL_REMOVE_VARIABLE;
		static const char *SQL_REMOVE_DATASET_VARIABLES;
		////////////////////////////////////////////
		static const char *SQL_FIND_VARIABLE_TYPE;
		static const char *SQL_FIND_DATASET_INDIVS_COUNT;
		static const char *SQL_FIND_INDIVS_BY_DATASET_AND_STATUS_COUNT;
		static const char *SQL_FIND_INDIVS_BY_DATASET_AND_STATUS;
		static const char *SQL_FIND_DATASET_INDIVS;
		static const char *SQL_FIND_DATASET_INDIVS_PAGED;
		static const char *SQL_INDIV_BY_DATASET_AND_SIGLE;
		static const char *SQL_FIND_INDIV_BY_ID;
		static const char *SQL_INSERT_INDIV;
		static const char *SQL_UPDATE_INDIV;
		static const char *SQL_REMOVE_INDIV;
		static const char *SQL_REMOVE_DATASET_INDIVS;
		///////////////////////////////////////
		static const char *SQL_FIND_DATASET_VALUES_COUNT;
		static const char *SQL_FIND_DATASET_VALUES;
		static const char *SQL_FIND_DATASET_BY_STATUS_VALUES_COUNT;
		static const char *SQL_FIND_DATASET_BY_STATUS_VALUES;
		static const char *SQL_VALUES_BY_VARIABLE_INDIV;
		static const char *SQL_VALUES_BY_VARIABLE_NOT_NULL;
		static const char *SQL_VALUES_BY_VARIABLE_NOT_NULL_COUNT;
		static const char *SQL_VALUES_PAIR_BY_VARIABLE_NOT_NULL;
		static const char *SQL_INSERT_VALUE;
		static const char *SQL_UPDATE_VALUE;
		static const char *SQL_REMOVE_VALUE;
		static const char *SQL_REMOVE_VARIABLE_VALUES;
		static const char *SQL_REMOVE_INDIV_VALUES;
		static const char *SQL_VALUE_BY_ID;
		/////////////////////////////////////////
		static const char *SQL_CREATE_VIEW_VALUES;
		static const char *SQL_INDIVS_VIEW;
		static const char *SQL_VARS_VIEW;
		static const char *SQL_DATASETS_VIEW;
		/////////////////////////////////////////
		static const char *SQL_GET_DATASET_INDIV_IDS;
	}
	;

	///////////////////////////////////
	template<class TSTRING>
	template<class IFSTREAM>
	bool StatDataManager<TSTRING>::process_data(IFSTREAM &inFile,
		const typename StatDataManager<TSTRING>::StringType &datasetSigle,
		typename StatDataManager<TSTRING>::DatasetType &oSet,
		const typename StatDataManager<TSTRING>::Char &delim,
		const typename StatDataManager<TSTRING>::StringType &na) {
			if (!this->is_valid()) {
				return (false);
			}
			if (!inFile.is_open()) {
				return (false);
			}
			bool bRet = false;
			try {
				intra::ImportData<StringType> oData;
				oData.open(inFile, delim, na);
				size_t ncols = oData.cols();
				if (ncols < 1) {
					return (false);
				}
				//
				oSet.sigle(datasetSigle);
				bRet = this->fill_data(oData, oSet);
			} catch (...) {
				return (false);
			}
			return (bRet);
	} // process_data
	template<class TSTRING>
	template<class IFSTREAM>
	bool StatDataManager<TSTRING>::process_data(IFSTREAM &inFile,
		const typename StatDataManager<TSTRING>::StringType &datasetSigle,
		const typename  StatDataManager<TSTRING>::Char &delim,
		const  typename StatDataManager<TSTRING>::StringType &na) {
			DatasetType oSet;
			return this->process_data(inFile, datasetSigle, oSet, delim, na);
	} // process_data
	template<class TSTRING>
	bool StatDataManager<TSTRING>::fill_data(
		const intra::ImportData<TSTRING> &oData,
		typename StatDataManager<TSTRING>::DatasetType &oSet) {
			if (!this->is_valid()) {
				return (false);
			}
			bool bRet = false;
			int nId = oSet.id();
			if (nId != 0) {
				bRet = this->get_dataset_by_id(nId, oSet);
				nId = oSet.id();
				if (!bRet) {
					nId = 0;
				}
			}
			StringType sigle = oSet.sigle();
			if (nId == 0) {
				bRet = this->get_dataset_by_sigle(sigle, oSet);
				nId = oSet.id();
				if (!bRet) {
					nId = 0;
				}
			} // nId
			if (nId == 0) {
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
			} // nId
			nId = oSet.id();
			if (nId == 0) {
				return (false);
			}
			std::vector<VariableType> oVecv;
			if (!this->get_dataset_variables(nId, oVecv)) {
				return (false);
			}
			for (auto it = oVecv.begin(); it != oVecv.end(); ++it) {
				const VariableType & vv = *it;
				VariableType *pv = oSet.register_variable(vv.sigle());
				if (pv != nullptr) {
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
				const VariableType *pVar = oSet.find_variable(sigle);
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
			std::vector<IndivType> oInds;
			for (size_t i = 0; i < nrows; ++i) {
				sigle.clear();
				if (oData.get_string_value(i, nIndex, sigle)) {
					if (!sigle.empty()) {
						auto pInd = oSet.find_indiv(sigle);
						if (pInd == nullptr) {
							IndivType oInd;
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
			std::vector<IndivType> oVecx;
			if (!this->get_dataset_indivs(nId, oVecx)) {
				return (false);
			}
			std::map<size_t, int> oIndsMap;
			for (auto it = oVecx.begin(); it != oVecx.end(); ++it) {
				const IndivType &oInd = *it;
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
				std::vector<ValueType> oVec;
				std::vector<boost::any> vals;
				oData.get_col_values(icol, vals);
				for (size_t irow = 0; irow < nrows; ++irow) {
					boost::any v;
					if (oIndsMap.find(irow) == oIndsMap.end()) {
						continue;
					}
					int nIndivId = oIndsMap[irow];
					oData.get_value(irow, icol, v);
					ValueType cur;
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
			return bRet;
	} // fill_data
	template<class TSTRING>
	void StatDataManager<TSTRING>::get_indiv_sigle_name(
		typename StatDataManager<TSTRING>::StringType &s) const {
			std::string sx("NOM");
			StringType sz(sx.length(), ' ');
			std::copy(sx.begin(), sx.end(), sz.begin());
			s = sz;
	}
	template<class TSTRING>
	void StatDataManager<TSTRING>::get_indiv_status_name(
		typename StatDataManager<TSTRING>::StringType &s) const {
			std::string sx("STATUS");
			StringType sz(sx.length(), ' ');
			std::copy(sx.begin(), sx.end(), sz.begin());
			s = sz;
	}
	/////////////////////////////////////////
	template<class TSTRING>
	bool StatDataManager<TSTRING>::load_dataset(
		typename StatDataManager<TSTRING>::DatasetType &cur) {
			int nId = cur.id();
			bool bOk = false;
			if (nId != 0) {
				bOk = this->get_dataset_by_id(nId, cur);
			}
			if (!bOk) {
				const StringType sigle = cur.sigle();
				if (!this->get_dataset_by_sigle(sigle, cur)) {
					return (false);
				}
				nId = cur.id();
			}
			std::vector<VariableType> vecVars;
			if (!this->get_dataset_variables(nId, vecVars)) {
				return (false);
			}
			std::vector<IndivType> vecInds;
			if (!this->get_dataset_indivs(nId, vecInds)) {
				return (false);
			}
			cur.clear();
			std::for_each(vecInds.begin(), vecInds.end(), [&](const IndivType &v) {
				PIndiv oInd = std::make_shared<IndivType>(v);
				IndivType *p = oInd.get();
				if (p != nullptr) {
					p->is_changed(false);
					cur.indivs().push_back(oInd);
				}
			});
			std::for_each(vecVars.begin(), vecVars.end(), [&](const VariableType &v) {
				PVariable oVar = std::make_shared<VariableType>(v);
				VariableType *p = oVar.get();
				if (p != nullptr) {
					p->is_changed(false);
					cur.variables().push_back(oVar);
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
				std::vector<ValueType> vecVals;
				if (!this->get_dataset_values(nId, vecVals, skip, taken)) {
					return (false);
				}
				if (vecVals.empty()) {
					break;
				}
				std::for_each(vecVals.begin(), vecVals.end(), [&](const ValueType &val) {
					if (!val.value().empty()) {
						ValueType *p = cur.register_value(val);
						if (p != nullptr) {
							p->is_changed(false);
						}
					} // not_empty
				});
				skip += taken;
			} // while skip
			return (true);
	} // load_dataset
	template<class TSTRING>
	bool StatDataManager<TSTRING>::remove_dataset(
		const  typename  StatDataManager<TSTRING>::DatasetType &cur) {
			assert(this->is_valid());
			int nDatasetId = cur.id();
			if (nDatasetId == 0){
				return (false);
			}
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement pStmtFindVars = this->find_statement(SQL_FIND_DATASET_VARIABLES_IDS);
			PStatement pStmtDeleteVals = this->find_statement(SQL_REMOVE_VARIABLE_VALUES);
			PStatement pStmtDeleteVar = this->find_statement(SQL_REMOVE_VARIABLE);
			PStatement pStmtRemoveIndivs = this->find_statement(SQL_REMOVE_DATASET_INDIVS);
			PStatement stmt = this->find_statement(SQL_REMOVE_DATASET);

			if ((!stmt->is_valid()) || (!pStmtFindVars->is_valid()) || (!pStmtDeleteVals->is_valid()) ||
				(!pStmtDeleteVar->is_valid()) || (!pStmtRemoveIndivs->is_valid()) ){
					this->rollback_transaction();
					return (false);
			}
			pStmtFindVars->reset();
			pStmtFindVars->set_parameter(1,nDatasetId);
			if (pStmtFindVars->exec()){
				while (pStmtFindVars->has_values()){
					DbValue v;
					int nVarId = 0;
					if (stmt->col_value(0, v)) {
						nVarId = v.int_value();
					}
					if (nVarId != 0){
						pStmtDeleteVals->reset();
						pStmtDeleteVals->set_parameter(1,nVarId);
						pStmtDeleteVals->exec();
						pStmtDeleteVar->reset();
						pStmtDeleteVar->set_parameter(1,nVarId);
						pStmtDeleteVar->exec();
					}// nVarId
					if (!pStmtFindVars->next()){
						break;
					}
				} // var
			}// vars
			pStmtRemoveIndivs->reset();
			pStmtRemoveIndivs->set_parameter(1,nDatasetId);
			pStmtRemoveIndivs->exec();
			stmt->reset();
			stmt->set_parameter(1, nDatasetId);
			stmt->exec();
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			return (true);
	} // update_dataset
	template<class TSTRING>
	bool StatDataManager<TSTRING>::update_dataset(
		const  typename  StatDataManager<TSTRING>::DatasetType &cur) {
			assert(this->is_valid());
			if (!cur.is_updateable()) {
				return (false);
			}
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement stmt = this->find_statement(SQL_UPDATE_DATASET);
			if (!stmt->is_valid()) {
				this->rollback_transaction();
				return (false);
			}
			stmt->reset();
			if (!stmt->set_parameter(1, cur.sigle())) {
				this->rollback_transaction();
				return (false);
			}
			if (!stmt->set_parameter(2, cur.name())) {
				this->rollback_transaction();
				return (false);
			}
			if (!stmt->set_parameter(3, cur.description())) {
				this->rollback_transaction();
				return (false);
			}
			if (!stmt->set_parameter(4, cur.id())) {
				this->rollback_transaction();
				return (false);
			}
			if (!stmt->exec()) {
				this->rollback_transaction();
				return (false);
			}
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			return (true);
	} // update_dataset
	template<class TSTRING>
	bool StatDataManager<TSTRING>::insert_dataset(
		const  typename  StatDataManager<TSTRING>::DatasetType &cur) {
			assert(this->is_valid());
			if (!cur.is_writeable()) {
				return (false);
			}
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement stmt = this->find_statement(SQL_INSERT_DATASET);
			if (!stmt->is_valid()) {
				this->rollback_transaction();
				return (false);
			}
			stmt->reset();
			StringType sigle = cur.sigle();
			if (!stmt->set_parameter(1, cur.sigle())) {
				this->rollback_transaction();
				return (false);
			}
			if (!stmt->set_parameter(2, cur.name())) {
				this->rollback_transaction();
				return (false);
			}
			if (!stmt->set_parameter(3, cur.description())) {
				this->rollback_transaction();
				return (false);
			}
			if (!stmt->exec()) {
				this->rollback_transaction();
				return (false);
			}
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			DatasetType xcur;
			if (!this->get_dataset_by_sigle(sigle, xcur)) {
				return (false);
			}
			int nId = xcur.id();
			assert(nId != 0);
			bool bRet = this->check_pred_dataset(nId);
			return (bRet);
	} // insert_dataset
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_dataset_by_sigle(
		const  typename  StatDataManager<TSTRING>::StringType &xSigle,
		typename StatDataManager<TSTRING>::DatasetType &cur) {
			assert(this->is_valid());
			PStatement stmt = this->find_statement(SQL_FIND_DATASET_BY_SIGLE);
			if (!stmt->is_valid()) {
				return (false);
			}
			stmt->reset();
			StringType sx = to_upper(trim(xSigle));
			if (!stmt->set_parameter(1, sx.c_str())) {
				return (false);
			}
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			this->read_dataset(stmt,cur);
			return (true);
	} // get_dataset_by_sigle
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_dataset_by_id(int xId,
		typename StatDataManager<TSTRING>::DatasetType &cur) {
			assert(this->is_valid());
			PStatement stmt = this->find_statement(SQL_FIND_DATASET_BY_ID);
			if (!stmt->is_valid()) {
				return (false);
			}
			stmt->reset();
			if (!stmt->set_parameter(1, xId)) {
				return (false);
			}
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			this->read_dataset(stmt, cur);
			return (true);
	} // get_dataset_by_id
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_variable_by_dataset_and_sigle(int nDatasetId,
		const  typename  StatDataManager<TSTRING>::StringType &xSigle,
		typename StatDataManager<TSTRING>::VariableType &cur) {
			assert(this->is_valid());
			PStatement stmt = this->find_statement(SQL_VARIABLE_BY_DATASET_AND_SIGLE);
			if (!stmt->is_valid()) {
				return (false);
			}
			stmt->reset();
			StringType sx = to_upper(trim(xSigle));
			stmt->set_parameter(1, nDatasetId);
			stmt->set_parameter(2, sx);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			this->read_variable(stmt, cur);
			return (true);
	} // get_variable_by_dataset_and_sigle
	template<class TSTRING>
	bool StatDataManager<TSTRING>::check_variable(int nDatasetId,
		const  typename  StatDataManager<TSTRING>::StringType &sigle,
		const  typename  StatDataManager<TSTRING>::StringType &type, bool bCateg,
		const  typename StatDataManager<TSTRING>::StringType &genre) {
			assert(this->is_valid());
			{
				PStatement stmt = this->find_statement(SQL_VARIABLE_BY_DATASET_AND_SIGLE);
				if (!stmt->is_valid()) {
					return (false);
				}
				stmt->reset();
				StringType sx = to_upper(trim(sigle));
				stmt->set_parameter(1, nDatasetId);
				stmt->set_parameter(2, sx);
				if (!stmt->exec()) {
					return (false);
				}
				if (stmt->has_values()) {
					return (true);
				}
			}
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement stmtInsert = this->find_statement(SQL_INSERT_VARIABLE);
			if (!stmtInsert->is_valid()) {
				return (false);
			}
			stmtInsert->reset();
			int nz = (bCateg) ? 1 : 0;
			StringType sSigle = trim(sigle);
			StringType sType = trim(type);
			StringType sGenre = trim(genre);
			stmtInsert->set_parameter(1, nDatasetId);
			stmtInsert->set_parameter(2, sSigle);
			stmtInsert->set_parameter(3, sType);
			stmtInsert->set_parameter(4, nz);
			stmtInsert->set_parameter(5, sSigle);
			stmtInsert->set_parameter(6, sSigle);
			stmtInsert->set_parameter(7, sGenre);
			if (!stmtInsert->exec()) {
				this->rollback_transaction();
				return (false);
			}
			if (!this->commit_transaction()) {
				this->rollback_transaction();
				return (false);
			}
			return (true);
	} // check_variable
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_dataset_indivs_by_status_count(int nId,
		const  typename  StatDataManager<TSTRING>::StringType &status, int &nCount) {
			assert(this->is_valid());
			nCount = 0;
			PStatement stmt = this->find_statement(SQL_FIND_INDIVS_BY_DATASET_AND_STATUS_COUNT);
			if (!stmt->is_valid()) {
				return false;
			}
			stmt->reset();
			StringType sStatus = to_upper(trim(status));
			stmt->set_parameter(1, nId);
			stmt->set_parameter(2, sStatus);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			DbValue v;
			if (stmt->col_value(0, v)) {
				nCount = v.int_value();
			}
			return (true);
	} // get_dataset_indivs_count
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_indiv_by_dataset_sigle(int nDatasetId,
		const  typename StatDataManager<TSTRING>::StringType &xSigle,
		typename StatDataManager<TSTRING>::IndivType &cur) {
			assert(this->is_valid());
			PStatement stmt = this->find_statement(SQL_INDIV_BY_DATASET_AND_SIGLE);
			if (!stmt->is_valid()) {
				return (false);
			}
			stmt->reset();
			StringType sigle = to_upper(trim(xSigle));
			stmt->set_parameter(1, nDatasetId);
			stmt->set_parameter(2, sigle);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			this->read_indiv(stmt, cur);
			return (true);
	} // get_indiv_by_dataset_sigle
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_dataset_by_status_values_count(int nId,
		const  typename  StatDataManager<TSTRING>::StringType &status, int &nCount) {
			assert(this->is_valid());
			nCount = 0;
			PStatement stmt = this->SQL_FIND_ALL_DATASETS(SQL_FIND_DATASET_BY_STATUS_VALUES_COUNT);
			if (!stmt->is_valid()) {
				return false;
			}
			stmt->reset();
			StringType sStatus = to_upper(trim(status));
			stmt->set_parameter(1, nId);
			stmt->set_parameter(2, sStatus);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			DbValue v;
			if (stmt->col_value(0, v)) {
				nCount = v.int_value();
			}
			return (true);
	} // get_dataset_by_status_values_count
	/////////////////////////////////////
	template<class TSTRING>
	StatDataManager<TSTRING>::StatDataManager() :
		m_intransaction(false) {
	} // StatDataManager
	template<class TSTRING>
	bool StatDataManager<TSTRING>::begin_transaction(void) {
		if (this->m_intransaction) {
			return (true);
		}
		assert(this->is_valid());
		PStatement stmt = this->find_statement(SQL_BEGIN_TRANSACTION);
		if (!stmt->is_valid()) {
			return (false);
		}
		if (stmt->exec()) {
			this->m_intransaction = true;
			return (true);
		}
		return (false);
	} // begin_transaction
	template<class TSTRING>
	bool StatDataManager<TSTRING>::commit_transaction(void) {
		if (!this->m_intransaction) {
			return (false);
		}
		assert(this->is_valid());
		PStatement stmt = this->find_statement(SQL_COMMIT_TRANSACTION);
		if (!stmt->is_valid()) {
			return (false);
		}
		if (stmt->exec()) {
			this->m_intransaction = false;
			return (true);
		}
		return (false);
	} // commit_transaction
	template<class TSTRING>
	bool StatDataManager<TSTRING>::rollback_transaction(void) {
		if (!this->m_intransaction) {
			return (false);
		}
		assert(this->is_valid());
		PStatement stmt = this->find_statement(SQL_ROLLBACK_TRANSACTION);
		if (!stmt->is_valid()) {
			return (false);
		}
		if (stmt->exec()) {
			this->m_intransaction = false;
			return (true);
		}
		return (false);
	} // rollback_transaction
	template<class TSTRING>
	bool StatDataManager<TSTRING>::close(void) {
		bool bRet = true;
		Database *pbase = this->m_database.get();
		if (pbase != nullptr) {
			this->reset();
			if (pbase->close()) {
				this->m_database.reset();
			} else {
				bRet = false;
			}
		}
		return (bRet);
	} // close
	template<class TSTRING>
	StatDataManager<TSTRING>::~StatDataManager() {
		this->reset();
		this->m_database.reset();
	}
	template<class TSTRING>
	bool StatDataManager<TSTRING>::open(
		const  typename  StatDataManager<TSTRING>::StringType &filename) {
			this->m_database.reset(new sqlite::Database());
			sqlite::Database *pBase = this->m_database.get();
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
	template<class TSTRING>
	bool StatDataManager<TSTRING>::is_valid(void) const {
		sqlite::Database *pbase = this->m_database.get();
		return ((pbase != nullptr) && pbase->is_open());
	} // is_valid
	template<class TSTRING>
	bool StatDataManager<TSTRING>::check_schema(void) {
		sqlite::Database *pbase = this->m_database.get();
		assert(pbase != nullptr);
		assert(pbase->is_open());
		int i = 0;
		while (StatDataManager<TSTRING>::CREATE_SQL[i] != nullptr) {
			const char *pszSQL = StatDataManager<TSTRING>::CREATE_SQL[i];
			if (!pbase->exec_sql(pszSQL)) {
				return (false);
			}
			++i;
		} // ok
		return (true);
	} // check_schema
	template<class TSTRING>
	void StatDataManager<TSTRING>::init_tables(std::vector<TableDef> &oVec) const {
		oVec.clear();
	} // init_tables
	template<class TSTRING>
	bool StatDataManager<TSTRING>::check_variable_values(int nDatasetId,
		int nVarId) {
			std::vector<int> oInds;
			if (!this->get_dataset_indivs_ids(nDatasetId, oInds)) {
				return (false);
			}
			if (oInds.empty()) {
				return (true);
			}
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement stmtFetch = this->find_statement(SQL_VALUES_BY_VARIABLE_INDIV);
			PStatement stmtInsert = this->find_statement(SQL_INSERT_VALUE);
			if ((!stmtFetch->is_valid()) || (!stmtInsert->is_valid())) {
				return (false);
			}
			for (auto jt = oInds.begin(); jt != oInds.end(); ++jt) {
				int nIndivId = *jt;
				stmtFetch->reset();
				stmtFetch->set_parameter(1, nVarId);
				stmtFetch->set_parameter(2, nIndivId);
				if (!stmtFetch->exec()) {
					return (false);
				}
				if (stmtFetch->has_values()) {
					continue;
				}
				stmtInsert->reset();
				stmtInsert->set_parameter(1, nVarId);
				stmtInsert.set_parameter(2, nIndivId);
				stmtInsert->set_parameter_null(3);
				if (!stmtInsert->exec()) {
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
	template<class TSTRING>
	bool StatDataManager<TSTRING>::check_indiv_values(int nDatasetId,
		int nIndivId) {
			std::vector<int> oVars;
			if (!this->get_dataset_variables_ids(nDatasetId, oVars)) {
				return (false);
			}
			if (oVars.empty()) {
				return (true);
			}
			if (!this->begin_transaction()) {
				return (false);
			}
			PStatement stmtFetch = this->find_statement(SQL_VALUES_BY_VARIABLE_INDIV);
			PStatement stmtInsert = this->find_statement(QL_INSERT_VALUE);
			if ((!stmtFetch->is_valid()) || (!stmtInsert->is_valid())) {
				return (false);
			}
			for (auto jt = oVars.begin(); jt != oVars.end(); ++jt) {
				int nVarId = *jt;
				stmtFetch->reset();
				stmtFetch->set_parameter(1, nVarId);
				stmtFetch->set_parameter(2, nIndivId);
				if (!stmtFetch->exec()) {
					return (false);
				}
				if (stmtFetch->has_values()) {
					continue;
				}
				stmtInsert->reset();
				stmtInsert->set_parameter(1, nVarId);
				stmtInsert->set_parameter(2, nIndivId);
				stmtInsert->set_parameter_null(3);
				if (!stmtInsert->exec()) {
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
	template<class TSTRING>
	bool StatDataManager<TSTRING>::check_values(int nDatasetId) {
		std::vector<int> oInds;
		if (!this->get_dataset_indivs_ids(nDatasetId, oInds)) {
			return (false);
		}
		if (oInds.empty()) {
			return (true);
		}
		std::vector<int> oVars;
		if (!this->get_dataset_variables_ids(nDatasetId, oVars)) {
			return (false);
		}
		if (!this->begin_transaction()) {
			return (false);
		}
		PStatement stmtFetch = this->find_statement(SQL_VALUES_BY_VARIABLE_INDIV);
		PStatement stmtInsert = this->find_statement(SQL_INSERT_VALUE);
		if ((!stmtFetch->is_valid()) || (!stmtInsert->is_valid())) {
			return (false);
		}
		for (auto it = oVars.begin(); it != oVars.end(); ++it) {
			int nVarId = *it;
			for (auto jt = oInds.begin(); jt != oInds.end(); ++jt) {
				int nIndivId = *jt;
				stmtFetch->reset();
				stmtFetch->set_parameter(1, nVarId);
				stmtFetch->set_parameter(2, nIndivId);
				if (!stmtFetch->exec()) {
					return (false);
				}
				if (stmtFetch->has_values()) {
					continue;
				}
				stmtInsert->reset();
				stmtInsert->set_parameter(1, nVarId);
				stmtInsert->set_parameter(2, nIndivId);
				stmtInsert->set_parameter_null(3);
				if (!stmtInsert->exec()) {
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
	template<class TSTRING>
	bool StatDataManager<TSTRING>::check_pred_dataset(int nDatasetId) {
		std::vector<TableDef> oDefs;
		this->init_tables(oDefs);
		if (oDefs.empty()) {
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
	template<class TSTRING>
	void StatDataManager<TSTRING>::convert_value(
		const  typename  StatDataManager<TSTRING>::ValueType &s,
		const  typename  StatDataManager<TSTRING>::StringType &vartype, boost::any &v) {
			StringType sz = to_lower(trim(vartype));
			std::string sx(sz.length(), ' ');
			std::copy(sz.begin(), sz.end(), sx.begin());
			if (sx == "bool") {
				v = boost::any(s.bool_value());
			} else if (sx == "short") {
				v = boost::any(s.short_value());
			} else if (sx == "int") {
				v = boost::any(s.int_value());
			} else if (sx == "float") {
				v = boost::any(s.float_value());
			} else if (sx == "double") {
				v = boost::any(s.double_value());
			} else if (sx == "string") {
				std::string ss;
				s.string_value(ss);
				v = boost::any(ss);
			}
	} //convert_value
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_value_by_variable_indiv(int nVarId,
		int nIndivId, intra::Value &cur) {
			assert(this->is_valid());
			PStatement stmt = this->find_statement(SQL_VALUES_BY_VARIABLE_INDIV);
			if (!stmt->is_valid()) {
				return (false);
			}
			stmt->reset();
			stmt->set_parameter(1, nVarId);
			stmt->set_parameter(2, nIndivId);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			this->read_value(stmt,cur);
			return (true);
	} // get_dataset_indivs
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_dataset_values_count(int nId, int &nCount) {
		assert(this->is_valid());
		nCount = 0;
		PStatement stmt = this->find_statement(SQL_FIND_DATASET_VALUES_COUNT);
		if (!stmt->is_valid()) {
			return false;
		}
		stmt->reset();
		stmt->set_parameter(1, nId);
		if (!stmt->exec()) {
			return (false);
		}
		if (!stmt->has_values()) {
			return (false);
		}
		DbValue v;
		if (stmt->col_value(0, v)) {
			nCount = v.int_value();
		}
		return (true);
	} // get_dataset_values_count
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_variable_values_count(int nVarId,
		int &nCount) {
			assert(this->is_valid());
			nCount = 0;
			PStatement stmt = this->find_statement(SQL_VALUES_BY_VARIABLE_NOT_NULL_COUNT);
			if (stmt->is_valid()) {
				return false;
			}
			stmt->reset();
			stmt->set_parameter(1, nVarId);
			if (!stmt->exec()) {
				return (false);
			}
			if (!stmt->has_values()) {
				return (false);
			}
			sqlite::DbValue v;
			if (stmt->col_value(0, v)) {
				nCount = v.int_value();
			}
			return (true);
	}
	template<class TSTRING>
	bool StatDataManager<TSTRING>::get_dataset_indivs_count(int nId, int &nCount) {
		assert(this->is_valid());
		nCount = 0;
		PStatement stmt = this->find_statement(SQL_FIND_DATASET_INDIVS_COUNT);
		if (!stmt->is_valid()) {
			return false;
		}
		stmt->reset();
		stmt->set_parameter(1, nId);
		if (!stmt->exec()) {
			return (false);
		}
		if (!stmt->has_values()) {
			return (false);
		}
		DbValue v;
		if (stmt->col_value(0, v)) {
			nCount = v.int_value();
		}
		return (true);
	} // get_dataset_indivs_count

	///////////////////////////////////////
#include "sqltext.h"
	////////////////////////
} /* namespace intrasqlite */
#endif /* STATDATAMANAGER_H_ */
