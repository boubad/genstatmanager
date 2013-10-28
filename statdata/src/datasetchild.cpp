/*
 * datasetchild.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */
#include "datasetchild.h"
///////////////////////////////
namespace intra {
/////////////////////////////////////
DatasetChild::DatasetChild() :
		m_datasetid(0), m_pset(nullptr) {

}
DatasetChild::DatasetChild(Dataset &oSet) :
		m_datasetid(oSet.id()), m_pset(&oSet) {

}
DatasetChild::DatasetChild(Dataset *pSet) :
		m_datasetid(0), m_pset(pSet) {
	if (pSet != nullptr) {
		this->m_datasetid = pSet->id();
	}
}
DatasetChild::DatasetChild(const DatasetChild &other) :
		NamedItem(other), m_datasetid(other.m_datasetid), m_pset(other.m_pset) {

}
DatasetChild & DatasetChild::operator=(const DatasetChild &other) {
	if (this != &other) {
		NamedItem::operator=(other);
		this->m_datasetid = other.m_datasetid;
		this->m_pset = other.m_pset;
	}
	return (*this);
}
DatasetChild::~DatasetChild() {

}
bool DatasetChild::operator==(const DatasetChild &other) const {
	if ((this->id() != 0) && (this->id() == other.id())) {
		return (true);
	}
	if (this->m_datasetid != other.m_datasetid) {
		return (false);
	}
	return (NamedItem::operator==(other));
}
bool DatasetChild::operator<(const DatasetChild &other) const {
	if (this->m_datasetid < other.m_datasetid) {
		return (true);
	} else if (this->m_datasetid > other.m_datasetid) {
		return (false);
	}
	return (NamedItem::operator<(other));
}
bool DatasetChild::is_writeable(void) const {
	return ((this->m_datasetid != 0) && NamedItem::is_writeable());
}
OStream & DatasetChild::writeTo(OStream &os) const {
	return NamedItem::writeTo(os);
}
////////////////////////////////////////
}// namespace intra
