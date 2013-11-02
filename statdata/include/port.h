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
#ifndef INTRA_USE_WSTRING
	typedef char Char;
	typedef std::string String;
	typedef std::istream IStream;
	typedef std::ostream OStream;
	typedef std::stringstream StringStream;
	typedef std::ifstream IFStream;
	typedef std::ofstream OFStream;
#define INTRA_DEFAULT_NA	"na"
#define INTRA_DEFAULT_DELIM '\t'
#define INTRA_COMMA	 ','
#define INTRA_TAB	'\t'
#else
	typedef wchar_t Char;
	typedef std::wstring String;
	typedef std::wistream IStream;
	typedef std::wostream OStream;
	typedef std::wstringstream StringStream;
	typedef std::wifstream IFStream;
	typedef std::wofstream OFStream;
#define INTRA_DEFAULT_NA	L"na"
#define INTRA_DEFAULT_DELIM L'\t'
#define INTRA_COMMA	 ','
#define INTRA_TAB	'\t'
#endif
	/////////////////////////////////////
}// namespace intra
//////////////////////////////
#endif /* PORT_H_ */
