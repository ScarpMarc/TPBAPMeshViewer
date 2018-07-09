#include "stdafx.h"

#include "MeshClass.h"
#include "CMDDEFS.h"
#include "FILENAMEDEFS.h"
#include "utilities.h"
#include "rendering.h"

#include <iostream>
#include <sstream>
#include <ctime>

#include <experimental\filesystem>

namespace fs = std::experimental::filesystem;

using namespace std;

float pivot = 0.5;
fs::path exported_texture_file_names_folder_relative((std::string)"Extracted Texture Names Data");
fs::path exported_meshes_folder_relative("Extracted Data");
//fs::path meshes_folder_relative("BD_Meshes");
fs::path exported_texture_file_names_folder_absolute(fs::current_path().string() + (string)"/" + exported_texture_file_names_folder_relative.string());
fs::path exported_meshes_folder_absolute(fs::current_path().string() + (string)"/" + exported_meshes_folder_relative.string());
fs::path meshes_folder_absolute(fs::current_path().string()/* + (string)"/" + meshes_folder_relative.string()*/);
bool running = true;
std::vector<fs::path> filenames;
std::vector<int> file_ids_to_convert;
std::vector<int> file_ids_to_render;
std::vector<int> file_ids_to_save;
std::vector<std::string> commands;
std::vector<mesh> meshes;
GLFWwindow* window;

int main()
{
	cout << "Treasure Planet: Battle at Procyon mesh opener" << endl << endl << "by InkySka" << endl ;
	cout << "STANDALONE MODE " << endl << endl;
	do
	{
		string temp; //all-purpose
		
		cout << "TPI> ";

		getline(cin, temp);
		istringstream parser(temp);
		while (parser)
		{
			temp.erase();
			parser >> temp;
			if (temp != "\n")commands.push_back(temp);
		}
		
		//command reading
		if (commands[0] == "/find")
		{
			TPI_findValidFiles();
		}
		else if (commands[0] == "/list")
		{
			if (commands[1] == "-folder")
			{
				if (filenames.size() > 0) TPI_list(0);
				else cout << "No files in the list! Use /find first." << endl;
			}
			else if (commands[1] == "-toingest")
			{
				if (file_ids_to_convert.size() > 0) TPI_list(1);
				else cout << "No files marked for ingestion! Use /ingest first." << endl;
			}
			else if (commands[1] == "-torender")
			{
				if (file_ids_to_render.size() > 0) TPI_list(2);
				else cout << "No files marked for rendering! Use /render first." << endl;
			}
			else if (commands[1] == "-tosave")
			{
				if (file_ids_to_save.size() > 0) TPI_list(3);
				else cout << "No files marked for rendering! Use /render first." << endl;
			}
			else if (commands[1] == "-loaded")
			{
				if (meshes.size() > 0) TPI_list(4);
				else cout << "No files ingested! Use /ingest first." << endl;
			}
		}
		else if (commands[0] == "/ingest")
		{
			//filenames_to_convert.erase(filenames_to_convert.begin(), filenames_to_convert.end());
			file_ids_to_convert = TPI_I_normalizeInput(filenames.size());
			removeDuplicates(file_ids_to_convert);
			
			if (file_ids_to_convert.size() > 0) TPI_ingestMeshes();
			else cout << "No files to convert! Use /ingest first." << endl;
			//file_ids_to_convert.erase(file_ids_to_convert.begin(), file_ids_to_convert.end());
		}
		else if (commands[0] == "/render")
		{
			vector<int> temp_ids = TPI_I_normalizeInput(meshes.size());
			file_ids_to_render.insert(file_ids_to_render.end(), temp_ids.begin(), temp_ids.end());
			removeDuplicates(file_ids_to_render);

			if (file_ids_to_render.size() > 0)
			{
				initLibs();
				
				for (int i = 0; i < file_ids_to_render.size(); ++i)
				{
					createWindow("TPI mesh viewer: " + (meshes[file_ids_to_render[i]].getFilePath().stem().string()));
					vector<float> temp_3D_data(meshes[i].prepare3DDataForOpenGLRendering());
					draw(temp_3D_data);
					/*for (int i = 2; i < temp_3D_data.size(); i+=3)
					{
						cout << temp_3D_data[i - 2] << " " << temp_3D_data[i - 1] << " " << temp_3D_data[i] << endl;
					}*/
				}
			}
			else cout << "No files to convert! Use /ingest first." << endl;
		}
		else if (commands[0] == "/exit")
		{
			running = false;
		}
		else cout << "Wrong command!" << endl;

		commands.erase(commands.begin(), commands.end());
	} while (running);

	return 0;
}

