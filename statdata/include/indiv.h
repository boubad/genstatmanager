/*
 * indiv.h
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */

#ifndef INDIV_H_
#define INDIV_H_
/////////////////////////////
#include "datasetchild.h"
///////////////////////////////
namespace intra {
//////////////////////////////
class Indiv : public DatasetChild {
public:
	Indiv();
	Indiv(Dataset &oSet);
	Indiv(Dataset *pSet);
	Indiv(const Indiv &other);
	Indiv & operator=(const Indiv &other);
	virtual ~Indiv();
public:
	bool operator==(const Indiv &other) const;
	bool operator<(const Indiv &other) const;
public:
	inline const String & status(void) const {
		return (this->m_status);
	}
	void status(const String &s);
protected:
	virtual void post_change_sigle(const String &oldSigle, const String &newSigle);
private:
	String m_status;
};// class Indiv
////////////////////////////////
}// namespace intra
////////////////////////////////
#endif /* INDIV_H_ */
