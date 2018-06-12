#include "stdafx.h"
#include "utilities.h"
#include <bitset>
#include <fstream>

#include <iostream>

using namespace std;

ifstream file;
//std::vector<vertex> vertices; //resized in findVertices

int findVertices(int argc, char** argv)
{
	for (int i = 0; i < argc-1; ++i)
	{
		file.open(argv[i+1], ios::binary);
		data_4bytes temp;
		unsigned int vertNum(0);

		file.seekg(24); //at the number of vertices
		file.read(temp.c, 4); //get number of vertices
		vertNum = temp.i;

		//cout << vertNum << endl << endl;

		/*
		GET VERTEX DATA
		*/

		meshes[i].vertices.resize(vertNum);

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

			if (temp.ui != (unsigned int)0xFFFFFFFF) return i+1;//failsafe. if this occurs, it is an error on mesh i+1 (not 0!)
				

			/*
			END OF FAILSAFE
			assuming we have a valid sequence. going back to the start of the vertex's data
			*/

			/*
			READING VERTEX COORDINATES (x, y, z)
			*/
			file.seekg(currPos);
			file.read(temp.c, 4);
			meshes[i].vertices[j].x = temp.f;
			file.read(temp.c, 4);
			meshes[i].vertices[j].y = temp.f;
			file.read(temp.c, 4);
			meshes[i].vertices[j].z = temp.f;

			/*
			READING DATA INTO A STRING
			file implementation is still obscure
			4 bytes at a time for future use
			should be 16 bytes.
			*/
			while (temp.ui != (unsigned int)0xFFFFFFFF)
			{
				file.read(temp.c, 4);
				meshes[i].vertices[j].other_data.push_back(temp.ui);
			}
		}
		file.close();
	}
	return 0;
}
