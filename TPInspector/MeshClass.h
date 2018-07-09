#pragma once
#include <fstream>
#include <vector>
#include <experimental\filesystem>
#include "FILENAMEDEFS.h"
#include "Offsets.h"

namespace fs = std::experimental::filesystem;

struct vertex
{
	std::vector<float> getCoords()
	{
		std::vector<float> output;
		output.push_back(x);
		output.push_back(y);
		output.push_back(z);
		return output;
	}
	float x, y, z;
	float u, v;
	float transparency;
	float other_data_a, other_data_b; //currently unknown. placeholder

	unsigned int dim;
};

struct face
{
	std::vector<unsigned short> getVerticesIDs()
	{
		return verticesIDs;
	}
	std::vector<unsigned short> verticesIDs;
	unsigned short material_n;

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


/// <summary>
/// The mesh class
/// </summary>
/// <remarks>
/// This class contains information on the actual mesh.
/// </remarks>
class mesh
{
public:
	mesh(const fs::path& i_file_path) :
		file_path(i_file_path) 
	{}

	void ingest();//read from file
	void findOffsets();
	void DEV_saveOtherData(const fs::path& i_folder_path = exported_meshes_folder_absolute) const;

	std::vector<std::vector<vertex>> getVertexData() const;
	std::vector<std::vector<face>> getFaceData() const;

	std::vector<float> prepare3DDataForOpenGLRendering(const unsigned int& i_sub_mesh_to_load = 0);

	void saveToFile() const;
	fs::path getFilePath() const;

	/*two meshes are equal if they are from the same file, 
	
	****OLD and to prevent
	any problems, they must also have the same indexes (which assures they have the same type,
	the same number of vertices, the same number of faces, etc)******
	*/
	inline bool operator ==(const mesh& other) const
	{
		return (this->file_path == other.file_path) /*&&
			(this->vertex_data_offset == other.vertex_data_offset) &&
			(this->face_data_offset == other.face_data_offset)*/
			;
	}
	
private:
	std::vector<vertex> findVertices(const std::streamoff& i_offset);
	std::vector<face> findFaces(const std::streamoff& i_offset);
	std::vector<material> findMaterials(const std::streamoff& i_offset);
	void saveOBJToFile(const fs::path& i_folder_path = exported_meshes_folder_absolute) const;
	void saveMTLToFile(const fs::path& i_folder_path = exported_meshes_folder_absolute) const;
	void saveTextureNamesToFile(const fs::path& i_folder_path = exported_texture_file_names_folder_absolute) const;
	void tidyUp();//populate actually used materials. there are some that appear in the mesh's file but are not used in the faces

	unsigned int type;

	fs::path file_path;

	std::vector<std::streamoff> vertex_data_offset;

	std::vector<std::streamoff> face_data_offset;

	std::streamoff material_data_offset;

	//std::streamoff collision_data_offset;

	std::vector<std::vector<vertex>> vertices;

	std::vector<std::vector<face>> faces;
	
	std::vector<material> materials;
	std::vector<std::vector<material>> actually_used_materials;

	bool error_flag = false;
	std::vector<std::string> errors;
};