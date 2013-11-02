/*
 * dataset.h
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */

#ifndef DATASET_H_
#define DATASET_H_
//////////////////////////////
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
//////////////////////////////
#include "indiv.h"
#include "variable.h"
#include "value.h"
///////////////////////////////
namespace intra {
//////////////////////////////////
template<class TSTRING = std::string>
class Dataset: public NamedItem<TSTRING> {
public:
	typedef TSTRING String;
	typedef Variable<TSTRING> VariableType;
	typedef Indiv<TSTRING> IndivType;
	typedef std::shared_ptr<VariableType> PVariable;
	typedef std::shared_ptr<IndivType> PIndiv;
	typedef std::shared_ptr<Value> PValue;
	typedef NamedItem<TSTRING> NamedItemType;
	typedef std::vector<PVariable> VariableVector;
	typedef std::vector<PIndiv> IndivVector;
	typedef std::vector<PValue> ValueVector;
	typedef Dataset<TSTRING> DatasetType;
private:
	VariableVector m_vars;
	IndivVector m_inds;
	ValueVector m_vals;
public:
	Dataset() {
	}
	Dataset(const DatasetTYpe &other) :
			NamedItemType(other), m_vars(other.m_vars), m_inds(other.m_inds), m_vals(
					other.m_vals) {
	}
	DatasetType & operator=(const DatasetType &other) {
		if (this != &other) {
			NamedItemType::operator=(other);
			this->m_vars = other.m_vars;
			this->m_inds = other.m_inds;
			this->m_vals = other.m_vals;
		}
		return (*this);
	}
	virtual ~Dataset() {
	}
public:
	bool operator==(const Dataset &other) const {
		if ((this->id() != 0) && (this->id() == other.id())) {
			return (true);
		}
		return (NamedItemType::operator=(other));
	}
	bool operator<(const Dataset &other) const {
		return (NamedItemType::operator<(other));
	}
public:
	inline const VariableVector & variables(void) const {
		return (this->m_vars);
	}
	inline VariablVector & variables(void) {
		return (this->m_vars);
	}
	inline const IndivVector & indivs(void) const {
		return (this->m_inds);
	}
	inline IndivVector & indivs(void) {
		return (this->m_inds);
	}
	inline const ValueVector & values(void) const {
		return (this->m_vals);
	}
	inline ValueVector & values(void) {
		return (this->m_vals);
	}
public:
	void clear(void) {
		this->m_vals.clear();
		this->m_vars.clear();
		this->m_inds.clear();
	}
	const VariableType *find_variable(int nVarId) const {
		VariableType *pRet = nullptr;
		const VariableVector &oVec = this->m_vars;
		auto iend = oVec.end();
		auto it = std::find_if(oVec.begin(), iend,
				[=](const PVariable &oVar)->bool {
					const VariableType *p = oVar.get();
					return ((p != nullptr) && (p->id() == nVarId));
				});
		if (it != iend) {
			pRet = (*it).get();
		}
		return pRet;
	} // find_variable
	const VariableType *find_variable(const String &sigle) const {
		VariableType *pRet = nullptr;
		const VariableVector &oVec = this->m_vars;
		auto iend = oVec.end();
		String ss = to_upper(trim(sigle));
		auto it = std::find_if(oVec.begin(), iend,
				[=](const PVariable &oVar)->bool {
					const VariableType *p = oVar.get();
					bool bRet = false;
					if (p != nullptr) {
						String s = to_upper(trim(p->sigle()));
						bRet = (s == ss);
					} // p
					return bRet;
				});
		if (it != iend) {
			pRet = (*it).get();
		}
		return pRet;
	} // find_variable
	const VariableType *find_variable(const VariableType &oVar) const {
		VariableType *pRet = nullptr;
		const VariableVector &oVec = this->m_vars;
		auto iend = oVec.end();
		String ss = to_upper(trim(oVar.sigle()));
		int nVarId = oVar.id();
		auto it = std::find_if(oVec.begin(), iend,
				[=](const PVariable &o)->bool {
					const VariableType *p = o.get();
					bool bRet = false;
					if (p != nullptr) {
						if ((nVarId != 0) && (nVarId == p->id())) {
							bRet = true;
						} else {
							String s = to_upper(trim(p->sigle()));
							bRet = (s == ss);
						}
					} // p
					return bRet;
				});
		if (it != iend) {
			pRet = (*it).get();
		}
		return pRet;
	} // find_variable
	VariableType *register_variable(const VariableType &oVar) {
		int nVarId = oVar.id();
		String sigle = trim(oVar.sigle());
		if ((nVarId == 0) && sigle.empty()) {
			return (nullptr);
		}
		VariableType *pRet = const_cast<VariableType *>(this->find_variable(
				oVar));
		if (pRet != nullptr) {
			*pRet = oVar;
		} else {
			PVariable o = std::make_shared < VariableType > (oVar);
			pRet = o.get();
			if (p != nullptr) {
				this->m_vars.push_back(o);
			}
		}
		return pRet;
	} // register_variable
	template <class ALLOCVEC>
	size_t get_all_variables(std::vector<PVariable,ALLOCVEC> &oRes) const {
		oRes.clear();
		const VariableVector &oVec = this->m_vars;
		oRes = std::vector<PVariable,ALLOCVEC>(oVec.begin(),oVec.end());
		return (oRes.size());
	}// getAllVariables
};
// class Dataset
//////////////////////////////////
}// namespace intre
////////////////////////////
#endif /* DATASET_H_ */
