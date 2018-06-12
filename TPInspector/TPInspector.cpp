#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "utilities.h"
#include <sstream>

#include <vector>

#include "windows.h"

#include <iterator>
#include <cstdlib> 

using namespace std;

//bool isLittleEndian = true;

int main(int argc, char** argv)
{
	int temp_result = findVertices(argc, argv);
	switch (temp_result)
	{
	case 0: break;
	case 1: 
	{
		cout << "ERROR - expected end of vertex(0xFFFFFFFF)" << endl;
		break;
	}
	default: cout << "Unknown error\n";
	}
	cout << "Done. Found " << vertices.size() << " vertices.\n";
	
    return 0;
}

