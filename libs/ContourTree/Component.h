//////////////////////////////////////////////////////////////////
//
//	Simplified Flexible Isosurface
//
//	Hamish Carr, 2003
//
//	Module:	Component.h
//
//	See Component.tex for details
//
//////////////////////////////////////////////////////////////////

#ifndef COMPONENT_H
#define COMPONENT_H 1

//	macro for adding / removing debug information
#define DEBUG_COMPONENT 1

#include <stddef.h>													//	needed for NULL, of all things

//	this class represents three different things:
//		i)	a component in the union-find structure when computing the join (split) tree
//		ii)	a superarc in the join (split) tree
//		iii)	additional information on seeds to be used later for generating paths
//	for i), we store a local pointer to the representative component, accessing it using Component() to perform path-compression
//	Remaining members are related to the superarc
//	Since we know that the values at the supernodes are unique, we can assume that there is a high-valued and low-valued vertex
//	this in turn allows us to avoid using half-edge representation, and to economize on storage as a result
//	We also need to be able to retrieve the superarcs at a given supernode, without wasting storage at non-super nodes:
//	Since we must already store a component pointer for each node, we can do the following:
//		i)	an ordinary (non-super) node N will point to a component which does not have N as hiEnd or loEnd: this can be detected, if needed
//		ii)	a supernode S will always point (in the join tree) to the superarc whose hiEnd is S (v.v. in the split tree)
//		iii)	we then need to associate the other superarcs at S with this initial superarc:  this is done by having circular edge-lists at each
//			supernode S:  thus Component(S) will give us the downwards superarc, followed by Component(S)->nextHi, Component(S)->nextHi->nextLo, &c.
//	Finally, for each superarc whose loEnd is S, we need to keep track of an adjacent vertex in that component:  once transferred to the contour tree, this will allow
//	us to find a path to use for interpolation

class Component													//	a component in Union-Find: dual existence as superarc in JT/ST
	{
	private:
	Component *ufComponent;										//	current (path-compressed) component in union-find structure
																//	private is to enforce path-compression
	public:
	float *hiEnd, *loEnd;											//	pointers to the top & bottom of the superarc
	Component *nextHi, *lastHi;										//	neighbours in circular edge-lists at supernodes
	Component *nextLo, *lastLo;										//	ditto for back-link

	float *seedFrom, *seedTo;										//	seed for transferral to contour tree

	Component() {ufComponent = this; nextHi = nextLo = lastHi = lastLo = NULL; 
#ifdef DEBUG_COMPONENT
		seedFrom = seedTo = hiEnd = loEnd = NULL;						//	do this to make debug code easier
#endif
														}		//	constructor sets it to point to itself initially
	
	Component *component()											//	path-compressing call to obtain the Union-Find component
		{														//	implemented in header to encourage inlining
		while (ufComponent->ufComponent != ufComponent)				//	as long as it's not pointing to the final representative
			ufComponent = ufComponent->ufComponent;					//	path-compress it
		return ufComponent;										//	return the one at the end
		} // end of component()

	void mergeTo(Component *newUF) { ufComponent = newUF;}				//	merges component onto another component

	}; // end of class Component

#undef DEBUG_COMPONENT
#endif
