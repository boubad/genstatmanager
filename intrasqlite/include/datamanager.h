/*
 * datamanager.h
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */

#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_
////////////////////////////
#include "statdatamanager.h"
/////////////////////////
namespace intra {
////////////////////////////
extern bool process_data(const std::string &srcfilename,
		const std::string &databaseFilename, const std::string &datasetSigle,
		char delim = '\t', const std::string na = std::string("na"));
/////////////////////////////
}// intra
////////////////////////////////
#endif /* DATAMANAGER_H_ */
