//	Simplified Flexible Isosurfaces
//	Hamish Carr, 2003

//	Supernode.cpp:		class representing supernodes in contour tree

#include "Supernode.h"
#include "Superarc.h"	// need this for the NO_SUPERARC constant

//	Initializer
void Supernode::Initialize(float *theValue)
	{ // Initialize()
	//	copy the known value
	value = theValue;
	//	and make the rest predictable
	upList = downList = NO_SUPERARC;
	upDegree = downDegree = 0;
	xPosn = yPosn = 0.5;
	valid_index = -1;
	colour = -1;
	} // Initialize()
	
//	functions to test whether a node is a leaf
bool Supernode::IsUpperLeaf()
	{ // IsUpperLeaf()
	return ((upDegree == 0) && (downDegree == 1));	
	} // IsUpperLeaf()
	
bool Supernode::IsLowerLeaf()
	{ // IsLowerLeaf()
	return ((upDegree == 1) && (downDegree == 0));	
	} // IsLowerLeaf()
	
bool Supernode::IsRegular()
	{ // IsRegular()
	return ((upDegree == 1) && (downDegree == 1));	
	} // IsRegular()