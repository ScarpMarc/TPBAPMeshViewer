/*
	Treasure Planet: Battle at Procyon Inspector - mesh viewer and editor.

	*** by InkySka ***

	e-mail: underscore.marczero@gmail.com
	Twitter: @InkySka

	Official repository: https://github.com/InkySka/TPBAPMeshViewer
	.mdb files info: https://treasure-planet.wikia.com/wiki/.mdb_mesh_files
	Note: reverse-engineering is still undergoing and any help is appreciated.

	Treasure Planet Wikia: https://treasure-planet.wikia.com/

	(C) InkySka 2018 - published under the MIT Licence.

	Mesh class Save-to-file file: implementation for the mesh class' saving functions.
*/

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

void mesh::convertToMDB() const
{
	ofstream file;

	file.open(file_path, std::ifstream::binary);

	data_4bytes writer;
	writer.ui = vertices[0].size();

	file.write(writer.c, 4); //update number of vertices
	for (int i = 0; i < 1; ++i)
	{
		try
		{
			//get vertex data
			for (unsigned int j = 0; j < vertices[i].size(); ++j)
			{
				//file.seekp(vertex_data_offset[i], ios::beg); //at the number of vertices

				

				writer.ui = 32;//sizeof(vertices[i][j]) - 4;
				file.write(writer.c, 4); //number of bytes for the vertex data

				//writing the vertex's coordinates
				writer.f = vertices[i][j].x;
				file.write(writer.c, 4);
				writer.f = vertices[i][j].y;
				file.write(writer.c, 4);
				writer.f = vertices[i][j].z;
				file.write(writer.c, 4);

				writer.f = vertices[i][j].u;
				file.write(writer.c, 4);
				writer.f = pivoter(vertices[i][j].v);
				file.write(writer.c, 4);

				writer.ui = 0;
				file.write(writer.c, 4);
				writer.ui = 0;
				file.write(writer.c, 4);
				//file.seekp(8, ios::cur); //ignore unknown data!

				writer.f = vertices[i][j].transparency;
				file.write(writer.c, 4);
			}
			for (unsigned int j = 0; j < faces[i].size(); ++j)
			{
				//at the number of faces

				writer.ui = faces[i].size();

				file.write(writer.c, 4); //update number of faces

				writer.ui = 8;//sizeof(faces[i][j]) - 4;
				file.write(writer.c, 4); //number of bytes for the face data

										 //writing the face stuff

				writer.s[0] = faces[i][j].verticesIDs[0];
				writer.s[1] = faces[i][j].verticesIDs[1];
				file.write(writer.c, 4);

				writer.s[0] = faces[i][j].verticesIDs[2];
				writer.s[1] = faces[i][j].material_n;
				file.write(writer.c, 4);
			}
		}
		catch (exception& exc)
		{

		}
	}
	file.close();
}



void mesh::saveOBJToFile(const fs::path& i_folder_path) const
{
	for (unsigned int i = 0; i < type; ++i)
	{
		ofstream file;
		fs::path modifying_temp(i_folder_path);
		fs::path saveto_path;

		if (type == 1)
		{
			fs::create_directory(modifying_temp);
			saveto_path = modifying_temp.string() + (string)"/" + file_path.filename().string(); // .../.../filename_i.mdb
		}
		else
		{
			modifying_temp += ((string)"/" + file_path.stem().string()); // .../exportfolder/filename/
			fs::create_directory(modifying_temp);
			fs::path temp_filename(file_path.stem().string() + "_" + to_string(i + 1) + file_path.extension().string()); //.../exportfolder/filename/filename_i.mdb
			saveto_path = modifying_temp.string() + (string)"/" + temp_filename.string();
		}
		
		saveto_path.replace_extension(".obj");//filename_i.obj
		file.open(saveto_path);

		file << setprecision(16);

		for (unsigned int j = 0; j < vertices[i].size(); ++j)
		{
			file << "v " << vertices[i][j].x << " " << vertices[i][j].y << " " << vertices[i][j].z << endl;
		}
		for (unsigned int j = 0; j < vertices[i].size(); ++j)
		{
			file << "vt " << vertices[i][j].u << " " << vertices[i][j].v << endl;
		}
		for (unsigned int j = 0; j < faces[i].size(); ++j) //NEEDS REWORK
		{
			file << "usemtl Material_" << faces[i][j].material_n + 1 << endl;
			file << "f ";
			for (unsigned int k = 0; k < faces[i][j].verticesIDs.size(); ++k) //NEEDS REWORK
			{
				file << faces[i][j].verticesIDs[k] + 1;
			}
			cout << endl;
		}

		file.close();
	}
}

void mesh::saveMTLToFile(const fs::path& i_folder_path) const
{
	for (unsigned int i = 0; i < type; ++i)
	{
		ofstream file;
		fs::path modifying_temp(i_folder_path);
		fs::path saveto_path;

		if (type == 1)
		{
			fs::create_directory(modifying_temp);
			saveto_path = modifying_temp.string() + (string)"/" + file_path.filename().string(); // .../.../filename_i.mdb
		}
		else
		{
			modifying_temp += ((string)"/" + file_path.stem().string()); // .../exportfolder/filename/
			fs::create_directory(modifying_temp);
			fs::path temp_filename(file_path.stem().string() + "_" + to_string(i + 1) + file_path.extension().string()); //.../exportfolder/filename/filename_i.mdb
			saveto_path = modifying_temp.string() + (string)"/" + temp_filename.string();
		}

		saveto_path.replace_extension(".mtl");//filename_i.obj

		file.open(saveto_path);

		for (unsigned int j = 0; j < actually_used_materials[i].size(); ++j)
		{
			//wip
		}

		file.close();
	}
}

void mesh::saveTextureNamesToFile(const fs::path& i_folder_path) const
{
	for (unsigned int i = 0; i < type; ++i)
	{
		ofstream file;
		fs::path modifying_temp(i_folder_path);
		fs::path saveto_path;

		if (type == 1)
		{
			fs::create_directory(modifying_temp);
			saveto_path = modifying_temp.string() + (string)"/" + file_path.filename().string(); // .../.../filename_i.mdb
		}
		else
		{
			modifying_temp += ((string)"/" + file_path.stem().string()); // .../exportfolder/filename/
			fs::create_directory(modifying_temp);
			fs::path temp_filename(file_path.stem().string() + "_" + to_string(i + 1) + file_path.extension().string()); //.../exportfolder/filename/filename_i.mdb
			saveto_path = modifying_temp.string() + (string)"/" + temp_filename.string();
		}

		saveto_path.replace_extension(".txt");//filename_i.txt

		file.open(saveto_path);

		for (unsigned int j = 0; j < actually_used_materials[i].size(); ++j)
		{
			file << actually_used_materials[i][j].texture_name << endl;
		}

		file.close();
	}	
}

void mesh::DEV_saveOtherData(const fs::path& i_folder_path) const
{
	for (unsigned int i = 0; i < type; ++i)
	{
		ofstream file;
		fs::path modifying_temp(i_folder_path);
		fs::path saveto_path;

		if (type == 1)
		{
			fs::create_directory(modifying_temp);
			saveto_path = modifying_temp.string() + (string)"/" + file_path.filename().string(); // .../.../filename_i.mdb
		}
		else
		{
			modifying_temp += ((string)"/" + file_path.stem().string()); // .../exportfolder/filename/
			fs::create_directory(modifying_temp);
			fs::path temp_filename(file_path.stem().string() + "_" + to_string(i + 1) + file_path.extension().string()); //.../exportfolder/filename/filename_i.mdb
			saveto_path = modifying_temp.string() + (string)"/" + temp_filename.string();
		}

		saveto_path.replace_extension(".obj");//filename_i.obj
		file.open(saveto_path);

		file << setprecision(16);

		for (unsigned int j = 0; j < vertices[i].size(); ++j)
		{
			file << "v " << vertices[i][j].other_data_a << " " << vertices[i][j].other_data_b << endl;
		}
		file.close();
	}

	
}