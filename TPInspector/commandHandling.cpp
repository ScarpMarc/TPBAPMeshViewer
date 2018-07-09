#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"

#include <experimental\filesystem>

#include <iostream>
#include <sstream>
#include <ctime>

namespace fs = std::experimental::filesystem;

using namespace std;

void TPI_ingestMeshes()
{
	clock_t start = 0, end = 0;
	double duration = 0;

	start = clock();
	for (unsigned int i = 0; i < file_ids_to_convert.size(); ++i)
	{
		clock_t start_sub, end_sub;
		double duration_sub;

		meshes.push_back(filenames[file_ids_to_convert[i]]);

		cout << "MESH " << i + 1 << " - " << meshes[i].getFilePath() << endl;

		cout << "Ingesting and tidying... (can take a while)" << endl;
		start_sub = clock();
		meshes[i].findOffsets();
		meshes[i].ingest();
		end_sub = clock();
		duration_sub = (end_sub - start_sub) / (double)CLOCKS_PER_SEC;
		cout << "Done in " << to_string(duration_sub) << " seconds" << endl << endl;
	}
	end = clock();
	duration = (end - start) / (double)CLOCKS_PER_SEC;

	if (file_ids_to_convert.size() > 1)
	{
		cout << "-----------------------" << endl;
		cout << "Done in a total of " << to_string(duration) << " seconds" << endl << endl;
	}
}

void TPI_saveToFile()
{
	clock_t start = 0, end = 0;
	double duration = 0;

	fs::create_directory(exported_texture_file_names_folder_absolute);
	fs::create_directory(exported_meshes_folder_absolute);

	start = clock();
	for (unsigned int i = 0; i < file_ids_to_convert.size(); ++i)
	{
		clock_t start_sub, end_sub;
		double duration_sub;

		cout << "Mesh: " << meshes[i].getFilePath() << endl;

		cout << "Saving..." << endl;
		start_sub = clock();
		meshes[i].saveToFile();
		end_sub = clock();
		duration_sub = (end_sub - start_sub) / (double)CLOCKS_PER_SEC;
		cout << "Done in " << to_string(duration_sub) << " seconds" << endl << endl;
		cout << endl;
	}
	end = clock();
	duration = (end - start) / (double)CLOCKS_PER_SEC;
	cout << "-----------------------" << endl;
	cout << "Done in a total of " << to_string(duration) << " seconds" << endl << endl;
	cout << "Exported 3D data folder: " << exported_meshes_folder_absolute << endl;
	cout << "Exported texture names folder: " << exported_texture_file_names_folder_absolute << endl << endl;
}

void TPI_findValidFiles(const std::string & I_extension)
{
	filenames = fileFinderByExtension(meshes_folder_absolute, I_extension);
	cout << "Found " << filenames.size() << " valid files (type /list to get the list)" << endl << endl;
}

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
		for (unsigned int i = 0; i < file_ids_to_convert.size(); ++i)
		{
			cout << i + 1 << ". " << filenames[file_ids_to_convert[i]] << endl;
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

void TPI_changeFolder()
{
}

void TPI_changeOutputFolder()
{
}

/*std::vector<fs::path> generateFilenameList()
{
	std::vector<fs::path>;
}*/


///<summary>
///Gets the user's input and makes it readable by the program.
///</summary>
///<returns>
///A vector of integers with the IDs of the files to be converted. The IDs are taken from the filenames vector.
///</returns>
vector<int> TPI_I_normalizeInput(const vector<int>& input)
{
	vector<int> temp;

	if (commands[1] == "-all")
	{
		return input;
	}
	if (commands[1] == "-list")
	{
		/*
		we want something like:
		1 4 7 9-12 15
		or	1, 4, 7, 9-12, 15
		*/
		for (int i = 2; i < commands.size() - 1; ++i)
		{
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

vector<int> TPI_I_normalizeInput(const int& input)
{
	vector<int> temp;

	if (commands[1] == "-all")
	{
		for (int i = 0; i < input; ++i)
		{
			temp.push_back(i);
		}
		return temp;
	}
	if (commands[1] == "-list")
	{
		/*
		we want something like:
		1 4 7 9-12 15
		or	1, 4, 7, 9-12, 15
		*/
		for (int i = 2; i < commands.size() - 1; ++i)
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
