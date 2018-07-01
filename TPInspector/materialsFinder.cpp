#pragma once
#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"


using namespace std;

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