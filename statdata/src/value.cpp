/*
 * value.cpp
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */
///////////////////////////////////////
#include <boost/algorithm/string.hpp>
////////////////////////////////////////
#include "value.h"
#include "variable.h"
#include "indiv.h"
//////////////////////////////////////////
namespace intra {
///////////////////////////////////
Value::Value() :
		m_variableid(0), m_individ(0), m_pvar(nullptr), m_pind(nullptr) {
}
Value::Value(Variable *pVar, Indiv *pInd) :
		m_variableid(pVar->id()), m_individ(pInd->id()), m_pvar(pVar), m_pind(
				pInd) {
}
Value::Value(Variable &oVar, Indiv &oInd) :
		m_variableid(oVar.id()), m_individ(oInd.id()), m_pvar(&oVar), m_pind(
				&oInd) {
}
Value::Value(const Value &other) :
		BaseItem(other), m_variableid(other.m_variableid), m_individ(
				other.m_individ), m_pvar(other.m_pvar), m_pind(other.m_pind), m_val(
				other.m_val) {
}
Value & Value::operator=(const Value &other) {
	if (this != &other) {
		BaseItem::operator=(other);
		this->m_variableid = other.m_variableid;
		this->m_individ = other.m_individ;
		this->m_pvar = other.m_pvar;
		this->m_pind = other.m_pind;
		this->m_val = other.m_val;
	}
	return (*this);
}
bool Value::operator==(const Value &other) const {
	if ((this->id() != 0) && (this->id() == other.id())) {
		return (true);
	}
	return ((this->m_variableid == other.m_variableid)
			&& (this->m_individ == other.m_individ));
}
bool Value::operator<(const Value &other) const {
	if (this->m_individ < other.m_individ) {
		return true;
	} else if (this->m_individ > other.m_individ) {
		return false;
	}
	if (this->m_variableid < other.m_variableid) {
		return true;
	} else if (this->m_variableid > other.m_variableid) {
		return (false);
	}
	return (false);
}
bool Value::is_writeable(void) const {
	return ((this->m_variableid != 0) && (this->m_individ != 0));
}
void Value::indiv(Indiv *p) {
	this->m_pind = p;
	if (p != nullptr) {
		this->m_individ = p->id();
	} else {
		this->m_individ = 0;
	}
}
void Value::variable(Variable *p) {
	this->m_pvar = p;
	if (p != nullptr) {
		this->m_variableid = p->id();
	} else {
		this->m_variableid = 0;
	}
}
void Value::value(const boost::any &s) {
	this->m_val = s;
	this->is_changed(true);
}
Value::~Value() {
}
bool Value::bool_value(void) const {
	bool oRet = false;
	const boost::any &v = this->m_val;
	if (!v.empty()) {
		const std::type_info & t = v.type();
		if (t == typeid(bool)) {
			oRet = boost::any_cast<bool>(v);
		} else if (t == typeid(short)) {
			short x = boost::any_cast<short>(v);
			oRet = (x != 0) ? true : false;
		} else if (t == typeid(int)) {
			int x = boost::any_cast<int>(v);
			oRet = (x != 0) ? true : false;
		} else if (t == typeid(float)) {
			float x = boost::any_cast<float>(v);
			oRet = (x != 0.0f) ? true : false;
		} else if (t == typeid(double)) {
			double x = boost::any_cast<double>(v);
			oRet = (x != 0.0) ? true : false;
		} else if (t == typeid(std::string)) {
			std::string x = boost::any_cast<std::string>(v);
			std::string xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				auto c = *(xx.begin());
				oRet = (c == 't') || (c == '1') || (c == 'v') || (c == 'o');
			} // not empty
		} else if (t == typeid(std::wstring)) {
			std::wstring x = boost::any_cast<std::wstring>(v);
			std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				auto c = *(xx.begin());
				oRet = (c == L't') || (c == L'1') || (c == L'v') || (c == L'o');
			} // not empty
		}
	} // not empty
	return (oRet);
} // bool_value
short Value::short_value(void) const {
	short oRet = -1;
	const boost::any &v = this->m_val;
	if (!v.empty()) {
		const std::type_info & t = v.type();
		if (t == typeid(bool)) {
			bool b = boost::any_cast<bool>(v);
			oRet = (b) ? 1 : 0;
		} else if (t == typeid(short)) {
			short x = boost::any_cast<short>(v);
			oRet = x;
		} else if (t == typeid(int)) {
			int x = boost::any_cast<int>(v);
			oRet = (short) x;
		} else if (t == typeid(float)) {
			float x = boost::any_cast<float>(v);
			oRet = (short) x;
		} else if (t == typeid(double)) {
			double x = boost::any_cast<double>(v);
			oRet = (short) x;
		} else if (t == typeid(std::string)) {
			std::string x = boost::any_cast<std::string>(v);
			std::string xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				std::stringstream in(xx);
				in >> oRet;
			} // not empty
		} else if (t == typeid(std::wstring)) {
			std::wstring x = boost::any_cast<std::wstring>(v);
			std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				std::wstringstream in(xx);
				in >> oRet;
			} // not empty
		}
	} // not empty
	return (oRet);
} // short_value
int Value::int_value(void) const {
	int oRet = -1;
	const boost::any &v = this->m_val;
	if (!v.empty()) {
		const std::type_info & t = v.type();
		if (t == typeid(bool)) {
			bool b = boost::any_cast<bool>(v);
			oRet = (b) ? 1 : 0;
		} else if (t == typeid(short)) {
			short x = boost::any_cast<short>(v);
			oRet = (int) x;
		} else if (t == typeid(int)) {
			int x = boost::any_cast<int>(v);
			oRet = x;
		} else if (t == typeid(float)) {
			float x = boost::any_cast<float>(v);
			oRet = (int) x;
		} else if (t == typeid(double)) {
			double x = boost::any_cast<double>(v);
			oRet = (int) x;
		} else if (t == typeid(std::string)) {
			std::string x = boost::any_cast<std::string>(v);
			std::string xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				std::stringstream in(xx);
				in >> oRet;
			} // not empty
		} else if (t == typeid(std::wstring)) {
			std::wstring x = boost::any_cast<std::wstring>(v);
			std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				std::wstringstream in(xx);
				in >> oRet;
			} // not empty
		}
	} // not empty
	return (oRet);
} // int_value
float Value::float_value(void) const {
	float oRet = -1.0f;
	const boost::any &v = this->m_val;
	if (!v.empty()) {
		const std::type_info & t = v.type();
		if (t == typeid(bool)) {
			bool b = boost::any_cast<bool>(v);
			oRet = (b) ? 1.0f : 0.0f;
		} else if (t == typeid(short)) {
			short x = boost::any_cast<short>(v);
			oRet = (float) x;
		} else if (t == typeid(int)) {
			int x = boost::any_cast<int>(v);
			oRet = (float) x;
		} else if (t == typeid(float)) {
			float x = boost::any_cast<float>(v);
			oRet = x;
		} else if (t == typeid(double)) {
			double x = boost::any_cast<double>(v);
			oRet = (float) x;
		} else if (t == typeid(std::string)) {
			std::string x = boost::any_cast<std::string>(v);
			std::string xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				std::stringstream in(xx);
				in >> oRet;
			} // not empty
		} else if (t == typeid(std::wstring)) {
			std::wstring x = boost::any_cast<std::wstring>(v);
			std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				std::wstringstream in(xx);
				in >> oRet;
			} // not empty
		}
	} // not empty
	return (oRet);
} // float_value
double Value::double_value(void) const {
	double oRet = -1.0;
	const boost::any &v = this->m_val;
	if (!v.empty()) {
		const std::type_info & t = v.type();
		if (t == typeid(bool)) {
			bool b = boost::any_cast<bool>(v);
			oRet = (b) ? 1.0 : 0.0;
		} else if (t == typeid(short)) {
			short x = boost::any_cast<short>(v);
			oRet = (double) x;
		} else if (t == typeid(int)) {
			int x = boost::any_cast<int>(v);
			oRet = (double) x;
		} else if (t == typeid(float)) {
			float x = boost::any_cast<float>(v);
			oRet = (double) x;
		} else if (t == typeid(double)) {
			double x = boost::any_cast<double>(v);
			oRet = x;
		} else if (t == typeid(std::string)) {
			std::string x = boost::any_cast<std::string>(v);
			std::string xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				std::stringstream in(xx);
				in >> oRet;
			} // not empty
		} else if (t == typeid(std::wstring)) {
			std::wstring x = boost::any_cast<std::wstring>(v);
			std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
			if (!xx.empty()) {
				std::wstringstream in(xx);
				in >> oRet;
			} // not empty
		}
	} // not empty
	return (oRet);
} // double_value
bool Value::string_value(std::string &s) const {
	bool bRet = false;
	s.clear();
	const boost::any &v = this->m_val;
	if (!v.empty()) {
		const std::type_info & t = v.type();
		if (t == typeid(bool)) {
			bool b = boost::any_cast<bool>(v);
			s = (b) ? "True" : "False";
			bRet = true;
		} else if (t == typeid(short)) {
			short x = boost::any_cast<short>(v);
			std::stringstream os;
			os << x;
			s = os.str();
			bRet = true;
		} else if (t == typeid(int)) {
			int x = boost::any_cast<int>(v);
			std::stringstream os;
			os << x;
			s = os.str();
			bRet = true;
		} else if (t == typeid(float)) {
			float x = boost::any_cast<float>(v);
			std::stringstream os;
			os << x;
			s = os.str();
			bRet = true;
		} else if (t == typeid(double)) {
			double x = boost::any_cast<double>(v);
			std::stringstream os;
			os << x;
			s = os.str();
			bRet = true;
		} else if (t == typeid(std::string)) {
			std::string x = boost::any_cast<std::string>(v);
			s = x;
			bRet = true;
		} else if (t == typeid(std::wstring)) {
			std::wstring x = boost::any_cast<std::wstring>(v);
			std::string sx(x.length(), ' ');
			std::copy(x.begin(), x.end(), sx.begin());
			s = sx;
			bRet = true;
		}
	} // not empty
	return (bRet);
} // string_value
bool Value::string_value(std::wstring &s) const {
	bool bRet = false;
	s.clear();
	const boost::any &v = this->m_val;
	if (!v.empty()) {
		const std::type_info & t = v.type();
		if (t == typeid(bool)) {
			bool b = boost::any_cast<bool>(v);
			s = (b) ? L"True" : L"False";
			bRet = true;
		} else if (t == typeid(short)) {
			short x = boost::any_cast<short>(v);
			std::wstringstream os;
			os << x;
			s = os.str();
			bRet = true;
		} else if (t == typeid(int)) {
			int x = boost::any_cast<int>(v);
			std::wstringstream os;
			os << x;
			s = os.str();
			bRet = true;
		} else if (t == typeid(float)) {
			float x = boost::any_cast<float>(v);
			std::wstringstream os;
			os << x;
			s = os.str();
			bRet = true;
		} else if (t == typeid(double)) {
			double x = boost::any_cast<double>(v);
			std::wstringstream os;
			os << x;
			s = os.str();
			bRet = true;
		} else if (t == typeid(std::wstring)) {
			std::wstring x = boost::any_cast<std::wstring>(v);
			s = x;
			bRet = true;
		} else if (t == typeid(std::string)) {
			std::string x = boost::any_cast<std::string>(v);
			std::wstring sx(x.length(), L' ');
			std::copy(x.begin(), x.end(), sx.begin());
			s = sx;
			bRet = true;
		}
	} // not empty
	return (bRet);
} // string_value
/////////////////////////////////
} // namespace value
