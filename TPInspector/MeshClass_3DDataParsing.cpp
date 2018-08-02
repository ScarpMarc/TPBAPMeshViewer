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

Mesh class 3D Data Parsing file: implementation for the mesh class' 3D data-parsing functions.
*/

#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"

using namespace std;

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
			temp.material_n = reader.s[1];

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
			temp.id = i + 1;
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

			temp.texture_name.erase(temp.texture_name.rfind("."));
			temp.texture_name += (string) ".dds";

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

vector<vertex> mesh::findVertices(const std::streamoff& i_offset)
{
	ifstream file(file_path, std::ifstream::binary);
	data_4bytes reader;
	unsigned int number_of_vertices_to_read;
	vector<vertex> output;

	try
	{
		file.seekg(i_offset); //at the number of vertices

		file.read(reader.c, 4); //get number of vertices
		number_of_vertices_to_read = reader.ui;
	}
	catch (exception& exc)
	{
		error_flag = true;
		errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: " + exc.what());
		return output;
	}

	//get vertex data
	for (unsigned int i = 0; i < number_of_vertices_to_read; ++i)
	{
		vertex temp;
		try
		{
			file.read(reader.c, 4); //number of bytes for the vertex data
			temp.dim = reader.ui;

			//reading vertex[i]'s coordinates
			file.read(reader.c, 4);
			temp.x = reader.f;
			file.read(reader.c, 4);
			temp.y = reader.f;
			file.read(reader.c, 4);
			temp.z = reader.f;

			file.read(reader.c, 4);
			temp.u = reader.f;
			file.read(reader.c, 4);
			temp.v = reader.f; 
										/*
										file implementation is still obscure
										4 bytes at a time for future use
										*/
			file.read(reader.c, 4);
			temp.other_data_a = reader.f;
			file.read(reader.c, 4);
			temp.other_data_b = reader.f;

			/*for (unsigned int j = 32; j < temp.dim - 3; j += 4)
			{
			file.seekg
			}*/

			file.read(reader.c, 4);
			temp.transparency = reader.f;
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
