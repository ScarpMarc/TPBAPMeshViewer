#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"
#include <algorithm>

using namespace std;

void mesh::tidyUp()
{
	for (unsigned int i = 0; i < faces_HQ.size(); ++i)
	{
		if (faces_HQ[i].material_n <= materials.size())
		{
			actually_used_materials_HQ.push_back(materials[faces_HQ[i].material_n - 1]);
			sort(actually_used_materials_HQ.begin(), actually_used_materials_HQ.end());
			actually_used_materials_HQ.erase(unique(actually_used_materials_HQ.begin(), actually_used_materials_HQ.end()), actually_used_materials_HQ.end());
		}
	}	
	if (type == 2)
	{
		for (unsigned int i = 0; i < faces_LQ.size(); ++i)
		{
			if (faces_LQ[i].material_n <= materials.size())
			{
				actually_used_materials_LQ.push_back(materials[faces_LQ[i].material_n - 1]);
				sort(actually_used_materials_LQ.begin(), actually_used_materials_LQ.end());
				actually_used_materials_LQ.erase(unique(actually_used_materials_LQ.begin(), actually_used_materials_LQ.end()), actually_used_materials_LQ.end());
			}
		}
	}
}

void mesh::ingest()
{
	ifstream file(file_path, std::ifstream::binary);
	data_4bytes reader;

	try
	{
		file.seekg(O_mesh_type, file.beg); //mesh type

		file.read(reader.c, 4); //get mesh type
		type = reader.ui;

		file.seekg(O_vertices, file.beg); //number of vertices

		file.read(reader.c, 4); //get number of vertices
		number_of_vertices_HQ = reader.ui;

		for (unsigned int i = 0; i < number_of_vertices_HQ; ++i)//skip vertices to get to the faces' data
		{
			std::streamoff to_skip;

			file.read(reader.c, 4); //get vertex size
			to_skip = reader.ui; //skip

			file.seekg(to_skip, ios::cur);
		}

		//we should be right at the number of faces
		face_data_offset_HQ = file.tellg();

		file.read(reader.c, 4); //get number of vertices
		number_of_faces_HQ = reader.ui;

		for (unsigned int i = 0; i < number_of_faces_HQ; ++i)//skip vertices to get to the faces' data
		{
			std::streamoff to_skip;

			file.read(reader.c, 4); //get face size
			to_skip = reader.ui; //skip

			file.seekg(to_skip, ios::cur);
		}

		if (type == 2)
		{
			file.seekg(12, ios::cur);  //skip unknown data before LQ mesh

			vertex_data_offset_LQ = file.tellg();

			file.read(reader.c, 4); //get number of vertices
			number_of_vertices_LQ = reader.ui;

			for (unsigned int i = 0; i < number_of_vertices_LQ; ++i)//skip vertices to get to the faces' data
			{
				std::streamoff to_skip;

				file.read(reader.c, 4); //get vertex size
				to_skip = reader.ui; //skip

				file.seekg(to_skip, ios::cur);
			}

			//we should be right at the number of faces
			face_data_offset_LQ = file.tellg();

			file.read(reader.c, 4); //get number of vertices
			number_of_faces_LQ = reader.ui;

			for (unsigned int i = 0; i < number_of_faces_LQ; ++i)//skip vertices to get to the faces' data
			{
				std::streamoff to_skip;

				file.read(reader.c, 4); //get face size
				to_skip = reader.ui; //skip

				file.seekg(to_skip, ios::cur);
			}
		}

		//materials
		file.seekg(4, ios::cur); //skip separator

		material_data_offset = file.tellg();

		file.read(reader.c, 4); //get number of materials
		number_of_materials = reader.ui;
	}
	catch (exception& exc)
	{
		error_flag = true;
		errors.push_back("Reading error in " + (string)__FUNCTION__ + " line " + (char)__LINE__ + "\nException thrown: " + exc.what());
		return;
	}
	

	vertices_HQ = findVertices(vertex_data_offset_HQ);
	if (type == 2) vertices_LQ = findVertices(vertex_data_offset_LQ);

	faces_HQ = findFaces(face_data_offset_HQ);
	if (type == 2) faces_LQ = findFaces(face_data_offset_LQ);

	materials = findMaterials(material_data_offset);

	tidyUp();
}

fs::path mesh::getFilePath() const
{
	return file_path;
}
