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

	Mesh class Rendering file: implementation for the mesh class' rendering functions.
*/

#include "stdafx.h"

#include "MeshClass.h"
#include "rendering.h"

#include "OpenGL_Include\shader.hpp"

using namespace glm;

void mesh::render() const
{
	std::vector<float> data_3D_temp(prepare3DDataForOpenGLRendering());

	const char* i_name_c = file_path.string().c_str();
	// Open a window and create its OpenGL context


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 600, i_name_c, NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//create array
	const GLfloat* mesh_data = &data_3D_temp[0];

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders(vertex_shader_path.string().c_str(), fragment_shader_path.string().c_str());

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_data), mesh_data, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(mesh_data),                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}