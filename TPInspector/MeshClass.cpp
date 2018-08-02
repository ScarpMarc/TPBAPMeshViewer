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

	Mesh class file: implementation for the mesh class' miscellaneous functions - 
		including (but not limited to) functions that operate on its data.
*/

#include "stdafx.h"

#include "utilities.h"
#include "MeshClass.h"
#include "rendering.h"
#include <algorithm>

#include "OpenGL_Include\shader.hpp"

using namespace std;

vector<float> mesh::prepare3DDataForOpenGLRendering(const unsigned int& i_sub_mesh_to_load) const
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
				temp.push_back(materials[faces[i][j].material_n]);
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
