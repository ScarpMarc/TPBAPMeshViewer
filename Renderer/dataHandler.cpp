#include "stdafx.h"

#include <vector>
#include "functions.h"

using namespace std;

vector<GLfloat> prepare3DData(const mesh& i_mesh, const unsigned int i_sub_mesh_to_load)
{
	vector<GLfloat> output;

	/*GLuint vertex_array_ID;
	glGenVertexArrays(1, &vertex_array_ID);
	glBindVertexArray(vertex_array_ID);*/
	unsigned int sub_mesh_to_load(i_sub_mesh_to_load);

	vector<vector<vertex>> vertices(i_mesh.getVertexData());
	vector<vector<face>> faces(i_mesh.getFaceData());

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
			vector<GLfloat> current_vertex_coordinates(vertices[sub_mesh_to_load][vertices_ids_current_face[j]].getCoords());
			for (unsigned int k = 0; k < 3; ++k)
			{
				output.push_back(current_vertex_coordinates[k]);
			}
		}
	}
	return output;
}
