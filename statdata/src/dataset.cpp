/*
 * dataset.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */
#include "dataset.h"
///////////////////////////////////////
#include <boost/algorithm/string.hpp>
////////////////////////////////
#include "variable.h"
#include "indiv.h"
#include "value.h"
///////////////////////////////
namespace intra {
////////////////////////////////
Value *Dataset::create_value(int nVarId, int nIndId) {
	Value *p = const_cast<Value *>(this->find_value_by_variable_indiv(nVarId,
			nIndId));
	if (p != nullptr) {
		return (nullptr);
	}
	Variable *pVar = const_cast<Variable *>(this->find_variable_by_id(nVarId));
	Indiv *pInd = const_cast<Indiv *>(this->find_indiv_by_id(nIndId));
	if ((pVar == nullptr) || (pInd == nullptr)) {
		return (nullptr);
	}
	Dataset *pSet = pVar->dataset();
	if (pSet == nullptr) {
		pSet = pInd->dataset();
	}
	if (pSet == nullptr) {
		return (nullptr);
	}
	PValue oVal = std::make_shared < Value > (pVar, pInd);
	pSet->values().push_back(oVal);
	return p;
} // createValue
Value *Dataset::create_value(const String &sigleVar, const String &sigleInd) {
	Value *p = const_cast<Value *>(this->find_value_by_variable_indiv(sigleVar,
			sigleInd));
	if (p != nullptr) {
		return (nullptr);
	}
	Variable *pVar = const_cast<Variable *>(this->find_variable_by_sigle(
			sigleVar));
	Indiv *pInd = const_cast<Indiv *>(this->find_indiv_by_sigle(sigleInd));
	if ((pVar == nullptr) || (pInd == nullptr)) {
		return (nullptr);
	}
	Dataset *pSet = pVar->dataset();
	if (pSet == nullptr) {
		pSet = pInd->dataset();
	}
	if (pSet == nullptr) {
		return (nullptr);
	}
	PValue oVal = std::make_shared < Value > (pVar, pInd);
	pSet->values().push_back(oVal);
	return p;
} // create_value
Indiv *Dataset::create_indiv(int nIndId) {
	Indiv *p = const_cast<Indiv *>(this->find_indiv_by_id(nIndId));
	if (p == nullptr) {
		PIndiv oInd = std::make_shared<Indiv>();
		p = oInd.get();
		if (p != nullptr) {
			p->id(nIndId);
			p->dataset(this);
			StringStream os;
			os << nIndId;
			String ss = os.str();
			this->m_inds[ss] = oInd;
			return (p);
		}
	}
	return (nullptr);
} // create_indiv
Indiv *Dataset::create_indiv(const String &sigle) {
	String ss = boost::trim_copy(sigle);
	if (ss.empty()) {
		return (nullptr);
	}
	Indiv *p = const_cast<Indiv *>(this->find_indiv_by_sigle(ss));
	if (p == nullptr) {
		PIndiv oInd = std::make_shared<Indiv>();
		p = oInd.get();
		if (p != nullptr) {
			p->sigle(ss);
			p->dataset(this);
			this->m_inds[ss] = oInd;
			return (p);
		}
	}
	return (nullptr);
} // create_variable
Variable *Dataset::create_variable(const String &sigle) {
	String ss = boost::trim_copy(sigle);
	if (ss.empty()) {
		return (nullptr);
	}
	Variable *p = const_cast<Variable *>(this->find_variable_by_sigle(ss));
	if (p == nullptr) {
		PVariable oVar = std::make_shared<Variable>();
		p = oVar.get();
		if (p != nullptr) {
			p->sigle(ss);
			p->dataset(this);
			this->m_vars[ss] = oVar;
			return (p);
		}
	}
	return (nullptr);
} // create_variable
Variable *Dataset::create_variable(int nVarId) {
	Variable *p = const_cast<Variable *>(this->find_variable_by_id(nVarId));
	if (p == nullptr) {
		PVariable oVar = std::make_shared<Variable>();
		p = oVar.get();
		if (p != nullptr) {
			p->id(nVarId);
			p->dataset(this);
			StringStream os;
			os << nVarId;
			String ss = os.str();
			this->m_vars[ss] = oVar;
			return (p);
		}
	}
	return (nullptr);
} // create_variable
////////////////////////////
const Value *Dataset::find_value_by_variable_indiv(const Variable *pVar,
		const Indiv *pInd) const {
	if ((pVar == nullptr) || (pInd == nullptr)) {
		return (nullptr);
	}
	const ValueVector &oVec = this->m_vals;
	auto iend = oVec.end();
	for (auto it = oVec.begin(); it != iend; ++it) {
		PValue oVal = *it;
		const Value *pVal = oVal.get();
		if (pVal != nullptr) {
			if ((pVal->variable() == pVar) && (pVal->indiv() == pInd)) {
				return (pVal);
			}
		} // pVal
	} // it
	return (nullptr);
} // find_value_by_variable_indiv
const Value *Dataset::find_value_by_variable_indiv(int nVarId,
		int nIndId) const {
	const Variable *pVar = this->find_variable_by_id(nVarId);
	const Indiv *pInd = this->find_indiv_by_id(nIndId);
	if ((pVar == nullptr) || (pInd == nullptr)) {
		return (nullptr);
	}
	return (this->find_value_by_variable_indiv(pVar, pInd));
} // find_value_by_variable_indiv
const Value *Dataset::find_value_by_variable_indiv(const String &sigleVar,
		const String &sigleInd) const {
	const Variable *pVar = this->find_variable_by_sigle(sigleVar);
	const Indiv *pInd = this->find_indiv_by_sigle(sigleInd);
	if ((pVar == nullptr) || (pInd == nullptr)) {
		return (nullptr);
	}
	return (this->find_value_by_variable_indiv(pVar, pInd));
} // find_value_by_variable_indiv
const Variable *Dataset::find_variable_by_sigle(const String &s) const {
	String ss = boost::to_upper_copy(boost::trim_copy(s));
	const VariableMap & oMap = this->m_vars;
	auto it = oMap.find(ss);
	if (it != oMap.end()) {
		PVariable oVar = (*it).second;
		return (oVar.get());
	}
	return (nullptr);
} // find_variable_by_sigle
const Indiv *Dataset::find_indiv_by_sigle(const String &s) const {
	String ss = boost::to_upper_copy(boost::trim_copy(s));
	const IndivMap & oMap = this->m_inds;
	auto it = oMap.find(ss);
	if (it != oMap.end()) {
		PIndiv oInd = (*it).second;
		return (oInd.get());
	}
	return (nullptr);
} // find_indiv_by_sigle
const Variable *Dataset::find_variable_by_id(int nVarId) const {
	const VariableMap & oMap = this->m_vars;
	auto iend = oMap.end();
	for (auto it = oMap.begin(); it != iend; ++it) {
		PVariable oVar = (*it).second;
		const Variable *p = oVar.get();
		if ((p != nullptr) && (p->id() == nVarId)) {
			return p;;
		}
	} // it
	return (nullptr);
} // find_variable_by_id
const Indiv *Dataset::find_indiv_by_id(int nIndId) const {
	const IndivMap & oMap = this->m_inds;
	auto iend = oMap.end();
	for (auto it = oMap.begin(); it != iend; ++it) {
		PIndiv oVar = (*it).second;
		const Indiv *p = oVar.get();
		if ((p != nullptr) && (p->id() == nIndId)) {
			return p;;
		}
	} // it
	return (nullptr);
} // find_indiv_by_id
////////////////////////////
Dataset::Dataset() {

}
Dataset::Dataset(const Dataset &other) :
		NamedItem(other), m_vars(other.m_vars), m_inds(other.m_inds), m_vals(
				other.m_vals) {
}
Dataset & Dataset::operator=(const Dataset &other) {
	if (this != &other) {
		NamedItem::operator=(other);
		this->m_vars = other.m_vars;
		this->m_inds = other.m_inds;
		this->m_vals = other.m_vals;
	}
	return (*this);
}
Dataset::~Dataset() {
}
bool Dataset::operator==(const Dataset &other) const {
	return NamedItem::operator==(other);
}
bool Dataset::operator<(const Dataset &other) const {
	return NamedItem::operator<(other);
}
///////////////////////////////
}// namespace intra
