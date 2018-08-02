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

	Command handling file: definitions for all of the commands found in main.
*/

#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"

#include <experimental\filesystem>

#include <iostream>
#include <sstream>
#include <ctime>

namespace fs = std::experimental::filesystem;

using namespace std;

/*
	void TPI_ingestMeshes()

	Fetches all of the files in filenames using their IDs (from file_ids_to_convert).
	It then opens their .mdb files and loads everything into RAM.

	Called by: /ingest

	Note: see the mesh class for information on initialization.
*/
void TPI_ingestMeshes()
{
	/*
		Duration timer for the entire process.
	*/
	clock_t start = 0, end = 0;
	double duration = 0;

	start = clock();

	for (unsigned int i = 0; i < file_ids_to_convert.size(); ++i)
	{
		/*
			Duration timer for individual meshes.
		*/
		clock_t start_sub, end_sub;
		double duration_sub;

		/*
			Creating the mesh object in the mesh vector.
			Also displaying some info.
		*/
		meshes.push_back(filenames[file_ids_to_convert[i]]);

		cout << "MESH " << i + 1 << " - " << meshes[i].getFilePath() << endl;

		cout << "Ingesting and tidying... (can take a while)" << endl;

		/*
			Starting the timer and getting the info from the file.
		*/
		start_sub = clock();
		meshes[i].findOffsets();
		meshes[i].ingest();

		/*
			Stopping the clock and displaying some info.
		*/
		end_sub = clock();
		duration_sub = (end_sub - start_sub) / (double)CLOCKS_PER_SEC;
		cout << "Done in " << to_string(duration_sub) << " seconds" << endl << endl;
	}

	/*
		Stopping the clock and displaying some info.
	*/
	end = clock();
	duration = (end - start) / (double)CLOCKS_PER_SEC;

	if (file_ids_to_convert.size() > 1)
	{
		cout << "-----------------------" << endl;
		cout << "Done in a total of " << to_string(duration) << " seconds" << endl << endl;
	}
}

/*
	void TPI_saveToFile()

	Called by: /save -OBJ.

	Saves the meshes whose IDs are in file_ids_to_convert to .OBJ and .MTL.
*/
void TPI_saveToFile()
{
	/*
		Duration timer for the entire process.
	*/
	clock_t start = 0, end = 0;
	double duration = 0;

	/*
		Creating directory.
	*/
	fs::create_directory(exported_texture_file_names_folder_absolute);
	fs::create_directory(exported_meshes_folder_absolute);

	start = clock();

	for (unsigned int i = 0; i < file_ids_to_convert.size(); ++i)
	{
		/*
			Duration timer for individual meshes.
		*/
		clock_t start_sub, end_sub;
		double duration_sub;

		/*
			Starting the clock, saving and displaying some info.
		*/
		cout << "Mesh: " << meshes[i].getFilePath() << endl;

		cout << "Saving..." << endl;

		start_sub = clock();
		meshes[i].saveToFile();

		/*
		Stopping the clock and displaying some info.
		*/
		end_sub = clock();
		duration_sub = (end_sub - start_sub) / (double)CLOCKS_PER_SEC;
		cout << "Done in " << to_string(duration_sub) << " seconds" << endl << endl;
		cout << endl;
	}

	/*
		Stopping the clock and displaying some info.
	*/
	end = clock();
	duration = (end - start) / (double)CLOCKS_PER_SEC;
	cout << "-----------------------" << endl;
	cout << "Done in a total of " << to_string(duration) << " seconds" << endl << endl;
	cout << "Exported 3D data folder: " << exported_meshes_folder_absolute << endl;
	cout << "Exported texture names folder: " << exported_texture_file_names_folder_absolute << endl << endl;
}

/*
	void TPI_findValidFiles(const std::string& I_extension = mesh_data_extension);

	Called by: /find.

	Finds valid files in the meshes directory with the specified extension. 
	Load file names into the filenames vector.
*/
void TPI_findValidFiles(const std::string & I_extension)
{
	filenames = fileFinderByExtension(meshes_folder_absolute, I_extension);
	cout << "Found " << filenames.size() << " valid files (type /list to get the list)" << endl << endl;
}

/*
	void TPI_list(const int& mode)

	Called by: /list.

	Lists all of the files in the specified vector (specified through mode).

	Output format:
		1. absolute_file_path
		2. absolute_file_path
		...

	Mode:
		0 - Files in folder (from filenames).
		1 - Files to ingest (from filenames[file_ids_to_convert]).
		2 - Meshes to render (from filenames[file_ids_to_render]).
		3 - Meshes to save (from filenames[file_ids_to_save]).
		4 - Ingested meshes (from meshes).
*/
void TPI_list(const int& mode)
{
	switch (mode)
	{
	case 0:
	{
		cout << "Files in folder: " << endl;
		for (unsigned int i = 0; i < filenames.size(); ++i)
		{
			cout << i + 1 << ". " << filenames[i] << endl;	
		}
		break;
	}
	case 1:
	{
		cout << "Files currently marked for ingestion: " << endl;
		for (unsigned int i = 0; i < file_ids_to_convert.size(); ++i)
		{
			cout << i + 1 << ". " << filenames[file_ids_to_convert[i]] << endl;
		}
		break;
	}
	case 2:
	{
		cout << "Files to be rendered: " << endl;
		for (unsigned int i = 0; i < file_ids_to_render.size(); ++i)
		{
			cout << i + 1 << ". " << filenames[file_ids_to_render[i]] << endl;
		}
		break;
	}
	case 3:
	{
		cout << "Files to be saved: " << endl;
		for (unsigned int i = 0; i < file_ids_to_save.size(); ++i)
		{
			cout << i + 1 << ". " << filenames[file_ids_to_save[i]] << endl;
		}
		break;
	}
	case 4:
	{
		cout << "Ingested files: " << endl;
		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			cout << i + 1 << ". " << meshes[i].getFilePath() << endl;
		}
		break;
	}
	default: cout << "Internal error in parameter passed." << endl;
	}
}

//coming soon
void TPI_changeFolder()
{
}

//coming soon
void TPI_changeOutputFolder()
{
}

/*std::vector<fs::path> generateFilenameList()
{
	std::vector<fs::path>;
}*/


/*
	OVERLOADED vector<int> TPI_I_normalizeInput(const vector<int>& input) OPERATES ON THE FILENAMES VECTOR.
	OVERLOADED vector<int> TPI_I_normalizeInput(const int& input, const int& index_to_start_from) OPERATES ON THE MESHES VECTOR.

	WARNING: the first overload (TPI_I_normalizeInput(const vector<int>& input)) has to be used
		ONLY TO INGEST FILES because it only checks if they have been found. 
		The second overload also checks if they have been loaded into memory.
		
	a.k.a. list generator.

	Called by: all of the commands that require a list.

	Commands accepted: see in MAIN.

	Takes an INT vector and outputs an INT vector with the selected files.

	Note: this function is utterly overcomplicated, and one of the most cryptic.
		It is only used during command interpretation.
	
	HANDLE WITH CARE.
*/
vector<int> TPI_I_normalizeInput(const vector<int>& input)
{
	vector<int> temp;

	/*
		Return all of the items present in the input vector.
	*/
	if (commands[1] == "-all")
	{
		return input;
	}

	/*
		Return some of the items present in the input vector using the list.
	*/
	if (commands[1] == "-list")
	{
		/*
		we want something like:
		1 4 7 9-12 15
		or	1, 4, 7, 9-12, 15
		*/
		for (int i = 2; i < commands.size() - 1; ++i)
		{
			/*
				String normalisation (string to int, deal with comma and dash).
			*/
			try
			{
				string substring;
				size_t start_of_next_part;
				int temp_item = stoi(commands[i], &start_of_next_part);
				if (temp_item <= input.size())
				{
					substring = commands[i].substr(start_of_next_part);
					if (substring[0] == (char)'-')
					{
						commands[i].erase(0, start_of_next_part + 1); //erase the dash
						int temp_item_end = stoi(commands[i]);//get the second number
						if (temp_item_end <= input.size() && temp_item_end > temp_item) //if the 2nd number is within the range and is more than the first one
						{
							for (int j = temp_item - 1; j <= temp_item_end - 1; ++j)
							{
								temp.push_back(j);
							}
						}
						else if (temp_item_end < temp_item) cout << "Error! First item bigger than the last (" << temp_item << " > " << temp_item_end << ")." << endl;
						else if (temp_item_end > input.size()) cout << "Error! Out of range." << endl;
					}
					else
					{
						temp.push_back(temp_item - 1);
					}
				}
				else cout << "Error! Out of range." << endl;
			}
			catch (exception& exc)
			{
				cout << "Error! Wrong input. Was something not a number?" << endl;
				commands.erase(commands.begin(), commands.end());
				temp.erase(file_ids_to_convert.begin(), file_ids_to_convert.end());
				break;
			}
		}
	}
	else
	{
		/*
			The user inputted some file names.
			This makes their path absolute and searches for them in the filenames vector.
		*/
		for (int i = 1; i < commands.size() - 1; ++i)
		{
			fs::path temp_path(commands[i]);

			if (!temp_path.has_extension()) temp_path.replace_extension(mesh_data_extension);

			if (temp_path.string()[0] == '/')
			{
				std::string temp_del(temp_path.string());
				temp_del.erase(0);
				temp_path = fs::path(temp_del);
			}

			if (temp_path.is_relative())
				temp_path = fs::path(meshes_folder_absolute.string() + (string)"\\" + temp_path.string());

			if (fs::is_regular_file(temp_path))
			{
				/*
					Checking if the file has been found.
				*/
				int temp_find = input.size();
				int temp_find_id = find(filenames.begin(), filenames.end(), temp_path) - filenames.begin();
				if (temp_find_id != filenames.size()) temp_find = find(input.begin(), input.end(), temp_find_id) - input.begin();
				if(temp_find != input.size()) temp.push_back(temp_find);
			}
			else cout << "File " << temp_path.string() << " not found." << endl;
		}
	}
	return temp;
}

/*
	OVERLOADED vector<int> TPI_I_normalizeInput(const vector<int>& input) OPERATES ON THE FILENAMES VECTOR.
	OVERLOADED vector<int> TPI_I_normalizeInput(const int& input, const int& index_to_start_from) OPERATES ON THE MESHES VECTOR.

	WARNING: the first overload (TPI_I_normalizeInput(const vector<int>& input)) has to be used
		ONLY TO INGEST FILES because it only checks if they have been found. 
		The second overload also checks if they have been loaded into memory.
		

	a.k.a. list generator.

	Called by: all of the commands that require a list.

	Commands accepted: see in MAIN.

	Takes an INT vector and outputs an INT vector with the selected files.

	Note: this function is utterly overcomplicated, and one of the most cryptic.
		It is only used during command interpretation.
		HANDLE WITH CARE.
*/
vector<int> TPI_I_normalizeInput(const int& input, const int& index_to_start_from)
{
	vector<int> temp;

	if (commands[index_to_start_from] == "-all")
	{
		for (int i = 0; i < input; ++i)
		{
			temp.push_back(i);
		}
		return temp;
	}
	if (commands[index_to_start_from] == "-list")
	{
		/*
		we want something like:
		1 4 7 9-12 15
		or	1, 4, 7, 9-12, 15
		*/
		for (int i = index_to_start_from + 1; i < commands.size() - 1; ++i)
		{
			try
			{
				string substring;
				size_t start_of_next_part;
				int temp_item = stoi(commands[i], &start_of_next_part);
				if (temp_item <= input)
				{
					substring = commands[i].substr(start_of_next_part);
					if (substring[0] == (char)'-')
					{
						commands[i].erase(0, start_of_next_part + 1); //erase the dash
						int temp_item_end = stoi(commands[i]);//get the second number
						if (temp_item_end <= input && temp_item_end > temp_item) //if the 2nd number is within the range and is more than the first one
						{
							for (int j = temp_item - 1; j <= temp_item_end - 1; ++j)
							{
								temp.push_back(j);
							}
						}
						else if (temp_item_end < temp_item) cout << "Error! First item bigger than the last (" << temp_item << " > " << temp_item_end << ")." << endl;
						else if (temp_item_end > input) cout << "Error! Out of range." << endl;
					}
					else
					{
						temp.push_back(temp_item - 1);
					}
				}
				else cout << "Error! Out of range." << endl;
			}
			catch (exception& exc)
			{
				cout << "Error! Wrong input. Was something not a number?" << endl;
				commands.erase(commands.begin(), commands.end());
				temp.erase(file_ids_to_convert.begin(), file_ids_to_convert.end());
				temp.push_back(-1);
				break;
			}
		}
	}
	else
	{
		for (int i = 1; i < commands.size() - 1; ++i)
		{
			fs::path temp_path(commands[i]);

			if (!temp_path.has_extension()) temp_path.replace_extension(mesh_data_extension);

			if (temp_path.string()[0] == '/')
			{
				std::string temp_del(temp_path.string());
				temp_del.erase(0);
				temp_path = fs::path(temp_del);
			}

			if (temp_path.is_relative())
				temp_path = fs::path(meshes_folder_absolute.string() + (string)"\\" + temp_path.string());

			//make sure it is a file
			if (fs::is_regular_file(temp_path))
			{
				int temp_find = find(filenames.begin(), filenames.end(), temp_path) - filenames.begin();
				//make sure it has been loaded
				if (temp_find != filenames.size())
				{
					int temp_find_mesh = find(meshes.begin(), meshes.end(), mesh(temp_path)) - meshes.begin();
					if (temp_find_mesh != meshes.size())
					{
						temp.push_back(temp_find);
					}
				}
			}
			else cout << "File " << temp_path.string() << " not found." << endl;
		}
	}
	return temp;
}
