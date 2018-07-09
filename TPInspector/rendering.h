#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

extern GLFWwindow* window;

void initLibs();
void createWindow(const std::string& i_name = "TPI Mesh Viewer");
void draw(const std::vector<float>& i_3D_data);