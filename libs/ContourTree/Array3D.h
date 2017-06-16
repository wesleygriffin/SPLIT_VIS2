//	IsoSurface II
//	Hamish Carr, 1999

//	Array3D.h:	template class for 3D arrays

#ifndef ARRAY_3D_H
#define ARRAY_3D_H 1

//	boolean controls for array
#define RANGE_CHECK 1																//	turn range-checking on
#define ZERO_ARRAY 1																//	initialize all arrays to 0's
#define MAX_ARRAY_SIZE 100000000														//	max. total elements

#ifdef DEBUG_MALLOC
#include "Malloc.h"
#endif

#include <stdlib.h>
#include <stdio.h>

template <class ElementType>
class Array3D
	{ // class Array3D
	private:
	ElementType *block;																//	the block of elements
	long xDim, yDim, zDim;															//	dimensions in each direction
	long nElements;																//	total number of elements
	long rowSize, sliceSize;															//	size of a row (and slice)
	ElementType dummy;
	
	public:	
	Array3D<ElementType>()															//	default constructor
		{ // Array3D constructor
		nElements = xDim = yDim = zDim = rowSize = sliceSize = 0;							//	make things predictable to start with
		block = NULL;																//	ditto
		} // Array3D constructor

	~Array3D<ElementType>()															//	destructor
		{ // Array3D destructor
		free(block);																//	release the block		
		} // Array3D destructor

	void Construct(long XDim, long YDim, long ZDim, ElementType *newBlock = NULL)				//	actual constructor - allows us to delay construction
		{ // Construct()
#ifdef RANGE_CHECK
		if ((XDim < 0) || (YDim < 0) || (ZDim < 0))										//	if any dimension is 0 or negative
			{ printf("Array size %ld x %ld x %ld is illegal\n", XDim, YDim, ZDim); return; }
		if ((XDim * YDim * ZDim < XDim * YDim)	|| (XDim * YDim < XDim))						//	i.e. computational rollover
			{ printf("Array size %ld x %ld x %ld caused computation of size to rollover.\n", XDim, YDim, ZDim);  return;}
		if (XDim * YDim * ZDim > MAX_ARRAY_SIZE)										//	more than we are prepared for right now	
			{ printf("Requested array size %ld x %ld x %ld has %ld elements: more than the %ld elements allowed.\n", XDim, YDim, ZDim, 
				XDim * YDim * ZDim, MAX_ARRAY_SIZE); return; }
#endif
		xDim = XDim; yDim = YDim; zDim = ZDim;											//	copy the parameters
		rowSize = zDim; sliceSize = zDim * yDim;										//	compute secondary parameters
		nElements = sliceSize * xDim;													//	compute total size
		if (block != NULL) free(block);												//	free up any previous memory
		if (newBlock == NULL)														//	if we weren't passed the data
			if (xDim * yDim * zDim <= 0) 												//	paranoia:  make sure we don't call malloc() improperly
				block = NULL;														//	empty array has a null pointer
			else
#ifdef ZERO_ARRAY
				block = (ElementType *) calloc(nElements, sizeof (ElementType));				//	use calloc() to allocate, and zero the elements
#else
				block = (ElementType *) malloc(nElements * sizeof (ElementType));			//	use malloc() to allocate without zeroing
#endif
		else // i.e. we were passed the data
			block = newBlock;														//	save a pointer to it
		} // Construct()

	ElementType & operator() (long x, long y, long z)										//	use function call to access array elements
		{
#ifdef RANGE_CHECK																	//	if range-checking is turned on
		if (block == NULL)															//	i.e. not constructed yet
			{ printf("Attempt to access element of uninitialized array.\n"); return dummy; }
		if ((x < 0) || (x >= xDim) || (y < 0) || (y >= yDim) || (z < 0) || (z >= zDim))			//	if any of the indices is out of range
			{ printf("Array reference (%ld,%ld,%ld) out of valid bounds: (0-%ld, 0-%ld, 0-%ld)\n", x, y, z, xDim-1, yDim-1, zDim-1); return *block; }
#endif
		return block[z + rowSize * y + sliceSize * x];									//	compute address & return
		} // end of operator ()

	long XDim() { return xDim; }														//	accessor to find out size
	long YDim() { return yDim; }														//	accessor to find out size
	long ZDim() { return zDim; }														//	accessor to find out size
	long RowSize() { return rowSize; }													//	accessor to find out rowsize
	long SliceSize() { return sliceSize; }												//	accessor to find out slicesize
	long NElements() { return nElements; }												//	accessor for nElements
	
	void ComputeIndex(ElementType *element, long &X, long &Y, long &Z)						//	computes position & returns it in X, Y, Z
		{ // ComputeIndex()	
		long position = element - block;												//	find the offset from the base of the array
#ifdef RANGE_CHECK
		if ((position < 0) || (position >= nElements))									//	check to see if the pointer is in the right range
			{ printf("Attempt to compute index of pointer (%X) outside valid bounds (%X-%X)\n", (long) element, (long) block, (long) block+nElements-1); return; }
#endif
		Z = position % zDim;														//	compute the z coordinate
		position = (position - Z) / zDim;												//	compute the x-y plane
		Y = position % yDim;														//	compute the y coordinate
		X = (position - Y) / yDim;													//	compute the column
		} // ComputeIndex()	
	}; // class Array3D

#endif	
