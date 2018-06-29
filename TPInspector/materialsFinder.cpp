#pragma once
#include "stdafx.h"

#include <fstream>
#include <vector>
#include <string>

#include "utilities.h"

#include <iostream>

using namespace std;

int findMaterials(vector<string> I_filenames)
{
	ifstream file;
	for (int i = 0; i < I_filenames.size(); ++i)
	{
		cout << "Faces " << I_filenames[i] << endl;
		file.open(I_filenames[i], std::ifstream::binary);
		data_4bytes temp;

		file.seekg(meshes[i].material_data_offset, file.beg);//jump past the last face and the separator

		file.read(temp.c, 4);//get number of materials
		meshes[i].number_of_materials = temp.ui;

		cout <<std::hex<< "Offset " << meshes[i].material_data_offset << endl;
		cout << std::hex << "N. of m. " << meshes[i].number_of_materials << endl;
		meshes[i].materials.resize(meshes[i].number_of_materials);

		//get material data

		for (unsigned int j = 0; j < meshes[i].number_of_materials; ++j)
		{
			file.read(temp.c, 4); //number of bytes for the material data
			unsigned int numBytesMtl(temp.ui);
			cout << std::hex << "Bytes mtl " << numBytesMtl << endl;

			file.read(temp.c, 4); //read texture string length
			unsigned int numBytesStr(temp.ui);
			cout << std::hex << "Bytes str " << numBytesStr << endl;

			meshes[i].materials[i].texture_name = ""; //init

			for (int k = 0; k < numBytesStr; ++k)
			{
				char tempChar[1];
				file.read(tempChar, 1);
				meshes[i].materials[i].texture_name += tempChar[0];
			}
			cout << std::hex << "str " << meshes[i].materials[i].texture_name << endl;
			for (unsigned int k = (numBytesStr+4); k < numBytesMtl; k += 4)
			{
				file.read(temp.c, 4);
				meshes[i].materials[j].other_data.push_back(temp.ui);
			}
		}
		file.close();
	}
	return 0;
}


