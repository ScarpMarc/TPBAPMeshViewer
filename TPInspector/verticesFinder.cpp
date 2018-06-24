#include "stdafx.h"
#include "utilities.h"
#include <bitset>
#include <fstream>

using namespace std;

int findVertices(const int& I_numMeshes, char** I_filename)
{
	ifstream file;
	for (int i = 0; i < I_numMeshes; ++i)
	{
		file.open(I_filename[i], std::ifstream::binary);
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
			for (unsigned int k = 24; k<numBytesVertex + 1; k += 4)
			{
				file.read(temp.c, 4);
				meshes[i].vertices[j].other_data.push_back(temp.ui);
			}
			if (temp.uc[3] != 0xFF)
			{
				meshes[i].vertices[j].isValid = false;
				meshes[i].invalid_vertex_indices.push_back(j);
			}
		}
		meshes[i].face_data_offset = file.tellg();
		file.close();
	}
	return 0;
}