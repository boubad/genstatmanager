/*
 * datasetchild.h
 *
 *  Created on: 26 oct. 2013
 *      Author: boubad
 */

#ifndef DATASETCHILD_H_
#define DATASETCHILD_H_
///////////////////////////////
#include "dataset.h"
/////////////////////////////
namespace intra {
////////////////////////////////
class DatasetChild: public NamedItem {
protected:
	DatasetChild();
	DatasetChild(Dataset &oSet);
	DatasetChild(Dataset *pSet);
	DatasetChild(const DatasetChild &other);
	DatasetChild & operator=(const DatasetChild &other);
public:
	virtual ~DatasetChild();
public:
	bool operator==(const DatasetChild &other) const;
	bool operator<(const DatasetChild &other) const;
public:
	virtual bool is_writeable(void) const;
	virtual OStream & writeTo(OStream &os) const;
public:
	inline int dataset_id(void) const {
		return (this->m_datasetid);
	}
	inline void dataset_id(int n) {
		this->m_datasetid = n;
	}
	inline Dataset *dataset(void) const {
		return (this->m_pset);
	}
	inline void dataset(Dataset *pset) {
		this->m_pset = pset;
		if (pset != nullptr) {
			this->m_datasetid = pset->id();
		}
	}
private:
	int m_datasetid;
	Dataset *m_pset;
};
// class DatasetChild
//////////////////////////////////
}// namespace intra
/////////////////////////////////
#endif /* DATASETCHILD_H_ */
