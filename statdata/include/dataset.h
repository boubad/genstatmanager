/*
 * dataset.h
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */

#ifndef DATASET_H_
#define DATASET_H_
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
	typedef Value ValueType;
	typedef Variable<TSTRING> VariableType;
	typedef Indiv<TSTRING> IndivType;
	typedef typename VariableType::PVariable PVariable;
	typedef typename IndivType::PIndiv PIndiv;
	typedef typename Value::PValue PValue;
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
	Dataset(const DatasetType &other) :
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
	inline VariableVector & variables(void) {
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
		const VariableVector &oVec = this->variables();
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
		const VariableVector &oVec = this->variables();
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
		const VariableVector &oVec = this->variables();
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
			if (pRet != nullptr) {
				this->variables().push_back(o);
			}
		}
		return pRet;
	} // register_variable
	VariableType *register_variable(const String &sigle) {
			if (sigle.empty()) {
				return (nullptr);
			}
			VariableType *pRet = const_cast<VariableType *>(this->find_variable(sigle));
			if (pRet != nullptr) {
				return pRet;
			} else {
				PVariable o = std::make_shared < VariableType > ();
				pRet = o.get();
				if (pRet != nullptr) {
					pRet->sigle(sigle);
					pRet->name(sigle);
					pRet->description(sigle);
					VariableVector &oVec = this->variables();
					oVec.push_back(o);
				}
			}
			return pRet;
		} // register_variable
	template<class ALLOCVEC>
	size_t get_all_variables(std::vector<PVariable, ALLOCVEC> &oRes) const {
		oRes.clear();
		const VariableVector &oVec = this->m_vars;
		oRes = std::vector<PVariable, ALLOCVEC>(oVec.begin(), oVec.end());
		return (oRes.size());
	} // getAllVariables
	  //
	const IndivType *find_indiv(int nIndId) const {
		IndivType *pRet = nullptr;
		const IndivVector &oVec = this->indivs();
		auto iend = oVec.end();
		auto it = std::find_if(oVec.begin(), iend,
				[=](const PIndiv &oVar)->bool {
					const IndivType *p = oVar.get();
					return ((p != nullptr) && (p->id() == nIndId));
				});
		if (it != iend) {
			pRet = (*it).get();
		}
		return pRet;
	} // find_indiv
	const IndivType *find_indiv(const String &sigle) const {
		IndivType *pRet = nullptr;
		const IndivVector &oVec = this->indivs();
		auto iend = oVec.end();
		String ss = to_upper(trim(sigle));
		auto it = std::find_if(oVec.begin(), iend,
				[=](const PIndiv &oVar)->bool {
					const IndivType *p = oVar.get();
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
	} // find_indiv
	const IndivType *find_indiv(const IndivType &oVar) const {
		IndivType *pRet = nullptr;
		const IndivVector &oVec = this->indivs();
		auto iend = oVec.end();
		String ss = to_upper(trim(oVar.sigle()));
		int nVarId = oVar.id();
		auto it = std::find_if(oVec.begin(), iend, [=](const PIndiv &o)->bool {
			const IndivType *p = o.get();
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
	} // find_indiv
	IndivType *register_indiv(const IndivType &oVar) {
		int nVarId = oVar.id();
		String sigle = trim(oVar.sigle());
		if ((nVarId == 0) && sigle.empty()) {
			return (nullptr);
		}
		IndivType *pRet = const_cast<IndivType *>(this->find_indiv(oVar));
		if (pRet != nullptr) {
			*pRet = oVar;
		} else {
			PIndiv o = std::make_shared < IndivType > (oVar);
			pRet = o.get();
			if (pRet != nullptr) {
				this->indivs().push_back(o);
			}
		}
		return pRet;
	} // register_indiv
	IndivType *register_indiv(const String &sigle) {
			if (sigle.empty()) {
				return (nullptr);
			}
			IndivType *pRet = const_cast<IndivType *>(this->find_indiv(sigle));
			if (pRet != nullptr) {
				return pRet;
			} else {
				PIndiv o = std::make_shared < IndivType > ();
				pRet = o.get();
				if (pRet != nullptr) {
					pRet->sigle(sigle);
					pRet->name(sigle);
					pRet->description(sigle);
					this->indivs().push_back(o);
				}
			}
			return pRet;
		} // register_indiv
	template<class ALLOCVEC>
	size_t get_all_indivs(std::vector<PIndiv, ALLOCVEC> &oRes) const {
		oRes.clear();
		const IndivVector &oVec = this->indivs();
		oRes = std::vector<PIndiv, ALLOCVEC>(oVec.begin(), oVec.end());
		return (oRes.size());
	} // get_all_indivs
	  //
	const ValueType *find_value(int nIndId, int nVarId) const {
		if ((nIndId == 0) || (nVarId == 0)) {
			return (nullptr);
		}
		const VariableType *pVar = this->find_variable(nVarId);
		if (pVar == nullptr) {
			return (nullptr);
		}
		const IndivType *pInd = this->find_indiv(nIndId);
		if (pInd == nullptr) {
			return (nullptr);
		}
		const ValueVector &oVec = this->values();
		auto iend = oVec.end();
		auto it =
				std::find_if(oVec.begin(), iend,
						[=](const PValue &oVal)->bool {
							const ValueType *p = oVal.get();
							if ((p != nullptr) && (p->variable_id() == nVarId) && (p->indiv_id() == nIndId)) {
								return (true);
							}
							return (false);
						});
		if (it != iend) {
			return (*it).get();
		}
		return (nullptr);
	}	// find_value
	const ValueType *find_value(const String &sigleInd,
			const String &sigleVar) const {
		if (sigleInd.empty() || sigleVar.empty()) {
			return (nullptr);
		}
		const VariableType *pVar = this->find_variable(sigleVar);
		if (pVar == nullptr) {
			return (nullptr);
		}
		const IndivType *pInd = this->find_indiv(sigleInd);
		if (pInd == nullptr) {
			return (nullptr);
		}
		int nVarId = pVar->id();
		int nIndId = pInd->id();
		if ((nVarId == 0) || (nIndId == 0)) {
			return (nullptr);
		}
		const ValueVector &oVec = this->values();
		auto iend = oVec.end();
		auto it =
				std::find_if(oVec.begin(), iend,
						[=](const PValue &oVal)->bool {
							const ValueType *p = oVal.get();
							if ((p != nullptr) && (p->variable_id() == nVarId) && (p->indiv_id() == nIndId)) {
								return (true);
							}
							return (false);
						});
		if (it != iend) {
			return (*it).get();
		}
		return (nullptr);
	}	// find_value
	const ValueType *find_value(const ValueType &xVal) const {
		return (this->find_value(xVal.indiv_id(), xVal.variable_id()));
	}	// find_value
	ValueType *register_value(const ValueType &oVal) {
		int nIndId = oVal.indiv_id();
		int nVarId = oVal.variable_id();
		if ((nIndId == 0) || (nVarId == 0)) {
			return (nullptr);
		}
		ValueType *pVal = const_cast<ValueType *>(this->find_value(nIndId,
				nVarId));
		if (pVal != nullptr) {
			pVal->value(oVal.value());
			return (pVal);
		}
		VariableType *pVar = const_cast<VariableType *>(this->find_variable(
				nVarId));
		if (pVar == nullptr) {
			return (nullptr);
		}
		IndivType *pInd = const_cast<IndivType *>(this->find_indiv(nIndId));
		if (pInd == nullptr) {
			return (nullptr);
		}
		PValue o = std::make_shared < ValueType > (oVal);
		if ((pVal = o.get()) == nullptr) {
			return (nullptr);
		}
		this->values().push_back(o);
		pInd->values().push_back(o);
		pVar->values().push_back(o);
		return pVal;
	}	// registerValue
};
// class Dataset
//////////////////////////////////
}// namespace intre
////////////////////////////
#endif /* DATASET_H_ */
