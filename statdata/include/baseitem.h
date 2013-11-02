/*
 * baseItem.h
 *
 *  Created on: 25 oct. 2013
 *      Author: boubad
 */

#ifndef BASEITEM_H_
#define BASEITEM_H_
///////////////////////////////////
#include <cassert>
#include <string>
#include <ostream>
#include <sstream>
#include <fstream>
///////////////////////////////////////
#include <boost/algorithm/string.hpp>
////////////////////////////////////////
namespace intra {
///////////////////////////////////////////
template<class TSTRING>
inline TSTRING trim(const TSTRING &s) {
	return (boost::trim_copy(s));
}
template<class TSTRING>
inline TSTRING to_upper(const TSTRING &s) {
	return (boost::to_upper_copy(s));
}
///////////////////////////////////////
class BaseItem {
protected:
	BaseItem() :
			m_id(0), m_version(1), m_changed(false) {
	}
	BaseItem(const BaseItem &other) :
			m_id(other.m_id), m_version(other.m_version), m_changed(
					other.m_changed) {
	}
	BaseItem & operator=(const BaseItem &other) {
		if (this != &other) {
			this->m_id = other.m_id;
			this->m_version = other.m_version;
			this->m_changed = other.m_changed;
		}
		return (*this);
	}
public:
	virtual ~BaseItem() {
	}
public:
	bool operator==(const BaseItem &other) const {
		return (this->m_id == other.m_id);
	}
	bool operator<(const BaseItem &other) const {
		return (this->m_id < other.m_id);
	}
public:
	virtual bool is_writeable(void) const {
		return (false);
	}
public:
	inline bool is_removeable(void) const {
		return (this->id() != 0);
	}
	inline bool is_updateable(void) const {
		return (this->is_writeable() && this->is_removeable());
	}
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
private:
	int m_id;
	int m_version;
	bool m_changed;
};
///////////////////////////////
} /* namespace intra */
////////////////////////////////////////
#endif /* BASEITEM_H_ */
