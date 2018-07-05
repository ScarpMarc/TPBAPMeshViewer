#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"
#include <algorithm>

#include <iostream>

using namespace std;

void mesh::tidyUp()
{
	for (unsigned int i = 0; i < type; ++i)
	{
		vector<material> temp;
		for (unsigned int j = 0; j < faces[i].size(); ++j)
		{
			if (faces[i][j].material_n <= materials.size())
			{
				temp.push_back(materials[faces[i][j].material_n - 1]);
			}
			sort(temp.begin(), temp.end());
			temp.erase(unique(temp.begin(), temp.end()), temp.end());
		}
		actually_used_materials.push_back(temp);
	}	
}


void mesh::ingest()
{
	for (unsigned int i = 0; i < type; ++i)
	{
		vertices.push_back(findVertices(vertex_data_offset[i]));
		faces.push_back(findFaces(face_data_offset[i]));
	}

	materials = findMaterials(material_data_offset);
	tidyUp();
}

fs::path mesh::getFilePath() const
{
	return file_path;
}

void mesh::findOffsets()
{
	ifstream file(file_path, std::ifstream::binary);
	data_4bytes reader;

	try
	{
		file.seekg(O_mesh_type, file.beg); //mesh type

		file.read(reader.c, 4); //get mesh type
		type = reader.ui;

		file.seekg(O_vertices, file.beg); //number of vertices

		for (unsigned int i = 0; i < type; ++i)
		{
			vertex_data_offset.push_back(file.tellg());

			file.read(reader.c, 4); //get number of vertices
			unsigned int vertices_to_skip = reader.ui;

			for (unsigned int j = 0; j < vertices_to_skip; ++j)//skip vertices to get to the faces' data
			{
				std::streamoff to_skip;

				file.read(reader.c, 4); //get vertex size
				to_skip = reader.ui; //skip

				file.seekg(to_skip, ios::cur);
			}

			//we should be right at the number of faces
			face_data_offset.push_back(file.tellg());

			file.read(reader.c, 4); //get number of faces
			unsigned int faces_to_skip = reader.ui;

			for (unsigned int j = 0; j < faces_to_skip; ++j)//skip faces
			{
				std::streamoff to_skip;

				file.read(reader.c, 4); //get face size
				to_skip = reader.ui; //skip

				file.seekg(to_skip, ios::cur);
			}
			file.seekg(4, ios::cur); //skip separator
			if (type > (unsigned int)1 && i < type - (unsigned int)1)
				file.seekg(8, ios::cur); // skip some other stuff
		}

		//materials
		material_data_offset = file.tellg();
	}
	catch (exception& exc)
	{
		error_flag = true;
		errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: " + exc.what());
		return;
	}
	file.close();
}

vector<vector<vertex>> mesh::getVertexData() const
{
	vector<vector<vertex>> output;

	for (int i = 0; i < type; ++i)
	{
		vector<vertex> temp;
		for (int j = 0; j < vertices[i].size(); ++j)
		{
			temp.push_back(vertices[i][j]);
		}
		output.push_back(temp);
	}
	return output;
}

vector<vector<face>> mesh::getFaceData() const
{
	vector<vector<face>> output;

	for (int i = 0; i < type; ++i)
	{
		vector<face> temp;
		for (int j = 0; j < faces[i].size(); ++j)
		{
			temp.push_back(faces[i][j]);
		}
		output.push_back(temp);
	}
	return output;
}