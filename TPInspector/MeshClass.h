#pragma once
#include <fstream>
#include <vector>
#include <experimental\filesystem>
#include "Offsets.h"

namespace fs = std::experimental::filesystem;

struct vertex
{
	float x, y, z;
	float u, v;
	float transparency;
	std::vector<unsigned int> other_data; //currently unknown. placeholder

	unsigned int dim;
};

struct face
{
	short vertex1, vertex2, vertex3, material_n;

	bool hasMoreData = false;//for testing purposes
	std::vector<unsigned int> other_data; //even if there should be nothing

	unsigned int dim;
};

struct material
{
	std::string texture_name;
	std::vector<unsigned int> other_data;

	unsigned int dim;

	inline bool operator< (const material& other) const
	{
		return this->texture_name < other.texture_name;
	}
	inline bool operator== (const material& other) const
	{
		return this->texture_name == other.texture_name;
	}
};

class mesh
{
public:
	void tidyUp();//populate actually used materials
	void ingest();//read from file //calls, in order, findVertices, findFaces, then findVertices again if necessary
	void findVertices(std::streamoff i_offset);
	void findFaces(std::streamoff i_offset);
	void findMaterials(std::streamoff i_offset);
	int type;

	fs::path file_path;

	unsigned int number_of_faces, number_of_vertices, number_of_materials;

	std::streamoff vertex_data_offset = O_vertices;
	std::streamoff face_data_offset;
	std::streamoff material_data_offset;
	std::streamoff collision_data_offset;

	std::vector<vertex> vertices;
	std::vector<vertex> collisionData;

	std::vector<face> faces;

	std::vector<material> materials;
	std::vector<material> actually_used_materials;

	bool error_flag = false;
	std::vector<std::string> errors;
};

