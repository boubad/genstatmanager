/*
 * baseItem.cpp
 *
 *  Created on: 25 oct. 2013
 *      Author: boubad
 */

#include "baseitem.h"
////////////////////////////////////
namespace intra {
//////////////////////////////////////////
BaseItem::BaseItem() :
		m_id(0), m_version(1), m_changed(false) {
}
BaseItem::BaseItem(const BaseItem &other) :
		m_id(other.m_id), m_version(other.m_version), m_changed(other.m_changed) {

}
BaseItem & BaseItem::operator=(const BaseItem &other) {
	if (this != &other) {
		this->m_id = other.m_id;
		this->m_version = other.m_version;
		this->m_changed = other.m_changed;
	}
	return (*this);
}
BaseItem::~BaseItem() {
}
bool BaseItem::is_writeable(void) const {
	return (false);
}
bool BaseItem::is_removeable(void) const {
	return (this->m_id != 0);
}
bool BaseItem::is_updateable(void) const {
	return (this->is_removeable() && this->is_writeable());
}
OStream & BaseItem::writeTo(OStream &os) const {
	os << this->m_id;
	return (os);
}
////////////////////////////
} /* namespace intra */
