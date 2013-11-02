/*
 * variable.h
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_
////////////////////////////
#include "datasetchild.h"
///////////////////////////////
namespace intra {
////////////////////////////////////
template<class TSTRING>
class Variable: public DatasetChild<TSTRING> {
public:
	typedef TSTRING String;
private:
	bool m_categ;
	String m_type;
	String m_genre;
public:
	Variable() :m_categ(false) {
	}
	Variable(const Variable<TSTRING> &other):DatasetChild<TSTRING>(other),m_categ(other.m_categ),
			m_type(other.m_type),m_genre(other.m_genre){
	}
	Variable<TSTRING> & operator=(const Variable<TSTRING> &other):DatasetChild<TSTRING>(other),
			m_categ(other.m_categ),m_type(other.m_type),m_genre(other.m_genre){
	}
	virtual ~Variable(){
	}
public:
	bool operator==(const Variable &other) const;
	bool operator<(const Variable &other) const;
public:
	virtual bool is_writeable(void) const;
public:
	inline bool is_categvar(void) const {
		return (this->m_categ);
	}
	void is_categvar(bool b);
	inline const String & vartype(void) const {
		return (this->m_type);
	}
	void vartype(const String &s);
	inline const String & genre(void) const {
		return (this->m_genre);
	}
	void genre(const String &s);
protected:
	virtual void post_change_sigle(const String &oldSigle,
			const String &newSigle);

};
// class Variable
///////////////////////////////////
}// namespace intra
#endif /* VARIABLE_H_ */
