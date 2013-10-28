/*
 * baseItem.h
 *
 *  Created on: 25 oct. 2013
 *      Author: boubad
 */

#ifndef BASEITEM_H_
#define BASEITEM_H_
///////////////////////////////////
#include "port.h"
/////////////////////////////////
namespace intra {
///////////////////////////////////////
class BaseItem {
protected:
	BaseItem();
	BaseItem(const BaseItem &other);
	BaseItem & operator=(const BaseItem &other);
public:
	virtual ~BaseItem();
public:
	bool operator==(const BaseItem &other) const {
		return (this->m_id == other.m_id);
	}
	bool operator<(const BaseItem &other) const {
		return (this->m_id < other.m_id);
	}
public:
	virtual bool is_writeable(void) const;
	virtual bool is_removeable(void) const;
	virtual bool is_updateable(void) const;
public:
	inline int id(void) const {
		return (this->m_id);
	}
	inline void id(int n) {
		this->m_id = n;
	}
	inline int version(void) const {
		return (this->m_version);
	}
	inline void version(int n) {
		this->m_version = n;
	}
	inline bool is_changed(void) const {
		return (this->m_changed);
	}
	inline void is_changed(bool b) {
		this->m_changed = b;
	}
public:
	virtual OStream & writeTo(OStream &os) const;
private:
	int m_id;
	int m_version;
	bool m_changed;
};
///////////////////////////////
} /* namespace intra */
////////////////////////////////////////
inline std::ostream & operator<<(std::ostream &os, const intra::BaseItem &d) {
	return (d.writeTo(os));
}
////////////////////////////////////////
#endif /* BASEITEM_H_ */
