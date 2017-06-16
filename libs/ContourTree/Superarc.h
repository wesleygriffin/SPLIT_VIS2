//	IsoSurface II
//	Hamish Carr, 2000

//	Superarc.h:		class representing superarcs in CT
//	Palatino-12; 5 n-spaces/tab
#ifndef SUPERARC_H
#define SUPERARC_H 1

#include <stddef.h>														//	needed for NULL, of all things

//	this class represents superarcs in the contour tree, including seed information
//	Since we know that the values at the supernodes are unique, we can assume that there is a high-valued and low-valued vertex
//	this in turn allows us to avoid using half-edge representation, and to economize on storage as a result
//	WARNING: CHANGE TO PREVIOUS FUNCTIONALITY
//	This version keeps separate lists at supernodes for up-arcs and down-arcs
//	Thus, the circular lists between pointers are only for the up- or down- arcs
//	i.e. at each supernode, nextHi/lastHi cycles around the down arcs
//	nextLo/lastLo cycles around the up arcs

//	instead of NULL, define NO_SUPERARC and NO_SUPERNODE
#define NO_SUPERARC -1
#define NO_SUPERNODE -1

class Superarc															//	a Superarc in Union-Find: dual existence as superarc in JT/ST
	{
	public:
	//	"pointers" for circular lists at top and bottom
	long nextUp, lastUp;												//	circular list of up-arcs
	long nextDown, lastDown;												//	circular list of down-arcs

	//	"pointers" to supernodes at top and bottom
	long topID, bottomID;												//	ID of supernodes at the two ends

	//	used by flexible isosurface to keep track of where each arc is in arrays
	long valid_index;													//	used to store position in valid[]
	long index;														//	position in active[], selected[], restorable[]

	//	computed approximation of volume: node count
	long nodesOnArc, nodesThisSideOfTop, nodesThisSideOfBottom;					//	for counting the size of 
	//	this one assumes (for now) that the samples are actually short integers (16-bit) not floats
	float sampleSumTop, sampleSumBottom, sampleSumOnArc;

	//	for use in collapses: tracks the two arcs that merge to make a new arc
	long topArc, bottomArc;												//	ID's of top, bottom arcs in collapsed path

	//	one byte per superarc for various flags
	unsigned char arcFlags;												//	flags for whether the arc is live, &c.

	//	seeds for isosurface generation
	float *seedFromHi, *seedFromLo, *seedToHi, *seedToLo;						//	seeds for generating isosurfaces

	//	the seed value on this edge
	float seedValue;													//	isovalue of current contour

	//	the colour to use for this edge
	short colour;

	//	flags for various conditions: we use a single flag byte to store all of these
	static const unsigned char isValid = 1;
	static const unsigned char isActive = 2;
	static const unsigned char isSelected = 4;
	static const unsigned char isRestorable = 8;
	static const unsigned char isSuppressed = 16;
	static const unsigned char isDirty = 32;
	static const unsigned char isOnBackQueue = 64;
	static const unsigned char wasActiveBeforePruning = 128;

	//	no constructor, because we call malloc()
	void Initialize(long TopID, long BottomID);								//	initializer

	//	Routines to set seeds
	void SetHighSeed(float *SeedFromHi, float *SeedToHi);					//	set high end seed
	void SetLowSeed(float *SeedFromLo, float *SeedToLo);						//	set low end seed

	//	flag accessors
	void SetFlag(unsigned char whichFlag);									//	set a particular flag
	void ClearFlag(unsigned char whichFlag);								//	clear a particular flag		
	bool CheckFlag(unsigned char whichFlag);								//	and check a particular flag

	}; // end of class Superarc

#endif
