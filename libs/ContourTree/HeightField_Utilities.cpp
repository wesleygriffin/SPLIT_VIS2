//	IsoSurface II
//	Hamish Carr, 2000

//	HeightField_Utilities.cpp:		utility routines
//	Palatino-12; 5 n-spaces/tab

#include "HeightField.h"													//	include the header for the definitions

long HeightField::Size()													//	returns the largest dimension
	{ // Size()
	return xDim > yDim ? xDim > zDim ? xDim : zDim: yDim > zDim ? yDim : zDim;	//	well, return it
	} // Size()
