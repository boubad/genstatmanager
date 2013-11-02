/*
 * datasetchild.h
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */

#ifndef DATASETCHILD_H_
#define DATASETCHILD_H_
///////////////////////////////
#include "nameditem.h"
/////////////////////////////
namespace intra {
////////////////////////////////
template <class TSTRING>
class DatasetChild: public NamedItem<TSTRING> {
protected:
	DatasetChild():m_datasetid(0){
	}
	DatasetChild<TSTRING> & operator=(const DatasetChild<TSTRING> &other) {
		if (this != &other){
			NamedItem<TSTRING>::operator=(other);
			this->m_datasetid = other.m_datasetid;
		}
		return (*this);
	}
public:
	virtual ~DatasetChild(){
	}
public:
	bool operator==(const DatasetChild<TSTRING> &other) const {
		if (this->m_datasetid != other.m_datasetid){
			return (false);
		}
		return (NamedItem<TSTRING>::operator==(other));
	}
	bool operator<(const DatasetChild<TSTRING> &other) const {
		return (NamedItem<TSTRING>::operator=(other));
	}
public:
	virtual bool is_writeable(void) const {
		return ((this->m_datasetid != 0) && NamedItem<TSTRING>::is_writeable());
	}
public:
	inline int dataset_id(void) const {
		return (this->m_datasetid);
	}
	inline void dataset_id(int n) {
		this->m_datasetid = n;
	}
private:
	int m_datasetid;
};
// class DatasetChild
//////////////////////////////////
}// namespace intra
/////////////////////////////////
#endif /* DATASETCHILD_H_ */
