#include "stdafx.h"

#include <iomanip>

#include "utilities.h"
#include "MeshClass.h"

#include <iostream>
#include "FILENAMEDEFS.h"

namespace fs = std::experimental::filesystem;

using namespace std;

void mesh::saveToFile() const
{
	saveOBJToFile();
	//saveMTLToFile();
	saveTextureNamesToFile();
}

void mesh::saveOBJToFile(const fs::path& i_folder_path) const
{
	ofstream file;
	fs::path modifying_temp(i_folder_path);
	fs::path saveto_path;

	if (type == 1)
	{
		fs::path temp_filename(file_path.filename().replace_extension(".obj"));
		saveto_path = modifying_temp.string() + (string)"/" + temp_filename.string();
		file.open(saveto_path);
	}
	else
	{
		fs::path temp_filename(file_path.stem().string() + (string)"_HQ" + file_path.extension().string());
		temp_filename.replace_extension(".obj");
		saveto_path = modifying_temp.string() + (string)"/" + temp_filename.string();
		file.open(saveto_path);
	}

	file << setprecision(16);

	for (unsigned int i = 0; i < number_of_vertices_HQ; ++i)
	{
		file << "v " << vertices_HQ[i].x << " " << vertices_HQ[i].y << " " << vertices_HQ[i].z << endl;
	}
	for (unsigned int i = 0; i < number_of_vertices_HQ; ++i)
	{
		file << "vt " << vertices_HQ[i].u << " " << vertices_HQ[i].v << endl;
	}
	for (unsigned int i = 0; i < number_of_faces_HQ; ++i) //NEEDS REWORK
	{
		file << "usemtl Material_" << faces_HQ[i].material_n << endl;
		file << "f " << faces_HQ[i].vertex1 << " " << faces_HQ[i].vertex2 << " " << faces_HQ[i].vertex3 << endl;
	}

	file.close();

	if (type == 2)
	{
		fs::path temp_filename(file_path.stem().string() + (string)"_LQ" + file_path.extension().string());
		temp_filename.replace_extension(".obj");
		fs::path saveto_path(modifying_temp.string() + (string)"/" + temp_filename.string());
		file.open(saveto_path);


		file << setprecision(16);

		for (unsigned int i = 0; i < number_of_vertices_LQ; ++i)
		{
			file << "v " << vertices_LQ[i].x << " " << vertices_LQ[i].y << " " << vertices_LQ[i].z << endl;
		}
		for (unsigned int i = 0; i < number_of_vertices_LQ; ++i)
		{
			file << "vt " << vertices_LQ[i].u << " " << vertices_LQ[i].v << endl;
		}
		for (unsigned int i = 0; i < number_of_faces_LQ; ++i) //NEEDS REWORK
		{
			file << "usemtl Material_" << faces_LQ[i].material_n << endl;
			file << "f " << faces_LQ[i].vertex1 << " " << faces_LQ[i].vertex2 << " " << faces_LQ[i].vertex3 << endl;
		}
	}

	file.close();
}

void mesh::saveMTLToFile(const fs::path& i_folder_path) const
{
	ofstream file;
	fs::path modifying_temp(i_folder_path);

	fs::path temp_filename(file_path.filename().replace_extension(".mtl"));
	fs::path saveto_path(modifying_temp.string() + temp_filename.string());
	file.open(saveto_path);

	for (unsigned int i = 0; i < number_of_materials; ++i)
	{
		
	}
	
	file.close();
}

void mesh::saveTextureNamesToFile(const fs::path& i_folder_path) const
{
	ofstream file;
	fs::path modifying_temp(i_folder_path);

	if (type == 1)
	{
		fs::path temp_filename(file_path.filename().replace_extension(".txt"));
		fs::path saveto_path(modifying_temp.string() + (string)"/" + temp_filename.string());
		file.open(saveto_path);
	}
	else
	{
		fs::path temp_filename(file_path.stem().string() + (string)"_HQ" + file_path.extension().string());
		temp_filename.replace_extension(".txt");
		fs::path saveto_path(modifying_temp.string() + (string)"/" + temp_filename.string());
		file.open(saveto_path);
	}

	for (unsigned int i = 0; i < actually_used_materials_HQ.size(); ++i)
	{
		file << actually_used_materials_HQ[i].texture_name << endl;	
	}

	file.close();

	if (type == 2)
	{
		fs::path temp_filename(file_path.stem().string() + (string)"_LQ" + file_path.extension().string());
		temp_filename.replace_extension(".txt");
		fs::path saveto_path(modifying_temp.string()+ (string)"/" + temp_filename.string());
		file.open(saveto_path);

		for (unsigned int i = 0; i < actually_used_materials_LQ.size(); ++i)
		{
			file << actually_used_materials_LQ[i].texture_name << endl;
		}
	}

	file.close();
}

