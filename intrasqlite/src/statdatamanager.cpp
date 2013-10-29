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
//////////////////////////////////////////
const char *StatDataManager::SQL_COMMIT_TRANSACTION = "COMMIT TRANSACTION";
const char *StatDataManager::SQL_BEGIN_TRANSACTION = "BEGIN TRANSACTION";
const char *StatDataManager::SQL_ROLLBACK_TRANSACTION = "ROLLBACK TRANSACTION";
////////////////////////////////////
const char *StatDataManager::SQL_CREATE_DATASET =
		"CREATE TABLE IF NOT EXISTS dbdataset("
				" datasetid INTEGER PRIMARY KEY AUTOINCREMENT,"
				" optlock INTEGER NOT NULL DEFAULT 1,"
				" sigle TEXT NOT NULL UNIQUE,"
				" nom TEXT DEFAULT NULL,"
				" description TEXT DEFAULT NULL"
				" )";
//////////////////////////////////////
const char *StatDataManager::SQL_FIND_ALL_DATASETS =
		"SELECT datasetid,optlock,sigle,nom,description"
				" FROM dbdataset ORDER BY sigle";
const char *StatDataManager::SQL_FIND_DATASET_BY_ID =
		"SELECT datasetid,optlock,sigle,nom,description"
				" FROM dbdataset WHERE datasetid = ?1";
const char *StatDataManager::SQL_FIND_DATASET_BY_SIGLE =
		"SELECT datasetid,optlock,sigle,nom,description"
				" FROM dbdataset WHERE UPPER(LTRIM(RTRIM(sigle))) = ?1";
const char *StatDataManager::SQL_INSERT_DATASET =
		"INSERT INTO dbdataset (sigle,nom,description)"
				" VALUES (?1,?2,?3)";
const char *StatDataManager::SQL_UPDATE_DATASET =
		"UPDATE dbdataset SET optlock = optlock + 1,"
				" sigle = ?1, nom = ?2, description = ?3 WHERE datasetid = ?4";
const char *StatDataManager::SQL_REMOVE_DATASET =
		"DELETE FROM dbdataset WHERE datasetid = ?1";
///////////////////////////////////////
const char *StatDataManager::SQL_CREATE_VARIABLE =
		"CREATE TABLE IF NOT EXISTS dbvariable("
				" variableid INTEGER PRIMARY KEY AUTOINCREMENT,"
				" optlock INTEGER NOT NULL DEFAULT 1,"
				" datasetid INTEGER NOT NULL,"
				" sigle TEXT NOT NULL,"
				" vartype TEXT NOT NULL,"
				" categvar INTEGER NOT NULL DEFAULT 0,"
				" nom TEXT DEFAULT NULL,"
				" description TEXT DEFAULT NULL,"
				" genre TEXT DEFAULT NULL,"
				" CONSTRAINT uc_variable UNIQUE (datasetid, sigle),"
				" CONSTRAINT fk_variable_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
				" )";
/////////////////////////////////////////////////////////
const char *StatDataManager::SQL_FIND_DATASET_VARIABLES =
		"SELECT variableid, optlock, datasetid , sigle, vartype, categvar, nom, description, genre"
				" FROM dbvariable WHERE datasetid = ?1 ORDER BY categvar DESC, sigle ASC";
const char *StatDataManager::SQL_VARIABLE_BY_DATASET_AND_SIGLE =
		"SELECT variableid,optlock,datasetid,sigle,vartype,categvar,nom,description, genre"
				" FROM dbvariable WHERE datasetid = ?1 AND UPPER(LTRIM(RTRIM(sigle))) = ?2";
const char *StatDataManager::SQL_INSERT_VARIABLE =
		"INSERT INTO dbvariable (datasetid,sigle,vartype,categvar,nom,description,genre)"
				" VALUES (?1, ?2,?3,?4,?5,?6,?7)";
const char *StatDataManager::SQL_UPDATE_VARIABLE =
		"UPDATE dbvariable SET optlock = optlock + 1,"
				" sigle = ?1, vartype = ?2, categvar = ?3, nom = ?4, description = ?5, genre = ?6 WHERE variableid = ?7";
const char *StatDataManager::SQL_REMOVE_VARIABLE =
		"DELETE FROM dbvariable WHERE variableid = ?1";
////////////////////////////////////////////////////////
const char *StatDataManager::SQL_CREATE_INDIV =
		"CREATE TABLE IF NOT EXISTS dbindiv("
				" individ INTEGER PRIMARY KEY AUTOINCREMENT,"
				" optlock INTEGER NOT NULL DEFAULT 1,"
				" datasetid INTEGER NOT NULL,"
				" sigle TEXT NOT NULL,"
				" nom TEXT DEFAULT NULL,"
				" description TEXT DEFAULT NULL,"
				" status TEXT DEFAULT NULL,"
				" CONSTRAINT uc_indiv UNIQUE (datasetid, sigle),"
				" CONSTRAINT fk_indiv_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
				" )";
////////////////////////////////////////////////
const char *StatDataManager::SQL_FIND_DATASET_INDIVS_COUNT =
		"SELECT COUNT(*) FROM dbindiv"
				" WHERE datasetid = ?1";
const char *StatDataManager::SQL_FIND_DATASET_INDIVS =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status"
				" FROM dbindiv WHERE datasetid = ?1 ORDER BY sigle";
const char *StatDataManager::SQL_INDIV_BY_DATASET_AND_SIGLE =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status"
				" FROM dbindiv WHERE datasetid = ?1 AND UPPER(LTRIM(RTRIM(sigle))) = ?2";
const char *StatDataManager::SQL_INSERT_INDIV =
		"INSERT INTO dbindiv (datasetid,sigle,nom,description,status)"
				" VALUES(?1,?2,?3,?4, ?5)";
const char *StatDataManager::SQL_UPDATE_INDIV =
		"UPDATE dbindiv SET optlock = OPTLOCK + 1,"
				" sigle = ?1, nom = ?2, description = ?3, status = ?4 WHERE individ = ?5";
const char *StatDataManager::SQL_REMOVE_INDIV =
		"REMOVE FROM dbindiv WHERE individ = ?1";
const char *StatDataManager::SQL_FIND_INDIVS_BY_DATASET_AND_STATUS_COUNT =
		"SELECT COUNT(*) FROM dbindiv WHERE datasetid = ?1 AND UPPER(LTRIM(RTRIM(status))) = ?2";
const char *StatDataManager::SQL_FIND_INDIVS_BY_DATASET_AND_STATUS =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status"
				" FROM dbindiv WHERE datasetid = ?1 AND UPPER(LTRIM(RTRIM(status))) = ?2"
				" ORDER BY sigle ASC"
				" LIMIT ?3 OFFET ?4";
const char *StatDataManager::SQL_GET_DATASET_INDIV_IDS =
		"SELECT individ FROM dbindiv"
				" WHERE datasetid = ?1 ORDER BY individ ASC";
////////////////////////////////////////////////
const char *StatDataManager::SQL_CREATE_VALUE =
		"CREATE TABLE IF NOT EXISTS dbvalue("
				" valueid INTEGER PRIMARY KEY AUTOINCREMENT,"
				" optlock INTEGER NOT NULL DEFAULT 1,"
				" variableid INTEGER NOT NULL,"
				" individ INTEGER NOT NULL,"
				" stringval TEXT NULL,"
				" CONSTRAINT uc_value UNIQUE (variableid, individ),"
				" CONSTRAINT fk_value_variable FOREIGN KEY (variableid) REFERENCES dbvariable (variableid) ON DELETE CASCADE,"
				" CONSTRAINT fk_value_indiv FOREIGN KEY (individ) REFERENCES dbindiv (individ) ON DELETE CASCADE"
				" )";
/////////////////////////////////////////////////////
const char *StatDataManager::SQL_FIND_VARIABLE_TYPE =
		"SELECT vartype FROM dbvariable WHERE variableid = ?1";
const char *StatDataManager::SQL_FIND_DATASET_VALUES_COUNT = "SELECT COUNT(*)"
		" FROM dbvalue a, dbvariable b"
		" WHERE a.variableid = b.variableid AND b.datasetid = ?1";
const char *StatDataManager::SQL_FIND_DATASET_BY_STATUS_VALUES_COUNT =
		"SELECT COUNT(*)"
				" FROM dbvalue a, dbindiv b"
				" WHERE a.individ = b.individ AND b.datasetid = ?1 AND UPPER(LTRIM(RTRIM(b.status))) = ?2";
const char *StatDataManager::SQL_FIND_DATASET_VALUES =
		"SELECT a.valueid,a.optlock,a.variableid,a.individ,a.stringval"
				" FROM dbvalue a, dbvariable b"
				" WHERE a.variableid = b.variableid AND b.datasetid = ?1"
				" ORDER BY a.variableid ASC, a.individ ASC"
				" LIMIT ?2 OFFSET ?3";
const char *StatDataManager::SQL_FIND_DATASET_BY_STATUS_VALUES =
		"SELECT a.valueid,a.optlock,a.variableid,a.individ,a.stringval"
				" FROM dbvalue a, dbindiv b"
				" WHERE a.individ = b.individ AND b.datasetid = ?1 AND UPPER(LTRIM(RTRIM(b.status))) = ?2"
				" ORDER BY a.variableid ASC, a.individ ASC"
				" LIMIT ?3 OFFSET ?4";
const char *StatDataManager::SQL_VALUES_BY_VARIABLE_INDIV =
		"SELECT valueid,optlock,variableid,individ,stringval"
				" FROM dbvalue WHERE variableid = ?1 AND individ = ?2";
const char *StatDataManager::SQL_VALUE_BY_ID =
		"SELECT valueid,optlock,variableid,individ,stringval"
				" FROM dbvalue WHERE valueid = ?1";
const char *StatDataManager::SQL_INSERT_VALUE =
		"INSERT INTO dbvalue (variableid,individ,stringval)"
				" VALUES(?1,?2,?3)";
const char *StatDataManager::SQL_UPDATE_VALUE =
		"UPDATE dbvalue SET optlock = optlock + 1,"
				" stringval = ?1 WHERE valueid = ?2 ";
const char *StatDataManager::SQL_REMOVE_VALUE =
		"DELETE from dbvalue WHERE valueid = ?1";
//////////////////////////////////////////////////////
const char *StatDataManager::SQL_CREATE_VIEW_VALUES =
		"CREATE VIEW IF NOT EXISTS"
				" valview AS"
				" SELECT v.individ AS indid, v.variableid AS varid, v.stringval AS valeur, a.sigle AS colname, "
				" b.sigle AS rowname, b.status AS status, a.datasetid AS datasetid, a.genre AS genre"
				" FROM dbvalue v, dbvariable a, dbindiv b"
				" WHERE a.variableid = v.variableid AND b.individ = v.individ"
				" ORDER BY v.individ ASC, v.variableid ASC, b.status ASC";
const char *StatDataManager::SQL_INDIVS_VIEW = "CREATE VIEW IF NOT EXISTS"
		" indview AS"
		" SELECT individ, sigle, status, datasetid "
		" FROM dbindiv"
		" ORDER BY individ ASC";
const char *StatDataManager::SQL_VARS_VIEW = "CREATE VIEW IF NOT EXISTS"
		" varview AS"
		" SELECT variableid , sigle, vartype, categvar, datasetid, genre "
		" FROM dbvariable"
		" ORDER BY variableid ASC";
const char *StatDataManager::SQL_DATASETS_VIEW = "CREATE VIEW IF NOT EXISTS"
		" datasetview AS"
		" SELECT datasetid , sigle"
		" FROM dbdataset"
		" ORDER BY sigle ASC";
//////////////////////////////////////////////////////
const char *StatDataManager::CREATE_SQL[] = {
		StatDataManager::SQL_CREATE_DATASET,
		StatDataManager::SQL_CREATE_VARIABLE, StatDataManager::SQL_CREATE_INDIV,
		StatDataManager::SQL_CREATE_VALUE,
		StatDataManager::SQL_CREATE_VIEW_VALUES,
		StatDataManager::SQL_INDIVS_VIEW, StatDataManager::SQL_VARS_VIEW,
		StatDataManager::SQL_DATASETS_VIEW, nullptr };
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
				Value *p = cur.create_value(nVarId,nIndId);
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
StatDataManager::StatDataManager(const std::string &s) :
		m_intransaction(false) {
	this->open(s);
} // StatDataManager
StatDataManager::StatDataManager(const std::wstring &ss) :
		m_intransaction(false) {
	this->open(ss);
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
		} else {
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
	oVec.push_back(TableDef(true, "DOSSIER", "int", "I"));
	oVec.push_back(TableDef(true, "SEXE", "string", "I"));
	oVec.push_back(TableDef(true, "SERIEBAC", "string", "I"));
	oVec.push_back(TableDef(true, "OPTIONBAC", "string", "I"));
	oVec.push_back(TableDef(true, "LV1", "string", "I"));
	oVec.push_back(TableDef(true, "LV2", "string", "I"));
	oVec.push_back(TableDef(false, "MATHS_BAC", "float", "I"));
	oVec.push_back(TableDef(false, "PHYS_BAC", "float", "I"));
	oVec.push_back(TableDef(false, "PHYS_T", "float", "I"));
	oVec.push_back(TableDef(false, "MATHS_T", "float", "I"));
	oVec.push_back(TableDef(false, "HISTGEO_T", "float", "I"));
	oVec.push_back(TableDef(false, "LV1_T", "float", "I"));
	oVec.push_back(TableDef(false, "LV2_T", "float", "I"));
	oVec.push_back(TableDef(false, "MATHS_P", "float", "I"));
	oVec.push_back(TableDef(false, "PHYS_P", "float", "I"));
	oVec.push_back(TableDef(false, "HISTGEO_P", "float", "I"));
	oVec.push_back(TableDef(false, "LV1_P", "float", "I"));
	oVec.push_back(TableDef(false, "LV2_P", "float", "I"));
	oVec.push_back(TableDef(false, "MOY_S1", "float", "R"));
	oVec.push_back(TableDef(false, "REG_MOY_S1", "float", "C"));
	oVec.push_back(TableDef(false, "PROB_OK_DT_S1", "float", "C"));
	oVec.push_back(TableDef(false, "PROB_OK_BAYES_S1", "float", "C"));
	oVec.push_back(TableDef(true, "JURY_S1", "string", "C"));
	oVec.push_back(TableDef(true, "PREDDT_S1", "string", "C"));
	oVec.push_back(TableDef(true, "PREDBAYES_S1", "string", "C"));
	oVec.push_back(TableDef(true, "PREDREG_S1", "string", "C"));
	oVec.push_back(TableDef(true, "QUALITY", "string", "C"));
} // init_tables
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
	{
		std::vector<TableDef> oDefs;
		this->init_tables(oDefs);
		sqlite::Statement stmt(pBase, SQL_VARIABLE_BY_DATASET_AND_SIGLE);
		if (!stmt.is_valid()) {
			return (false);
		}
		for (auto it = oDefs.begin(); it != oDefs.end(); ++it) {
			const TableDef &oDef = *it;
			std::string sx = boost::to_upper_copy(boost::trim_copy(oDef.name));
			stmt.reset();
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
					if (nId != 0) {
						oVars.push_back(nId);
					}
				}
			}
		} // it
	} // vars
	if (!this->begin_transaction()) {
		return (false);
	}
	sqlite::Statement stmtFetch(pBase, SQL_VALUES_BY_VARIABLE_INDIV);
	sqlite::Statement stmtInsert(pBase, SQL_INSERT_VALUE);
	if ((!stmtFetch.is_valid()) || (!stmtInsert.is_valid())) {
		return (false);
	}
	std::string sval;
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
			stmtInsert.set_parameter(3, sval);
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
