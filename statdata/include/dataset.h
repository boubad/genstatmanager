/*
 * dataset.h
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */

#ifndef DATASET_H_
#define DATASET_H_
//////////////////////////////
#include <map>
#include <vector>
#include <memory>
//////////////////////////////
#include "nameditem.h"
///////////////////////////////
namespace intra {
/////////////////////////////////////
class Variable;
class Indiv;
class Value;
///////////////////////////////////
typedef std::shared_ptr<Variable> PVariable;
typedef std::shared_ptr<Indiv> PIndiv;
typedef std::shared_ptr<Value> PValue;
///////////////////////////////////
class Dataset: public NamedItem {
public:
	typedef std::map<String, PVariable> VariableMap;
	typedef std::map<String, PIndiv> IndivMap;
	typedef std::vector<PValue> ValueVector;
public:
	Dataset();
	Dataset(const Dataset &other);
	Dataset & operator=(const Dataset &other);
	virtual ~Dataset();
public:
	bool operator==(const Dataset &other) const;
	bool operator<(const Dataset &other) const;
public:
	inline const VariableMap & variables(void) const {
		return (this->m_vars);
	}
	inline VariableMap & variables(void) {
		return (this->m_vars);
	}
	inline const IndivMap & indivs(void) const {
		return (this->m_inds);
	}
	inline IndivMap & indivs(void) {
		return (this->m_inds);
	}
	inline const ValueVector & values(void) const {
		return (this->m_vals);
	}
	inline ValueVector & values(void) {
		return (this->m_vals);
	}
public:
	const Variable *find_variable_by_sigle(const String &s) const;
	const Variable *find_variable_by_id(int nVarId) const;
	const Indiv *find_indiv_by_sigle(const String &s) const;
	const Indiv *find_indiv_by_id(int nIndId) const;
	const Value *find_value_by_variable_indiv(const String &sigleVar,
			const String &sigleInd) const;
	const Value *find_value_by_variable_indiv(int nVarId, int nIndId) const;
	const Value *find_value_by_variable_indiv(const Variable *pVar,
			const Indiv *pInd) const;
public:
	Variable *create_variable(const String &sigle);
	Variable *create_variable(int nVarId);
	Indiv *create_indiv(const String &sigle);
	Indiv *create_indiv(int nIndId);
	Value *create_value(const String &sigleVar, const String &sigleInd);
	Value *create_value(int nVarId, int nIndId);
private:
	VariableMap m_vars;
	IndivMap m_inds;
	ValueVector m_vals;
};
// class Dataset
//////////////////////////////////
}// namespace intre
////////////////////////////
#endif /* DATASET_H_ */
