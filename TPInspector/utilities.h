#pragma once

#include <vector>

extern bool isLittleEndian;

const std::string master_extension(".mdb");

extern float pivot; //defined in main

int findVertices(std::vector<std::string> I_filenames);


int findFaces(std::vector<std::string> I_filenames);
int findMaterials(std::vector<std::string> I_filenames);
int meshCleaner(std::vector<std::string> I_filenames);
int saveOBJToFile(std::vector<std::string> I_filenames);
int saveMTLToFile(std::vector<std::string> I_filenames);
int saveTexturesToFile(std::vector<std::string> I_filenames);

std::vector<std::string> fileFinderByExtension(const std::string& I_folder, const std::string& I_ext, const bool& I_includeSubDirs = false);
std::string getFileExtension(const std::string& I_filename);

extern inline float pivoter(const float& I_val);

//unsigned int getFromCharArray(const char * I_arr);

/*struct vertex
{
	float x, y, z;
	float u, v;
	float transparency;
	std::vector<unsigned int> other_data; //currently unknown. placeholder
	bool isValid = true;

	unsigned int dim;
};

struct face
{
	short vertex1, vertex2, vertex3, material_n;
	bool isValid = true;
	bool hasMoreData = false;//for testing purposes
	std::vector<unsigned int> other_data; //even if there should be nothing

	unsigned int dim;
};

struct material
{
	std::string texture_name;
	std::vector<unsigned int> other_data;

	unsigned int dim;

	inline bool operator< (const material& other)
	{
		return this->texture_name < other.texture_name;
	}
	inline bool operator== (const material& other)
	{
		return this->texture_name == other.texture_name;
	}
};

struct mesh
{
	unsigned int number_of_faces, number_of_vertices, number_of_materials;

	std::streamoff face_data_offset;
	std::streamoff material_data_offset;
	std::streamoff other_data_offset;

	std::vector<vertex> vertices;
	std::vector<int> invalid_vertex_indices;
	std::vector<vertex> collisionData;

	std::vector<face> faces;
	std::vector<int> invalid_face_indices;

	std::vector<material> materials;
	std::vector<material> used_materials;
};*/

union data_4bytes
{
	int i;
	unsigned int ui;
	float f;
	char c[4];
	unsigned char uc[4];
	short s[2];
};