#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"

using namespace std;

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
		errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: "+exc.what());
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
			temp.v = pivoter(reader.f); //pivoted value!
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
