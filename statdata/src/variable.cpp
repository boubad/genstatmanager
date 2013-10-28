/*
 * variable.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */
///////////////////////////////////////
#include <boost/algorithm/string.hpp>
////////////////////////////////////////
#include "variable.h"
#include "dataset.h"
#include "value.h"
/////////////////////////////
namespace intra {
////////////////////////////////////
void Variable::post_change_sigle(const String &oldSigle,
		const String &newSigle) {
	typedef Dataset::VariableMap VariableMap;
	Dataset *pSet = this->dataset();
	if (pSet == nullptr) {
		return;
	}
	String sOld = boost::to_upper_copy(boost::trim_copy(oldSigle));
	String sNew = boost::to_upper_copy(boost::trim_copy(newSigle));
	VariableMap &oMap = pSet->variables();
	auto it1 = oMap.find(sOld);
	PVariable oVar;
	if (it1 != oMap.end()) {
		oVar = (*it1).second;
		oMap.erase(it1);
	}
	auto it2 = oMap.find(sNew);
	if (it2 != oMap.end()) {
		oMap.erase(it2);
	}
	if (oVar.get() == nullptr) {
		oVar = std::make_shared < Variable > (*this);
	}
	oMap[sNew] = oVar;
} //post_change_sigle
///////////////////////////////////
Variable::Variable() :
		m_categ(false) {

}
Variable::Variable(Dataset &oSet) :
		DatasetChild(oSet), m_categ(false) {

}
Variable::Variable(Dataset *pSet) :
		DatasetChild(pSet), m_categ(false) {

}
Variable::Variable(const Variable &other) :
		DatasetChild(other), m_categ(other.m_categ), m_type(other.m_type), m_genre(
				other.m_genre) {

}
Variable & Variable::operator=(const Variable &other) {
	if (this != &other) {
		DatasetChild::operator=(other);
		this->m_categ = other.m_categ;
		this->m_type = other.m_type;
		this->m_genre = other.m_genre;
	}
	return (*this);
}
bool Variable::operator==(const Variable &other) const {
	return (DatasetChild::operator ==(other));
}
bool Variable::operator<(const Variable &other) const {
	return (DatasetChild::operator<(other));
}
bool Variable::is_writeable(void) const {
	return ((!this->m_type.empty()) && DatasetChild::is_writeable());
}
void Variable::is_categvar(bool b){
	if (this->m_categ != b){
		this->m_categ = b;
		this->is_changed(true);
	}
}
void Variable::vartype(const String &s) {
	String ss = boost::trim_copy(s);
	if (ss.length() > 15) {
		ss = boost::trim_copy(ss.substr(0, 15));
	}
	String old = boost::to_upper_copy(this->m_type);
	String sx = boost::to_upper_copy(ss);
	if (sx != old) {
		this->m_type = ss;
		this->is_changed(true);
	}
} // type
void Variable::genre(const String &s) {
	String ss = boost::trim_copy(s);
	if (ss.length() > 15) {
		ss = boost::trim_copy(ss.substr(0, 15));
	}
	String old = boost::to_upper_copy(this->m_genre);
	String sx = boost::to_upper_copy(ss);
	if (sx != old) {
		this->m_genre = ss;
		this->is_changed(true);
	}
} // genre
Variable::~Variable() {
}
//////////////////////////////////
}// namespace intra

