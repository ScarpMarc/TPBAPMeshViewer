#pragma once

#include <vector>

extern bool isLittleEndian;

extern float pivot; //defined in main

int findVertices(const int& I_numMeshes, char** I_filename);
int findFaces(const int& I_numMeshes, char** I_filename);
int saveOBJToFile(const int& I_numMeshes, char** I_filename);
int saveMTLToFile(const int& I_numMeshes, char** I_filename);
extern inline float pivoter(const float& I_val);

//unsigned int getFromCharArray(const char * I_arr);

struct vertex
{
	float x, y, z;
	float u, v;
	std::vector<unsigned int> other_data; //currently unknown. placeholder
	bool isValid = true;
};

struct face
{
	short vertex1, vertex2, vertex3, material_n;
	bool isValid = true;
	bool hasMoreData = false;//for testing purposes
	std::vector<unsigned int> other_data; //even if there should be nothing
};

struct mesh
{
	unsigned int number_of_faces, number_of_vertices;

	std::streamoff face_data_offset;

	std::vector<vertex> vertices;
	std::vector<int> invalid_vertex_indices;

	std::vector<face> faces;
	std::vector<int> invalid_face_indices;
};

union data_4bytes
{
	int i;
	unsigned int ui;
	float f;
	char c[4];
	unsigned char uc[4];
	short s[2];
};

//extern std::vector<vertex> vertices; //temp only
extern std::vector<mesh> meshes;

