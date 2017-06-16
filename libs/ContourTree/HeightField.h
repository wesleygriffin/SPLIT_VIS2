//	IsoSurface II
//	Hamish Carr, 1999

//	HeightField.h:		class representing the height field
//	Palatino-12; 5 n-spaces/tab

#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H 1

#define HEIGHTFIELD_DEBUG_PRINT 1
#include <string.h>
#include <fstream>														//	include file streams
#include "Component.h"													//	include the Component class for the Join Tree & Split Tree
#include "Superarc.h"													//	include the Superarc class for the contour tree
#include "Supernode.h"													//	include the Supernode class for the contour tree
#include "Array3D.h"													//	and the template class for 3-D arrays
#include <GL/freeglut.h>
using namespace std;

//#include <sys/time.h>
#include <ctime>

#define TIMING_BUFFER_SIZE 2048576L
//	declare a large memory buffer into which we can dump
extern char *timingBuffer, *timingStart;
//	some timing structures
extern time_t startTime, lastTime, thisTime, thatTime;
//	and a function to call at exit to print the buffer
void printTimingBuffer();
//	this routine flushes the buffer to standard out
void flushTimingBuffer();

#define PRIORITY_HEIGHT 1
#define PRIORITY_VOLUME 2
#define PRIORITY_HYPERVOLUME 3
#define PRIORITY_APPROX_HYPERVOLUME 4

//	these are for use in computing actual path lengths, &c.
extern int pathLength, triangleCount;

//	we start by defining a function for use when comparing heights:  this is passed to qsort(), and used any time we need to compare heights
//	thus, it includes the symbolic perturbation scheme

//	compareHeightVoid():	compares two heights in an array
int compareHeightVoid(const void *e1, const void *e2);							//	comparison function for passing to qsort()
//	compareHeight():		compares two heights in an array
int compareHeight(const float *d1, const float *d2);							//	comparison function for all other purposes

//	this class is largely intended to encapsulate the height field, and the algorithms to compute the contour tree, and render the result
const long maxNeighbours = 24;											//	actually, it's less than this, but this covers all versions so far

const long noContourSelected = -1;											//	constant for empty selection

const long nDotLayout = 256;												//	no. of vertices to use for dot layout

#define N_SURFACE_COLOURS 9
extern GLfloat surface_colour[N_SURFACE_COLOURS][4];							//	colours for individual contours								
extern GLfloat select_colour[4];											//	colour for selected component		
extern GLfloat basic_colour[4];											//	colour for most components		
	
class HeightField														//	our class for a height field
	{
	public:
	bool differentColouredContours;										//	flag for whether all the contours should be same colour

	private:
	//	the actual data array exists for the life of the height field
	long xDim, yDim, zDim;												//	size of the data array
	long nVertices;														//	total # of vertices
	Array3D<float> height;												//	3-D array holding the height values
	float maxHeight, minHeight;										//	maximum & minimum height value

	//	these flags are principally used for rendering, BUT are also used during the contour-tree algorithm
	//	to mark vertices that have already been assigned to individual superarcs
	Array3D<unsigned long> visitFlags;										//	1-D array for flags used in contour-following rendering

	//	the following are working storage, shared between several routines, and can be disposed of when the contour tree has been found:
	float **heightSort;													//	1-D array for sorting the heights
	Array3D<Component *> joinComponent;								//	3-D array of pointers into Union-Find (also stores join tree, implicitly)
	Array3D<Component *> splitComponent;								//	ditto for the split tree
	Array3D<long> nodeLookup;											//	temporary array to set supernodes with
	Array3D<float *> joinArcs, splitArcs;									//	used to keep track of arcs in join (split) tree: low level

	long neighbourQueue[maxNeighbours][3];								//	queue for finding neighbours
	long nNeighbours;													//	logical size of the queue
	float **leafQueue;													//	leaf queue for merge step
	long leafQSize;														//	current size of leaf queue
	long vertex;														//	index used by CreateJoinTree() & CreateSplitTree()
	long *supernodeQueue;												//	queue used by CollapseEpsilonEdges()
	bool *wasChecked;													//	flag array for CollapseEpsilonEdges()
	long supernodeQueueSize;											//	logical size of queue

	float minus_infinity, plus_infinity;										//	these are to simplify processing of join tree and split tree
	Component *joinRoot, *splitRoot;										//	pointers to bottom (top) components of join (split) tree
	
	//	storage for the contour tree once computed
	Superarc *superarcs;													//	array holding all the edges of the contour tree
	Supernode *supernodes;												//	and one holding all the nodes
	long nSuperarcs;													//	number of superarcs in the final tree
	long nSupernodes;													//	number of supernodes in the final tree	
	long nValidArcs, nActiveArcs, nSelectedArcs, nRestorableArcs;					//	number of arcs in each category
	long nValidNodes;													//	number of valid nodes
	long *valid, *active, *selected, *restorable, *validNodes;						//	arrays holding various classes of objects

	long nJoinSupernodes, nSplitSupernodes;									//	running count of nodes in join & split trees
	long nextSuperarc, savedNextSuperarc;									//	next available superarc in contour tree
	long nextSupernode;												//	next available supernode ID

	//	these variables allow us to do flexible isosurfaces
	public:
	long selectionRoot;													//	ID of component being dragged
	float currentSelectionValue;											//	current value of drag	
	private:
	float selectionRootValue;											//	value at which drag started
	long restorableSelectionRoot;											//	ID of component being dragged
	float restorableCurrentValue;										//	current value of drag	
	float restorableRootValue;											//	value at which drag started
	long restorableOrigin;												//	origin of "restorables": must be one of:
	static const long restorableSelection = 0;									//	selection, or
	static const long restorableActive = 1;									//	complement of selection

	//	and some dealing with tree collapse
	private:
	long *collapseRecord;												//	record of which vertices were collapsed
	long nNonEpsilonArcs;												//	the number of non-epsilon arcs
	public:
	long *collapseBounds;												//	here's the array to store them
	int collapsePriority;
	//	used by log collapse pane
	float logTreeSize, logPriorityBound;									//	logs of the current tree size & bound
	long *nodeColour, *arcColour;											//	colours assigned to arc, node respectively

	private:
	long *pathLengths;													//	length of path used to construct the contour
	long *nContourTriangles;												//	size of particular surface (# of triangles)
	float *extractionTime;												//	time to extract a particular surface
	long maxPathLength;												//	the longest path length found
	float avgPathLength;												//	average path length
	long totalTriangles;													//	total triangles displayed
	float totalExtractionTime;												//	sum of extraction times for surfaces
	float frameRate;													//	make frame rate available for display
	//struct timeval lastFrameTime, thisFrameTime;								//	for computing frame time & rate
	time_t lastFrameTime, thisFrameTime;								//	for computing frame time & rate
	
	int baseDisplayListID;												//	base ID for set of display lists

	//	these are members intended for use in measuring performance
	public:
	long recursionDepth, maxRecursionDepth;								//	for measuring depth of recursion
	long nCellsVisited, nTrianglesGenerated;									//	number of cells visited & triangles generated
	long nCellsIntersected;
	long nSimplicesToDraw, nSimplicesToUnFlag;								//	used only when debugging - how many we've drawn into	
	long constructionState;												//	the state of construction
	long nSteps;														//	number of steps to perform before returning (for debug)
	int sampleSize;													//	size (in bytes) of individual samples
	
//	this one is a hack for computing correct Riemann sums
	float sampleSum;


//	constructor:		constructs the object
	HeightField(int argc, char **argv);										//	constructor
//	destructor:			destroys the object, and deallocates dynamically allocated storage (including height array)
	~HeightField();														//	destructor

	//	loads a file from ASCII text
	void LoadFromASCII(char *fileName);
	void LoadFromRaw(FILE *inFile);
//	void LoadFromNifti(char *fileName);

//	CreateJoinTree()
	void CreateJoinTree();												//	does the down sweep to create the join tree
																	//	may be called repeatedly, for partial processing
																	//	returns TRUE iff the join tree has now been computed
//	CreateSplitTree()
	void CreateSplitTree();												//	does the up sweep to create the split tree
																	//	may be called repeatedly, for partial processing
																	//	returns TRUE iff the split tree has now been computed

//	AugmentTrees()
	void AugmentTrees();												//	augments join and split trees with nodes of other

//	CombineTrees()
	void CombineTrees();												//	combines join and split trees to produce contour tree
																	//	returns TRUE only when the contour tree has been computed

//	CleanUpConstruction()
	void CleanUpConstruction();											//	cleans up dynamically allocated memory

private:		//	used internally
//	QueueNeighbour()
	void QueueNeighbour(long x, long y, long z);								//	queues up the given neighbour, if it is in the field

//	QueueJoinNeighbours()
	void QueueJoinNeighbours(long x, long y, long z);							//	queues up all neighbours in the join sweep

//	QueueJoinNeighbours()
	void QueueSplitNeighbours(long x, long y, long z);							//	queues up all neighbours in the split sweep

	long AddSuperarc(float *hiEnd, float *loEnd, float *seedHiFrom, float *seedHiTo, float *seedLoFrom, float *seedLoTo);
																	//	adds a superarc to the contour tree
	long AddSuperarc(long topID, long bottomID, float *seedHiFrom, float *seedHiTo, float *seedLoFrom, float *seedLoTo);
																	//	adds a superarc to the contour tree
	long GetSupernode(float *theVertex);									//	get supernode for a vertex, creating it if necessary
	void AddUpArc(long theNode, long theArc);								//	add an up arc at a given node
	void AddDownArc(long theNode, long theArc);							//	add an down arc at a given node
	//	routine to find seeds for edges that do not otherwise have them	
	float *FindDescendingNeighbour(float *theVertex);						//	find a descending neighbour. Any descending neighbour
	bool RemoveEdgeIfEpsilonHeight(long theArc);							//	checks arc, removes it if necessary, returns true if it did

public:	//	public drawing routines
	//	drawing routines
	void MarchingCubes(float ht);										//	draws level set using marching cubes
	void MarchingSimplices(float ht);										//	draws level set using marching simplices

	//	These routines implement the flexible isosurface interface
	void SetIsosurface(float ht);										//	sets the selected components to an isosurface
	void SetLocal(float ht);											//	sets the selected components to a local set

	void SetComponentHeight(int component, float height);						//	sets the seed value for a specific component
	void SelectComponent(int newSelectionRoot);								//	initializes the selection arrays, using existing value
	void SelectComponent(int newSelectionRoot, float selectionValue);			//	initializes the selection arrays
	void UpdateSelection(float height);									//	updates the selection arrays during a drag
	void ProportionalUpdateSelection(float height);							//	version of update selection constrained to edge
	void CommitSelection();												//	commits the selection

	void ComputeStatistics();											//	computes statistics on average path length, &c.

	void Unselect();													//	clears selection, transferring to active status

	void DeleteSelection();												//	deactivates the selected components
	void DeleteComplement();												//	deactivates the complement of the selection
	void RestoreDeleted();												//	restores the last saved set									
	void DeactivateAll();												//	deactivates all components

	bool ArcSelected(int arcIndex);										//	accessor for selected flag
	int PickNode(float x, float y);										//	picks a node: returns -1 if it fails
	void UpdateNodeXPosn(int node, float x);								//	updates a node's x position
	int PickArc(float x, float y);										//	picks an arc: returns -1 if it fails
	long GetSuperarcColour(long theArc);									//	accessor to retrieve arc colour
	void SetSuperarcColour(long theArc, long theColour);						//	accessor to set arc colour


	void FlexibleContours(bool useDisplayLists = false, bool pickColours = false);	//	renders flexible contours, using contour-following
	void RenderContour(long arc, bool useDisplayLists, bool pickColours);			//	renders an individual contour

	void DrawEdges();													//	draws the edges of the voxels
	void DrawContourTree();												//	draws the contour tree
	void DrawPlanarContourTree(bool useColouredTree);	//	draws a planar version of the contour tree
	void PlotLogCollapse();												//	plots the log collapse curve
	void DrawJoinTree();												//	draws the join tree
	void DrawSplitTree();												//	draws the split tree
	void FollowHierarchicalPathSeed(int sArc, float ht);						//	follows hier. path seed to generate contour
	void SetNodeXPositions();											//	sets the x-positions for the nodes
	void UseDotToSetPositions(int nArcs);									//	sets the x-positions for nodes by calling dot
	void SetInitialColours();											//	sets the initial colours
	
	//	routines needed for collapsing
	void ResetCollapsePriority(long whichCollapsePriority);					//	resets the collapse priority & recollapses
	long CollapseVertex(long whichSupernode);								//	collapses a vertex: returns the new superarcs ID
	void CollapseToSuperarc(long whichSuperarc);								//	collapses to a known superarc						
	void CollapseEpsilonEdges();											//	collapses the epsilon-height edges
	void CollapseLeafPruning(int targetSize, long prunedVolumeBound);			//	leaf-prunes until one of the bounds is reached
	void RemoveArc(long theArc);											//	routine to remove an arc from the tree
	void SetNext(int atNode, Superarc *target, Superarc *newNext);				//	resets the "next" pointer at a particular node
	void SetLast(int atNode, Superarc *target, Superarc *newLast);				//	resets the "last" pointer at a particular node 

	void UpdateFromLogPriorityBound();										//	updates the size of the tree
	void UpdateFromLogTreeSize();											//	ditto, but from tree size
	long FindLogBound(float logBound);										//	given a fixed tree size, finds the corresponding bound
	long FindLogTreeSize(float logSize);									//	and vice versa
    void UpdateToFixedSize(long targetSize);									//	ditto, but from tree size
	void SingleCollapse();												//	collapses a single node after constructing collapse tree
	void SingleUnCollapse();												//	similarly, uncollapses a single node
	float InitialLogPriorityBound();										//	utility routine for initializing slider

// *************	
// ** WARNING **
// *************	
//
// This routine assumes that the fromNode's arcs have already had the hiID/loID and hiEnd/loEnd correctly reset to the onto node
//
	void TransferArcLists(int ontoNode, int fromNode);						//	transfer's arcs from one node to another

	//	housekeeping routines for flexible isosurfaces
	void ClearAllArrays();												//	clears active[], selected[], restorable[]
	void ClearActive();													//	clears active list
	void ClearSelection();												//	clears selected list
	void ClearRestorable();												//	clears restorable list
	void ClearSuppressed();												//	resets all suppressed arcs to active
	void ClearBackQueue();												//	removes all vertices from the back queue
	void AddToActive(long arc, float seedValue);							//	adds an arc to the active list		
	void RemoveFromActive(long arc);										//	removes arc from the active list
	void AddToValid(long arc);											//	adds an arc to the valid list		
	void RemoveFromValid(long arc);										//	removes arc from the valid list
	void AddToSelected(long arc);											//	adds an arc to the selected list		
	void AddToRestorable(long arc);										//	adds an arc to the restorable list		
	void RemoveFromRestorable(long arc);									//	removes arc from the restorable list
	void AddToValidNodes(long node);										//	adds an node to the valid list		
	void RemoveFromValidNodes(long node);									//	removes node from the valid list

	//	routine to simplify changing metric	
	long Priority(long arc, bool upSweep);									//.	computes priority for a given arc

	//	base-level drawing routines
	void FollowContour(float ht, Superarc &theArc);							//	given a superarc spanning ht, draws the corresponding contour

	//	routines called by ContourFollowing()
	void FollowSurface(float ht, float *p1, float *p2);						//	routine to follow surface from seed edge
	void IntersectSurface(float ht, long x, long y, long z, int theEntryFaceEdge);		//	intersects surface starting at specified cell & edge
	void Visit(long xc, long yc, long zc, long cubeContour);						//	sets the flag for a particular contour in a cell
	void UnVisit(long xc, long yc, long zc, long cubeContour);					//	resets the flag for a contour
	bool Visited(long xc, long yc, long zc, long cubeContour);					//	tests whether flag set for contour
	void UnFlagSurface(float ht, long xc, long yc, long zc, int theEntryFaceEdge);	//	clean out the flags
	void RenderTriangle(float ht, long x, long y, long z, float *cubeVert, int edge1, int edge2, int edge3);
																	//	renders the triangle specified by three edges
	void CentralDifferenceNormal(long x, long y, long z, GLfloat *normal);		//	uses c.d. to estimate normal at a vertex of the mesh
	void InterpolateVertex(long x, long y, long z, int edge, float *cubeVert, float ht);
																	//	interpolates a vertex (with normal) & sends to OpenGL
	//	accessor functions
	float MaxHeight() { return maxHeight; };								//	accessor function
	float MinHeight() { return minHeight; };
	long Size();														//	returns the largest dimension

	long XDim() { return xDim; }											//	accessors for individual dimensions
	long YDim() { return yDim; }
	long ZDim() { return zDim; }

	//	debug routines

        void PrintValidValues(char *fname);

	void PrintPointerIndices(float *vertex);									//	prints out the pointer indices
	void PrintField();													//	prints out the contents of the height field
	void PrintParameters();												//	prints out the dimensions, offset, &c.
	void PrintSortOrder();												//	prints out the heights in sorted order
	void PrintVisitFlags();												//	prints out the "visited" flags
	void PrintJoinComponents();											//	prints out the Union-Find info for the join sweep
	void PrintJoinTree();													//	prints out the join tree (i.e. join components, with duplicates suppressed)
	void PrintSplitComponents();											//	prints out the Union-Find info for the split sweep
	void PrintSplitTree();												//	prints out the split tree (i.e. split components, with duplicates suppressed)
	void PrintContourComponents();										//	prints out the contour components computed so far	
	void PrintContourTree();												//	prints out the contour tree, once it is fully computed
	void PrintNeighbourQueue();											//	prints off the queue that identifies the neighbours of a vertex
	void PrintLeafQueue();												//	prints off the leaf queue
//	this routine generates a dot file for generating graph layouts
//	filename is: dot/basefilename_serialNum.dot
	void ContourTreeToDotFile(char *basefilename, int serialNum, bool postscriptOutput = true, bool forceYPosition = false, long highlightEdge = -1);
																	//	prints out the contour tree, once it is fully computed
	void JoinArcsToDotFile(char *basefilename, int serialNum);					//	prints the contents of the joinArcs array
	void SplitArcsToDotFile(char *basefilename, int serialNum);					//	prints the contents of the splitArcs array
	void PrintCollapseRecord();											//	prints the array storing collapse details

	void CheckJoinTree();												//	checks join tree for consistency
	void CheckVisitFlags();												//	checks the "visited" flags
	void CheckSplitTree();												//	checks split tree for consistency
	void CheckContourTree();											//	checks contour tree for consistency
	void CheckNodeCounts();											//	checks consistency of node counts on arcs

	void PrintConstructionState();											//	print the current construction state

	void DoDotLayout();													//	perform dot-based layout at user request
	void SaveTreeLayout();												//	save the contour tree's current layout
	void LoadTreeLayout();												//	load the contour tree's current layout

	}; // end of class HeightField

#endif
