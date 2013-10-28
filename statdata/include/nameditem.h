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
typedef std::string String;
//////////////////////////////////
class NamedItem: public BaseItem {
protected:
	NamedItem();
	NamedItem(const NamedItem &other);
	NamedItem & operator=(const NamedItem &other);
public:
	virtual ~NamedItem();
public:
	bool operator==(const NamedItem &other) const;
	bool operator<(const NamedItem &other) const;
public:
	inline const String & sigle(void) const {
		return (this->m_sigle);
	}
	void sigle(const String &s);
	inline const String & name(void) const {
		return (this->m_name);
	}
	void name(const String &s);
	inline const String & description(void) const {
		return (this->m_desc);
	}
	void description(const String &s);
public:
	virtual bool is_writeable(void) const;
	virtual OStream & writeTo(OStream &os) const;
protected:
	virtual void post_change_sigle(const String &oldSigle, const String &newSigle);
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
