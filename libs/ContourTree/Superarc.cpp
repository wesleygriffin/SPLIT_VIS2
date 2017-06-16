//	IsoSurface II
//	Hamish Carr, 2000

//	Superarc.cpp:		class representing superarcs in CT
//	Palatino-12; 5 n-spaces/tab
#include "Superarc.h"													//	needed for NULL, of all things

//	no constructor, because we call malloc()
void Superarc::Initialize(long TopID, long BottomID)							//	initializer
	{ // Initialize()
	topID = TopID; bottomID = BottomID;									//	set these to the known top & bottom

	//	initialize the rest to "empty"
	nextUp = lastUp = nextDown = lastDown = NO_SUPERARC;						//	set these to "none"
	valid_index = index = NO_SUPERARC;										//	and these
	topArc = bottomArc = NO_SUPERARC;										//	and these
	nodesOnArc = nodesThisSideOfTop = nodesThisSideOfBottom = 0;				//	zero out the volumetric information
	arcFlags = 0;														//	and the flags
	seedFromHi = seedToHi = seedFromLo = seedToLo = NULL;						//	and the seeds
	seedValue = 0.0;
	colour = -1;
	sampleSumTop = sampleSumBottom = sampleSumOnArc = 0;
	} // Initialize()

//	Routines to set seeds
void Superarc::SetHighSeed(float *SeedFromHi, float *SeedToHi)					//	set high end seed
	{ // SetHighSeed()
	seedFromHi = SeedFromHi; seedToHi = SeedToHi;							//	copy the pointers
	} // SetHighSeed()

void Superarc::SetLowSeed(float *SeedFromLo, float *SeedToLo)					//	set low end seed
	{ // SetLowSeed()
	seedFromLo = SeedFromLo; seedToLo = SeedToLo;							//	copy the pointers
	} // SetLowSeed()

void Superarc::SetFlag(unsigned char whichFlag)								//	set a particular flag
	{ // SetFlag()
	arcFlags |= whichFlag;
	} // SetFlag()
		
void Superarc::ClearFlag(unsigned char whichFlag)								//	clear a particular flag
	{ // ClearFlag()
	arcFlags &= ~whichFlag;												//	AND with bitwise inverse
	} // ClearFlag()
		
bool Superarc::CheckFlag(unsigned char whichFlag)								//	and check a particular flag
	{ // CheckFlag()
	return ((arcFlags & whichFlag) != 0);									//	check the flag and return boolean value
	} // CheckFlag()