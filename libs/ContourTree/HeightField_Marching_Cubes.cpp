//	IsoSurface II
//	Hamish Carr, 2000

//	HeightField_Marching_Cubes.cpp:	routine to display height field using Marching Cubes
//	Palatino-12; 5 n-spaces/tab

#include "HeightField.h"
#include <GL/freeglut.h>															//	and GLUT
#include <math.h>
#include "FollowCubeTables.h"

static GLfloat march_colour[4] = 		{	1.0,		0.0,		0.0,		1.0};				//	colour for rendering components		

void InterpolatePoint(float x1, float y1, float z1, float h1, float x2, float y2, float z2, float h2, float ht, float* result);
																				//	linear interpolation along an edge

void HeightField::MarchingCubes(float ht)												//	draws level set using marching cubes
	{ // MarchingCubes()
	recursionDepth = 0; maxRecursionDepth = 0;											//	set the counters
	nCellsVisited = 0; nTrianglesGenerated = 0;	 nCellsIntersected = 0;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, march_colour);					//	set the colour

	long x, y, z;																	//	x, y, z coordinates
	float cubeVert[8];																//	local array holding vertices of cube
	int theCase;																	//	ID of type of cube for MC table
	int whichVertex;																//	vertex index for loops
	int whichSurface, whichTri;														//	indices for surface, triangle resp.
		
	glBegin(GL_TRIANGLES);																//	start describing triangles
	for (x = 0; x < xDim - 1; x ++)													//	loop through all x values
		for (y = 0; y < yDim - 1; y ++)												//	and all y values
			for (z = 0; z < zDim - 1; z ++)											//	and all z values
				{ // loop through cells
				nCellsVisited++;
				cubeVert[0] = height(x, y, z);
				cubeVert[1] = height(x, y, z+1);
				cubeVert[2] = height(x, y+1, z);
				cubeVert[3] = height(x, y+1, z+1);
				cubeVert[4] = height(x+1, y, z);
				cubeVert[5] = height(x+1, y, z+1);
				cubeVert[6] = height(x+1, y+1, z);
				cubeVert[7] = height(x+1, y+1, z+1);

				theCase = 0;
				for (whichVertex = 0; whichVertex < 8; whichVertex++)						//	loop through corners, computing facet lookup
					{ // for each vertex
					if (ht < cubeVert[whichVertex])									//	if the corner is above desired height
						theCase |= (1 << whichVertex);								//	set bit flag												
					} // for each vertex
//				printf("i: %2d j: %2d k: %2d. Case: %2d.\n", x, y, z, theCase);
				
				if ((theCase > 0) && (theCase < 255)) nCellsIntersected++;
				
				for (whichSurface = 0; whichSurface < nSurfaces[theCase]; whichSurface++)		//	do all the surfaces in the cell
					for (whichTri = 0; whichTri < 3*nTriangles[theCase][whichSurface]; whichTri+= 3)//	walk through the triangles of the surface
						{ // triangle loop
						RenderTriangle(ht, x, y, z, cubeVert, 	
							mcFollowTriangles[theCase][whichSurface][whichTri + 0], 
							mcFollowTriangles[theCase][whichSurface][whichTri + 1], 
							mcFollowTriangles[theCase][whichSurface][whichTri + 2]);
						} // triangle loop
				} //	end of loop through cells 
	glEnd();
	} // MarchingCubes()

void InterpolatePoint(float x1, float y1, float z1, float h1, float x2, float y2, float z2, float h2, float ht, float* result)
																				//	linear interpolation along an edge
	{
	float delta, one_delta;															//	parameters of interpolation
	if (h1 == h2) delta = 0.0; else delta = (h1 - ht) / (h1 - h2);							//	compute parameter from p1 to p2
	one_delta = 1.0 - delta;															//	and inverse parameter
	result[0] = one_delta*x1 + delta*x2; result[1] = one_delta*y1 + delta*y2;					//	compute resultant vertex
	result[2] = one_delta*z1 + delta*z2;				
	} // end of InterpolatePoint() 
