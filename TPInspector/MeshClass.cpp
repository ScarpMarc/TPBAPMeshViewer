#include "stdafx.h"
#include "MeshClass.h"
#include <algorithm>

using namespace std;

void mesh::tidyUp()
{
	for (int i = 0; i < number_of_faces; ++i)
	{
		if (faces[i].material_n <= number_of_materials)
		{
			actually_used_materials.push_back(materials[faces[i].material_n - 1]);
			sort(actually_used_materials.begin(), actually_used_materials.end());
			actually_used_materials.erase(unique(actually_used_materials.begin(), actually_used_materials.end()), actually_used_materials.end());
		}
	}		
}

void mesh::ingest()
{

}
