#include "stdafx.h"
#include "utilities.h"
#include <iostream>
#include <bitset>

unsigned int getFromCharArray(const char * I_arr)
{
	unsigned int temp(0);

	if (isLittleEndian)
	{
		for (int i = 0; i <4; ++i)
		{
			temp |= (((unsigned int)I_arr[i]) << (i * 8));
		}
	}
	else
	{
		for (int i = 0; i <4; ++i)
		{
			temp |= (((unsigned int)I_arr[i]) << ((3-i) * 8));
		}
	}

	return temp;
}
 
