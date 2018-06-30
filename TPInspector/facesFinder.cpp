#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"

using namespace std;

void mesh::findFaces(std::streamoff i_offset)
{
	ifstream file(file_path, std::ifstream::binary);
	data_4bytes reader;

	try
	{
		file.seekg(i_offset, file.beg);//jump past the last vertex

		file.read(reader.c, 4);//get number of faces
		number_of_faces = reader.ui;
	}
	catch (exception& exc)
	{
		error_flag = true;
		errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: " + exc.what());
		return;
	}	

	//get face data
	for (unsigned int i = 0; i < number_of_faces; ++i)
	{
		face temp;
		try
		{
			file.read(reader.c, 4); //number of bytes for the face data. not really needed
			temp.dim = reader.ui;

			file.read(reader.c, 4);
			temp.vertex1 = reader.s[0] + 1;
			temp.vertex2 = reader.s[1] + 1;
			file.read(reader.c, 4);
			temp.vertex3 = reader.s[0] + 1;
			temp.material_n = reader.s[1] + 1;

			if (temp.dim > 8) //if there are more than 8 bytes
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
			return;
		}
	}
	
	file.close();
}
