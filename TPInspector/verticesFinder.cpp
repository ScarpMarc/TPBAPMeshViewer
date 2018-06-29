#include "stdafx.h"
#include "utilities.h"
#include <bitset>
#include <fstream>

#include <iostream>

using namespace std;

int findVertices(std::vector<std::string> I_filenames)
{
	ifstream file;
	for (int i = 0; i < I_filenames.size(); ++i)
	{
		file.open(I_filenames[i], std::ifstream::binary);
		data_4bytes temp;

		file.seekg(24); //at the number of vertices
		
		file.read(temp.c, 4); //get number of vertices
		meshes[i].number_of_vertices = temp.i;
		//get vertex data
		meshes[i].vertices.resize(meshes[i].number_of_vertices);
		for (unsigned int j = 0; j < meshes[i].number_of_vertices; ++j)
		{
			file.read(temp.c, 4); //number of bytes for the vertex data
			unsigned int numBytesVertex(temp.ui);

			//reading vertex[j]'s coordinates
			file.read(temp.c, 4);
			meshes[i].vertices[j].x = temp.f;
			file.read(temp.c, 4);
			meshes[i].vertices[j].y = temp.f;
			file.read(temp.c, 4);
			meshes[i].vertices[j].z = temp.f;
			file.read(temp.c, 4);

			meshes[i].vertices[j].u = temp.f;
			file.read(temp.c, 4);
			meshes[i].vertices[j].v = pivoter(temp.f); //pivoted value!
			/*
			READING DATA INTO A STRING
			file implementation is still obscure
			4 bytes at a time for future use
			should be 16 bytes.
			*/
			for (unsigned int k = 24; k<numBytesVertex - 3; k += 4)
			{
				file.read(temp.c, 4);
				meshes[i].vertices[j].other_data.push_back(temp.ui);
			}
			file.read(temp.c, 4);
			meshes[i].vertices[j].transparency = temp.f;
		}
		meshes[i].face_data_offset = file.tellg();
		cout << "\n\nFACE OFFSET "<<std::hex << meshes[i].face_data_offset << endl;
		file.close();
	}
	return 0;
}
