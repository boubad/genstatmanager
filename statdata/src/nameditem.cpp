/*
 * nameditem.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */
///////////////////////////////////////
#include <boost/algorithm/string.hpp>
////////////////////////////////////////
#include "nameditem.h"
//////////////////////////////////////////
namespace intra {
/////////////////////////////////
NamedItem::NamedItem() {

}
NamedItem::NamedItem(const NamedItem &other) :
		BaseItem(other), m_sigle(other.m_sigle), m_name(other.m_name), m_desc(
				other.m_desc) {

}
NamedItem & NamedItem::operator=(const NamedItem &other) {
	if (this != &other) {
		BaseItem::operator=(other);
		this->m_sigle = other.m_sigle;
		this->m_name = other.m_name;
		this->m_desc = other.m_desc;
	}
	return (*this);
}
NamedItem::~NamedItem() {

}
bool NamedItem::operator==(const NamedItem &other) const {
	if ((this->id() != 0) && (this->id() == other.id())) {
		return (true);
	}
	return (this->m_sigle == other.m_sigle);
}
bool NamedItem::operator<(const NamedItem &other) const {
	return (this->m_sigle < other.m_sigle);
}
void NamedItem::sigle(const String &s) {
	String ss = boost::trim_copy(s);
	if (ss.length() > 31) {
		ss = boost::trim_copy(ss.substr(0, 31));
	}
	String old = boost::to_upper_copy(this->m_sigle);
	String sx = boost::to_upper_copy(ss);
	if (sx != old) {
		this->m_sigle = ss;
		this->post_change_sigle(old,ss);
		this->is_changed(true);
	}
} // sigle
void NamedItem::name(const String &s) {
	String ss = boost::trim_copy(s);
	if (ss.length() > 63) {
		ss = boost::trim_copy(ss.substr(0, 63));
	}
	String old = boost::to_upper_copy(this->m_name);
	String sx = boost::to_upper_copy(ss);
	if (sx != old) {
		this->m_name = ss;
		this->is_changed(true);
	}
} // name
void NamedItem::description(const String &s) {
	String ss = boost::trim_copy(s);
	if (ss.length() > 255) {
		ss = boost::trim_copy(ss.substr(0, 255));
	}
	String old = boost::to_upper_copy(this->m_desc);
	String sx = boost::to_upper_copy(ss);
	if (sx != old) {
		this->m_desc = ss;

		this->is_changed(true);
	}
} // description
bool NamedItem::is_writeable(void) const {
	return ((!this->m_sigle.empty()));
}
OStream & NamedItem::writeTo(OStream &os) const {
	os << this->m_sigle;
	return os;
}
void NamedItem::post_change_sigle(const String &oldSigle, const String &newSigle){

}
////////////////////////////////////////
}// namespace intra

