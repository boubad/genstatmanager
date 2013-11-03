/*
 * value.h
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */

#ifndef VALUE_H_
#define VALUE_H_
///////////////////////////
#include <cassert>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
//////////////////////////////////
#include <boost/any.hpp>
/////////////////////////////////
#include "baseitem.h"
/////////////////////////////////////////
namespace intra {
/////////////////////////////////////
class Value: public BaseItem {
public:
	typedef std::shared_ptr<Value> PValue;
private:
	int m_variableid;
	int m_individ;
	boost::any m_val;
public:
	Value();
	Value(const Value &other);
	Value & operator=(const Value &other);
	virtual ~Value();
public:
	bool operator==(const Value &other) const;
	bool operator<(const Value &other) const;
public:
	virtual bool is_writeable(void) const;
public:
	inline int variable_id(void) const {
		return (this->m_variableid);
	}
	inline void variable_id(int n) {
		this->m_variableid = n;
	}
	inline int indiv_id(void) const {
		return (this->m_individ);
	}
	inline void indiv_id(int n) {
		this->m_individ = n;
	}
	inline const boost::any & value(void) const {
		return (this->m_val);
	}
	void value(const boost::any &s);
public:
	inline void value(bool bVal) {
		this->m_val = boost::any(bVal);
	}
	inline void value(int v) {
		this->m_val = boost::any(v);
	}
	inline void value(float v) {
		this->m_val = boost::any(v);
	}
	inline void value(double v) {
		this->m_val = boost::any(v);
	}
	inline void value(const std::string &v) {
		this->m_val = boost::any(v);
	}
	inline void value(const std::wstring &v) {
		this->m_val = boost::any(v);
	}
	inline bool empty(void) const {
		return (this->m_val.empty());
	}
	inline bool is_valid(void) const {
		return (!this->m_val.empty());
	}
public:
	bool bool_value(void) const;
	short short_value(void) const;
	int int_value(void) const;
	float float_value(void) const;
	double double_value(void) const;
	bool string_value(std::string &s) const;
	bool string_value(std::wstring &s) const;
};
// class Value
///////////////////////////////////////////
}// namespace intra
////////////////////////////////////
#endif /* VALUE_H_ */
