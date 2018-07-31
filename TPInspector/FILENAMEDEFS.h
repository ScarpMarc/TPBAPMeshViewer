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

	File names definitions.
*/

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


extern fs::path vertex_shader_path;
extern fs::path fragment_shader_path;