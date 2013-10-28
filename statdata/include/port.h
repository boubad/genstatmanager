/*
 * port.h
 *
 *  Created on: 27 oct. 2013
 *      Author: boubad
 */

#ifndef PORT_H_
#define PORT_H_
/////////////////////////////
#include <cassert>
#include <string>
#include <ostream>
#include <sstream>
#include <fstream>
//////////////////////////////////
namespace intra {
/////////////////////////////////////
typedef std::string String;
typedef std::istream IStream;
typedef std::ostream OStream;
typedef std::stringstream StringStream;
typedef std::ifstream IFStream;
typedef std::ofstream OFStream;
/////////////////////////////////////
}// namespace intra
//////////////////////////////
#endif /* PORT_H_ */
