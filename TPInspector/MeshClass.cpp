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

	for (unsigned int i = 0; i < type; ++i)
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

	for (unsigned int i = 0; i < type; ++i)
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

vector<float> mesh::prepare3DDataForOpenGLRendering(const unsigned int& i_sub_mesh_to_load)
{
	vector<float> output;

	/*GLuint vertex_array_ID;
	glGenVertexArrays(1, &vertex_array_ID);
	glBindVertexArray(vertex_array_ID);*/
	unsigned int sub_mesh_to_load(i_sub_mesh_to_load);

	if (sub_mesh_to_load > vertices.size()) sub_mesh_to_load = 0; //basic check
																  /*
																  FORMAT

																  myMesh
																  {
																  {x y z
																  x y z
																  x y z}
																  {x y z
																  x y z
																  x y z}
																  etc
																  }

																  */

	for (unsigned int i = 0; i < faces[sub_mesh_to_load].size(); ++i)
	{
		vector<unsigned short> vertices_ids_current_face(faces[sub_mesh_to_load][i].getVerticesIDs());
		for (unsigned int j = 0; j < vertices_ids_current_face.size(); ++j)
		{
			vector<float> current_vertex_coordinates(vertices[sub_mesh_to_load][vertices_ids_current_face[j]].getCoords());
			for (unsigned int k = 0; k < 3; ++k)
			{
				output.push_back(current_vertex_coordinates[k]);
			}
		}
	}
	return output;
}