#include "stdafx.h"
#include "utilities.h"
#include <bitset>
#include <fstream>

using namespace std;

ifstream file;
std::vector<vertex> vertices;

int findVertices(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		file.open(argv[1], ios::binary);

		data_4bytes temp;
		wchar_t temp4(0);
		unsigned int vertNum(0);

		file.seekg(24); //at the number of vertices
		file.read(temp.c, 4); //get number of vertices
		vertNum = temp.i;

		//cout << vertNum << endl << endl;

		/*
		GET VERTEX DATA
		*/

		vertices.resize(vertNum);

		for (int j = 0; j < vertNum; ++j)
		{
			file.read(temp.c, 4); //number of bytes for the vertex data
			unsigned int numBytesVertex(temp.ui);

			//cout << std::hex << numBytesVertex << endl << endl;

			/*
			FAILSAFE STUFF
			*/
			streampos currPos = file.tellg(); //saving curent position in the stream

			file.seekg((numBytesVertex - 4), ios::cur);
			file.read(temp.c, 4);

			if (temp.ui != (unsigned int)0xFFFFFFFF) return 1;//failsafe. if this occurs, it is an error
				

			/*
			END OF FAILSAFE
			assuming we have a valid sequence. going back to the start of the vertex's data
			*/

			/*
			READING VERTEX COORDINATES (x, y, z)
			*/
			file.seekg(currPos);
			file.read(temp.c, 4);
			vertices[j].x = temp.f;
			file.read(temp.c, 4);
			vertices[j].y = temp.f;
			file.read(temp.c, 4);
			vertices[j].z = temp.f;

			/*
			READING DATA INTO A STRING
			file implementation is still obscure
			4 bytes at a time for future use
			should be 16 bytes.
			*/
			while (temp.ui != (unsigned int)0xFFFFFFFF)
			{
				file.read(temp.c, 4);
				vertices[j].other_data.push_back(temp.ui);
			}
		}
	}
	return 0;
}
