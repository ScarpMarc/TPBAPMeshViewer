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

Mesh class 3D Offset Parsing file: implementation for the mesh class' offset-parsing functions.
*/

#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"

using namespace std;

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
			if (type >(unsigned int)1 && i < type - (unsigned int)1)
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
