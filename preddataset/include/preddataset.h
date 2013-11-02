#ifndef __PREDDATASET_H__
#define __PREDDATASET_H__
/////////////////////////////////////////////
#include "statdatamanager.h"
///////////////////////////////////////////////////
namespace intrasqlite {
	////////////////////////////////////////////
	class PredStatDataManager : public StatDataManager {
	public:
		PredStatDataManager();
		virtual ~PredStatDataManager();
	protected:
		virtual void init_tables(std::vector<TableDef> &oVec) const;
	private:
		// no implementation
		PredStatDataManager(const PredStatDataManager &other);
		PredStatDataManager & operator=(const PredStatDataManager &other);
	};// class PredStatDataManager
	////////////////////////////////////////////////
}// namespace intrasqlite
/////////////////////////////////////////////////
#endif // __PREDDATASET_H__
