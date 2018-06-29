#include "stdafx.h"
#include <vector>
#include <experimental\filesystem>

#include "utilities.h"
#include "FILENAMEDEFS.h"

#include <iostream>

namespace fs = std::experimental::filesystem;

using namespace std;

inline float pivoter(const float & I_val)
{
	return (2 * pivot) - I_val;
}

vector<string> fileFinderByExtension(const string& I_folder, const string& I_ext, const bool& I_includeSubDirs)
{
	//vector temporaneo
	vector<string> out;

	//trova tutti gli oggetti della directory...
	for (const fs::directory_entry& in : fs::directory_iterator(I_folder))
	{
		//...se bisogna fare una ricerca recursiva nelle cartelle...
		if (fs::is_directory(in) && I_includeSubDirs && (in.path().filename() != exportedFolder))
		{
			//recursione con nuova path
			vector<string> temp(fileFinderByExtension(in.path().string(), I_ext, true));
			for (const std::string& i : temp)
			{
				if (fs::is_regular_file(in) && getFileExtension(in.path().string()) == master_extension)
				{
					out.push_back(i);
				}
			}
		}
		else if (fs::is_regular_file(in) && getFileExtension(in.path().string()) == master_extension)
			out.push_back(in.path().string());
	}
	return out;
}

std::string getFileExtension(const std::string & I_filename)
{
	//rende l'estensione del file CON punto
	string out(I_filename);

	size_t dotPos(out.rfind('.'));

	if(dotPos == string::npos) out="NOT_FOUND";
	else out.erase(0, dotPos);

	return out;
}

int meshCleaner(std::vector<std::string> I_filenames)
{
	for (int i = 0; i < I_filenames.size(); ++i)
	{
		for (int j = 0; j < meshes[i].number_of_faces; ++j)
		{		
			if (meshes[i].faces[j].material_n <= meshes[i].number_of_materials)
			{
				meshes[i].used_materials.push_back(meshes[i].materials[meshes[i].faces[j].material_n-1]);
				sort(meshes[i].used_materials.begin(), meshes[i].used_materials.end());
				meshes[i].used_materials.erase(unique(meshes[i].used_materials.begin(), meshes[i].used_materials.end()), meshes[i].used_materials.end());
			}
		}
	}
	return 0;
}

