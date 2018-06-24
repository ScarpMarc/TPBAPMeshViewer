#include "stdafx.h"
#include "utilities.h"
#include <fstream>
#include <string>
#include <iomanip>

#include <iostream>

using namespace std;

int saveOBJToFile(const int& I_numMeshes, char** I_filename)
{
	ofstream file;
	for (int i = 0; i < I_numMeshes; ++i)
	{
		string tempFilename = I_filename[i];
		tempFilename += ".obj";
		
		file << setprecision(16);

		file.open(tempFilename);
		for (int j = 0; j < meshes[i].number_of_vertices; ++j)
		{
			file << "v " << meshes[i].vertices[j].x << " " << meshes[i].vertices[j].y << " " << meshes[i].vertices[j].z << endl;
		}
		for (int j = 0; j < meshes[i].number_of_vertices; ++j)
		{
			file << "vt " << meshes[i].vertices[j].u << " " << meshes[i].vertices[j].v << endl;
		}
		for (int j = 0; j < meshes[i].number_of_faces; ++j)
		{
			file << "usemtl Material_" << meshes[i].faces[j].material_n << endl;
			file << "f " << meshes[i].faces[j].vertex1 << " " << meshes[i].faces[j].vertex2 << " " << meshes[i].faces[j].vertex3 << endl;
		}
		file.close();
	}
	return 0;
}

int saveMTLToFile(const int& I_numMeshes, char** I_filename)
{
	ofstream file;
	for (int i = 0; i < I_numMeshes; ++i)
	{
		string tempFilename = I_filename[i];
		tempFilename += ".mtl";

		
		file.close();
	}
	return 0;
}