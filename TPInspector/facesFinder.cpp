#include "stdafx.h"
#include "utilities.h"
#include <fstream>

#include <iostream>

using namespace std;

int findFaces(std::vector<std::string> I_filenames)
{
	ifstream file;
	for (int i = 0; i < I_filenames.size(); ++i)
	{
		
		file.open(I_filenames[i], std::ifstream::binary);
		data_4bytes temp;

		file.seekg(meshes[i].face_data_offset, file.beg);//jump past the last vertex

		file.read(temp.c, 4);//get number of faces
		meshes[i].number_of_faces = temp.ui;

		meshes[i].faces.resize(meshes[i].number_of_faces);

		//get face data
		
		for (unsigned int j = 0; j < meshes[i].number_of_faces; ++j)
		{
			file.read(temp.c, 4); //number of bytes for the face data. not really needed
			unsigned int numBytesFace(temp.ui);

			file.read(temp.c, 4);
			meshes[i].faces[j].vertex1 = temp.s[0]+1;
			meshes[i].faces[j].vertex2 = temp.s[1]+1;
			file.read(temp.c, 4);
			meshes[i].faces[j].vertex3 = temp.s[0]+1;
			meshes[i].faces[j].material_n = temp.s[1]+1;

			if (numBytesFace > 8) //if there are more than 8 bytes
			{
				meshes[i].faces[j].hasMoreData = true;
				for (unsigned int k = 8; k < numBytesFace+1; k+=4)
				{
					file.read(temp.c, 4);
					meshes[i].faces[j].other_data.push_back(temp.ui);
				}
			}
		}
		//4-byte separator
		file.read(temp.c, 4);
		cout <<std::hex<< "\n\n\nTEMP READ "<<temp.ui << endl;
		file.seekg(temp.ui, ios::cur);
		meshes[i].material_data_offset = file.tellg();
		cout << "MTL OFFSET " << meshes[i].material_data_offset<<endl;
		file.close();
	}
	return 0;
}
