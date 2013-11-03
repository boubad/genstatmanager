/*
 * statdatamanager.h
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

#ifndef STATDATAMANAGER_H_
#define STATDATAMANAGER_H_
////////////////////////////////////
#ifdef _MSC_VER
#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif
#endif // _MSC_VER/////////////////////////////////#include <memory>#include <string>
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
private:
	// no implementation
	StatDataManager(const StatDataManager<TSTRING> &other);
	StatDataManager<TSTRING> & operator=(const StatDataManager<TSTRING> &other);
private:
	bool m_intransaction;
	std::unique_ptr<sqlite::Database> m_database;
protected:
	virtual void init_tables(std::vector<TableDef> &oVec) const;
	virtual void get_indiv_sigle_name(StringType &s) const;
	virtual void get_indiv_status_name(StringType &s) const;
	bool begin_transaction(void);
	bool commit_transaction(void);
	bool rollback_transaction(void);
	bool check_pred_dataset(int nDatasetId);
	void convert_value(const ValueType &s, const StringType &vartype,
			boost::any &v);
public:
	StatDataManager();
	virtual ~StatDataManager();
public:
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
	bool get_variable_by_dataset_and_sigle(int nDatasetId,
			const StringType &xSigle, VariableType &cur);
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
public:
	template<class ALLOCVAL>
	bool remove_values(const std::vector<ValueType, ALLOCVAL> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_REMOVE_VALUE);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const ValueType &cur = *it;
			int nId = cur.id();
			if (nId != 0) {
				stmt.reset();
				stmt.set_parameter(1, nId);
				if (!stmt.exec()) {
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
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmtFetch(pBase, SQL_VALUES_BY_VARIABLE_INDIV);
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_VALUE);
		sqlite::Statement stmtUpdate(pBase, SQL_UPDATE_VALUE);
		sqlite::Statement stmtFirst(pBase, SQL_VALUE_BY_ID);
		if ((!stmtFetch.is_valid())
				|| (!stmtUpdate.is_valid() || (!stmtFirst.is_valid()))
				|| (!stmtInsert.is_valid())) {
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
				stmtFirst.reset();
				stmtFirst.set_parameter(1, nId);
				if (!stmtFirst.exec()) {
					this->rollback_transaction();
					return (false);
				}
				if (stmtFirst.has_values()) {
					stmtUpdate.reset();
					if (bNull) {
						stmtUpdate.set_parameter_null(1);
					} else {
						stmtUpdate.set_parameter(1, sval);
					}
					stmtUpdate.set_parameter(2, nId);
					if (!stmtUpdate.exec()) {
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
			stmtFetch.reset();
			stmtFetch.set_parameter(1, nVarId);
			stmtFetch.set_parameter(2, nIndivId);
			if (!stmtFetch.exec()) {
				return (false);
			}
			if (stmtFetch.has_values()) {
				nId = 0;
				sqlite::DbValue v;
				if (stmtFetch.col_value(0, v)) {
					nId = v.int_value();
				}
				if (nId != 0) {
					stmtUpdate.reset();
					if (bNull) {
						stmtUpdate.set_parameter_null(1);
					} else {
						stmtUpdate.set_parameter(1, sval);
					}
					stmtUpdate.set_parameter(2, nId);
					if (!stmtUpdate.exec()) {
						this->rollback_transaction();
						return (false);
					}
				} // nId
				continue;
			} else {
				stmtInsert.reset();
				stmtInsert.set_parameter(1, nVarId);
				stmtInsert.set_parameter(2, nIndivId);
				if (bNull) {
					stmtInsert.set_parameter_null(3);
				} else {
					stmtInsert.set_parameter(3, sval);
				}
				if (!stmtInsert.exec()) {
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
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_VALUES_BY_VARIABLE_NOT_NULL);
		sqlite::Statement stmtVarType(pBase, SQL_FIND_VARIABLE_TYPE);
		StringType vartype;
		if ((!stmt.is_valid()) || (!stmtVarType.is_valid())) {
			return (false);
		}
		stmtVarType.set_parameter(1, nVarId);
		if (!stmtVarType.exec()) {
			return (false);
		}
		if (!stmtVarType.has_values()) {
			return (false);
		}
		sqlite::DbValue vv;
		stmtVarType.col_value(0, vv);
		vv.string_value(vartype);
		//
		stmt.set_parameter(1, nVarId);
		stmt.set_parameter(2, taken);
		stmt.set_parameter(3, skip);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			ValueType cur;
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
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
		return (true);
	} // get_variable_values
	template<class ALLOCVEC>
	bool get_dataset_values(int nDatasetId,
			std::vector<ValueType, ALLOCVEC> &oVec, int skip, int taken) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_VALUES);
		sqlite::Statement stmtVarType(pBase, SQL_FIND_VARIABLE_TYPE);
		std::map<int, std::string> oMap;
		if ((!stmt.is_valid()) || (!stmtVarType.is_valid())) {
			return (false);
		}
		stmt.set_parameter(1, nDatasetId);
		stmt.set_parameter(2, taken);
		stmt.set_parameter(3, skip);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			ValueType cur;
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
					if (oMap.find(nx) != oMap.end()) {
						vartype = oMap[nx];
					} else {
						stmtVarType.reset();
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
						if (!vartype.empty()) {
							oMap[nx] = vartype;
						}
					}
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
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
		return (true);
	} // get_dataset_indivs
	template<class ALLOCVEC>
	bool get_dataset_by_status_values(int nDatasetId, const StringType &status,
			std::vector<ValueType, ALLOCVEC> &oVec, int skip, int taken) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_STATUS_VALUES);
		sqlite::Statement stmtVarType(pBase, SQL_FIND_VARIABLE_TYPE);
		std::map<int, std::string> oMap;
		if ((!stmt.is_valid()) || (!stmtVarType.is_valid())) {
			return (false);
		}
		StringType sStatus = boost::to_upper_copy(boost::trim_copy(status));
		stmt.set_parameter(1, nDatasetId);
		stmt.set_parameter(2, sStatus);
		stmt.set_parameter(3, taken);
		stmt.set_parameter(4, skip);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			ValueType cur;
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
					if (oMap.find(nx) != oMap.end()) {
						vartype = oMap[nx];
					} else {
						stmtVarType.reset();
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
						if (!vartype.empty()) {
							oMap[nx] = vartype;
						}
					}
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
					intra::Value x(vx);
					boost::any vy;
					this->convert_value(x, vartype, vy);
					cur.value(vy);
				}
			}
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
		return (true);
	} // get_dataset_by_status_values
	template<class ALLOCIND>
	bool remove_indivs(const std::vector<IndivType, ALLOCIND> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_REMOVE_INDIV);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const IndivType &cur = *it;
			int nId = cur.id();
			if (nId != 0) {
				stmt.reset();
				stmt.set_parameter(1, nId);
				if (!stmt.exec()) {
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
	} // remove_indivs
	template<class ALLOCIND>
	bool maintains_indivs(const std::vector<IndivType, ALLOCIND> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmtFetch1(pBase, SQL_FIND_INDIV_BY_ID);
		sqlite::Statement stmtFetch2(pBase, SQL_INDIV_BY_DATASET_AND_SIGLE);
		if ((!stmtFetch1.is_valid()) || (!stmtFetch2.is_valid())) {
			this->rollback_transaction();
			return (false);
		}
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_INDIV);
		sqlite::Statement stmtUpdate(pBase, SQL_UPDATE_INDIV);
		if ((!stmtUpdate.is_valid()) || (!stmtInsert.is_valid())) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const IndivType &cur = *it;
			int nId = cur.id();
			int nDatasetId = cur.dataset_id();
			StringType sigle = cur.sigle();
			if (nId != 0) {
				stmtFetch1.reset();
				stmtFetch1.set_parameter(1, nId);
				if (!stmtFetch1.exec()) {
					this->rollback_transaction();
					return (false);
				}
				if (!stmtFetch1.has_values()) {
					nId = 0;
				}
			} // nId
			if ((nId == 0) && (nDatasetId != 0)) {
				StringType ss = boost::to_upper_copy(boost::trim_copy(sigle));
				stmtFetch2.reset();
				stmtFetch2.set_parameter(1, nDatasetId);
				stmtFetch2.set_parameter(2, ss);
				if (!stmtFetch2.exec()) {
					this->rollback_transaction();
					return (false);
				}
				if (stmtFetch2.has_values()) {
					sqlite::DbValue v;
					if (stmtFetch2.col_value(0, v)) {
						nId = v.int_value();
					}
				}
			}
			if ((!sigle.empty()) && (nId != 0)) {
				stmtUpdate.reset();
				stmtUpdate.set_parameter(1, sigle);
				stmtUpdate.set_parameter(2, cur.name());
				stmtUpdate.set_parameter(3, cur.description());
				stmtUpdate.set_parameter(4, cur.status());
				stmtUpdate.set_parameter(5, nId);
				if (!stmtUpdate.exec()) {
					this->rollback_transaction();
					return (false);
				}
			} else if (cur.is_writeable() && (nDatasetId != 0)) {
				stmtInsert.reset();
				stmtInsert.set_parameter(1, nDatasetId);
				stmtInsert.set_parameter(2, sigle);
				stmtInsert.set_parameter(3, cur.name());
				stmtInsert.set_parameter(4, cur.description());
				stmtInsert.set_parameter(5, cur.status());
				if (!stmtInsert.exec()) {
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
	bool get_dataset_by_status_indivs(int nDatasetId,
			std::vector<IndivType, ALLOCVEC> &oVec, const StringType &status,
			int skip, int taken) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_INDIVS_BY_DATASET_AND_STATUS);
		if (!stmt.is_valid()) {
			return (false);
		}
		StringType sStatus = boost::to_upper_copy(boost::trim_copy(status));
		stmt.set_parameter(1, nDatasetId);
		stmt.set_parameter(2, sStatus);
		stmt.set_parameter(3, taken);
		stmt.set_parameter(4, skip);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			IndivType cur;
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
					StringType s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(5, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(6, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.status(s);
					}
				}
			}
			oVec.push_back(cur);
			if (!stmt.next()) {
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
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_INDIVS);
		if (!stmt.is_valid()) {
			return (false);
		}
		stmt.set_parameter(1, nDatasetId);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			IndivType cur;
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
					StringType s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(5, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(6, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.status(s);
					}
				}
			}
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
		return (true);
	} // get_dataset_indivs
	template<class ALLOCINT>
	bool get_dataset_indivs_ids(int nDatasetId,
			std::vector<int, ALLOCINT> &oInds) {
		assert(this->is_valid());
		oInds.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_GET_DATASET_INDIV_IDS);
		if (!stmt.is_valid()) {
			return false;
		}
		stmt.set_parameter(1, nDatasetId);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			sqlite::DbValue v;
			if (stmt.col_value(0, v)) {
				int nId = v.int_value();
				oInds.push_back(nId);
			}
			if (!stmt.next()) {
				break;
			}
		} // while
		return (true);
	} // get_dataset_indivs_ids
	template<class ALLOCVAR>
	bool remove_variables(const std::vector<VariableType, ALLOCVAR> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_REMOVE_VARIABLE);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const VariableType &cur = *it;
			int nId = cur.id();
			if (nId != 0) {
				stmt.reset();
				stmt.set_parameter(1, nId);
				if (!stmt.exec()) {
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
	} // remove_variables
	template<class ALLOCVAR>
	bool maintains_variables(const std::vector<VariableType, ALLOCVAR> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmtFetch1(pBase, SQL_FIND_VARIABLE_BY_ID);
		sqlite::Statement stmtFetch2(pBase, SQL_VARIABLE_BY_DATASET_AND_SIGLE);
		if ((!stmtFetch1.is_valid()) || (!stmtFetch2.is_valid())) {
			this->rollback_transaction();
			return (false);
		}
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_VARIABLE);
		sqlite::Statement stmtUpdate(pBase, SQL_UPDATE_VARIABLE);
		if ((!stmtUpdate.is_valid()) || (!stmtInsert.is_valid())) {
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
				stmtFetch1.reset();
				stmtFetch1.set_parameter(1, nId);
				if (!stmtFetch1.exec()) {
					this->rollback_transaction();
					return (false);
				}
				if (!stmtFetch1.has_values()) {
					nId = 0;
				}
			} // nId
			if ((nId == 0) && (nDatasetId != 0)) {
				StringType ss = boost::to_upper_copy(boost::trim_copy(sigle));
				stmtFetch2.reset();
				stmtFetch2.set_parameter(1, nDatasetId);
				stmtFetch2.set_parameter(2, ss);
				if (!stmtFetch2.exec()) {
					this->rollback_transaction();
					return (false);
				}
				if (stmtFetch2.has_values()) {
					sqlite::DbValue v;
					if (stmtFetch2.col_value(0, v)) {
						nId = v.int_value();
					}
				}
			}
			if ((nId != 0) && (!sigle.empty())) {
				stmtUpdate.reset();
				stmtUpdate.set_parameter(1, sigle);
				stmtUpdate.set_parameter(2, cur.vartype());
				stmtUpdate.set_parameter(3, nz);
				stmtUpdate.set_parameter(4, cur.name());
				stmtUpdate.set_parameter(5, cur.description());
				stmtUpdate.set_parameter(6, cur.genre());
				stmtUpdate.set_parameter(7, nId);
				if (!stmtUpdate.exec()) {
					this->rollback_transaction();
					return (false);
				}
			} else if ((nDatasetId != 0) && (!sigle.empty())) {
				stmtInsert.reset();
				stmtInsert.set_parameter(1, nDatasetId);
				stmtInsert.set_parameter(2, sigle);
				stmtInsert.set_parameter(3, cur.vartype());
				stmtInsert.set_parameter(4, nz);
				stmtInsert.set_parameter(5, cur.name());
				stmtInsert.set_parameter(6, cur.description());
				stmtInsert.set_parameter(7, cur.genre());
				if (!stmtInsert.exec()) {
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
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_VARIABLES_IDS);
		if (!stmt.is_valid()) {
			return (false);
		}
		stmt.set_parameter(1, nDatasetId);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			{
				sqlite::DbValue v;
				if (stmt.col_value(0, v)) {
					int nId = v.int_value();
					oVec.push_back(nId);
				}
			}
			if (!stmt.next()) {
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
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_VARIABLES);
		if (!stmt.is_valid()) {
			return (false);
		}
		stmt.set_parameter(1, nDatasetId);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			VariableType cur;
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
					StringType s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					StringType s;
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
					StringType s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(7, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(8, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.genre(s);
					}
				}
			}
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
		return (true);
	} // get_dataset_variables
	template<class ALLOCVEC>
	bool get_all_datasets(std::vector<DatasetType, ALLOCVEC> &oVec) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_ALL_DATASETS);
		if (!stmt.is_valid()) {
			return (false);
		}
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			DatasetType cur;
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
					StringType s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					StringType s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			oVec.push_back(cur);
			if (!stmt.next()) {
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
	////////////////////////////////////////////
	static const char *SQL_FIND_VARIABLE_TYPE;
	static const char *SQL_FIND_DATASET_INDIVS_COUNT;
	static const char *SQL_FIND_INDIVS_BY_DATASET_AND_STATUS_COUNT;
	static const char *SQL_FIND_INDIVS_BY_DATASET_AND_STATUS;
	static const char *SQL_FIND_DATASET_INDIVS;
	static const char *SQL_INDIV_BY_DATASET_AND_SIGLE;
	static const char *SQL_FIND_INDIV_BY_ID;
	static const char *SQL_INSERT_INDIV;
	static const char *SQL_UPDATE_INDIV;
	static const char *SQL_REMOVE_INDIV;
	///////////////////////////////////////
	static const char *SQL_FIND_DATASET_VALUES_COUNT;
	static const char *SQL_FIND_DATASET_VALUES;
	static const char *SQL_FIND_DATASET_BY_STATUS_VALUES_COUNT;
	static const char *SQL_FIND_DATASET_BY_STATUS_VALUES;
	static const char *SQL_VALUES_BY_VARIABLE_INDIV;
	static const char *SQL_VALUES_BY_VARIABLE_NOT_NULL;
	static const char *SQL_VALUES_BY_VARIABLE_NOT_NULL_COUNT;
	static const char *SQL_INSERT_VALUE;
	static const char *SQL_UPDATE_VALUE;
	static const char *SQL_REMOVE_VALUE;
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
///////////////////////////////////////
#include "sqltext.h"
///////////////////////////////////
template<class TSTRING>
template<class IFSTREAM>
bool StatDataManager<TSTRING>::process_data(IFSTREAM &inFile,
		const StatDataManager<TSTRING>::StringType &datasetSigle,
		StatDataManager<TSTRING>::DatasetType &oSet,
		const StatDataManager<TSTRING>::Char &delim,
		const StatDataManager<TSTRING>::StringType &na) {
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
		const StatDataManager<TSTRING>::StringType &datasetSigle,
		const StatDataManager<TSTRING>::Char &delim,
		const StatDataManager<TSTRING>::StringType &na) {
	DatasetType oSet;
	return this->process_data(inFile, datasetSigle, oSet, delim, na);
} // process_data
template<class TSTRING>
bool StatDataManager<TSTRING>::fill_data(
		const intra::ImportData<TSTRING> &oData,
		StatDataManager<TSTRING>::DatasetType &oSet) {
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
	if (!bRet) {
		return (false);
	}
	bRet = this->load_dataset(oSet);
	return bRet;
} // fill_data
template<class TSTRING>
void StatDataManager<TSTRING>::get_indiv_sigle_name(
		StatDataManager<TSTRING>::StringType &s) const {
#ifndef INTRA_USE_WSTRING
	s = "NOM";
#else
	s = L"NOM";
#endif
}
template<class TSTRING>
void StatDataManager<TSTRING>::get_indiv_status_name(
		StatDataManager<TSTRING>::StringType &s) const {
#ifndef INTRA_USE_WSTRING
	s = "STATUS";
#else
	s = L"STATUS";
#endif
}
/////////////////////////////////////////
template<class TSTRING>
bool StatDataManager<TSTRING>::load_dataset(
		StatDataManager<TSTRING>::DatasetType &cur) {
	//
	cur.clear();
	//
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
	std::for_each(vecInds.begin(), vecInds.end(), [&](const IndivType &v) {
		IndivType *p = cur.register_indiv(v.sigle());
		if (p != nullptr) {
			p->id(v.id());
			p->version(v.version());
			p->name(v.name());
			p->description(v.description());
			p->is_changed(false);
		}
	});
	std::for_each(vecVars.begin(), vecVars.end(), [&](const VariableType &v) {
		VariableType *p = cur.register_variable(v.sigle());
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
		std::vector<ValueType> vecVals;
		if (!this->get_dataset_values(nId, vecVals, skip, taken)) {
			return (false);
		}
		if (vecVals.empty()) {
			break;
		}
		std::for_each(vecVals.begin(), vecVals.end(),
				[&](const ValueType &val) {
					if (!val.value().empty()) {
						int nVarId = val.variable_id();
						int nIndId = val.indiv_id();
						ValueType vx;
						vx.indiv_id(nIndId);
						vx.variable_id(nVarId);
						ValueType *p = cur.register_value(vx);
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
template<class TSTRING>
bool StatDataManager<TSTRING>::remove_dataset(
		const StatDataManager<TSTRING>::DatasetType &cur) {
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
template<class TSTRING>
bool StatDataManager<TSTRING>::update_dataset(
		const StatDataManager<TSTRING>::DatasetType &cur) {
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
template<class TSTRING>
bool StatDataManager<TSTRING>::insert_dataset(
		const StatDataManager<TSTRING>::DatasetType &cur) {
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
	StringType sigle = cur.sigle();
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
		const StatDataManager<TSTRING>::StringType &xSigle,
		StatDataManager<TSTRING>::DatasetType &cur) {
	assert(this->is_valid());
	sqlite::Database *pBase = this->m_database.get();
	sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_SIGLE);
	if (!stmt.is_valid()) {
		return (false);
	}
	StringType sx = boost::to_upper_copy(boost::trim_copy(xSigle));
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
			StringType s;
			if (v.string_value(s)) {
				cur.sigle(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(3, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.name(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(4, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.description(s);
			}
		}
	}
	return (true);
} // get_dataset_by_sigle
template<class TSTRING>
bool StatDataManager<TSTRING>::get_dataset_by_id(int xId,
		StatDataManager<TSTRING>::DatasetType &cur) {
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
			StringType s;
			if (v.string_value(s)) {
				cur.sigle(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(3, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.name(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(4, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.description(s);
			}
		}
	}
	return (true);
} // get_dataset_by_id
template<class TSTRING>
bool StatDataManager<TSTRING>::get_variable_by_dataset_and_sigle(int nDatasetId,
		const StatDataManager<TSTRING>::StringType &xSigle,
		StatDataManager<TSTRING>::VariableType &cur) {
	assert(this->is_valid());
	sqlite::Database *pBase = this->m_database.get();
	sqlite::Statement stmt(pBase, SQL_VARIABLE_BY_DATASET_AND_SIGLE);
	if (!stmt.is_valid()) {
		return (false);
	}
	StringType sx = boost::to_upper_copy(boost::trim_copy(xSigle));
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
			StringType s;
			if (v.string_value(s)) {
				cur.sigle(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(4, v)) {
			StringType s;
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
			StringType s;
			if (v.string_value(s)) {
				cur.name(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(7, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.description(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(8, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.genre(s);
			}
		}
	}
	return (true);
} // get_variable_by_dataset_and_sigle
template<class TSTRING>
bool StatDataManager<TSTRING>::check_variable(int nDatasetId,
		const StatDataManager<TSTRING>::StringType &sigle,
		const StatDataManager<TSTRING>::StringType &type, bool bCateg,
		const StatDataManager<TSTRING>::StringType &genre) {
	assert(this->is_valid());
	sqlite::Database *pBase = this->m_database.get();
	{
		sqlite::Statement stmt(pBase, SQL_VARIABLE_BY_DATASET_AND_SIGLE);
		if (!stmt.is_valid()) {
			return (false);
		}
		StringType sx = boost::to_upper_copy(boost::trim_copy(sigle));
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
	StringType sSigle = boost::trim_copy(sigle);
	StringType sType = boost::trim_copy(type);
	StringType sGenre = boost::trim_copy(genre);
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
template<class TSTRING>
bool StatDataManager<TSTRING>::get_dataset_indivs_by_status_count(int nId,
		const StatDataManager<TSTRING>::StringType &status, int &nCount) {
	assert(this->is_valid());
	nCount = 0;
	sqlite::Database *pBase = this->m_database.get();
	sqlite::Statement stmt(pBase, SQL_FIND_INDIVS_BY_DATASET_AND_STATUS_COUNT);
	if (!stmt.is_valid()) {
		return false;
	}
	StringType sStatus = boost::to_upper_copy(boost::trim_copy(status));
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
template<class TSTRING>
bool StatDataManager<TSTRING>::get_indiv_by_dataset_sigle(int nDatasetId,
		const StatDataManager<TSTRING>::StringType &xSigle,
		StatDataManager<TSTRING>::IndivType &cur) {
	assert(this->is_valid());
	sqlite::Database *pBase = this->m_database.get();
	sqlite::Statement stmt(pBase, SQL_INDIV_BY_DATASET_AND_SIGLE);
	if (!stmt.is_valid()) {
		return (false);
	}
	StringType sigle = boost::to_upper_copy(boost::trim_copy(xSigle));
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
			StringType s;
			if (v.string_value(s)) {
				cur.sigle(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(4, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.name(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(5, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.description(s);
			}
		}
	}
	{
		sqlite::DbValue v;
		if (stmt.col_value(6, v)) {
			StringType s;
			if (v.string_value(s)) {
				cur.status(s);
			}
		}
	}
	return (true);
} // get_indiv_by_dataset_sigle
template<class TSTRING>
bool StatDataManager<TSTRING>::get_dataset_by_status_values_count(int nId,
		const StatDataManager<TSTRING>::StringType &status, int &nCount) {
	assert(this->is_valid());
	nCount = 0;
	sqlite::Database *pBase = this->m_database.get();
	sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_STATUS_VALUES_COUNT);
	if (!stmt.is_valid()) {
		return false;
	}
	StringType sStatus = boost::to_upper_copy(boost::trim_copy(status));
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
template<class TSTRING>
bool StatDataManager<TSTRING>::commit_transaction(void) {
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
template<class TSTRING>
bool StatDataManager<TSTRING>::rollback_transaction(void) {
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
template<class TSTRING>
bool StatDataManager<TSTRING>::close(void) {
	bool bRet = true;
	sqlite::Database *pbase = this->m_database.get();
	if (pbase != nullptr) {
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
}
template<class TSTRING>
bool StatDataManager<TSTRING>::open(
		const StatDataManager<TSTRING>::StringType &filename) {
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
		const char *pszSQL = CREATE_SQL[i];
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
template<class TSTRING>
bool StatDataManager<TSTRING>::check_values(int nDatasetId) {
	std::vector<int> oInds;
	if (!this->get_dataset_indivs_ids(nDatasetId, oInds)) {
		return (false);
	}
	if (oInds.empty()) {
		return (true);
	}
	sqlite::Database *pBase = this->m_database.get();
	std::vector<int> oVars;
	if (!this->get_dataset_variables_ids(nDatasetId, oVars)) {
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
		const StatDataManager<TSTRING>::ValueType &s,
		const StatDataManager<TSTRING>::StringType &vartype, boost::any &v) {
	std::string sx = boost::to_lower_copy(boost::trim_copy(vartype));
	if (sx == "bool") {
		v = boost::any(s.bool_value());
	} else if (sx == "short") {
		v = boost::any(s.short_value());
	} else if (sx == "integer") {
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
template<class TSTRING>
bool StatDataManager<TSTRING>::get_dataset_values_count(int nId, int &nCount) {
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
template<class TSTRING>
bool StatDataManager<TSTRING>::get_variable_values_count(int nVarId,
		int &nCount) {
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
template<class TSTRING>
bool StatDataManager<TSTRING>::get_dataset_indivs_count(int nId, int &nCount) {
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
#endif /* STATDATAMANAGER_H_ */
