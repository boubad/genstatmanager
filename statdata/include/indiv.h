/*
 * indiv.h
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */

#ifndef INDIV_H_
#define INDIV_H_
////////////////////////////////
#include "value.h"
#include "datasetchild.h"
///////////////////////////////
namespace intra {
/////////////////////////////////
template<class TSTRING>
class Indiv: public DatasetChild<TSTRING> {
public:
	typedef TSTRING String;
	typedef Value ValueType;
	typedef Value::PValue PValue;
	typedef Indiv<TSTRING> IndivType;
	typedef std::shared_ptr<IndivType> PIndiv;
	typedef std::vector<PValue> ValueVector;
	typedef std::vector<IndivType> IndivVector;
private:
	String m_status;
	ValueVector m_vals;
public:
	inline const ValueVector & values(void) const {
		return (this->m_vals);
	}
	inline ValueVector & values(void) {
		return (this->m_vals);
	}
public:
	Indiv() {
	}
	Indiv(const Indiv<TSTRING> &other) :
			DatasetChild<TSTRING>(other), m_status(other.m_status), m_vals(
					other.m_vals) {
	}
	Indiv<TSTRING> & operator=(const Indiv<TSTRING> &other) {
		if (this != &other) {
			DatasetChild<TSTRING>::operator=(other);
			this->m_status = other.m_status;
			this->m_vals = other.m_vals;
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
	template<class ALLOCVEC>
	size_t get_values(std::vector<PValue, ALLOCVEC> &oRes) const {
		oRes.clear();
		const ValueVector &oVec = this->m_vals;
		oRes = std::vector<PValue, ALLOCVEC>(oVec.begin(), oVec.end());
		return oRes.size();
	} // getValue
	template<class COMPARE, class ALLOCPAIR>
	size_t get_values(
			std::map<int, boost::any, COMPARE, ALLOCPAIR> &oMap) const {
		oMap.clear();
		const ValueVector &oVec = this->m_vals;
		auto iend = oVec.end();
		std::for_each(oVec.begin(), iend, [&](const PValue &oVal) {
			const ValueType *p = oVal.get();
			if (p != nullptr) {
				int nId = p->variable_id();
				if (nId != 0) {
					boost::any v = p->value();
					oMap[nId] = v;
				}
			}
		});
		return oMap.size();
	} // getValue_notnull
	template<class ALLOCVEC>
	size_t get_values_notnull(std::vector<PValue, ALLOCVEC> &oRes) const {
		oRes.clear();
		const ValueVector &oVec = this->m_vals;
		auto iend = oVec.end();
		std::for_each(oVec.begin(), iend, [&](const PValue &oVal) {
			const ValueType *p = oVal.get();
			if ((p != nullptr) && p->is_valid()) {
				PValue o = oVal;
				oRes.push_back(o);
			}
		});
		return oRes.size();
	} // getValue_notnull
	template<class COMPARE, class ALLOCPAIR>
	size_t get_values_notnull(
			std::map<int, boost::any, COMPARE, ALLOCPAIR> &oMap) const {
		oMap.clear();
		const ValueVector &oVec = this->m_vals;
		auto iend = oVec.end();
		std::for_each(oVec.begin(), iend, [&](const PValue &oVal) {
			const ValueType *p = oVal.get();
			if ((p != nullptr) && p->is_valid()) {
				int nId = p->variable_id();
				if (nId != 0) {
					boost::any v = p->value();
					if (!v.empty()) {
						oMap[nId] = v;
					}
				}
			}
		});
		return oMap.size();
	} // getValue_notnull
};
// class Indiv
////////////////////////////////
}// namespace intra
////////////////////////////////
#endif /* INDIV_H_ */
