#pragma once

#include <experimental\filesystem>

namespace fs = std::experimental::filesystem;

const std::string mesh_data_extension(".mdb");

extern fs::path exported_texture_file_names_folder_relative;
extern fs::path exported_meshes_folder_relative;
extern fs::path meshes_folder_relative;

////

extern fs::path exported_texture_file_names_folder_absolute;
extern fs::path exported_meshes_folder_absolute;
extern fs::path meshes_folder_absolute;