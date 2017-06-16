//	IsoSurface II
//	Hamish Carr, 2000

//	HeightField:			class representing the height field
//	HeightField_Creation:	routines dealing with construction, and computation of contour tree
//	Palatino-12; 10 m-spaces/tab

#include "HeightField.h"													//	include the header for the definitions
#include <math.h>														//	for HUGE_VAL
#include <stdio.h>
#include <GL/freeglut.h>
#include <iostream>
#include <ctime>

//#include "RicVolume.h"

using namespace std;

#define HEIGHT_FIELD_DEBUG_STEP_THROUGH 1
//#define DEBUG_JOIN_TREE 1												//	whether to include debug code for CreateJoinTree()
//#define DEBUG_SPLIT_TREE 1												//	whether to include debug code for CreateSplitTree()
//#define DEBUG_COMBINE_TREES 1											//	whether to include debug code for CombineTrees()
//#define DEBUG_CONSTRUCTOR 1												//	whether to include debug code for constructor
//#define DEBUG_QUEUE_NEIGHBOURS 1											//	whether to include debug code for QueueNeighbours
//#define DEBUG_ADD_SUPERARC 1											//	whether to include debug code for DEBUG_ADD_SUPERARC
//#define FLUSH_TIMING_BUFFER 0
#define FLUSH_TIMING_BUFFER 1
#define IGNORE_TIMING_BUFFER 0
//#define IGNORE_TIMING_BUFFER 1

//#define ENDIAN_SWAP_SHORTS 1

static int dotFileNo = 0;

//	declare a large memory buffer into which we can dump characters
char *timingBuffer, *timingStart;
//	some timing structures
time_t startTime, lastTime, thisTime, thatTime;

//	and a function to call at exit to print the buffer
void printTimingBuffer()
	{ // printTimingBuffer
	if (IGNORE_TIMING_BUFFER) return;
	printf("%s", timingStart);
	} // printTimingBuffer

//	this routine flushes the buffer to standard out
void flushTimingBuffer()
	{ // flushTimingBuffer
	if (! FLUSH_TIMING_BUFFER) return;
	if (IGNORE_TIMING_BUFFER) return;
	printf("%s", timingStart);
	// update the "start"
	timingBuffer = timingStart;
	*timingBuffer = '\0';
	} // flushTimingBuffer

//	compareHeightVoid():	compares two heights in an array
int compareHeightVoid(const void *e1, const void *e2)								//	comparison function for sorting, &c.
	{
	float **d1 = (float **) e1; float **d2 = (float **) e2;						//	convert to correct type
	return compareHeight(*d1, *d2);											//	and call the comparison function
	} // end of compareHeightVoid()

//	compareHeight():		compares two heights in an array
int compareHeight(const float *d1, const float *d2)								//	comparison function for all other purposes
	{
	if (*d1 < *d2) return -1;												//	and compute return values	
	if (*d1 > *d2) return 1;										
	if (d1 < d2) return -1;													//	break ties with pointer addresses (unique)	
	if (d1 > d2) return 1;
	printf("Major problem:  neither sorted higher.\n");
	printf("%p %p \n", d1, d2);
	return 0;
	} // end of compareHeight()

#define FILE_TYPE_ASCII 0
#define FILE_TYPE_VOL 1
#define FILE_TYPE_RAW 2
#define FILE_TYPE_MHA 3
#define FILE_TYPE_MIRG 4
#define FILE_TYPE_NIFTI 5

//	constructor:		constructs the object, and automatically computes the contour tree
HeightField::HeightField(int argc, char **argv)										//	constructor
	{ // HeightField()
//	HeightField() constructor needs to do the following:
//	A.	read in the dimensions
//	B.	set nVertices
//	C.	allocate leafQueue to a default size, and set leafQSize
//	D.	allocate height and heightSort
//	E.	read in the data, setting pointers in heightSort, and finding maxHeight and minHeight in the process
//	F.	call qsort() to sort heightSort
	//	set the first & last bytes of the buffer to '\0'
	timingStart = timingBuffer = (char *) malloc(TIMING_BUFFER_SIZE);
	timingBuffer[0] = '\0';
	timingBuffer[TIMING_BUFFER_SIZE - 1] = '\0';
	//gettimeofday(&lastFrameTime, NULL);
	//gettimeofday(&startTime, NULL);
	time(&lastFrameTime);
	time(&startTime);
	//timingBuffer += sprintf(timingBuffer, "Starting contour tree computation at: %d:%ld\n", startTime.tv_sec % 1000, (long) startTime.tv_usec);
	flushTimingBuffer();
	
// 	timingBuffer += sprintf(timingBuffer, "Reading data\n");
      
	long fileType = FILE_TYPE_ASCII;
     char *suffix;	//	suffix of file name
     suffix = strrchr(argv[1], '.');
     if (suffix != NULL)
     	{ // non-NULL suffix
		if (strcmp(suffix, ".vol") == 0)
			fileType = FILE_TYPE_VOL;
		else if (strcmp(suffix, ".raw") == 0)
			fileType = FILE_TYPE_RAW;
		else if (strcmp(suffix, ".i01") == 0)
			fileType = FILE_TYPE_RAW;
		else if (strcmp(suffix, ".img") == 0)
			fileType = FILE_TYPE_RAW;
  		else if (strcmp(suffix, ".mha") == 0)
			fileType = FILE_TYPE_MHA;
 		else if (strcmp(suffix, ".mirg") == 0)
			fileType = FILE_TYPE_MIRG;
		else if (strcmp(suffix, ".gz") == 0 || strcmp(suffix, ".nii") == 0)
			fileType = FILE_TYPE_NIFTI;
     	} // non-NULL suffix
      
     FILE *inFile;
	sampleSum = 0;
	switch(fileType)
		{ // switch on filetype
		case FILE_TYPE_ASCII:
			LoadFromASCII(argv[1]);
			break;
		case FILE_TYPE_RAW:
			if (argc < 6) { printf("Unable to open RAW format file %s unless sample size and dimensions are specified.\n", argv[1]); throw 0; }
			inFile = fopen(argv[1], "rb");
			if (inFile == NULL) { printf("Unable to open RAW format fiel %s\n", argv[1]); throw 0; }
			sampleSize = atol(argv[2]); if ((sampleSize < 1) || (sampleSize > 8)) { printf("Illegal sample size %d\n", sampleSize); throw 0; }
			xDim = atol(argv[3]); if ((xDim < 1) || (xDim > 2048)) { printf("Illegal x dimension %d\n", xDim); throw 0; }
			yDim = atol(argv[4]); if ((yDim < 1) || (yDim > 2048)) { printf("Illegal y dimension %d\n", yDim); throw 0; }
			zDim = atol(argv[5]); if ((zDim < 1) || (zDim > 2048)) { printf("Illegal z dimension %d\n", zDim); throw 0; }
			LoadFromRaw(inFile);
			break;
		case FILE_TYPE_VOL:
				{
				ifstream fin(argv[1]);
				if (!fin.is_open()) { printf("Unable to open file %s\n", argv[1]); throw 0; }
				fin >> zDim >> yDim >> xDim;
				if ((xDim < 1) || (xDim > 2048)) { printf("Illegal x dimension %d\n", xDim); throw 0; }
				if ((yDim < 1) || (yDim > 2048)) { printf("Illegal y dimension %d\n", yDim); throw 0; }
				if ((zDim < 1) || (zDim > 2048)) { printf("Illegal z dimension %d\n", zDim); throw 0; }
				}
			if (argc < 3) { printf("Unable to open VOL format file %s unless sample size is specified.\n", argv[1]); throw 0; }
			sampleSize = atol(argv[2]); if ((sampleSize < 1) || (sampleSize > 8)) { printf("Illegal sample size %d\n", sampleSize); throw 0; }
			inFile = fopen(argv[1], "rb");
			if (inFile == NULL) { printf("Unable to open VOL format file %s\n", argv[1]); throw 0; }
			//	rewind to the beginning of the block
			fseek(inFile, -xDim*yDim*zDim*sampleSize, SEEK_END);
			LoadFromRaw(inFile);
			break;
		case FILE_TYPE_MHA:
				{
                                FILE *dimFile = fopen(argv[1], "r");
				if (dimFile == NULL) { printf("Unable to open file %s\n", argv[1]); throw 0; }
				fscanf(dimFile, "NDims = 3\nDimSize = %d %d %d", &zDim, &yDim, &xDim);
                printf("Dimensions read: %d %d %d\n", xDim, yDim, zDim);
				if ((xDim < 1) || (xDim > 2048)) { printf("Illegal x dimension %d\n", xDim); throw 0; }
				if ((yDim < 1) || (yDim > 2048)) { printf("Illegal y dimension %d\n", yDim); throw 0; }
				if ((zDim < 1) || (zDim > 2048)) { printf("Illegal z dimension %d\n", zDim); throw 0; }
				}
			if (argc < 3) { printf("Unable to open MHA format file %s unless sample size is specified.\n", argv[1]); throw 0; }
			sampleSize = atol(argv[2]); if ((sampleSize < 1) || (sampleSize > 8)) { printf("Illegal sample size %d\n", sampleSize); throw 0; }
			inFile = fopen(argv[1], "rb");
			if (inFile == NULL) { printf("Unable to open MHA format file %s\n", argv[1]); throw 0; }
			//	rewind to the beginning of the block
			fseek(inFile, -xDim*yDim*zDim*sampleSize, SEEK_END);
			LoadFromRaw(inFile);
			break;
		case FILE_TYPE_MIRG:
				{
                FILE *dimFile = fopen(argv[1], "r");
				if (dimFile == NULL) { printf("Unable to open file %s\n", argv[1]); throw 0; }
				int nRead = fscanf(dimFile, "X =%d\nY =%d\nZ =%d", &zDim, &yDim, &xDim);
                printf("Dimensions read: %d %d %d\n", xDim, yDim, zDim);
				if ((xDim < 1) || (xDim > 2048)) { printf("Illegal x dimension %d\n", xDim); throw 0; }
				if ((yDim < 1) || (yDim > 2048)) { printf("Illegal y dimension %d\n", yDim); throw 0; }
				if ((zDim < 1) || (zDim > 2048)) { printf("Illegal z dimension %d\n", zDim); throw 0; }
				}
			sampleSize = 2;
			inFile = fopen(argv[1], "rb");
			if (inFile == NULL) { printf("Unable to open MIRG format file %s\n", argv[1]); throw 0; }
			//	rewind to the beginning of the block
			fseek(inFile, -xDim*yDim*zDim*sampleSize, SEEK_END);
			LoadFromRaw(inFile);
			break;
	/*	case FILE_TYPE_NIFTI:
		{
			FILE *dimFile = fopen(argv[1], "r");
			if (dimFile == NULL) { printf("Unable to open file %s\n", argv[1]); throw 0; }
			fclose(dimFile);
			RicVolume vol;
			vol.Read(argv[1]);
			zDim = vol.get_numz();
			yDim = vol.get_numy();
			xDim = vol.get_numx();
			printf("Dimensions read: %d %d %d\n", xDim, yDim, zDim);
			if ((xDim < 1) || (xDim > 2048)) { printf("Illegal x dimension %d\n", xDim); throw 0; }
			if ((yDim < 1) || (yDim > 2048)) { printf("Illegal y dimension %d\n", yDim); throw 0; }
			if ((zDim < 1) || (zDim > 2048)) { printf("Illegal z dimension %d\n", zDim); throw 0; }
			LoadFromNifti(argv[1]);
			break;
		}
*/
		} // switch on filetype      
    
    printf("SampleSum: %12.1f\n", sampleSum);
    
	// H.	invoke the routines to do the rest of construction
	CreateJoinTree();
	CreateSplitTree();
	AugmentTrees();
	CombineTrees();

	//	I.	clean up any dynamically allocated working memory	
	joinComponent.Construct(0, 0, 0);											//	reset join component to 0 logical size
	splitComponent.Construct(0, 0, 0);											//	reset split component to 0 logical size
//	we keep this, because it lets us know which vertices belong to which superarc
//	contourComponent.Construct(0, 0, 0);										//	reset contour component to 0 logical size	
	joinArcs.Construct(0,0,0); splitArcs.Construct(0,0,0);							//	likewise for join and split arc arrays
	free(leafQueue);
	leafQueue = NULL;
	free(heightSort);
	heightSort = NULL;
	
	//	J.	perform any additional processing (such as collapsing)
	SetNodeXPositions();													//	sets the x-positions of the nodes (must happen before epsilon collapse)
	CollapseEpsilonEdges();													//	collapses epsilon edges	
	collapseRecord = (long *) calloc(nNonEpsilonArcs, sizeof(long));				//	allocate array for collapse record
	collapseBounds = (long *) calloc(nNonEpsilonArcs, sizeof(long));				//	allocate array for collapse bounds
//	collapsePriority = PRIORITY_HYPERVOLUME;
//	CollapseLeafPruning(1, nVertices);												
	savedNextSuperarc = nextSuperarc;											//	store this for use in resetting collapse priority
	
	//	K.	allocate memory for variables associated with flexible isosurfaces
	active = (long *) malloc(sizeof(long) * nSuperarcs);							//	and ample room for the active set
	selected = (long *) malloc(sizeof(long) * nSuperarcs);							//	the selected set . . .
	restorable = (long *) malloc(sizeof(long) * nSuperarcs);						//	the restorable set
	selectionRoot = noContourSelected;											//	and, initially, no selection
	
	//	set the active, &c. lists
	nActiveArcs = nSelectedArcs = nRestorableArcs = 0;
	SetInitialColours();
	
	baseDisplayListID = glGenLists(nSuperarcs * 2);								//	create display lists for each superarc

	time(&thisTime);
// 	timingBuffer += sprintf(timingBuffer, "Construction complete at: %ld:%ld\n", thisTime.tv_sec % 1000, thisTime.tv_usec);
	//timingBuffer += sprintf(timingBuffer, "Construction took %8.5f seconds\n", (float)(thisTime.tv_sec - startTime.tv_sec) + 1E-6 * (thisTime.tv_usec - startTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Construction took %8.5f seconds\n", (float)difftime(thisTime,startTime));
	timingBuffer += sprintf(timingBuffer, "Contour tree has %ld superarcs\n", nSuperarcs);
	timingBuffer += sprintf(timingBuffer, "Memory occupied by contour tree & related structures: %ld\n", 
		nSuperarcs * sizeof(Superarc) * 2 + 			//	superarc storage, including collapsed superarcs
		nSupernodes * sizeof(Supernode) + 				//	supernode storage
		nSuperarcs * sizeof(long) * 5 +				//	valid (x2), active, selected, restorable
		nSupernodes * sizeof(long) +					//	validNodes
		nNonEpsilonArcs * sizeof(long) * 2				//	collapseRecord & collapseBounds
		);
	flushTimingBuffer();
	
	pathLengths = (long *) malloc(sizeof(long) * 2 * nSuperarcs);
	nContourTriangles = (long *) malloc(sizeof(long) * 2 * nSuperarcs);
	extractionTime = (float *) malloc(sizeof(float) * 2 * nSuperarcs);

 	for (int i = 0; i < 2 * nSuperarcs; i++)
 		{
 		pathLengths[i] = nContourTriangles[i] = 0; 
 		extractionTime[i] = 0.0;																	
		}
        PrintContourTree();
	//	register the function to dump the timing buffer to output
	atexit(printTimingBuffer);
	} // HeightField()
	
//	destructor:			destroys the object, and deallocates dynamically allocated storage
HeightField::~HeightField()													//	destructor
	{
	if (heightSort != NULL) free(heightSort);									//	release heightSort, if it exists
	if (leafQueue != NULL) free(leafQueue);										//	ditto for leaf queue	
	free(superarcs);														//	free the superarc array
	free(supernodes);														//	and the supernode array
	free(collapseRecord);
	free(collapseBounds);
	free(active); free(valid); free(validNodes); free(selected); free(restorable);
	} // end of destructor

//	CreateJoinTree()

void HeightField::CreateJoinTree()												//	does the down sweep to create the join tree
	{
//	CreateJoinTree() needs to do the following:
//	A.	create the array holding the join components, all initialized to NULL
//	B.	do a loop in downwards order, adding each vertex to the union-find:
//		i.	queue up the neighbours of the vertex
//		ii.	loop through all neighbours of the vertex:
//			a.	if the neighbour is lower than the vertex, skip
//			b.	if the neighbour belongs to a different component than the vertex
//				1.	and the vertex has no component yet, add the vertex to the component
//				2.	the vertex is a component, but is not yet a join, make it a join
//				3.	the vertex is a join, merge the additional component
//		iii.	if the vertex still has no (NULL) component, start a new one
//	C.	tie off the final component to minus_infinity

	long x, y, z;															//	coordinates of any given vertex
	long nbrX, nbrY, nbrZ;													//	coordinates of a neighbour
	long joinX, joinY, joinZ;												//	for computing top end of join arc
	long nNbrComponents;													//	# of neighbouring components
	Component *jComp;														//	local pointer to join component of vertex

//	A.	create the array holding the join components, all initialized to NULL
//	cout << "Starting computation of join tree." << endl;
	joinComponent.Construct(xDim, yDim, zDim);									//	create the join component array
	joinArcs.Construct(xDim, yDim, zDim);
	
	nSupernodes = 0;														//	initially, we know of none
	nJoinSupernodes = 0;	

//	B.	do a loop in downwards order, adding each vertex to the union-find:
	for (vertex = nVertices - 1; vertex >= 0; vertex--)							//	walk from high end of array
		{ // i
		if (vertex == nVertices - 2)
			{
			time(&thatTime);
// 			timingBuffer += sprintf(timingBuffer, "Starting computation of join tree at %ld: %ld\n", thatTime.tv_sec % 1000, thatTime.tv_usec);
// 			flushTimingBuffer();
			}
//		i.	queue up the neighbours of the vertex
		height.ComputeIndex(heightSort[vertex], x, y, z);							//	compute the indices of the vertex
#ifdef DEBUG_JOIN_TREE	
		printf("Examining vertex (%ld, %ld, %ld)\n", x, y, z);						//	print out the number of the vertex
//		PrintJoinComponents();
//		getc(stdin);
#endif

		QueueJoinNeighbours(x, y, z);										//	and set up the neighbour queue
		nNbrComponents = 0;												//	reset the count of neighbouring components
//		ii.	loop through all neighbours of the vertex:
		jComp = joinComponent(x, y, z);									//	retrieve a local pointer to the join component of the vertex
		for (long neighbour = 0; neighbour < nNeighbours; neighbour++)			//	loop through neighbours
			{
			nbrX = neighbourQueue[neighbour][0]; nbrY = neighbourQueue[neighbour][1]; nbrZ = neighbourQueue[neighbour][2];
//			a.	if the neighbour is lower than the vertex, skip
			if (compareHeight(&(height(nbrX, nbrY, nbrZ)), heightSort[vertex]) < 0)
																	//	if neighbour sorts lower
				continue;												//	skip to end of loop
			Component *nbrComponent = joinComponent(nbrX, nbrY, nbrZ)->component();
																	//	retrieve the neighbour's component from union-find
//			b.	if the neighbour belongs to a different component than the vertex
			if (jComp != nbrComponent)									//	compare components
				{ // B.ii.b.
//				1.	and the vertex has no component yet, add the vertex to the component
				if (nNbrComponents == 0)									//	this is the first neighbouring component
					{ // B.ii.b.1.
					joinComponent(x, y, z) = jComp = nbrComponent;			//	set the vertex to point to it
					nbrComponent->seedTo = (&height(nbrX, nbrY, nbrZ));		//	set the seed pointer to nbr (it's adjacent to the vertex)
					nbrComponent->seedFrom = heightSort[vertex];				//	but set both ends . . .
					height.ComputeIndex(nbrComponent->loEnd, joinX, joinY, joinZ);
					joinArcs(joinX, joinY, joinZ) = heightSort[vertex];		//	and add the corresponding join arc to the tree
					nbrComponent->loEnd = heightSort[vertex];				//	and update the lo end (for drawing purposes)
					nNbrComponents++;									//	increment the number of neighbouring components
					} // B.ii.b.1.
//				2.	the vertex is a component, but is not yet a join, make it a join
				else if (nNbrComponents == 1)								//	this is the second neighbouring component
					{ // B.ii.b.2.
					//	create a new component
//					printf("Join at (%3ld, %3ld, %3ld)\n", x, y, z); fflush(stdout);
					Component *newComponent = new Component;			//	create a new component
					newComponent->hiEnd = heightSort[vertex];				//	with the vertex at the high end
					newComponent->loEnd = heightSort[vertex];					//	make the low end predictable
					newComponent->nextHi = jComp;						//	set its nextHi pointer
					newComponent->lastHi = nbrComponent;					//	and its lastHi pointer
					//	update the neighbour's component
//					printf("Neighbour joining is: (%3ld, %3ld, %3ld)\n", nbrX, nbrY, nbrZ); fflush(stdout); 
					nbrComponent->seedTo = (&height(nbrX, nbrY, nbrZ));		//	set the seed pointer to nbr (it's adjacent to the vertex)
					nbrComponent->seedFrom = heightSort[vertex];			//	but set both ends . . .
					height.ComputeIndex(nbrComponent->loEnd, joinX, joinY, joinZ);
					joinArcs(joinX, joinY, joinZ) = heightSort[vertex];		//	and add the corresponding join arc to the tree
					nbrComponent->loEnd = heightSort[vertex];				//	make the vertex the low end of the neighbour's component
					nbrComponent->nextLo = newComponent;					//	set the nextLo pointer
					nbrComponent->lastLo = jComp;						//	and the lastLo pointer
					//	update the existing pointer for the vertex' component
//					height.ComputeIndex(jComp->hiEnd, nbrX, nbrY, nbrZ); 
//					printf("Existing component is: (%3ld, %3ld, %3ld)\n", nbrX, nbrY, nbrZ); fflush(stdout); 
					jComp->loEnd = heightSort[vertex];						//	make the vertex the low end of the component it was assigned to
					jComp->nextLo = nbrComponent;						//	set the nextLo pointer
					jComp->lastLo = newComponent;						//	and the lastLo pointer
					//	perform the merge
					nbrComponent->mergeTo(newComponent);				//	perform the actual merge
					jComp->mergeTo(newComponent);						//	for both old components
					//	and reset the join component
					joinComponent(x, y, z) = jComp = newComponent;			//	reset it
					nNbrComponents++;									//	increment the number of neighbouring components
					nSupernodes++;										//	and increment the number of supernodes
					nJoinSupernodes++;
					} // B.ii.b.2.
//				3.	the vertex is a join, merge the additional component
				else // i.e. nNbrComponents > 1
					{ // B.ii.b.3
					//	update the neighbour's component
					nbrComponent->seedTo = (&height(nbrX, nbrY, nbrZ));		//	set the seed pointer to nbr (it's adjacent to the vertex)
					nbrComponent->seedFrom = heightSort[vertex];			//	but set both ends . . .
					height.ComputeIndex(nbrComponent->loEnd, joinX, joinY, joinZ);
					joinArcs(joinX, joinY, joinZ) = heightSort[vertex];		//	and add the corresponding join arc to the tree
					nbrComponent->loEnd = heightSort[vertex];				//	make the vertex the low end of the neighbour's component
					nbrComponent->nextLo = jComp;						//	set the nextLo pointer
					nbrComponent->lastLo = jComp->lastHi;					//	and the lastLo pointer (NB: jComp is downwards)
					nbrComponent->lastLo->nextLo = nbrComponent;			//	reset the nextLo pointer of the old lastHi
					//	perform the merge
					nbrComponent->mergeTo(jComp);						//	perform the actual merge
					jComp->lastHi = nbrComponent;							//	and finally, the lastHi pointer
					} // B.ii.b.3
				} // B.ii.b. 
			} // neighbour
//		iii.	if the vertex still has no (NULL) component, start a new one
		if (jComp == NULL)												//	no neighbours found:  must be a local maximum
			{ // B.iii.
			joinComponent(x, y, z) = jComp = new Component;					//	create a new component
			jComp->hiEnd = jComp->loEnd = heightSort[vertex];					//	with the vertex at the high end (loEnd too, to give something to draw)
			jComp->loEnd = heightSort[vertex];								//	set it to something predictable
			jComp->nextHi = jComp;										//	set circular links to itself
			jComp->lastHi = jComp;										//	in both directions
			jComp->nextLo = jComp->lastLo = jComp;							//	set bottom end to point to self
			nSupernodes++;												//	and increment the number of supernodes
			nJoinSupernodes++;
			} // B.iii.
		} // i
//	C.	tie off the final component to minus_infinity
	height.ComputeIndex(jComp->loEnd, joinX, joinY, joinZ);					//	compute the low end
	joinArcs(joinX, joinY, joinZ) = &minus_infinity;							//	and add the corresponding join arc to the tree
	jComp->loEnd = &minus_infinity;										//	set the loEnd to minus infinity
	jComp->nextLo = jComp->lastLo = jComp;									//	set the circular list pointers
	joinRoot = jComp;													//	and store a pointer to it
#ifdef DEBUG_JOIN_TREE
//		PrintJoinComponents();
#endif
	time(&thisTime);
// 	timingBuffer += sprintf(timingBuffer, "Finished computation of join tree at %ld: %ld\n", thisTime.tv_sec % 1000, thisTime.tv_usec);
	timingBuffer += sprintf(timingBuffer, "Join Tree has %ld supernodes, and took %8.5f seconds to compute\n", nJoinSupernodes,
		//(float)(thisTime.tv_sec - thatTime.tv_sec) + 1E-6 * (thisTime.tv_usec - thatTime.tv_usec));
		(float)difftime(thisTime, thatTime));
	flushTimingBuffer();
//	PrintJoinTree();
	} // end of CreateJoinTree()
	
//	CreateSplitTree()
void HeightField::CreateSplitTree()										//	does the up sweep to create the split tree
	{
//	CreateSplitTree() needs to do the following:
//	A.	create the array holding the split components, all initialized to NULL
//	B.	do a loop in upwards order, adding each vertex to the union-find:
//		i.	queue up the neighbours of the vertex
//		ii.	loop through all neighbours of the vertex:
//			a.	if the neighbour is higher than the vertex, skip
//			b.	if the neighbour belongs to a different component than the vertex
//				1.	and the vertex has no component yet, add the vertex to the component
//				2.	the vertex is a component, but is not yet a split, make it a split
//				3.	the vertex is a split, merge the additional component
//		iii.	if the vertex still has no (NULL) component, start a new one
//	C.	tie off the final component to plus infinity
	long x, y, z;														//	coordinates of any given vertex
	long nbrX, nbrY, nbrZ;												//	coordinates of a neighbour
	long splitX, splitY, splitZ;											//	for computing bottom end of split arc
	long nNbrComponents;												//	# of neighbouring components
	Component *sComp;													//	local pointer to split component of vertex

#ifdef DEBUG_SPLIT_TREE
		printf("Starting computation of split tree\n");
#endif

//	A.	create the array holding the split components, all initialized to NULL
	time(&thatTime);
// 	timingBuffer += sprintf(timingBuffer, "Starting computation of split tree at %ld: %ld\n", thatTime.tv_sec % 1000, thatTime.tv_usec);
// 	flushTimingBuffer();
	splitComponent.Construct(xDim, yDim, zDim);								//	construct the array of split components
	splitArcs.Construct(xDim, yDim, zDim);
	nSplitSupernodes = 0;												//	reset count of split supernodes
	
//	B.	do a loop in upwards order, adding each vertex to the union-find:
	for (vertex = 0; vertex < nVertices; vertex++)							//	walk from low end of array
		{ // i
//		i.	queue up the neighbours of the vertex
		height.ComputeIndex(heightSort[vertex], x, y, z);						//	compute the indices of the vertex
#ifdef DEBUG_SPLIT_TREE
		printf("Examining vertex (%ld, %ld, %ld)\n", x, y, z);					//	print out the number of the vertex
//		PrintSplitComponents();
#endif

		QueueSplitNeighbours(x, y, z);									//	and set up the neighbour queue
		nNbrComponents = 0;												//	reset the count of neighbouring components
//		ii.	loop through all neighbours of the vertex:
		sComp = splitComponent(x, y, z);									//	retrieve a local pointer to the split component of the vertex
		for (long neighbour = 0; neighbour < nNeighbours; neighbour++)			//	loop through neighbours
			{
			nbrX = neighbourQueue[neighbour][0]; nbrY = neighbourQueue[neighbour][1]; nbrZ = neighbourQueue[neighbour][2];
//			a.	if the neighbour is higher than the vertex, skip
			if (compareHeight(&(height(nbrX, nbrY, nbrZ)), heightSort[vertex]) > 0)
																	//	if neighbour sorts higher
				continue;												//	skip to end of loop
#ifdef CHECK_INTERNAL_CONDITIONS
			if (compareHeight(&(height(nbrX, nbrY, nbrZ)), heightSort[vertex]) == 0)
																	//	this should *NEVER* be allowed to happen: a vertex cannot be its own neighbour
				{ printf("compareHeight says that %p (%f) and %p (%f) have identical heights\n", neighbour, *neighbour, heightSort[vertex], *(heightSort[vertex])); BAILOUT; } 
#endif			
			Component *nbrComponent = splitComponent(nbrX, nbrY, nbrZ)->component();
																	//	retrieve the neighbour's component from union-find
//			b.	if the neighbour belongs to a different component than the vertex
			if (sComp != nbrComponent)									//	compare components
				{ // B.ii.b.
//				1.	and the vertex has no component yet, add the vertex to the component
				if (nNbrComponents == 0)									//	this is the first neighbouring component
					{ // B.ii.b.1.
					splitComponent(x, y, z) = sComp = nbrComponent;			//	set the vertex to point to it
					nbrComponent->seedTo = (&height(nbrX, nbrY, nbrZ));		//	set the seed pointer to nbr (it's adjacent to the vertex)
					nbrComponent->seedFrom = heightSort[vertex];				//	but set both ends . . .
					height.ComputeIndex(nbrComponent->hiEnd, splitX, splitY, splitZ);
					splitArcs(splitX, splitY, splitZ) = heightSort[vertex];	//	and add the corresponding split arc to the tree
					nbrComponent->hiEnd = heightSort[vertex];				//	and update the hi end (for drawing purposes)
					nNbrComponents++;									//	increment the number of neighbouring components
					} // B.ii.b.1.
//				2.	the vertex is a component, but is not yet a split, make it a split
				else if (nNbrComponents == 1)								//	this is the second neighbouring component
					{ // B.ii.b.2.
					//	create a new component
					Component *newComponent = new Component;				//	create a new component
					newComponent->loEnd = heightSort[vertex];				//	with the vertex at the low end
					newComponent->hiEnd = heightSort[vertex];				//	make the high end something predictable
					newComponent->nextLo = sComp;							//	set its nextLo pointer
					newComponent->lastLo = nbrComponent;					//	and its lastLo pointer
					//	update the neighbour's component
					nbrComponent->seedTo = (&height(nbrX, nbrY, nbrZ));		//	set the seed pointer to nbr (it's adjacent to the vertex)
					nbrComponent->seedFrom = heightSort[vertex];				//	but set both ends . . .
					height.ComputeIndex(nbrComponent->hiEnd, splitX, splitY, splitZ);
					splitArcs(splitX, splitY, splitZ) = heightSort[vertex];	//	and add the corresponding split arc to the tree
					nbrComponent->hiEnd = heightSort[vertex];				//	make the vertex the high end of the neighbour's component
					nbrComponent->nextHi = newComponent;					//	set the nextHi pointer
					nbrComponent->lastHi = sComp;							//	and the lastHi pointer
					//	perform the merge
					nbrComponent->mergeTo(newComponent);					//	perform the actual merge
					sComp->mergeTo(newComponent);							//	for both old components
					//	update the existing pointer for the vertex' component
					sComp->hiEnd = heightSort[vertex];						//	make the vertex the high end of the component it was assigned to
					sComp->nextHi = nbrComponent;							//	set the nextHi pointer
					sComp->lastHi = newComponent;							//	and the lastHi pointer
					//	and reset the split component
					splitComponent(x, y, z) = sComp = newComponent;
					nNbrComponents++;									//	increment the number of neighbouring components
					//	check to see if its a join supernode
					if (joinComponent(x, y, z)->hiEnd != &height(x, y, z))		//	if it isn't a join supernode
						nSupernodes++;									//	increment the number of supernodes
					nSplitSupernodes++;
					} // B.ii.b.2.
//				3.	the vertex is a split, merge the additional component
				else // i.e. nNbrComponents > 1
					{ // B.ii.b.3
					//	update the neighbour's component
					nbrComponent->seedTo = (&height(nbrX, nbrY, nbrZ));		//	set the seed pointer to nbr (it's adjacent to the vertex)
					nbrComponent->seedFrom = heightSort[vertex];				//	but set both ends . . .
					height.ComputeIndex(nbrComponent->hiEnd, splitX, splitY, splitZ);
					splitArcs(splitX, splitY, splitZ) = heightSort[vertex];	//	and add the corresponding split arc to the tree
					nbrComponent->hiEnd = heightSort[vertex];				//	make the vertex the high end of the neighbour's component
					nbrComponent->nextHi = sComp;							//	set the nextHi pointer
					nbrComponent->lastHi = sComp->lastLo;					//	and the lastHi pointer (NB: sComp is upwards)
					nbrComponent->lastHi->nextHi = nbrComponent;				//	reset the nextHi pointer of the old lastLo
					//	perform the merge
					nbrComponent->mergeTo(sComp);							//	perform the actual merge
					sComp->lastLo = nbrComponent;							//	and finally, the lastLo pointer
					} // B.ii.b.3
				} // B.ii.b. 
			} // neighbour
//		iii.	if the vertex still has no (NULL) component, start a new one
		if (sComp == NULL)												//	no neighbours found:  must be a local maximum
			{ // B.iii.
			splitComponent(x, y, z) = sComp = new Component;					//	create a new component
			sComp->loEnd = heightSort[vertex];								//	with the vertex at the low end
			sComp->hiEnd = heightSort[vertex];								//	and, just for good measure, at the high end
			sComp->nextLo = sComp;										//	set circular links to itself
			sComp->lastLo = sComp;										//	in both directions
			sComp->nextHi = sComp->lastHi = sComp;							//	tie off the upper end
			//	check to see if its a join supernode
			if (joinComponent(x, y, z)->hiEnd != &height(x, y, z))				//	if it isn't a join supernode
				nSupernodes++;											//	increment the number of supernodes
			nSplitSupernodes++;
			} // B.iii.
		} // i
//	C.	tie off the final component to plus infinity
	height.ComputeIndex(sComp->hiEnd, splitX, splitY, splitZ);
	splitArcs(splitX, splitY, splitZ) = &plus_infinity;						//	and add the corresponding split arc to the tree
	sComp->hiEnd = &plus_infinity;										//	set the hiEnd to plus infinity
	sComp->nextHi = sComp->lastHi = sComp;									//	set the circular list pointers
	splitRoot = sComp;													//	and store a pointer to it
#ifdef DEBUG_SPLIT_TREE
//		PrintSplitComponents();
#endif
	time(&thisTime);
// 	timingBuffer += sprintf(timingBuffer, "Finished computation of join tree at %ld: %ld\n", thisTime.tv_sec % 1000, thisTime.tv_usec);
	timingBuffer += sprintf(timingBuffer, "Split Tree has %ld supernodes, and took %8.5f seconds to compute\n", nSplitSupernodes,
		//(float) (thisTime.tv_sec - thatTime.tv_sec) + 1E-6 * (thisTime.tv_usec - thatTime.tv_usec));
		difftime(thisTime, thatTime));
	flushTimingBuffer();
//	PrintSplitTree();
	} // end of CreateSplitTree()
	
void HeightField::AugmentTrees()											//	augments join and split trees with nodes of other
	{ // AugmentTrees()
//	A.	Allocate space for the leaf queue, contourComponent, and the contour tree
//	B.	Augment the join and split trees and construct the leaf queue:  loop through vertices from high to low
//		 i.	if a vertex is not in the split tree, but is in the join tree
//			a.	add to the split tree
//			b.	if the vertex is degree 1 in the join tree
//				1.	add to leaf queue
//		 i.	if a vertex is not in the join tree, but is in the split tree
//			a.	add to the join tree
//			b.	if the vertex is degree 1 in the split tree
//				1.	add to leaf queue
	long x, y, z;														//	coordinates of vertex
	float *theVertex;													//	pointer to vertex in data set
	Component *jComp, *sComp;											//	pointers to component in join & split trees

	time(&thatTime);
// 	timingBuffer += sprintf(timingBuffer, "Starting augmentation of trees at %ld: %ld\n", thatTime.tv_sec % 1000, thatTime.tv_usec);
// 	flushTimingBuffer();

//	A.	Allocate space for the leaf queue, contourComponent, and the contour tree
	nextSuperarc = nextSupernode = 0;										//	set index to next available superarc
	nSuperarcs = nSupernodes - 1;											//	for a tree, this is always true
	superarcs = (Superarc *) malloc((sizeof(Superarc) * 2 * nSuperarcs));			//	allocate room for collapsed edges as well
	supernodes = (Supernode *) malloc((sizeof(Supernode) * nSupernodes));			//	allocate room forsupernodes
	leafQueue = (float **) malloc((sizeof(float *) * (1 + nSupernodes)));		//	we need to add one because the last one will be added twice (due to pts at inf)
	leafQSize = 0;														//	no nodes on the leaf queue (yet)
	valid = (long *) malloc(sizeof(long) * nSuperarcs * 2);					//	allocate room for collapsed superarcs as well
	validNodes = (long *) malloc(sizeof(long) * nSupernodes);					//	the set of valid supernodes
	nValidNodes = nValidArcs = 0;
	
//	B.	Augment the join and split trees and construct the leaf queue:  loop through vertices from high to low
	for (vertex = nVertices - 1; vertex >= 0; vertex--)
		{ // B.

#ifdef DEBUG_COMBINE_TREES
//	printf("Join Tree: \n");
//	PrintJoinTree();
//	printf("Split Tree: \n");
//	PrintSplitTree();
#endif
		theVertex = heightSort[vertex];										//	retrieve the vertex from the sort list
		height.ComputeIndex(theVertex, x, y, z);								//	compute its indices
		jComp = joinComponent(x, y, z);										//	retrieve the corresponding join component
		sComp = splitComponent(x, y, z);									//	and the corresponding split component
//		 i.	if a vertex is not in the split tree but is in the join tree
//			this test suffices for two purposes:	a.	augmenting the split tree	(for obvious reasons)
//										b.	to help find upper leaves, which can be neither splits nor local minima, and are therefore not in the split tree
		if ((sComp->loEnd != theVertex) && (jComp->hiEnd == theVertex))			//	if the component did not start at this vertex, it's not in the split tree
			{ // B.i.
//			a.	add to the split tree
//				to do this, I splice an additional component on to the top end of the existing component: thus, the existing component is valid for future tests
			Component *splice = new Component;								//	create it
			splice->hiEnd = sComp->hiEnd;	splice->loEnd = theVertex;				//	set the ends of the splice
			sComp->hiEnd = theVertex;									//	reset the upper end of the existing component
			if (sComp->nextHi == sComp)									//	this only happens at plus infinity
				splice->nextHi = splice->lastHi = splice;							//	link it to itself
			else														//	any other case
				{ // else sComp isn't highest arc
				splice->nextHi = sComp->nextHi; splice->lastHi = sComp->lastHi;		//	grab the circular edge-list pointers from sComp
				if (splice->nextHi->loEnd == splice->hiEnd)						//	this happens if the next edge around hiEnd is upwards
					splice->nextHi->lastLo = splice;							//	so reset the low-end circular list there
				else 													//	i.e. next edge around hiEnd is also downwards
					splice->nextHi->lastHi = splice;							//	reset the high-end circular list there
				if (splice->lastHi->loEnd == splice->hiEnd)						//	basically the same as before, only in backwards direction
					splice->lastHi->nextLo = splice;							//	reset the lo pointer
				else 
					splice->lastHi->nextHi = splice;							//	reset the hi pointer
				} // else sComp isn't highest arc
			splice->seedFrom = sComp->seedFrom; splice->seedTo = sComp->seedTo;		//	make sure that any seed is in the top component
			sComp->seedFrom = sComp->seedTo = NULL;								//	make sure that any seed is in the top component
			splice->nextLo = splice->lastLo = sComp;							//	set up circular list around the vertex
			sComp->nextHi = sComp->lastHi = splice;								//	above and below
			splitComponent(x, y, z) = splice;									//	and set up the link in the split tree so that we can retrieve the edge-list
			nSplitSupernodes++;
//			b.	if the vertex is degree 1 in the join tree
			if (jComp->nextHi == jComp)									//	only one edge in the circular list at the upper end
//				1.	add to leaf queue	
				{ // B. i. 1.
//				printf("Adding leaf %2ld (%p) to leaf queue\n", leafQSize, theVertex);
				leafQueue[leafQSize++] = theVertex;							//	add the vertex to the leaf queue
				} // B. i. 1.
			} // B. i.
//		 ii.	if a vertex is not in the join tree, but is in the split tree: see above for explanation
		if ((jComp->hiEnd != theVertex) && (sComp->loEnd == theVertex))			//	if the component did not start at this vertex, it's not in the join tree
			{ // B.ii.
//			a.	add to the join tree
//				to do this, I splice an additional component on to the top end of the existing component: thus, the existing component is valid for future tests
			Component *splice = new Component;								//	create it
			splice->hiEnd = jComp->hiEnd;	splice->loEnd = theVertex;				//	set the ends of the splice
			jComp->hiEnd = theVertex;										//	reset the upper end of the existing component
			if (jComp->nextHi == jComp)									//	this happens at local maxima
				splice->nextHi = splice->lastHi = splice;							//	link it to itself
			else														//	any other case
				{ // else sComp isn't highest arc
				splice->nextHi = jComp->nextHi; splice->lastHi = jComp->lastHi;		//	grab the circular edge-list pointers from sComp
				//	this next chunk is simpler than above, since join tree always has down-degree of 1
				splice->nextHi->lastLo = splice;								//	so reset the low-end circular list there
				splice->lastHi->nextLo = splice;								//	reset the lo pointer
				} // else sComp isn't highest arc
			splice->nextLo = splice->lastLo = jComp;							//	set up circular list around the vertex
			jComp->nextHi = jComp->lastHi = splice;							//	above and below
			long xx, yy, zz;												//	used for grabbing upper end
			height.ComputeIndex(splice->hiEnd, xx, yy, zz);						//	compute index of upper end
			joinComponent(xx, yy, zz) = splice;								//	and set pointer into edge-list for next stage
			nJoinSupernodes++;
//			b.	if the vertex is degree 1 in the split tree
			if (sComp->nextLo == sComp)									//	only one edge in the circular list at the lower end
//				1.	add to leaf queue	
				{ // B.ii.b.1.
//				printf("Adding leaf %2ld (%p) to leaf queue\n", leafQSize, theVertex);
				leafQueue[leafQSize++] = theVertex;							//	add the vertex to the leaf queue
				} // B.ii.b.1.
			} // B. ii.
		} // B.
	time(&thisTime);
// 	timingBuffer += sprintf(timingBuffer, "Finished augmentation of trees at %ld: %ld\n", thisTime.tv_sec % 1000, thisTime.tv_usec);
	timingBuffer += sprintf(timingBuffer, "Augmentation took %8.5f seconds to compute\n", 
		//(float)(thisTime.tv_sec - thatTime.tv_sec) + 1E-6 * (thisTime.tv_usec - thatTime.tv_usec));
		difftime(thisTime,thatTime));
	flushTimingBuffer();
	} // AugmentTrees()

//	CombineTrees()
void HeightField::CombineTrees()											//	combines join & split trees to produce contour tree
	{ // CombineTrees()
//	CombineTrees() needs to do the following:
//	C.	Loop through the leaves on the leaf queue
//		i.	if upper leaf
//			a.	add to contour tree
//			b.	delete from join tree
//			c.	delete from split tree
//			d.	test other end to see if it should be added to leaf queue
//		ii.	if lower leaf
//			a.	add to contour tree
//			b.	delete from split tree
//			c.	delete from join tree
//			d.	test other end to see if it should be added to leaf queue
//	D.	Clean up excess memory, &c.
	long x, y, z;														//	coordinates of vertex
	long xNext, yNext, zNext;											//	for walking down the arcs
	float *theVertex;													//	pointer to vertex in data set
	Component *jComp, *sComp;											//	pointers to component in join & split trees
	float *otherEnd;													//	other end of the arc added
	long newSuperarc;													//	the arc we just added
	long upArc, downArc;												//	indices for walks around vertices

//	JoinArcsToDotFile("joinArcs", dotFileNo++);
//	SplitArcsToDotFile("splitArcs", dotFileNo++);
	
#ifdef DEBUG_COMBINE_TREES
//	printf("About to start merge loop.\n");
//	printf("Join Tree: \n");
//	PrintJoinTree();
//	printf("Join Components: \n");
//	PrintJoinComponents();
//	printf("Split Tree: \n");
//	PrintSplitTree();
//	printf("Split Components: \n");
//	PrintSplitComponents();
//	printf("Leaf Queue: \n");
//	PrintLeafQueue();
#endif
	time(&thatTime);
	//timingBuffer += sprintf(timingBuffer, "Starting to merge join and split trees at %ld: %ld\n", (long)thatTime.tv_sec % 1000, (long)thatTime.tv_usec);
	timingBuffer += sprintf(timingBuffer, "Starting to merge join and split trees now.\n");
	flushTimingBuffer();

	nodeLookup.Construct(xDim, yDim, zDim);									//	build array of supernode ID's
	for (int i = 0; i < xDim; i++)										//	walk through each dimension
		for (int j = 0; j < yDim; j++)
			for (int k = 0; k < zDim; k++)
				{
				nodeLookup(i, j, k) = NO_SUPERNODE;						//	setting the ID to predictable value
				visitFlags(i, j, k) = 1;									//	and set the "visit" flag, too
				}

//	C.	Loop through the leaves on the leaf queue
	for (vertex = 0; vertex < nSuperarcs; vertex++)							//	keep going until we've added n-1 edges
		{ // C.
//		CheckContourTree();
//		printf("%ld\n", vertex);
//		if (vertex % 10 == 0) { printf("."); fflush(stdout); }
		
#ifdef DEBUG_COMBINE_TREES
	printf("Loop %ld\n", vertex);
//	printf("Join Tree: \n");
//	PrintJoinTree();
//	printf("Split Tree: \n");
//	PrintSplitTree();
//	printf("Leaf Queue: \n");
//	PrintLeafQueue();
//	printf("Contour Components: \n");
//	PrintContourComponents();
//	getc(stdin);
//	BAILOUT;
#endif
		theVertex = leafQueue[vertex];									//	retrieve the vertex from the leaf queue
		height.ComputeIndex(theVertex, x, y, z);							//	compute its indices
		jComp = joinComponent(x, y, z);									//	retrieve the corresponding join component
		sComp = splitComponent(x, y, z);									//	and the corresponding split component
//		i.	if upper leaf
		if (jComp->nextHi == jComp)										//	since we already know that it's a leaf, this suffices
			{ // C. i.
//			a.	add to contour tree
			otherEnd = jComp->loEnd;										//	grab a pointer to the other end
			if (jComp->seedFrom == NULL)									//	if there wasn't a seed stored on the edge
				newSuperarc = AddSuperarc(theVertex, otherEnd, theVertex, FindDescendingNeighbour(theVertex), NULL, NULL);
			else	
				newSuperarc = AddSuperarc(theVertex, otherEnd, NULL, NULL, jComp->seedFrom, jComp->seedTo);
			//	now compute the sum of the nodes this side of the top end: initial -1 excludes the vertex proper
			superarcs[newSuperarc].nodesThisSideOfTop = superarcs[newSuperarc].nodesThisSideOfBottom = nVertices - 1;
			superarcs[newSuperarc].sampleSumTop = sampleSum - *theVertex;
			for (downArc = superarcs[newSuperarc].nextDown; downArc != newSuperarc; downArc = superarcs[downArc].nextDown)
				{ // other downarcs exist
				superarcs[newSuperarc].nodesThisSideOfTop -= superarcs[downArc].nodesThisSideOfTop;
				superarcs[newSuperarc].sampleSumTop -= superarcs[downArc].sampleSumTop;
//				ContourTreeToDotFile("building", dotFileNo++);
				} // other downarcs exist
			//	up arcs: first we have to check if any exist
			upArc = supernodes[superarcs[newSuperarc].topID].upList;
			if (upArc != NO_SUPERARC)
				{ // there are uparcs
				do
					{ // walk around them
					superarcs[newSuperarc].nodesThisSideOfTop -= superarcs[upArc].nodesThisSideOfBottom;
					superarcs[newSuperarc].sampleSumTop -= superarcs[upArc].sampleSumBottom;
					upArc = superarcs[upArc].nextUp;
					} // walk around them					
				while (upArc != supernodes[superarcs[newSuperarc].topID].upList);
				} // there are uparcs
				
			visitFlags(x, y, z) = 0;										//	set flag for the vertex to mark "visited"
			for (float *nextNode = joinArcs(x, y, z); nextNode != otherEnd; nextNode = joinArcs(xNext, yNext, zNext))
				{ // nextNode											//	walk along join arcs to transfer nodes
				height.ComputeIndex(nextNode, xNext, yNext, zNext);			//	find the x, y, z indices for the next step
//				printf("Walking past (%1d, %1d, %1d): %8.5f\n", xNext, yNext, zNext, *nextNode);
				if (visitFlags(xNext, yNext, zNext) == 1)					//	1 => vertex is not yet on a superarc
					{ // first time this vertex has been processed
//					printf(": counting.");
					visitFlags(xNext, yNext, zNext) = 0;					//	set to 0 to mark that its used, and reset flags for rendering
					height.ComputeIndex(nextNode, xNext, yNext, zNext);		//	find the x, y, z indices
					superarcs[newSuperarc].nodesOnArc++;					//	increment the superarcs node count
					superarcs[newSuperarc].sampleSumOnArc += height(xNext, yNext, zNext);
					} // first time this vertex has been processed
//				printf("\n");
				} // nextNode
			//	compute the count of nodes this side of the bottom
			superarcs[newSuperarc].nodesThisSideOfBottom = superarcs[newSuperarc].nodesOnArc + (nVertices - superarcs[newSuperarc].nodesThisSideOfTop); 
			superarcs[newSuperarc].sampleSumBottom = sampleSum - superarcs[newSuperarc].sampleSumTop + superarcs[newSuperarc].sampleSumOnArc;
//			b.	delete from join tree
//				note that otherEnd is degree 2 or higher (i.e. it is guaranteed to have a downarc, even for last edge, because of existence of -infinity
			if (jComp->nextLo->hiEnd == otherEnd)							//	if the next edge at low end is downwards
				jComp->nextLo->lastHi = jComp->lastLo;						//	reset its lastHi pointer
			else	jComp->nextLo->lastLo = jComp->lastLo;						//	otherwise reset the lastLo pointer
			if (jComp->lastLo->hiEnd == otherEnd)							//	if the last edge at low end is downwards
				jComp->lastLo->nextHi = jComp->nextLo;						//	reset its nextHi pointer
			else	jComp->lastLo->nextLo = jComp->nextLo;						//	otherwise reset the nextLo pointer
			delete jComp;												//	get rid of the jComp edge
			joinComponent(x, y, z) = NULL;									//	get rid of it in the jComponent array as well
			nJoinSupernodes--;
//			c.	delete from split tree
//				since we have +infinity, there will always be an up and a down arc
//				this is true even for the last edge, since we start at an upper leaf U.  This means that there is a down-arc to the other remaining node, L.
//				and since sComp is the departing edge travelling upwards, UL must be downwards in the split tree.
//				all we do is collapse sComp & sComp->nextLo onto sComp->nextLo
			if (sComp->nextHi == sComp)									//	i.e. sComp leads to +inf
				sComp->nextLo->nextHi = sComp->nextLo->lastHi = sComp->nextLo;	//	set the upper arcs to itself (i.e. no higher neighbours)
			else	//	not an edge to +inf
				{ // not edge to +inf
				sComp->nextLo->nextHi = sComp->nextHi;						//	set the upper arcs
				sComp->nextLo->lastHi = sComp->lastHi;
				} //  not edge to +inf
			sComp->nextLo->hiEnd = sComp->hiEnd;							//	transfer the high end
			sComp->nextLo->seedFrom = sComp->seedFrom;						//	transfer the seed as well
			sComp->nextLo->seedTo = sComp->seedTo;							//	transfer the seed as well
			if (sComp->nextHi->loEnd == sComp->hiEnd)						//	if nextHi is an up-arc
				sComp->nextHi->lastLo = sComp->nextLo;						//	adjust the low end
			else	sComp->nextHi->lastHi = sComp->nextLo;						//	otherwise the high end
			if (sComp->lastHi->loEnd == sComp->hiEnd)						//	if lastHi is an up-arc
				sComp->lastHi->nextLo = sComp->nextLo;						//	adjust the low end
			else	sComp->lastHi->nextHi = sComp->nextLo;						//	otherwise the high end
			delete sComp;												//	delete the now-useless edge
			splitComponent(x, y, z) = NULL;									//	get rid of it in the sComponent array as well
			nSplitSupernodes--;
//			d.	test other end to see if it should be added to leaf queue
//			we have reduced the up-degree of otherEnd in the join tree, and haven't changed the degrees in the split tree
			height.ComputeIndex(otherEnd, x, y, z);							//	compute indices
			sComp = splitComponent(x, y, z);								//	retrieve the split component
			jComp = joinComponent(x, y, z);									//	and the join component
			if (	(	(jComp->nextHi == jComp)								//	two ways otherEnd can be a leaf:  upper 
				&& 	(sComp->nextLo->nextHi == sComp)		)				//		(jComp has no "higher end" nbr, sComp has one each way)
			|| (	(	(sComp->nextLo == sComp)							//	or lower
				&& 	(jComp->nextHi->nextLo == jComp)		)	)	)		//		(sComp has no "lower end" nbr, jComp has one each way)
				{
//				printf("Adding leaf %2ld (%p) to leaf queue\n", leafQSize, otherEnd);
				leafQueue[leafQSize++] = otherEnd;							//	so add it already
				}
			} // C. i.
		else // i.e. a lower leaf
			{ // C. ii.
//			a.	add to contour tree
			otherEnd = sComp->hiEnd;										//	grab a pointer to the other end
			if (sComp->seedFrom == NULL)									//	if there wasn't a seed stored on the edge
				newSuperarc = AddSuperarc(otherEnd, theVertex, otherEnd, FindDescendingNeighbour(otherEnd), NULL, NULL);
			else	
				newSuperarc = AddSuperarc(otherEnd, theVertex, sComp->seedFrom, sComp->seedTo, NULL, NULL);	
																	//	add the superarc to the contour tree
//			if (newSuperarc == 175)
//				ContourTreeToDotFile("building", dotFileNo++);
			//	now compute the sum of the nodes this side of the top end: initial -1 excludes the vertex proper
			superarcs[newSuperarc].nodesThisSideOfTop = superarcs[newSuperarc].nodesThisSideOfBottom = nVertices - 1;
			superarcs[newSuperarc].sampleSumBottom = sampleSum - *theVertex;
//			printf("Bot: %8.5f\n", superarcs[newSuperarc].sampleSumBottom);
			for (upArc = superarcs[newSuperarc].nextUp; upArc != newSuperarc; upArc = superarcs[upArc].nextUp)
				{ // other uparcs exist
//				printf("Up arc %d\n", upArc);
				superarcs[newSuperarc].nodesThisSideOfBottom -= superarcs[upArc].nodesThisSideOfBottom;
				superarcs[newSuperarc].sampleSumBottom -= superarcs[upArc].sampleSumBottom;
//				printf("Bot: %8.5f \n", superarcs[newSuperarc].sampleSumBottom);
				} // other uparcs exist
			//	down arcs: first we have to check if any exist
			downArc = supernodes[superarcs[newSuperarc].bottomID].downList;
			if (downArc != NO_SUPERARC)
				{ // there are downarcs
				do
					{ // walk around them
					superarcs[newSuperarc].nodesThisSideOfBottom -= superarcs[downArc].nodesThisSideOfTop;
					superarcs[newSuperarc].sampleSumBottom -= superarcs[downArc].sampleSumTop;
//					printf("Bot: %8.5f\n ", superarcs[newSuperarc].sampleSumBottom);
					downArc = superarcs[downArc].nextDown;
					} // walk around them					
				while (downArc != supernodes[superarcs[newSuperarc].bottomID].downList);
				} // there are downarcs
			
//			printf("On: %8.5f\n", superarcs[newSuperarc].sampleSumOnArc);
			visitFlags(x, y, z) = 0;										//	set flag for the vertex to mark "visited"
			for (float *nextNode = splitArcs(x, y, z); nextNode != otherEnd; nextNode = splitArcs(xNext, yNext, zNext))
				{ // nextNode											//	walk along split arcs to transfer nodes
				height.ComputeIndex(nextNode, xNext, yNext, zNext);			//	find the x, y, z indices
//				printf("Walking past (%1d, %1d, %1d): %8.5f\n", xNext, yNext, zNext, *nextNode);
				if (visitFlags(xNext, yNext, zNext))						//	1 => vertex is not yet on a superarc
					{ // first time this vertex has been processed
//					printf(": counting.");
					visitFlags(xNext, yNext, zNext) = 0;					//	set to 0 to mark that its used, and reset flags for rendering
					height.ComputeIndex(nextNode, xNext, yNext, zNext);		//	find the x, y, z indices
					superarcs[newSuperarc].nodesOnArc++;					//	increment the superarcs node count
					superarcs[newSuperarc].sampleSumOnArc += height(xNext, yNext, zNext);
//					printf("On: %8.5f\n", superarcs[newSuperarc].sampleSumOnArc);
					} // first time this vertex has been processed
//				printf("\n");
				} // nextNode

			//	compute the count of nodes this side of the top
			superarcs[newSuperarc].nodesThisSideOfTop = superarcs[newSuperarc].nodesOnArc + (nVertices - superarcs[newSuperarc].nodesThisSideOfBottom); 
			superarcs[newSuperarc].sampleSumTop = sampleSum - superarcs[newSuperarc].sampleSumBottom + superarcs[newSuperarc].sampleSumOnArc;
//			printf("Top: %8.5f\n", superarcs[newSuperarc].sampleSumTop);
//			b.	delete from split tree
//				note that otherEnd is degree 2 or higher (i.e. it is guaranteed to have a up arc, even for last edge, because of existence of -infinity)
			if (sComp->nextHi->loEnd == otherEnd)							//	if the next edge at high end is upwards
				sComp->nextHi->lastLo = sComp->lastHi;						//	reset its lastLo pointer
			else	sComp->nextHi->lastHi = sComp->lastHi;						//	otherwise reset the lastHi pointer
			if (sComp->lastHi->loEnd == otherEnd)							//	if the last edge at high end is upwards
				sComp->lastHi->nextLo = sComp->nextHi;						//	reset its nextLo pointer
			else	sComp->lastHi->nextHi = sComp->nextHi;						//	otherwise reset the nextHi pointer
			delete sComp;												//	get rid of the sComp edge
			splitComponent(x, y, z) = NULL;									//	get rid of it in the sComponent array as well
			nSplitSupernodes--;
//			c.	delete from join tree
//				since we have -infinity, there will always be an up and a down arc
//				this is true even for the last edge, since we start at a lower leaf L.  This means that there is an up-arc to the other remaining node, U.
//				and since jComp is the departing edge travelling downwards, LU must be upwards in the split tree.
//				all we do is collapse jComp & jComp->nextHi onto jComp->nextHi
			if (jComp->nextLo == jComp)									//	i.e. jComp leads to -inf
				jComp->nextHi->nextLo = jComp->nextHi->lastLo =jComp->nextHi;	//	set the lower arcs to itself (i.e. no higher neighbours)
			else	//	not an edge to -inf
				{ // not edge to -inf
				jComp->nextHi->nextLo = jComp->nextLo;						//	set the upper arcs
				jComp->nextHi->lastLo = jComp->lastLo;
				} //  not edge to -inf
			jComp->nextHi->loEnd = jComp->loEnd;							//	transfer the low end
			jComp->nextHi->seedFrom = jComp->seedFrom;						//	transfer the seed
			jComp->nextHi->seedTo = jComp->seedTo;							//	transfer the seed
			if (jComp->nextLo->hiEnd == jComp->loEnd)						//	if nextLo is a down-arc
				jComp->nextLo->lastHi = jComp->nextHi;						//	adjust the high end
			else	jComp->nextLo->lastLo = jComp->nextHi;						//	otherwise the low end
			if (jComp->lastLo->hiEnd == jComp->loEnd)							//	if lastLo is a down-arc
				jComp->lastLo->nextHi = jComp->nextHi;						//	adjust the high end
			else	jComp->lastLo->nextLo = jComp->nextHi;						//	otherwise the low end
			delete jComp;												//	delete the now-useless edge
			joinComponent(x, y, z) = NULL;									//	get rid of it in the jComponent array as well
			nJoinSupernodes--;
//			d.	test other end to see if it should be added to leaf queue
//			we have reduced the up-degree of otherEnd in the join tree, and haven't changed the degrees in the split tree
			height.ComputeIndex(otherEnd, x, y, z);							//	compute indices
			sComp = splitComponent(x, y, z);								//	retrieve the split component
			jComp = joinComponent(x, y, z);								//	and the join component
			if (	(	(jComp->nextHi == jComp)								//	two ways otherEnd can be a leaf:  upper 
				&& 	(sComp->nextLo->nextHi == sComp)		)				//		(jComp has no "higher end" nbr, sComp has one each way)
			|| (	(	(sComp->nextLo == sComp)								//	or lower
				&& 	(jComp->nextHi->nextLo == jComp)		)	)	)		//		(sComp has no "lower end" nbr, jComp has one each way)
				{
//				printf("Adding leaf %2ld (%p) to leaf queue\n", leafQSize, otherEnd);
				leafQueue[leafQSize++] = otherEnd;							//	so add it already
				}
			} // C. ii.
		} // C.
	// (x, y, z) i now set for the last of the "other ends", so set the visitFlag to 0
	visitFlags(x, y, z) = 0;
//	D.	Clean up excess memory, &c.
	//	start off by getting rid of the two remaining edges in the join and split tree
	nodeLookup.Construct(0,0,0);
//	delete joinRoot;														//	delete the root (remaining arc) in join tree
//	CheckContourTree();
//	delete splitRoot;														//	delete the root (remaining arc) in split tree
	joinRoot = splitRoot = NULL;											//	and reset to something predictable
	time(&thisTime);
 	timingBuffer += sprintf(timingBuffer, "Finished merging trees mow.\n");
	timingBuffer += sprintf(timingBuffer, "Merging took %8.5f seconds to compute: contour tree has %1d supernodes\n",
		//(float)(thisTime.tv_sec - thatTime.tv_sec) + 1E-6 * ((long)thisTime.tv_usec - (long)thatTime.tv_usec), nSupernodes);
		(float)difftime(thisTime, thatTime), nSupernodes);
	flushTimingBuffer();
	//PrintContourTree();
	} // end of CombineTrees()
	
//	QueueNeighbour()
void HeightField::QueueNeighbour(long x, long y, long z)							//	queues up the given neighbour, if it is in the field
	{
	if (x < 0) return;
	if (x >= xDim) return;
	if (y < 0) return;
	if (y >= yDim) return;
	if (z < 0) return;
	if (z >= zDim) return;
	neighbourQueue[nNeighbours][0] = x;
	neighbourQueue[nNeighbours][1] = y;
	neighbourQueue[nNeighbours][2] = z;
	nNeighbours++;
	} // end of QueueNeighbour()
	
//	QueueJoinNeighbours()
void HeightField::QueueJoinNeighbours(long x, long y, long z)						//	queues up all neighbours in join sweep
	{
	nNeighbours = 0;													//	reset the size of the queue

	QueueNeighbour(x-1, y, z); QueueNeighbour(x+1, y, z); 						//	corner adjacencies:  x-axis
	QueueNeighbour(x, y-1, z); QueueNeighbour(x, y+1, z); 						//	y-axis
	QueueNeighbour(x, y, z-1); QueueNeighbour(x, y, z+1);							//	z-axis
	} // end of QueueJoinNeighbours()

//	QueueSplitNeighbours()
void HeightField::QueueSplitNeighbours(long x, long y, long z)						//	queues up all neighbours in join sweep
	{
	nNeighbours = 0;													//	reset the size of the queue

	QueueNeighbour(x-1, y, z); QueueNeighbour(x+1, y, z); 						//	corner adjacencies:  x-axis
	QueueNeighbour(x, y-1, z); QueueNeighbour(x, y+1, z); 						//	y-axis
	QueueNeighbour(x, y, z-1); QueueNeighbour(x, y, z+1);							//	z-axis
	QueueNeighbour(x-1, y-1, z); QueueNeighbour(x+1, y+1, z);					//	face adjacencies: xy
	QueueNeighbour(x+1, y-1, z); QueueNeighbour(x-1, y+1, z);					//	face adjacencies: xy
	QueueNeighbour(x, y-1, z-1); QueueNeighbour(x, y+1, z+1); 					//	yz
	QueueNeighbour(x, y+1, z-1); QueueNeighbour(x, y-1, z+1); 					//	yz
	QueueNeighbour(x-1, y, z-1); QueueNeighbour(x+1, y, z+1);					//	xz
	QueueNeighbour(x-1, y, z+1); QueueNeighbour(x+1, y, z-1);					//	xz
	} // end of QueueSplitNeighbours()

long HeightField::AddSuperarc(float *hiEnd, float *loEnd, float *seedHiFrom, float *seedHiTo, float *seedLoFrom, float *seedLoTo)
	{ // AddSuperarc()													//	adds a superarc to the contour tree
	//	find the top and bottom supernodes	
//	printf("Adding ");
	long theTopNode = GetSupernode(hiEnd);									//	retrieve the supernode at the top end
//	printf(" to ");
	long theBottomNode = GetSupernode(loEnd);								//	retrieve the supernode at the bottom end
//	printf("\n");
	
	return AddSuperarc(theTopNode, theBottomNode, seedHiFrom, seedHiTo, seedLoFrom, seedLoTo);
	} // end of AddSuperarc()

long HeightField::AddSuperarc(long topID, long bottomID, float *seedHiFrom, float *seedHiTo, float *seedLoFrom, float *seedLoTo)
	{ // AddSuperarc()													//	adds a superarc to the contour tree
	//	create the new superarc
	long newSuperarc = nextSuperarc++;
	superarcs[newSuperarc].Initialize(topID, bottomID);						//	initialize the new superarc
	superarcs[newSuperarc].SetHighSeed(seedHiFrom, seedHiTo);					//	and set the seeds
	superarcs[newSuperarc].SetLowSeed(seedLoFrom, seedLoTo);					//	and set the seeds
	
	//	now add the arcs to the nodes at each end
	AddDownArc(topID, newSuperarc);										//	insert at top end
	AddUpArc(bottomID, newSuperarc);										//	and at bottom end

	//	and set the valid flag
	AddToValid(newSuperarc);

	//	before returning
	return newSuperarc;
	} // end of AddSuperarc()

long HeightField::GetSupernode(float *theVertex)								//	get supernode for a vertex, creating it if necessary
	{ // GetSupernode()
	long x, y, z;
	height.ComputeIndex(theVertex, x, y, z);								//	compute its indices
//	printf("(%1d,%1d,%1d)/%8.5f", x, y, z, *theVertex);
	long theNode = nodeLookup(x, y, z);									//	lookup the node
	if (theNode != NO_SUPERNODE) return theNode;								//	return in the easy case

	//	if there isn't one already, make one
	supernodes[nextSupernode].Initialize(theVertex);							//	initialize the next one
	nodeLookup(x, y, z) = nextSupernode;									//	set it in the lookup table
	AddToValidNodes(nextSupernode);
	return nextSupernode++;												//	increment & return the ID	
	} // GetSupernode()
	
void HeightField::AddDownArc(long theNode, long theArc)						//	add a down arc at a given node
	{ // AddDownArc()
	long topOfList = supernodes[theNode].downList;							//	grab the top of the list
	if (topOfList == NO_SUPERARC)											//	if there are no down arcs
		{ // first down arc
		superarcs[theArc].nextDown = superarcs[theArc].lastDown = theArc;		//	make it point to itself
		supernodes[theNode].downList = theArc;								//	and set the list up
		} // first down arc
	else
		{ // subsequent down arc
		long oldLast = superarcs[topOfList].lastDown;						//	grab the old "last" arc
		superarcs[theArc].lastDown = oldLast;								//	point this one at it
		superarcs[oldLast].nextDown = theArc;								//	and vice versa
		superarcs[topOfList].lastDown = theArc;								//	do the same for the top of the list
		superarcs[theArc].nextDown = topOfList;
		} // subsequent down arc
	supernodes[theNode].downDegree++;										//	increment the down-degree
	} // AddDownArc()
	
void HeightField::AddUpArc(long theNode, long theArc)							//	add an up arc at a given node
	{ // AddUpArc()
	long topOfList = supernodes[theNode].upList;								//	grab the top of the list
	if (topOfList == NO_SUPERARC)											//	if there are no up arcs
		{ // first up arc
		superarcs[theArc].nextUp = superarcs[theArc].lastUp = theArc;			//	make it point to itself
		supernodes[theNode].upList = theArc;								//	and set the list up
		} // first up arc
	else
		{ // subsequent up arc
		long oldLast = superarcs[topOfList].lastUp;							//	grab the old "last" arc
		superarcs[theArc].lastUp = oldLast;								//	point this one at it
		superarcs[oldLast].nextUp = theArc;								//	and vice versa
		superarcs[topOfList].lastUp = theArc;								//	do the same for the top of the list
		superarcs[theArc].nextUp = topOfList;
		} // subsequent up arc
	supernodes[theNode].upDegree++;										//	increment the up-degree
	} // AddUpArc()
	
//	routine to find seeds for edges that do not otherwise have them	
float *HeightField::FindDescendingNeighbour(float *theVertex)
	{ // FindDescendingNeighbour()
	long x, y, z;
	height.ComputeIndex(theVertex, x, y, z);								//	retrieve the indices
	QueueSplitNeighbours(x, y, z);											//	queue up its neighbours in join order
	for (int i = 0; i < nNeighbours; i++)									//	walk along queue
		{ // for i
		float *neighbour = &(height(neighbourQueue[i][0], neighbourQueue[i][1], neighbourQueue[i][2]));
		if (compareHeight(neighbour, theVertex) < 0)							//	if the neighbour is smaller than the vertex
			return neighbour;											//	it will make a valid seed
		} // for i
	printf("Major error at %s:%d: couldn't find descending neighbour.\n", __FILE__, __LINE__);
	return NULL;
	} // FindDescendingNeighbour()	

	//	loads a file from ASCII text
void HeightField::LoadFromASCII(char *fileName)
	{ // LoadFromASCII()
	ifstream fin(fileName);
	if (!fin.is_open()) { printf("Unable to open file %s\n", fileName); throw 0; }
	sampleSize = 0; // set to something predictable

	long i, j, k;															//	loop indices for general use

//	A.	copy the parameters into the object
	fin >> xDim >> yDim >> zDim;											//	read in the dimensions
	xDim += 2;
//	B.	set nVertices
	nVertices = xDim * yDim * zDim;
//      printf("vertices: %d (%d %d %d)\n",nVertices,xDim, yDim, zDim);

//	C.	allocate leafQueue to a default size, and set leafQSize
	leafQueue = NULL;														//	allocate space on leaf queue
	leafQSize = 0;															//	set logical size to zero
				
//	D.	allocate height, heightSort, and visitFlags
	height.Construct(xDim, yDim, zDim);										//	call function to initialize the height array
	heightSort = (float **)malloc(sizeof(float *) * nVertices);					//	allocate the array
	visitFlags.Construct(xDim, yDim, zDim);										//	call template function to set up array
			
//	E.	read in the data, setting pointers in heightSort, and finding maxHeight and minHeight in the process
	float **heightWalk = heightSort;											//	used to walk through heightSort: start at front of heightSort
	maxHeight = -HUGE_VAL; minHeight = HUGE_VAL;
	//minHeight = maxHeight = height(0, 0, 0);									//	initialize min, max heights to first element in array
	for (i = 0; i < xDim; i++)												//	walk through each dimension
		{ // E i
// 		printf("."); fflush(stdout);
		for (j = 0; j < yDim; j++)
			for (k = 0; k < zDim; k++)
				{ // E ijk
				float nextHeight;											//	temporary variable for reading in from array
				if (i == 0 || i == 2) nextHeight = 0;
				else
				fin >> nextHeight;											//	read it in
				height(i, j, k) = nextHeight;									//	store it in the array
				sampleSum += nextHeight;						//	add to the running sum
				*heightWalk = &(height(i, j, k));								//	copy a pointer into sorting array
				if (nextHeight > maxHeight) maxHeight = nextHeight;				//	update maxHeight
				if (nextHeight < minHeight) minHeight = nextHeight;				//	update minHeight
				heightWalk++;												//	step to next pointer in sorting array
				} // E ijk: end of loop through i, j, k
		} // E i	

	plus_infinity = HUGE_VAL;												//	set plus infinity in case we ever compare against it
	minus_infinity = -HUGE_VAL;												//	and minus infinity
	joinRoot = splitRoot = NULL;												//	set these to something predictable
	
	//	set nextAvailableComponent equal to componentBlockSize to force allocation on first call
// 	componentBlockSize = 1024;
// 	nextAvailableComponent = 1024;

#ifdef DEBUG_CONSTRUCTOR
//	PrintField();
#endif		
		
	time(&lastTime);
	timingBuffer += sprintf(timingBuffer, "Input size (n): %1ld x %1ld x %1ld = %1ld\n", xDim, yDim, zDim, nVertices);
// 	timingBuffer += sprintf(timingBuffer, "Reading data complete at: %ld:%ld\n", (lastTime.tv_sec) % 1000, lastTime.tv_usec);
	//timingBuffer += sprintf(timingBuffer, "Reading data took %8.5f seconds\n", (float)(lastTime.tv_sec - startTime.tv_sec) + 1E-6 * (lastTime.tv_usec - startTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Reading data took %8.5f seconds\n", difftime(lastTime,startTime));
// 	timingBuffer += sprintf(timingBuffer, "Sorting data.\n");
	flushTimingBuffer();
	
	qsort(heightSort, nVertices, sizeof(float *), compareHeightVoid);

#ifdef DEBUG_CONSTRUCTOR
	PrintSortOrder();
#endif		

	time(&thisTime);
// 	timingBuffer += sprintf(timingBuffer, "Sorting data complete at: %ld:%ld\n", thisTime.tv_sec % 1000, thisTime.tv_usec);
	//timingBuffer += sprintf(timingBuffer, "Sorting data took %8.5f seconds\n", (float)(thisTime.tv_sec - lastTime.tv_sec) + 1E-6 * (thisTime.tv_usec - lastTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Sorting data took %8.5f seconds\n", difftime(thisTime, lastTime));
	flushTimingBuffer();
	
	} // LoadFromASCII()	
	
//	LoadFromRaw
void HeightField::LoadFromRaw(FILE *inFile)
	{ // LoadFromRaw()
	long index;	//	loop index
	if (sampleSize < 1) { printf("Sample sizes less than 1 don't make sense.\n"); throw 0;}
	if (sampleSize > 2) { printf("Sample sizes greater than 2 not yet implemented.\n"); throw 0;}

//	B.	set nVertices
	nVertices = xDim * yDim * zDim;

//	C.	allocate leafQueue to a default size, and set leafQSize
	leafQueue = NULL;														//	allocate space on leaf queue
	leafQSize = 0;															//	set logical size to zero
				
//	D.	allocate height, heightSort, and visitFlags
	heightSort = (float **)malloc(sizeof(float *) * nVertices);					//	allocate the array
	visitFlags.Construct(xDim, yDim, zDim);										//	call template function to set up array
			
//	E.	read in the data, setting pointers in heightSort, and finding maxHeight and minHeight in the process
	maxHeight = -HUGE_VAL; minHeight = HUGE_VAL;
	//	allocate memory 
	float *floatData = (float *) malloc(sizeof(float) * nVertices);				//	allocate the memory for the float representation
	unsigned char *rawData = (unsigned char *) malloc(sizeof (unsigned char) * sampleSize * nVertices);
	
	long nSamplesRead = fread(rawData, sizeof(unsigned char), nVertices, inFile);		//	read in the block of data
	if (nSamplesRead != nVertices) { printf("Binary read of input file failed.\n"); throw 0; }	

	time(&lastTime);
	timingBuffer += sprintf(timingBuffer, "Input size (n): %1ld x %1ld x %1ld = %1ld\n", xDim, yDim, zDim, nVertices);
	//timingBuffer += sprintf(timingBuffer, "Reading data took %8.5f seconds\n", (float)(lastTime.tv_sec - startTime.tv_sec) + 1E-6 * (lastTime.tv_usec - startTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Reading data took %8.5f seconds\n", (float)difftime(lastTime, startTime));
	flushTimingBuffer();
	
	//	set up radix sort bins
	long nBins = (sampleSize == 1) ? 256 : 65536L;
	
	long *firstInBin = new long[nBins];
	for (int i = 0; i < nBins;i++) firstInBin[i] = -1;								//	initialize to "NONE"
	long *nextInBin = (long *) malloc(nVertices * sizeof(long));					//	and initialize the "next" array
	for (int i = 0; i < nVertices; i++) nextInBin[i] = -1;
		
	if (sampleSize == 1)
		for (index = 0; index < nVertices; index++)									//	walk through the data
			{ // walk through data
			nextInBin[index] = firstInBin[rawData[index]];							//	push into linked list at front of sorting bin
			firstInBin[rawData[index]] = index;									//	and reset the front of the bin
			floatData[index] = (float) rawData[index];								//	copy into the float array
			sampleSum += rawData[index];						//	add to the running sum
			} // walk through data
	else
		{ // 2-byte data
		unsigned short *rawShort = (unsigned short * ) rawData;
		for (index = 0; index < nVertices; index++)									//	walk through the data
			{ // walk through data
			unsigned short newShort = rawShort[index];
#ifdef ENDIAN_SWAP_SHORTS
			newShort = (newShort >> 8) | ((newShort & 0x00FF) << 8);
#endif
			nextInBin[index] = firstInBin[newShort];							//	push into linked list at front of sorting bin
			firstInBin[newShort] = index;									//	and reset the front of the bin
			floatData[index] = (float) newShort;								//	copy into the float array
			sampleSum += newShort;						//	add to the running sum
			} // walk through data
		} // 2-byte data		
	//	we have now loaded up the float array, and need to setup heightSort properly
	//	our bin sort has now pushed the elements into bins IN REVERSE ORDER, so we have to
	//	flip again in reading them out
	int bin = nBins;															//	which bin we are reading from
	long nextSample = -1;													//	which sample comes next
	for (index = nVertices-1; index >= 0; index--)
		{ // filling in heightSort
		while (nextSample == -1)												//	this bin is empty
			nextSample = firstInBin[--bin];									//	grab the first item in the next bin
		heightSort[index] = floatData+nextSample;								//	generate & store the pointer
		nextSample = nextInBin[nextSample];									//	grab the next item
		} // filling in heightSort
	delete []firstInBin;
	height.Construct(xDim, yDim, zDim, floatData);								//	call function to initialize the height array
	plus_infinity = HUGE_VAL;												//	set plus infinity in case we ever compare against it
	minus_infinity = -HUGE_VAL;												//	and minus infinity
	joinRoot = splitRoot = NULL;												//	set these to something predictable
	maxHeight = *(heightSort[nVertices-1]); minHeight = *(heightSort[0]);				//	set min. and max. height
//	PrintSortOrder();
	
	free (nextInBin); free(rawData);											//	release the working memory	
	time(&thisTime);
	//timingBuffer += sprintf(timingBuffer, "Sorting data took %8.5f seconds\n", (float)(thisTime.tv_sec - lastTime.tv_sec) + 1E-6 * (thisTime.tv_usec - lastTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Sorting data took %8.5f seconds\n", difftime(thisTime, lastTime));
	} // LoadFromRaw()


//	LoadFromNifti
/*void HeightField::LoadFromNifti(char *fileName)
{ // LoadFromNifti()
	RicVolume vol(fileName);
	if (vol.nvox == 0) { printf("Unable to open file %s\n", fileName); throw 0; }
	sampleSize = 0; // set to something predictable

	long i, j, k;															//	loop indices for general use

	//	A.	copy the parameters into the object
	zDim = vol.get_numz();
	yDim = vol.get_numy();
	xDim = vol.get_numx();											//	read in the dimensions

	//	B.	set nVertices
	nVertices = xDim * yDim * zDim;

	//	C.	allocate leafQueue to a default size, and set leafQSize
	leafQueue = NULL;														//	allocate space on leaf queue
	leafQSize = 0;															//	set logical size to zero

	//	D.	allocate height, heightSort, and visitFlags
	height.Construct(xDim, yDim, zDim);										//	call function to initialize the height array
	heightSort = (float **)malloc(sizeof(float *) * nVertices);					//	allocate the array
	visitFlags.Construct(xDim, yDim, zDim);										//	call template function to set up array

	//	E.	read in the data, setting pointers in heightSort, and finding maxHeight and minHeight in the process
	float **heightWalk = heightSort;											//	used to walk through heightSort: start at front of heightSort
	maxHeight = -HUGE_VAL; minHeight = HUGE_VAL;
	//minHeight = maxHeight = height(0, 0, 0);									//	initialize min, max heights to first element in array
	for (i = 0; i < xDim; i++)												//	walk through each dimension
	{ // E i
		// 		printf("."); fflush(stdout);
		for (j = 0; j < yDim; j++)
			for (k = 0; k < zDim; k++)
			{ // E ijk
				float nextHeight = vol.vox[i][j][k];								//	temporary variable for reading in from array
				height(i, j, k) = nextHeight;									//	store it in the array
				sampleSum += nextHeight;						//	add to the running sum
				*heightWalk = &(height(i, j, k));								//	copy a pointer into sorting array
				if (nextHeight > maxHeight) maxHeight = nextHeight;				//	update maxHeight
				if (nextHeight < minHeight) minHeight = nextHeight;				//	update minHeight
				heightWalk++;												//	step to next pointer in sorting array
			} // E ijk: end of loop through i, j, k
	} // E i	

	plus_infinity = HUGE_VAL;												//	set plus infinity in case we ever compare against it
	minus_infinity = -HUGE_VAL;												//	and minus infinity
	joinRoot = splitRoot = NULL;												//	set these to something predictable

	//	set nextAvailableComponent equal to componentBlockSize to force allocation on first call
	// 	componentBlockSize = 1024;
	// 	nextAvailableComponent = 1024;

#ifdef DEBUG_CONSTRUCTOR
	//	PrintField();
#endif		

	time(&lastTime);
	timingBuffer += sprintf(timingBuffer, "Input size (n): %1ld x %1ld x %1ld = %1ld\n", xDim, yDim, zDim, nVertices);
	// 	timingBuffer += sprintf(timingBuffer, "Reading data complete at: %ld:%ld\n", (lastTime.tv_sec) % 1000, lastTime.tv_usec);
	//timingBuffer += sprintf(timingBuffer, "Reading data took %8.5f seconds\n", (float)(lastTime.tv_sec - startTime.tv_sec) + 1E-6 * (lastTime.tv_usec - startTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Reading data took %8.5f seconds\n", difftime(lastTime, startTime));
	// 	timingBuffer += sprintf(timingBuffer, "Sorting data.\n");
	flushTimingBuffer();

	qsort(heightSort, nVertices, sizeof(float *), compareHeightVoid);

#ifdef DEBUG_CONSTRUCTOR
	PrintSortOrder();
#endif		

	time(&thisTime);
	// 	timingBuffer += sprintf(timingBuffer, "Sorting data complete at: %ld:%ld\n", thisTime.tv_sec % 1000, thisTime.tv_usec);
	//timingBuffer += sprintf(timingBuffer, "Sorting data took %8.5f seconds\n", (float)(thisTime.tv_sec - lastTime.tv_sec) + 1E-6 * (thisTime.tv_usec - lastTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Sorting data took %8.5f seconds\n", difftime(thisTime, lastTime));
	flushTimingBuffer();
} // LoadFromNifti()
*/
void HeightField::SetInitialColours()											//	sets the initial coloursf 
	{ // SetInitialColours()
	long theArc = collapseRecord[1];											//	find the initial arc
	long topNode = superarcs[theArc].topID, bottomNode = superarcs[theArc].bottomID;	//	and it's bounding nodes

	//	use colour 0 for the base edge
	superarcs[theArc].colour = supernodes[topNode].colour = supernodes[bottomNode].colour = 0;
	
	//	the rest are taken care of during UnCollapse()
	} // SetInitialColours()

	
