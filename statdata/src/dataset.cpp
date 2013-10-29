/*
 * dataset.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */
#include "dataset.h"
#include "variable.h"
#include "indiv.h"
#include "value.h"
///////////////////////////////////////
#include <boost/algorithm/string.hpp>
////////////////////////////////
namespace intra {
/////////////////////////////////////////
void Dataset::get_indivs_values(const Indiv *pInd, AnyIdMap &oRes) const {
	oRes.clear();
	if (pInd != nullptr) {
		const ValueVector &oVec = this->m_vals;
		auto iend = oVec.end();
		int nId = pInd->id();
		for (auto it = oVec.begin(); it != iend; ++it) {
			PValue v = *it;
			const Value *p = v.get();
			if ((p != nullptr) && p->is_valid()) {
				int nVarId = p->variable_id();
				const Indiv *ppInd = p->indiv();
				if (pInd == ppInd) {
					boost::any val = p->value();
					oRes[nVarId] = val;
				} else if ((nId != 0) && (p->indiv_id() == nId)) {
					boost::any val = p->value();
					oRes[nVarId] = val;
				}
			} // p
		} // it
	} // pvar
}
void Dataset::get_indivs_values(const intra::String &sigle,
		AnyIdMap &oRes) const {
	const Indiv *pInd = this->find_indiv_by_sigle(sigle);
	if (pInd != nullptr) {
		this->get_indivs_values(pInd, oRes);
	} else {
		oRes.clear();
	}
}
void Dataset::get_indivs_values(int nIndivId, AnyIdMap &oRes) const {
	const Indiv *pInd = this->find_indiv_by_id(nIndivId);
	if (pInd != nullptr) {
		this->get_indivs_values(pInd, oRes);
	} else {
		oRes.clear();
	}
}
//
void Dataset::get_variables_values(const Variable *pVar, AnyIdMap &oRes) const {
	oRes.clear();
	if (pVar != nullptr) {
		const ValueVector &oVec = this->m_vals;
		auto iend = oVec.end();
		int nId = pVar->id();
		for (auto it = oVec.begin(); it != iend; ++it) {
			PValue v = *it;
			const Value *p = v.get();
			if ((p != nullptr) && p->is_valid()) {
				int nIndivId = p->indiv_id();
				if ((nId != 0) && (p->variable_id() == nId)) {
					boost::any val = p->value();
					oRes[nIndivId] = val;
				} else {
					const Variable *ppv = p->variable();
					if (ppv == pVar) {
						boost::any val = p->value();
						oRes[nIndivId] = val;
					}
				}
			} // p
		} // it
	} // pvar
}
void Dataset::get_variables_values(const intra::String &sigle,
		AnyIdMap &oRes) const {
	const Variable *pVar = this->find_variable_by_sigle(sigle);
	if (pVar != nullptr) {
		this->get_variables_values(pVar, oRes);
	} else {
		oRes.clear();
	}
}
void Dataset::get_variables_values(int nVarId, AnyIdMap &oRes) const {
	const Variable *pVar = this->find_variable_by_id(nVarId);
	if (pVar != nullptr) {
		this->get_variables_values(pVar, oRes);
	} else {
		oRes.clear();
	}
}
//////////////////////////////////////
void Dataset::get_variables(std::vector<PVariable> &oVec) const {
	oVec.clear();
	const VariableMap &oMap = this->m_vars;
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		PVariable v = (*it).second;
		oVec.push_back(v);
	} // it
} // getVariables
void Dataset::get_variables_ids(std::vector<int> &oVec) const {
	oVec.clear();
	const VariableMap &oMap = this->m_vars;
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		PVariable v = (*it).second;
		const Variable *p = v.get();
		if (p != nullptr) {
			int nId = p->id();
			oVec.push_back(nId);
		}
	} // it
} // get_variables_ids
void Dataset::get_variables_sigles(std::vector<String> &oVec) const {
	oVec.clear();
	const VariableMap &oMap = this->m_vars;
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		PVariable v = (*it).second;
		const Variable *p = v.get();
		if (p != nullptr) {
			String s = p->sigle();
			oVec.push_back(s);
		}
	} // it
} // get_variables_sigles
void Dataset::get_indivs(std::vector<PIndiv> &oVec) const {
	oVec.clear();
	const IndivMap &oMap = this->m_inds;
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		PIndiv v = (*it).second;
		oVec.push_back(v);
	} // it
} // get_indivs
void Dataset::get_indivs_ids(std::vector<int> &oVec) const {
	oVec.clear();
	const IndivMap &oMap = this->m_inds;
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		PIndiv v = (*it).second;
		const Indiv *p = v.get();
		if (p != nullptr) {
			int nId = p->id();
			oVec.push_back(nId);
		}
	} // it
} // get_indivs_ids
void Dataset::get_indivs_sigles(std::vector<String> &oVec) const {
	oVec.clear();
	const IndivMap &oMap = this->m_inds;
	for (auto it = oMap.begin(); it != oMap.end(); ++it) {
		PIndiv v = (*it).second;
		const Indiv *p = v.get();
		if (p != nullptr) {
			String s = p->sigle();
			oVec.push_back(s);
		}
	} // it
} // get_indivs_ids
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
	return oVal.get();
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
	return oVal.get();
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
