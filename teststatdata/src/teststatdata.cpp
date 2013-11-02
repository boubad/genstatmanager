//============================================================================
// Name        : teststatdata.cpp
// Author      : Boubacar Diarra
// Version     :
// Copyright   : boubadiarra@gmail.com
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <gtest/gtest.h>
/////////////////////////////////
#include "IntraTestEnv.h"
//////////////////////////////////
int main(int argc, char *argv[]) {
	global_intraenv = reinterpret_cast<IntraTestEnv *>(::testing::AddGlobalTestEnvironment(new IntraTestEnv()));
	::testing::InitGoogleTest(&argc, argv);
	int nRet = RUN_ALL_TESTS();
#if (defined(_MSC_VER) && defined(_DEBUG))
	int nx;
	std::cout << std::endl << "Entrez un nombre pour quitter: ";
	std::cin >> nx;
#endif
	return (nRet);
}// main
