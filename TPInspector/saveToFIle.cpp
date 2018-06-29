#include "stdafx.h"
#include "utilities.h"
#include <fstream>
#include <string>
#include <iomanip>

#include <experimental\filesystem>

#include "FILENAMEDEFS.h"

#include <iostream>

namespace fs = std::experimental::filesystem;

using namespace std;

int saveOBJToFile(std::vector<std::string> I_filenames)
{
	ofstream file;
	for (int i = 0; i < I_filenames.size(); ++i)
	{
		string tempFilename = I_filenames[i];
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

int saveMTLToFile(std::vector<std::string> I_filenames)
{
	ofstream file;
	for (int i = 0; i < I_filenames.size(); ++i)
	{
		string tempFilename = I_filenames[i];
		tempFilename += ".mtl";

		
		file.close();
	}
	return 0;
}

int saveTexturesToFile(std::vector<std::string> I_filenames)
{
	ofstream file;
	for (int i = 0; i < I_filenames.size(); ++i)
	{
		fs::path tempPath(I_filenames[i]);
		string tempFilename = (exportedFolder.string() +"/"+ tempPath.filename().string())+".txt";

		ofstream file;

		file.open(tempFilename);
		for (int j = 0; j < meshes[i].used_materials.size(); ++j)
		{
			file << meshes[i].used_materials[j].texture_name << endl;
		}
		file.close();
	}
	return 0;
}