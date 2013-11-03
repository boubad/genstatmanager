/*
 * nameditem.h
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */

#ifndef NAMEDITEM_H_
#define NAMEDITEM_H_
//////////////////////////////////////
#include <string>
//////////////////////////////////
#include "baseitem.h"
//////////////////////////////////////
namespace intra {
///////////////////////////////////
template <class TSTRING>
class NamedItem: public BaseItem {
public:
	typedef TSTRING String;
protected:
	NamedItem(){
	}
	NamedItem(const NamedItem<TSTRING> &other): BaseItem(other),m_sigle(other.m_sigle),m_name(other.m_name),
			m_desc(other.m_desc){
	}
	NamedItem<TSTRING> & operator=(const NamedItem<TSTRING> &other) {
		if (this != &other){
			BaseItem::operator=(other);
			this->m_sigle = other.m_sigle;
			this->m_name = other.m_name;
			this->m_desc = other.m_desc;
		}
		return (*this);
	}
public:
	virtual ~NamedItem(){
	}
public:
	bool operator==(const NamedItem &other) const {
		if ((this->id() != 0) && (this->id() == other.id())) {
				return (true);
			}
			return (this->m_sigle == other.m_sigle);
	}
	bool operator<(const NamedItem &other) const{
		return (this->m_sigle < other.m_sigle);
	}
public:
	inline const String & sigle(void) const {
		return (this->m_sigle);
	}
	inline const String & name(void) const {
		return (this->m_name);
	}
	inline const String & description(void) const {
		return (this->m_desc);
	}
	void sigle(const String &s) {
		String ss = trim(s);
		if (ss.length() > 31) {
			ss = trim(ss.substr(0, 31));
		}
		String old = to_upper(this->m_sigle);
		String sx = to_upper(ss);
		if (sx != old) {
			this->m_sigle = ss;
			this->is_changed(true);
		}
	} // sigle
	void name(const String &s) {
		String ss = trim(s);
		if (ss.length() > 63) {
			ss = trim(ss.substr(0, 63));
		}
		String old = to_upper(this->m_name);
		String sx = to_upper(ss);
		if (sx != old) {
			this->m_name = ss;
			this->is_changed(true);
		}
	} // name
	void description(const String &s) {
		String ss = trim(s);
		if (ss.length() > 255) {
			ss = trim(ss.substr(0, 255));
		}
		String old = to_upper(this->m_desc);
		String sx = to_upper(ss);
		if (sx != old) {
			this->m_desc = ss;
			this->is_changed(true);
		}
	} // description
	virtual bool is_writeable(void) const {
		return (!this->m_sigle.empty());
	}
private:
	String m_sigle;
	String m_name;
	String m_desc;
};
// class NamedItem
//////////////////////////
}// namespace intra
////////////////////////////////////
#endif /* NAMEDITEM_H_ */
