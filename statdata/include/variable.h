/*
 * variable.h
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_
////////////////////////////
#include "value.h"
#include "datasetchild.h"
///////////////////////////////
namespace intra {
/////////////////////////////////////
template<class TSTRING>
class Variable: public DatasetChild<TSTRING> {
public:
	typedef TSTRING String;
	typedef Value ValueType;
	typedef Value::PValue PValue;
	typedef Variable<TSTRING> VariableType;
	typedef std::shared_ptr<VariableType> PVariable;
	typedef std::vector<PValue> ValueVector;
	typedef std::vector<VariableType> VariableVector;
private:
	bool m_categ;
	String m_type;
	String m_genre;
	ValueVector m_vals;
public:
	inline const ValueVector & values(void) const {
		return (this->m_vals);
	}
	inline ValueVector & values(void) {
		return (this->m_vals);
	}
public:
	Variable() :
			m_categ(false) {
	}
	Variable(const Variable<TSTRING> &other) :
			DatasetChild<TSTRING>(other), m_categ(other.m_categ), m_type(
					other.m_type), m_genre(other.m_genre), m_vals(other.m_vals) {
	}
	Variable<TSTRING> & operator=(const Variable<TSTRING> &other) {
		if (this != &other) {
			DatasetChild<TSTRING>::operator=(other);
			this->m_categ = other.m_categ;
			this->m_type = other.m_type;
			this->m_genre = other.m_genre;
			this->m_vals = other.m_genre;
		}
		return (*this);
	}
	virtual ~Variable() {
	}
public:
	bool operator==(const Indiv<TSTRING> &other) const {
		return (DatasetChild<TSTRING>::operator==(other));
	}
	bool operator<(const Indiv<TSTRING> &other) const {
		return (DatasetChild<TSTRING>::operator<(other));
	}
public:
	virtual bool is_writeable(void) const {
		return ((!this->m_type.empty()) && DatasetChild<TSTRING>::is_writeable());
	}
public:
	inline bool is_categvar(void) const {
		return (this->m_categ);
	}
	void is_categvar(bool b) {
		if (this->m_categ != b) {
			this->m_categ = b;
			this->is_changed(true);
		}
	}
	inline const String & vartype(void) const {
		return (this->m_type);
	}
	void vartype(const String &s) {
		String ss = trim(s);
		if (ss.length() > 15) {
			ss = trim(ss.substr(0, 15));
		}
		String old = to_upper(this->m_type);
		String sx = to_upper(ss);
		if (sx != old) {
			this->m_type = ss;
			this->is_changed(true);
		}
	}
	inline const String & genre(void) const {
		return (this->m_genre);
	}
	void genre(const String &s) {
		String ss = trim(s);
		if (ss.length() > 15) {
			ss = trim(ss.substr(0, 15));
		}
		String old = to_upper(this->m_genre);
		String sx = to_upper(ss);
		if (sx != old) {
			this->m_genre = ss;
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
// class Variable
///////////////////////////////////
}// namespace intra
#endif /* VARIABLE_H_ */
