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

	Utilities file: miscellaneous functions.
*/

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

vector<fs::path> fileFinderByExtension(const fs::path& I_folder, const string& I_extension, const bool& I_includeSubDirs)
{
	//vector temporaneo
	vector<fs::path> out;

	//trova tutti gli oggetti della directory...
	for (const fs::directory_entry& in : fs::directory_iterator(I_folder))
	{
		//...se bisogna fare una ricerca recursiva nelle cartelle...
		if (fs::is_directory(in) && I_includeSubDirs && (in.path().filename() != exported_meshes_folder_absolute))
		{
			//recursione con nuova path
			vector<fs::path> temp(fileFinderByExtension(in.path(), I_extension, true));
			for (const fs::path& i : temp)
			{
				if (fs::is_regular_file(in) && in.path().extension().string() == mesh_data_extension)
				{
					out.push_back(i);
				}
			}
		}
		else if (fs::is_regular_file(in) && (in.path().extension().string()) == mesh_data_extension)
			out.push_back(in.path().string());
	}
	return out;
}

vector<fs::path> fileFinderByExtension(const vector<fs::path>& I_files, const string& I_extension)
{
	//vector temporaneo
	vector<fs::path> out;

	//trova tutti gli oggetti della directory...
	for (const fs::path& in : I_files)
	{
		if (fs::is_regular_file(in) && in.extension().string() == mesh_data_extension)
			out.push_back(in.string());
	}
	return out;
}

void removeDuplicates(vector<int> I_vec)
{
	sort(I_vec.begin(), I_vec.end());
	I_vec.erase(unique(I_vec.begin(), I_vec.end()), I_vec.end());
}