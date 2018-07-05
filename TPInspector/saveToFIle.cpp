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
			file << "usemtl Material_" << faces[i][j].material_n << endl;
			file << "f ";
			for (unsigned int k = 0; k < faces[i][j].verticesIDs.size(); ++k) //NEEDS REWORK
			{
				file << faces[i][j].verticesIDs[k];
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