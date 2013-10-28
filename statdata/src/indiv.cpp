/*
 * indiv.cpp
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */
///////////////////////////////////////
#include <boost/algorithm/string.hpp>
#include <list>
////////////////////////////////////////
#include "indiv.h"
#include "dataset.h"
#include "value.h"
//////////////////////////////////////////
namespace intra {
/////////////////////////////////////
void Indiv::post_change_sigle(const String &oldSigle, const String &newSigle) {
	typedef Dataset::IndivMap IndivMap;
	Dataset *pSet = this->dataset();
	if (pSet == nullptr) {
		return;
	}
	String sOld = boost::to_upper_copy(boost::trim_copy(oldSigle));
	String sNew = boost::to_upper_copy(boost::trim_copy(newSigle));
	IndivMap &oMap = pSet->indivs();
	auto it1 = oMap.find(sOld);
	PIndiv oInd;
	if (it1 != oMap.end()) {
		oInd = (*it1).second;
		oMap.erase(it1);
	}
	auto it2 = oMap.find(sNew);
	if (it2 != oMap.end()) {
		oMap.erase(it2);
	}
	if (oInd.get() == nullptr) {
		oInd = std::make_shared < Indiv > (*this);
	}
	oMap[sNew] = oInd;
} //post_change_sigle
/////////////////////////////////
Indiv::Indiv() {

}
Indiv::Indiv(Dataset &oSet) :
		DatasetChild(oSet) {

}
Indiv::Indiv(Dataset *pSet) :
		DatasetChild(pSet) {

}
Indiv::Indiv(const Indiv &other) :
		DatasetChild(other), m_status(other.m_status) {

}
Indiv & Indiv::operator=(const Indiv &other) {
	if (this != &other) {
		DatasetChild::operator=(other);
	}
	return (*this);
}
void Indiv::status(const String &s) {
	String ss = boost::trim_copy(s);
	if (ss.length() > 15) {
		ss = boost::trim_copy(ss.substr(0, 15));
	}
	String old = boost::to_upper_copy(this->m_status);
	String sx = boost::to_upper_copy(ss);
	if (sx != old) {
		this->m_status = ss;
		this->is_changed(true);
	}
} // sigle
Indiv::~Indiv() {

}
///////////////////////////
}// namespace intra
