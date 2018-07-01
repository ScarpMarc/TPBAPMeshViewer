#pragma once

#include "MeshClass.h"
#include <vector>

class gameObject
{
	std::vector<mesh> meshes;
	std::string name;
	unsigned int ID, parent_ID;
};