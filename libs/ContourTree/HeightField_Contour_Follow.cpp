//	IsoSurface II
//	Hamish Carr, 2000
//
//	As modified 2002 for flexible isosurfaces
//	As further modified 2002 for marching cube contour following

//	HeightField_Contour_Following.cpp:	routine to display height field using contour following
//	Palatino-12; 5 n-spaces/tab

#include "HeightField.h"
#include <GL/freeglut.h>															//	and GLUT
#include "FollowCubeTables.h"

#include <queue>
#include <math.h>

//	a little class defining a single surface in a cell
class CellSurface
	{	// class CellSurface
	public:
	long x, y, z, entryFaceEdge;
	CellSurface(long X, long Y, long Z, long EntryFaceEdge) { x = X; y = Y; z = Z; entryFaceEdge = EntryFaceEdge; }
	}; 	// class CellSurface

GLfloat surface_colour[N_SURFACE_COLOURS][4] = {	
								1.0,		0.0,		0.0,		1.0,							//	red
								0.0,		1.0,		0.0,		1.0,							//	green
								0.0,		0.0,		1.0,		1.0,							//	blue

								1.0,		1.0,		0.0,		1.0,							//	yellow
								1.0,		0.0,		1.0,		1.0,							//	magenta
								0.0,		1.0,		1.0,		1.0,							//	cyan

								1.0,		0.5,		0.0,		1.0,							//	orange
								0.0,		0.5,		1.0,		1.0,							//	mid-blue

								0.5,		0.0,		0.5,		1.0							//	purple
							 };

GLfloat select_colour[4] = 		{	1.0,		0.0,		0.0,		1.0};						//	colour for selected component		
GLfloat basic_colour[4] = 		{	0.5,		0.5,		0.5,		1.0};						//	colour for most components		
void InterpolatePoint(float x1, float y1, float z1, float h1, float x2, float y2, float z2, float h2, float ht, float* result);
																					//	linear interpolation along an edge

void ComputeNormal(float *p, float *q, float *r, float *n);										//	computes normal for triangle pqr, puts it in n

//	these are for use in computing actual path lengths, &c.
int pathLength, triangleCount;

void HeightField::FlexibleContours(bool useDisplayLists, bool pickColours)							//	renders flexible contours, using contour-following
	{ // FlexibleContours()
	int theArc;																		//	index of the superarc
	int nContours = 0;																	//	count of how many contours were generated

//        printf("ActiveAc %d\n", nActiveArcs);
	for (theArc = 0; theArc < nActiveArcs; theArc++)											//	walk along array of superarcs
		{ // for theArc
		if (superarcs[active[theArc]].CheckFlag(Superarc::isSuppressed)) continue;					//	ignore suppressed arcs
		if (superarcs[active[theArc]].CheckFlag(Superarc::isSelected)) continue;					//	ignore selected flags (SHOULD be redundant)
		RenderContour(active[theArc], useDisplayLists, pickColours);
		nContours++;
		} // for theArc
	for (theArc = 0; theArc < nSelectedArcs; theArc++)										//	walk along array of superarcs
		{ // for theArc
		RenderContour(selected[theArc], useDisplayLists, pickColours);
		nContours++;
		} // for theArc
	return;
	} // FlexibleContours()

void HeightField::RenderContour(long arc, bool useDisplayLists, bool pickColours)					//	renders an individual arc
	{ // RenderContour()

	GLubyte pick_colour[3] = { 0, 0, 0};													//	for use picking colours
//	GLfloat shell_colour[3] = { 0, 0, 0};													//	for use picking colours
	if (pickColours)																	//	i.e. we want to pick something
		{ // pickColours	
		pick_colour[0] = arc % 256;														//	chop up into bytes
		pick_colour[1] = (arc / 256) % 256;
		pick_colour[2] = ((arc / 256) / 256);
		glColor3ubv(pick_colour);														//	and set the colour
		} // pickColours
	else // i.e. not pick colours
		{ // not pick colours
		if (superarcs[arc].CheckFlag(Superarc::isSelected))
			if (differentColouredContours)
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, basic_colour);
			else
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, select_colour);
		else
			if (differentColouredContours)
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, surface_colour[superarcs[arc].colour]);
// 					{
// 					shell_colour[0] = (superarcs[arc].seedValue - minHeight) / (0.25 * (maxHeight - minHeight));
// 					while (shell_colour[0] > 1.0)
// 						shell_colour[0] -= 1.0;
// 					shell_colour[1] = 1.0f - shell_colour[0];
// 					shell_colour[2] = 0.0f;
// 					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, shell_colour);
// 					}		
			else
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, basic_colour);
		} // not pick colours
	if (useDisplayLists)
		{ // display lists turned off
		if (superarcs[arc].CheckFlag(Superarc::isDirty))
			{ // dirty arc needs re-extraction
			superarcs[arc].ClearFlag(Superarc::isDirty);
			glNewList(baseDisplayListID + arc, GL_COMPILE_AND_EXECUTE);							//	start creating its display list
			if (superarcs[arc].CheckFlag(Superarc::isSelected))
				FollowHierarchicalPathSeed(arc, currentSelectionValue);
			else
				FollowHierarchicalPathSeed(arc, superarcs[arc].seedValue);
			} // dirty arc needs re-extraction
		glEndList();																	//	end the list
		glCallList(baseDisplayListID+arc);													//	call the stored list for this contour
		} // display lists turned on
	else
		{ // display lists turned off
		if (superarcs[arc].CheckFlag(Superarc::isSelected))
		{
                 //       printf("current\n");
           		FollowHierarchicalPathSeed(arc, currentSelectionValue);
		}
		else
                {
                        //printf("seed\n");
			FollowHierarchicalPathSeed(arc, superarcs[arc].seedValue);
               }
		} // display lists turned off
	} // RenderContour()

void HeightField::FollowContour(float ht, Superarc &theArc)									//	given a superarc spanning ht, draws the corresponding contour
	{ // FollowContour()

        //printf("%f\n",ht);
	long x, y, z;																		//	indices of point 
	float *neighbour;																	//	neighbouring point
	float *thisEnd, *thatEnd;															//	ends of an edge that intersects contour
	long nSteps = 1;																	//	remember to count the starting step
//printf("end %f %f %f %f\n",*theArc.seedToLo, *theArc.seedFromLo,*supernodes[theArc.seedFromLo,theArc.bottomID].value, *supernodes[theArc.seedFromLo,theArc.topID].value);
// 	printf("Isovalue: %8.5f\n", ht);
	if (theArc.seedFromLo != NULL)														//	look for a seed at the bottom end
		{ // seedFromLo exists
		thisEnd = theArc.seedFromLo;														//	path starts at seedLoFrom
		thatEnd = theArc.seedToLo;														//	then proceeds through seedLoTo
		while (*thatEnd <= ht)															//	repeat until we straddle
			{ // while thatEnd
			nSteps++;
			thisEnd = thatEnd;															//	move up "this end"
			height.ComputeIndex(thisEnd, x, y, z);											//	retrieve the indices
			QueueJoinNeighbours(x, y, z);													//	queue up its neighbours (by join order)
			for (int i = 0; i < nNeighbours; i++)											//	walk along queue
				{ // for i
				neighbour = &(height(neighbourQueue[i][0], neighbourQueue[i][1], neighbourQueue[i][2]));
				if (neighbour == thatEnd) continue;										//	this *CAN* happen, when the seed is adjacent to the lo end
				if (compareHeight(neighbour, thatEnd) > 0)									//	if the neighbour is larger than the best so far
					thatEnd = neighbour;												//	grab it as the next in path
				} // for i
			} // while thatEnd
        //             printf("%f %f\n", *thisEnd, *thatEnd);
		} // seedFromLo exists
	else if (theArc.seedFromHi != NULL)													//	look for a seed at the top end
		{ // seedFromHi exists	
		thisEnd = theArc.seedFromHi;														//	path starts at seedFromHi
		thatEnd = theArc.seedToHi;														//	then proceeds through seedToHi
		while (*thatEnd >= ht)															//	repeat until we straddle
			{ // while thatEnd
			nSteps++;
			thisEnd = thatEnd;															//	move up "this end"
			height.ComputeIndex(thisEnd, x, y, z);											//	retrieve the indices
			QueueSplitNeighbours(x, y, z);												//	queue up its neighbours in split order
			for (int i = 0; i < nNeighbours; i++)											//	walk along queue
				{ // for i
				neighbour = &(height(neighbourQueue[i][0], neighbourQueue[i][1], neighbourQueue[i][2]));
				if (neighbour == thatEnd) continue;										//	this *CAN* happen, when the seed is adjacent to the lo end
				if (compareHeight(neighbour, thatEnd) < 0)									//	if the neighbour is smaller than the best so far
					thatEnd = neighbour;												//	grab it as the next in path
				} // for i
			} // while thatEnd
		} // seedFromHi exists
	else
		{ // no seed
		printf("Major error at %s: %d: no seed available\n", __FILE__, __LINE__);
		return;
		} // no seed
	// thisEnd, thatEnd should now straddle the height value
	//	copy the # of steps to the static variable for return to caller
	pathLength = nSteps;
	//	and reset the triangle count
	triangleCount = 0;
	FollowSurface(ht, thisEnd, thatEnd);													//	so follow the surface
	} // FollowContour()

void HeightField::FollowSurface(float ht, float *p1, float *p2)								//	routine to follow surface from seed edge
	{ // FollowSurface()
	long x1, y1, z1, x2, y2, z2;															//	coordinates of the two points on the edge
	long xm, ym, zm, xc, yc, zc;															//	min values & values for the cell, respectively
	int i1, i2;																		//	indices for the two vertices
	long whichCubeEdge, theSurface, whichFaceEdge;											//	used to figure out where to start
	long theCase = 0;																	//	the marching cubes case
	long whichVertex;																	//	index for vertices
	float cubeVert[8];																	//	local array holding vertices of cube

	//	first convert p1, p2 to (x1, y1, z1) & (x2, y2, z2)
	height.ComputeIndex(p1, x1, y1, z1); height.ComputeIndex(p2, x2, y2, z2);						//	compute the indices	
	//	find the common cell to which they both belong (make sure it's in bounds)
	xm = x1 < x2 ? x1 : x2;		ym = y1 < y2 ? y1 : y2;		zm = z1 < z2 ? z1 : z2;				//	take the minimum in each dimension
	xc = xm; 					yc = ym; 					zc = zm;							//	and work out which cell we are in
	if (xc == xDim - 1) xc--;	if (yc == yDim - 1) yc--;	if (zc == zDim - 1) zc--;			//	make sure we stay inside a legal cell					
	//	compute indices of vertices with respect to this cell
	i1 = ((x1 - xc) << 2) + ((y1 - yc) << 1) + (z1 - zc);		i2 = ((x2 - xc) << 2) + ((y2 - yc) << 1) + (z2 - zc);	
        //printf("%d %d %d\n", xm, ym,zm);																					//	compute indices
	//	first, go into the cell we have identified
	whichCubeEdge = vertex2edge[i1][i2];													//	find which edge we are on in the cell
	if (whichCubeEdge == -1) 															//	error-check for bad edge
		{ printf("Major problem: %ld to %ld is not a valid seed edge\n", i1, i2); return;}

	//	now figure out which case the cube uses
	if ((xc < 0) || (yc < 0) || (zc < 0)) return;											//	do nothing if any index negative
	if ((xc > xDim-2) || (yc > yDim-2) || (zc > zDim-2)) return;								//	ditto for last face worth of vertices

	cubeVert[0] = height(xc, yc, zc);
	cubeVert[1] = height(xc, yc, zc+1);
	cubeVert[2] = height(xc, yc+1, zc);
	cubeVert[3] = height(xc, yc+1, zc+1);
	cubeVert[4] = height(xc+1, yc, zc);
	cubeVert[5] = height(xc+1, yc, zc+1);
	cubeVert[6] = height(xc+1, yc+1, zc);
	cubeVert[7] = height(xc+1, yc+1, zc+1);

	theCase = 0;
	for (whichVertex = 0; whichVertex < 8; whichVertex++)										//	loop through corners, computing facet lookup
		if (ht < cubeVert[whichVertex])													//	if the corner is above desired height
			theCase |= (1 << whichVertex);												//	set bit flag												

	theSurface = seedEdge2Surface[theCase][whichCubeEdge];										//	find the surface to which be belong
	if (theSurface == -1)																//	error check for bad surface
		{ printf("Major problem: Edge %ld in case %ld is not a valid seed edge\n", whichCubeEdge, theCase); return;}

	whichFaceEdge = surface2exitEdges[theCase][theSurface][0];									//	take the first exit edge (arbitrarily)

	IntersectSurface(ht, xc, yc, zc, whichFaceEdge);											//	start the surface off with the given edge
	UnFlagSurface(ht, xc, yc, zc, whichFaceEdge);											//	clean out the flags
	} // FollowSurface()
	
void HeightField::IntersectSurface(float ht, long x, long y, long z, int theEntryFaceEdge)			//	starts drawing cube at specified edge
	{ // IntersectSurface()	
	long theCase = 0;	

        //printf("%f %d %d %d %d\n", ht, x, y, z, theEntryFaceEdge);
															//	the marching cubes case
	long theSurface;																	//	the surface the edge belongs to
	long whichExitFaceEdge;																//	index for exit edges
	long theExitFaceEdge;																//	the exit edge & entry in next cube
	float cubeVert[8];																	//	local array holding vertices of cube
	long whichVertex;																	//	index for loop through vertices
	long whichTri;																		//	index for loop through triangles	
	queue<CellSurface> theQueue;															//	queue of surfaces to be processed.
	CellSurface theCellSurface = CellSurface(x, y, z, theEntryFaceEdge);							//	cell surface being processed
	theQueue.push(theCellSurface);														//	push the initial cell surface on queue
	while (! theQueue.empty())															//	while the queue is non-empty
		{ // loop to empty queue
                // printf("===========\n");
		theCellSurface = theQueue.front();													//	grab the front item
		theQueue.pop();																//	and pop to remove it
		if ((theCellSurface.x < 0) || (theCellSurface.y < 0) || (theCellSurface.z < 0)) continue;		//	check for out-of bounds
		if ((theCellSurface.x > xDim-2) || (theCellSurface.y > yDim-2) || (theCellSurface.z > zDim-2)) continue;

		cubeVert[0] = height(theCellSurface.x, theCellSurface.y, theCellSurface.z);				//	copy the vertex heights
		cubeVert[1] = height(theCellSurface.x, theCellSurface.y, theCellSurface.z+1);
		cubeVert[2] = height(theCellSurface.x, theCellSurface.y+1, theCellSurface.z);	
		cubeVert[3] = height(theCellSurface.x, theCellSurface.y+1, theCellSurface.z+1);
		cubeVert[4] = height(theCellSurface.x+1, theCellSurface.y, theCellSurface.z);	
		cubeVert[5] = height(theCellSurface.x+1, theCellSurface.y, theCellSurface.z+1);
		cubeVert[6] = height(theCellSurface.x+1, theCellSurface.y+1, theCellSurface.z);	
		cubeVert[7] = height(theCellSurface.x+1, theCellSurface.y+1, theCellSurface.z+1);

		theCase = 0;
		for (whichVertex = 0; whichVertex < 8; whichVertex++)									//	loop through corners, computing facet lookup
			if (ht < cubeVert[whichVertex])												//	if the corner is above desired height
				theCase |= (1 << whichVertex);											//	set bit flag												
	
		theSurface = entryEdge2Surface[theCase][theCellSurface.entryFaceEdge];					//	find the surface we are on
		if (Visited(theCellSurface.x, theCellSurface.y, theCellSurface.z, theSurface)) continue;		//	if it's been flagged, skip it													//	never revisit the same surface
		Visit(theCellSurface.x, theCellSurface.y, theCellSurface.z, theSurface);					//	mark the surface as "visited"
	
		// printf("i: %2d j: %2d k: %2d. Case: %3d Base Case: %3s.\n", 
		//	theCellSurface.x, theCellSurface.y, theCellSurface.z, theCase, caseName[baseCase[theCase]]);
		//printf("%d %d %d %d\n", x, xDim, y, yDim);
//		if ((theCellSurface.x < xDim/2) ) //&& (theCellSurface.y < yDim/2))
//			{
			for (whichTri = 0; whichTri < 3*nTriangles[theCase][theSurface]; whichTri+= 3)				//	walk through the triangles of the surface
				RenderTriangle(ht, 	theCellSurface.x, theCellSurface.y, theCellSurface.z, cubeVert, 	
								mcFollowTriangles[theCase][theSurface][whichTri + 0], 
								mcFollowTriangles[theCase][theSurface][whichTri + 1], 
								mcFollowTriangles[theCase][theSurface][whichTri + 2]);
//			}
		//	now follow the contour out each face
		for (whichExitFaceEdge = 0; whichExitFaceEdge < nExitEdges[theCase][theSurface]; whichExitFaceEdge++)
			{ // for each exit edge
                    //    printf("%d\n", whichExitFaceEdge);
			theExitFaceEdge = surface2exitEdges[theCase][theSurface][whichExitFaceEdge];				//	find the exit edge's ID
			
			theQueue.push(CellSurface(
				theCellSurface.x + exitDirection[theExitFaceEdge][0], 								//	first entry in row gives delta x
				theCellSurface.y + exitDirection[theExitFaceEdge][1],								//	second entry gives delta y
				theCellSurface.z + exitDirection[theExitFaceEdge][2], 								//	third entry gives delta z
				exit2entryEdge[theExitFaceEdge]));												//	convert from exit to entry
			} // for each exit edge
		} // loop to empty queue		
	} // IntersectSurface()

void HeightField::CentralDifferenceNormal(long x, long y, long z, GLfloat *normal)
	{ // CentralDifferenceNormal()
	if (x == 0) 			normal[0] = 2 * (height(x + 1, y, z) - height(x, y, z));
	else if (x == xDim - 1)	normal[0] = 2 * (height(x, y, z) - height(x - 1, y, z));
	else 				normal[0] = height(x + 1, y, z) - height(x - 1, y, z);

	if (y== 0) 			normal[1] = 2 * (height(x, y + 1, z) - height(x, y, z));
	else if (y == yDim - 1)	normal[1] = 2 * (height(x, y, z) - height(x, y - 1, z));
	else 				normal[1] = height(x, y + 1, z) - height(x, y - 1, z);

	if (z == 0) 			normal[2] = 2 * (height(x, y, z + 1) - height(x, y, z));
	else if (z == zDim - 1)	normal[2] = 2 * (height(x, y, z) - height(x, y, z - 1));
	else 				normal[2] = height(x, y, z + 1) - height(x, y, z - 1);
	} // CentralDifferenceNormal()

void HeightField::InterpolateVertex(long x, long y, long z, int edge, float *cubeVert, float ht)
	{ // InterpolateVertex()
	int v0 = edgeVertices[edge][0], v1 = edgeVertices[edge][1];
	int x0 = x + mcFollowVertexCoords[v0][0], y0 = y + mcFollowVertexCoords[v0][1], z0 = z + mcFollowVertexCoords[v0][2]; 
	int x1 = x + mcFollowVertexCoords[v1][0], y1 = y + mcFollowVertexCoords[v1][1], z1 = z + mcFollowVertexCoords[v1][2]; 
	float height0 = cubeVert[v0], height1 = cubeVert[v1];

	GLfloat norm0[3], norm1[3], norm[3], vertex[3];
	float normNorm;																	//	yes, the norm of the normal vector

	CentralDifferenceNormal(x0, y0, z0, norm0);
	CentralDifferenceNormal(x1, y1, z1, norm1);
	InterpolatePoint(norm0[0], norm0[1], norm0[2], height0, norm1[0], norm1[1], norm1[2], height1, ht, norm);

	normNorm = norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2];								//	compute the norm
	normNorm = 1.0 / sqrt(normNorm);												
	norm[0] *= normNorm; norm[1] *= normNorm; norm[2] *= normNorm;								//	and adjust the vector

	glNormal3fv(norm);	
	
	InterpolatePoint(x0, y0, z0, cubeVert[v0], x1, y1, z1, cubeVert[v1], ht, vertex);
	glVertex3fv(vertex);
	} // InterpolateVertex()

void HeightField::RenderTriangle(float ht, long x, long y, long z, float *cubeVert, int edge1, int edge2, int edge3)
																					//	renders the triangle specified by three edges
	{ // RenderTriangle()
	
// 	printf("Edge %2d: %2d to %2d\n", edge1, edgeVertices[edge1][0], edgeVertices[edge1][1]);
// 	printf("Edge %2d: %2d to %2d\n", edge2, edgeVertices[edge2][0], edgeVertices[edge2][1]);
// 	printf("Edge %2d: %2d to %2d\n", edge3, edgeVertices[edge3][0], edgeVertices[edge3][1]);
	
	//	call subroutine for each vertex
	InterpolateVertex(x, y, z, edge1, cubeVert, ht);
	InterpolateVertex(x, y, z, edge2, cubeVert, ht);
	InterpolateVertex(x, y, z, edge3, cubeVert, ht);

	//	increment the counter
	triangleCount++;
	} // RenderTriangle()	

void HeightField::Visit(long xc, long yc, long zc, long cubeContour)								//	sets the flag for a particular contour
	{ // Visit()
	visitFlags(xc, yc, zc) |= (1 << cubeContour);											//	do a shift to get the right bit	
	} // Visit()

void HeightField::UnVisit(long xc, long yc, long zc, long cubeContour)							//	resets the flag for a particular contour
	{ // UnVisit()
	visitFlags(xc, yc, zc) &= ~(1 << cubeContour);											//	do a shift to get the right bit	
	} // UnVisit()

bool HeightField::Visited(long xc, long yc, long zc, long cubeContour)							//	tests whether flag set for particular contour
	{ // Visited()
	if (visitFlags(xc, yc, zc) & (1 << cubeContour)) return true; else return false;				//	test the particular flag
	} // Visited()

void HeightField::UnFlagSurface(float ht, long x, long y, long z, int theEntryFaceEdge)				//	resets flags, starting at specified edge
	{ // UnFlagSurface()	
	long theCase = 0;																	//	the marching cubes case
	long theSurface;																	//	the surface the edge belongs to
	long whichExitFaceEdge;																//	index for exit edges
	long theExitFaceEdge;																//	the exit edge & entry in next cube
	float cubeVert[8];																	//	local array holding vertices of cube
	long whichVertex;																	//	index for loop through vertices
	
	queue<CellSurface> theQueue;															//	queue of surfaces to be processed.
	CellSurface theCellSurface = CellSurface(x, y, z, theEntryFaceEdge);							//	cell surface being processed
	
	theQueue.push(theCellSurface);														//	push the initial cell surface on queue
	
	while (! theQueue.empty())															//	while the queue is non-empty
		{ // loop to empty queue
		theCellSurface = theQueue.front();													//	grab the front item
		theQueue.pop();																//	and pop to remove it
		if ((theCellSurface.x < 0) || (theCellSurface.y < 0) || (theCellSurface.z < 0)) continue;		//	check for out-of bounds
		if ((theCellSurface.x > xDim-2) || (theCellSurface.y > yDim-2) || (theCellSurface.z > zDim-2)) continue;

		cubeVert[0] = height(theCellSurface.x, theCellSurface.y, theCellSurface.z);				//	copy the vertex heights
		cubeVert[1] = height(theCellSurface.x, theCellSurface.y, theCellSurface.z+1);
		cubeVert[2] = height(theCellSurface.x, theCellSurface.y+1, theCellSurface.z);	
		cubeVert[3] = height(theCellSurface.x, theCellSurface.y+1, theCellSurface.z+1);
		cubeVert[4] = height(theCellSurface.x+1, theCellSurface.y, theCellSurface.z);	
		cubeVert[5] = height(theCellSurface.x+1, theCellSurface.y, theCellSurface.z+1);
		cubeVert[6] = height(theCellSurface.x+1, theCellSurface.y+1, theCellSurface.z);	
		cubeVert[7] = height(theCellSurface.x+1, theCellSurface.y+1, theCellSurface.z+1);

		theCase = 0;
		for (whichVertex = 0; whichVertex < 8; whichVertex++)									//	loop through corners, computing facet lookup
			if (ht < cubeVert[whichVertex])												//	if the corner is above desired height
				theCase |= (1 << whichVertex);											//	set bit flag												
	
		theSurface = entryEdge2Surface[theCase][theCellSurface.entryFaceEdge];					//	find the surface we are on
		if (! Visited(theCellSurface.x, theCellSurface.y, theCellSurface.z, theSurface)) continue;	//	if it's been unflagged, skip it													//	never revisit the same surface
		UnVisit(theCellSurface.x, theCellSurface.y, theCellSurface.z, theSurface);					//	mark the surface as "visited"
	
		//	now follow the contour out each face
		for (whichExitFaceEdge = 0; whichExitFaceEdge < nExitEdges[theCase][theSurface]; whichExitFaceEdge++)
			{ // for each exit edge
			theExitFaceEdge = surface2exitEdges[theCase][theSurface][whichExitFaceEdge];				//	find the exit edge's ID
			
			theQueue.push(CellSurface(
				theCellSurface.x + exitDirection[theExitFaceEdge][0], 								//	first entry in row gives delta x
				theCellSurface.y + exitDirection[theExitFaceEdge][1],								//	second entry gives delta y
				theCellSurface.z + exitDirection[theExitFaceEdge][2], 								//	third entry gives delta z
				exit2entryEdge[theExitFaceEdge]));												//	convert from exit to entry
			} // for each exit edge
		} // loop to empty queue		
	} // UnFlagSurface()

void ComputeNormal(float *p, float *q, float *r, float *n)										//	computes normal for triangle pqr, puts it in n
	{ // ComputeNormal()	
	float pq[3], pr[3];																	//	local variables for edge vectors
	float nNorm;																		//	norm of this vector (I like unit vectors)
	
	for (int i = 0; i < 3; i++)															//	loop to compute pq, pr
		{
		pq[i] = q[i] - p[i]; pr[i] = r[i] - p[i];											//	just subtract . . .
		} // end of loop to compute vectors 

	n[0] = pq[1]*pr[2] - pr[1]*pq[2];														//	compute cross-product
	n[1] = pq[2]*pr[0] - pr[2]*pq[0];
	n[2] = pq[0]*pr[1] - pr[0]*pq[1];
	nNorm = n[0]*n[0] + n[1]*n[1] + n[2]*n[2];												//	compute the norm
	nNorm = 1.0 / sqrt(nNorm);												
	n[0] *= nNorm; n[1] *= nNorm; n[2] *= nNorm;												//	and adjust the vector
	} // ComputeNormal()	
	
