#pragma once

#include <vector>

extern bool isLittleEndian;

int findVertices(int argc, char** argv);

//unsigned int getFromCharArray(const char * I_arr);

struct vertex
{
	float x, y, z;
	std::vector<unsigned int> other_data; //currently unknown. placeholder
};

union data_4bytes
{
	int i;
	unsigned int ui;
	float f;
	char c[4];
};

extern std::vector<vertex> vertices;

