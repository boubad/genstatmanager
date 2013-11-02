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
template<class TSTRING>
class Indiv: public DatasetChild<TSTRING> {
public:
	typedef TSTRING String;
public:
	Indiv() {
	}
	template<class TSTRING, class ALLOCVECVAR, class ALLOCVECIND,
			class ALLOCVECVAL>
	Indiv(Dataset<TSTRING, ALLOCVECVAR, ALLOCVECIND, ALLOCVECVAL> &oSet) :
			DatasetChild(oSet) {
	}
	template<class TSTRING, class ALLOCVECVAR, class ALLOCVECIND,
			class ALLOCVECVAL>
	Indiv(Dataset<TSTRING, ALLOCVECVAR, ALLOCVECIND, ALLOCVECVAL> *pSet) :
			DatasetChild<TSTRING>(pSet) {
	}
	Indiv(const Indiv<TSTRING> &other) :
			DatasetChild<TSTRING>(other), m_status(other.m_status) {
	}
	Indiv<TSTRING> & operator=(const Indiv<TSTRING> &other) {
		if (this != &other) {
			DatasetChild<TSTRING>::operator=(other);
			this->m_status = other.m_status;
		}
		return (*this);
	}
	virtual ~Indiv() {
	}
public:
	bool operator==(const Indiv<TSTRING> &other) const {
		return (DatasetChild<TSTRING>::operator==(other));
	}
	bool operator<(const Indiv<TSTRING> &other) const {
		return (DatasetChild<TSTRING>::operator<(other));
	}
public:
	inline const String & status(void) const {
		return (this->m_status);
	}
	void status(const String &s) {
		String ss = trim(s);
		if (ss.length() > 15) {
			ss = trim(ss.substr(0, 15));
		}
		String old = to_upper(this->m_status);
		String sx = to_upper(ss);
		if (sx != old) {
			this->m_status = ss;
			this->is_changed(true);
		}
	}
private:
	String m_status;
};
// class Indiv
////////////////////////////////
}// namespace intra
////////////////////////////////
#endif /* INDIV_H_ */
