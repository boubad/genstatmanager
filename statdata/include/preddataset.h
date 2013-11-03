#ifndef __PREDDATASET_H__
#define __PREDDATASET_H__
/////////////////////////////////////////////
#include "statdatamanager.h"
///////////////////////////////////////////////////
namespace intrasqlite {
////////////////////////////////////////////
template<class TSTRING = std::string>
class PredStatDataManager: public StatDataManager<TSTRING> {
public:
	PredStatDataManager(){
	}
	virtual ~PredStatDataManager(){
	}
protected:
	virtual void init_tables(std::vector<TableDef> &oVec) const {
		oVec.clear();
		oVec.push_back(TableDef(true, "DOSSIER", "int", "X"));
		oVec.push_back(TableDef(true, "SEXE", "string", "I"));
		oVec.push_back(TableDef(true, "CUSERNAME", "string", "X"));
		oVec.push_back(TableDef(false, "CLASSEMENT", "int", "I"));
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
	}
private:
	// no implementation
	PredStatDataManager(const PredStatDataManager<TSTRING> &other);
	PredStatDataManager & operator=(const PredStatDataManager<TSTRING> &other);
};
// class PredStatDataManager
////////////////////////////////////////////////
}// namespace intrasqlite
/////////////////////////////////////////////////
#endif // __PREDDATASET_H__
