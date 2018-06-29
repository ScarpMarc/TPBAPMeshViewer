#pragma once

#include <string>
#include <experimental\filesystem>

namespace fs = std::experimental::filesystem;

const std::string exportedFolderRelative("Extracted Texture Data");
const std::string meshesFolder("BD_Meshes");

extern fs::path exportedFolder;