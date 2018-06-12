#pragma once

#include <bitset> 

extern bool isLittleEndian;
unsigned int getFromCharArray(const char * I_arr);

struct vertex
{
	float x, y, z;
	std::string other_data; //currently unknown. placeholder
};

union data_4bytes
{
	int i;
	unsigned int ui;
	float f;
	char c[4];
};