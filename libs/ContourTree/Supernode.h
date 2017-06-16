//	Simplified Flexible Isosurfaces
//	Hamish Carr, 2003

//	Supernode.h:		class representing supernodes in contour tree

#ifndef SUPERNODE_H
#define SUPERNODE_H

//	include superarc, so we can get a couple of constants from it
#include "Superarc.h"

//	class representing a supernode in the contour tree
class Supernode
	{ // class Supernode
	public:
	//	pointers to lists of up- and down- arcs
	long upList, downList;					
	//	degree of node
	long upDegree, downDegree;
	//	pointer to the corresponding spatial sample
	float *value;
	//	and position for drawing
	float xPosn, yPosn;
	//	index into list of valid nodes
	long valid_index;
	//	the colour to use for this edge
	short colour;

	//	Initializer
	void Initialize(float *theValue);
	
	//	functions to test whether a node is a leaf, &c.
	bool IsUpperLeaf();
	bool IsLowerLeaf();
	bool IsRegular();
	}; // class Supernode

#endif