#include "statdatamanager.h"
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
	const char *StatDataManager::SQL_FIND_DATASET_VARIABLES_IDS =
		"SELECT variableid FROM dbvariable WHERE datasetid = ?1";
	const char *StatDataManager::SQL_VARIABLE_BY_DATASET_AND_SIGLE =
		"SELECT variableid,optlock,datasetid,sigle,vartype,categvar,nom,description, genre"
		" FROM dbvariable WHERE datasetid = ?1 AND UPPER(LTRIM(RTRIM(sigle))) = ?2";
	const char *StatDataManager::SQL_FIND_VARIABLE_BY_ID =
		"SELECT variableid,optlock,datasetid,sigle,vartype,categvar,nom,description, genre"
		" FROM dbvariable WHERE variableid = ?1";
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
	const char *StatDataManager::SQL_FIND_INDIV_BY_ID =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status"
		" FROM dbindiv WHERE individ = ?1";
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
	const char *StatDataManager::SQL_VALUES_BY_VARIABLE_NOT_NULL =
		"SELECT valueid,optlock,variableid,individ,stringval"
		" FROM dbvalue WHERE variableid = ?1 AND stringval IS NOT NULL"
		" ORDER BY individ ASC LIMIT ?2 OFFSET ?3";
	const char *StatDataManager::SQL_VALUES_BY_VARIABLE_NOT_NULL_COUNT =
		"SELECT COUNT(*)"
		" FROM dbvalue WHERE variableid = ?1 AND stringval IS NOT NULL";
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
	const char *StatDataManager::CREATE_SQL [] = {
		StatDataManager::SQL_CREATE_DATASET,
		StatDataManager::SQL_CREATE_VARIABLE, StatDataManager::SQL_CREATE_INDIV,
		StatDataManager::SQL_CREATE_VALUE,
		StatDataManager::SQL_CREATE_VIEW_VALUES,
		StatDataManager::SQL_INDIVS_VIEW, StatDataManager::SQL_VARS_VIEW,
		StatDataManager::SQL_DATASETS_VIEW, nullptr };
	//////////////////////////////////////////////////
}// namespace intrasqlite