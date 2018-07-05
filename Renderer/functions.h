#pragma once

#include "..\..\TPInspector\TPInspector\MeshClass.h"

void initLibs();
void createWindow(const std::string& i_name = "TPI Mesh Viewer");
vector<GLfloat> prepare3DData(const mesh& i_mesh, const unsigned int i_sub_mesh_to_load = 0);