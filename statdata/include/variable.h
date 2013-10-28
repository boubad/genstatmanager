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
class Variable : public DatasetChild {
public:
	Variable();
	Variable(Dataset &oSet);
	Variable(Dataset *pSet);
	Variable(const Variable &other);
	Variable & operator=(const Variable &other);
	virtual ~Variable();
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
	virtual void post_change_sigle(const String &oldSigle, const String &newSigle);
private:
	bool m_categ;
	String m_type;
	String m_genre;
};// class Variable
///////////////////////////////////
}// namespace intra
#endif /* VARIABLE_H_ */
