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

	Mesh class file: implementation for the mesh class.
*/

#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"
#include <algorithm>

using namespace std;

/*
	void mesh::tidyUp()

	Tidies up the materials, generating the vector actually_used_materials.
	That is because there are some cases where a material is listed
		in the mesh file, but is not used by any polygon.
*/
void mesh::tidyUp()
{
	for (unsigned int i = 0; i < type; ++i)
	{
		vector<material> temp;
		for (unsigned int j = 0; j < faces[i].size(); ++j)
		{
			/*
				Creating a temporary vector with all of the materials actually used
			*/
			if (faces[i][j].material_n <= materials.size())
			{
				temp.push_back(materials[faces[i][j].material_n - 1]);
			}

			/*
				Sorting the temporary vector and erasing all redundant materials.
			*/
			sort(temp.begin(), temp.end());
			temp.erase(unique(temp.begin(), temp.end()), temp.end());
		}

		/*
			Populating actually_used_materials.
		*/
		actually_used_materials.push_back(temp);
	}	
}

/*
	void mesh::ingest()

	Ingests from file and then tidies up.
*/
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

/*
	fs::path mesh::getFilePath() const

	Returns file_path.
*/
fs::path mesh::getFilePath() const
{
	return file_path;
}

/*
	void mesh::scale(const float &i_scale)

	Scales the mesh.
*/
void mesh::scale(const float & i_scale)
{
	for (unsigned int i = 0; i < type; ++i)
	{
		for (unsigned int j = 0; j < vertices[i].size(); ++j)
		{
			vertices[i][j].x *= i_scale;
			vertices[i][j].y *= i_scale;
			vertices[i][j].z *= i_scale;
		}
	}
}

/*
	void mesh::findOffsets()

	Finds the offsets for every aspect of the mesh (eg. the vertices' data)
		and stores them in their variables.
	The function jumps past any data. It just collects the offsets.
*/
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

/*

*/
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


/*
std::vector<face> mesh::findFaces(const std::streamoff& i_offset)

*/
std::vector<face> mesh::findFaces(const std::streamoff& i_offset)
{
	ifstream file(file_path, std::ifstream::binary);
	data_4bytes reader;
	unsigned int number_of_faces_to_read;
	vector<face> output;

	try
	{
		file.seekg(i_offset, file.beg);//jump past the last vertex

		file.read(reader.c, 4);//get number of faces
		number_of_faces_to_read = reader.ui;
	}
	catch (exception& exc)
	{
		error_flag = true;
		errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: " + exc.what());
		return output;
	}

	//get face data
	for (unsigned int i = 0; i < number_of_faces_to_read; ++i)
	{
		face temp;
		try
		{
			file.read(reader.c, 4); //number of bytes for the face data. not really needed
			temp.dim = reader.ui;

			file.read(reader.c, 4);
			temp.verticesIDs.push_back(reader.s[0] + 1);
			temp.verticesIDs.push_back(reader.s[1] + 1);
			file.read(reader.c, 4);
			temp.verticesIDs.push_back(reader.s[0] + 1);
			temp.material_n = reader.s[1] + 1;

			if (temp.dim > 8) //if there are more than 8 bytes. failsafe
			{
				temp.hasMoreData = true;
				for (unsigned int j = 8; j < temp.dim + 1; j += 4)
				{
					file.read(reader.c, 4);
					temp.other_data.push_back(reader.ui);
				}
			}
		}
		catch (exception& exc)
		{
			error_flag = true;
			errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: " + exc.what());
			return output;
		}
		output.push_back(temp);
	}

	file.close();

	return output;
}

vector<material> mesh::findMaterials(const std::streamoff& i_offset)
{
	ifstream file(file_path, std::ifstream::binary);
	data_4bytes reader;
	vector<material> output;
	unsigned int number_of_materials_to_read;

	try
	{
		file.seekg(material_data_offset, file.beg);//jump past the last face and the separator
		file.read(reader.c, 4);//get number of materials
		number_of_materials_to_read = reader.ui;
	}
	catch (exception& exc)
	{
		error_flag = true;
		errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: " + exc.what());
		return output;
	}
	//get material data
	for (unsigned int i = 0; i < number_of_materials_to_read; ++i)
	{
		material temp;
		try
		{
			file.read(reader.c, 4); //number of bytes for the material data
			temp.dim = reader.ui;

			file.read(reader.c, 4); //read texture string length
			unsigned int numBytesStr(reader.ui);

			for (unsigned int j = 0; j < numBytesStr; ++j)
			{
				char tempChar[1];
				file.read(tempChar, 1);
				temp.texture_name += tempChar[0];
			}
			for (unsigned int j = (numBytesStr + 4); j < temp.dim; j += 4)
			{
				file.read(reader.c, 4);
				temp.other_data.push_back(reader.ui);
			}
		}
		catch (exception& exc)
		{
			error_flag = true;
			errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: " + exc.what());
			return output;
		}
		output.push_back(temp);
	}

	file.close();

	return output;
}