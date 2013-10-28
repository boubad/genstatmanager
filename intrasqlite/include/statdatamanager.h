/*
 * statdatamanager.h
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

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
//////////////////////////////////
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
class StatDataManager {
private:
	bool m_intransaction;
	std::unique_ptr<sqlite::Database> m_database;
public:
	StatDataManager();
	StatDataManager(const std::string &s);
	StatDataManager(const std::wstring &ss);
	virtual ~StatDataManager();
public:
	void init_tables(std::vector<TableDef> &oVec) const;
	bool begin_transaction(void);
	bool commit_transaction(void);
	bool rollback_transaction(void);
	bool check_pred_dataset(int nDatasetId);
	void convert_value(const intra::Value &s, const std::string &vartype,
			boost::any &v);
	bool get_value_by_variable_indiv(int nVarId, int nIndivId,
			intra::Value &cur);
	bool get_dataset_values_count(int nId, int &nCount);
	bool get_dataset_indivs_count(int nId, int &nCount);
	bool check_values(int nDatasetId);
	bool get_dataset_by_status_values_count(int nId,
			const intra::String &status, int &nCount);
	bool get_indiv_by_dataset_sigle(int nDatasetId, const intra::String &xSigle,
			intra::Indiv &cur);
	bool get_dataset_indivs_by_status_count(int nId,
			const intra::String &status, int &nCount);
	bool get_variable_by_dataset_and_sigle(int nDatasetId,
			const intra::String &xSigle, intra::Variable &cur);
	bool check_variable(int nDatasetId, const intra::String &sigle,
			const intra::String &type, bool bCateg, const intra::String &genre);
	bool load_dataset(intra::Dataset &cur);
	bool remove_dataset(const intra::Dataset &cur);
	bool update_dataset(const intra::Dataset &cur);
	bool insert_dataset(const intra::Dataset &cur);
	bool get_dataset_by_sigle(const intra::String &xSigle, intra::Dataset &cur);
	bool get_dataset_by_id(int xId, intra::Dataset &cur);
	bool open(const std::string &filename);
	bool open(const std::wstring &wfilename);
	bool is_valid(void) const;
	bool close(void);
public:
	template<class ALLOCVAL>
	bool remove_values(const std::vector<intra::Value, ALLOCVAL> &oVec) {
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
			const intra::Value &cur = *it;
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
	bool maintains_values(const std::vector<intra::Value, ALLOCVAL> &oVec) {
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
			const intra::Value &cur = *it;
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
					stmtUpdate.set_parameter(1, sval);
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
					stmtUpdate.set_parameter(1, sval);
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
				stmtInsert.set_parameter(3, sval);
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
	bool get_dataset_values(int nDatasetId,
			std::vector<intra::Value, ALLOCVEC> &oVec, int skip, int taken) {
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
			intra::Value cur;
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
	bool get_dataset_by_status_values(int nDatasetId,
			const intra::String &status,
			std::vector<intra::Value, ALLOCVEC> &oVec, int skip, int taken) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_STATUS_VALUES);
		sqlite::Statement stmtVarType(pBase, SQL_FIND_VARIABLE_TYPE);
		std::map<int, std::string> oMap;
		if ((!stmt.is_valid()) || (!stmtVarType.is_valid())) {
			return (false);
		}
		intra::String sStatus = boost::to_upper_copy(boost::trim_copy(status));
		stmt.set_parameter(1, nDatasetId);
		stmt.set_parameter(2, sStatus);
		stmt.set_parameter(3, taken);
		stmt.set_parameter(4, skip);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			intra::Value cur;
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
	bool remove_indivs(const std::vector<intra::Indiv, ALLOCIND> &oVec) {
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
			const intra::Indiv &cur = *it;
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
	bool maintains_indivs(const std::vector<intra::Indiv, ALLOCIND> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_INDIV);
		sqlite::Statement stmtUpdate(pBase, SQL_UPDATE_INDIV);
		if ((!stmtUpdate.is_valid()) || (!stmtInsert.is_valid())) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const intra::Indiv &cur = *it;
			int nId = cur.id();
			int nDatasetId = cur.dataset_id();
			intra::String sigle = cur.sigle();
			if (cur.is_updateable()) {
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
			} else if (cur.is_writeable()) {
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
			std::vector<intra::Indiv, ALLOCVEC> &oVec,
			const intra::String &status, int skip, int taken) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_INDIVS_BY_DATASET_AND_STATUS);
		if (!stmt.is_valid()) {
			return (false);
		}
		intra::String sStatus = boost::to_upper_copy(boost::trim_copy(status));
		stmt.set_parameter(1, nDatasetId);
		stmt.set_parameter(2, sStatus);
		stmt.set_parameter(3, taken);
		stmt.set_parameter(4, skip);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			intra::Indiv cur;
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
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
		return (true);
	} // get_dataset_by_status_indivs
	template<class ALLOCVEC>
	bool get_dataset_indivs(int nDatasetId,
			std::vector<intra::Indiv, ALLOCVEC> &oVec) {
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
			intra::Indiv cur;
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
	bool remove_variables(const std::vector<intra::Variable, ALLOCVAR> &oVec) {
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
			const intra::Variable &cur = *it;
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
	bool maintains_variables(
			const std::vector<intra::Variable, ALLOCVAR> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_VARIABLE);
		sqlite::Statement stmtUpdate(pBase, SQL_UPDATE_VARIABLE);
		if ((!stmtUpdate.is_valid()) || (!stmtInsert.is_valid())) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const intra::Variable &cur = *it;
			int nId = cur.id();
			int nDatasetId = cur.dataset_id();
			intra::String sigle = cur.sigle();
			int nz = (cur.is_categvar()) ? 1 : 0;
			if (cur.is_updateable()) {
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
			} else if (cur.is_writeable()) {
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
	template<class ALLOCVAR>
	bool get_dataset_variables(int nDatasetId,
			std::vector<intra::Variable, ALLOCVAR> &oVec) {
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
			intra::Variable cur;
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
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
		return (true);
	} // get_dataset_variables
	template<class ALLOCVEC>
	bool get_all_datasets(std::vector<intra::Dataset, ALLOCVEC> &oVec) {
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
			intra::Dataset cur;
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
	static const char *SQL_VARIABLE_BY_DATASET_AND_SIGLE;
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
	static const char *SQL_INSERT_INDIV;
	static const char *SQL_UPDATE_INDIV;
	static const char *SQL_REMOVE_INDIV;
	///////////////////////////////////////
	static const char *SQL_FIND_DATASET_VALUES_COUNT;
	static const char *SQL_FIND_DATASET_VALUES;
	static const char *SQL_FIND_DATASET_BY_STATUS_VALUES_COUNT;
	static const char *SQL_FIND_DATASET_BY_STATUS_VALUES;
	static const char *SQL_VALUES_BY_VARIABLE_INDIV;
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
////////////////////////////////////
} /* namespace intrasqlite */
#endif /* STATDATAMANAGER_H_ */
