#pragma once
#include <fstream>
#include <vector>
#include <experimental\filesystem>
#include "FILENAMEDEFS.h"
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
	unsigned short vertex1, vertex2, vertex3, material_n;

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
	mesh(const fs::path& i_file_path) :
		file_path(i_file_path) 
	{
		vertex_data_offset_HQ = O_vertices;
	}

	void ingest();//read from file
	void saveToFile() const;
	fs::path getFilePath() const;
	
private:
	std::vector<vertex> findVertices(const std::streamoff& i_offset);
	std::vector<face> findFaces(const std::streamoff& i_offset);
	std::vector<material> findMaterials(const std::streamoff& i_offset);
	void saveOBJToFile(const fs::path& i_folder_path = exported_meshes_folder_absolute) const;
	void saveMTLToFile(const fs::path& i_folder_path = exported_meshes_folder_absolute) const;
	void saveTextureNamesToFile(const fs::path& i_folder_path = exported_texture_file_names_folder_absolute) const;
	void tidyUp();//populate actually used materials

	int type;

	fs::path file_path;

	unsigned int number_of_faces_HQ, number_of_vertices_HQ, number_of_materials,
		number_of_faces_LQ, number_of_vertices_LQ ;

	std::streamoff vertex_data_offset_HQ;
	std::streamoff vertex_data_offset_LQ;

	std::streamoff face_data_offset_HQ;
	std::streamoff face_data_offset_LQ;

	std::streamoff material_data_offset;

	std::streamoff collision_data_offset;


	std::vector<vertex> vertices_HQ;
	std::vector<vertex> vertices_LQ;

	std::vector<face> faces_HQ;
	std::vector<face> faces_LQ;

	std::vector<material> materials;
	std::vector<material> actually_used_materials_HQ;
	std::vector<material> actually_used_materials_LQ;

	bool error_flag = false;
	std::vector<std::string> errors;
};
