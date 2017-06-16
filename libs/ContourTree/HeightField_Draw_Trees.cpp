//	IsoSurface II
//	Hamish Carr, 2000

//	HeightField_Draw_Trees.cpp:	routines to display voxels, trees, &c.
//	Palatino-12; 5 n-spaces/tab

#include "HeightField.h"
#include <GL/freeglut.h>															//	and GLUT
#include <math.h>

GLfloat edge_colour[4] = 			{	0.0,		0.0,		0.0,		1.0};						//	colour for edges if not coloured	
	
void HeightField::DrawEdges()																//	draws the edges of the voxels
	{ // DrawEdges()
	int i, j;																			//	indices for creating grid

	glBegin(GL_LINES);																	//	we will generate a bunch of lines
	
	for (i = 0; i < xDim; i++)															//	loop through all x values
		for (j = 0; j < yDim; j++)														//	and all y values
			{ //	set up a vertical line
			glVertex3i(i, j, 0);														//	start at the bottom
			glVertex3i(i, j, zDim-1);													//	and go to the top
			} //	end of one x-aligned line
	
	for (i = 0; i < xDim; i++)															//	loop through all x values
		for (j = 0; j < zDim; j++)														//	and all z values
			{ //	set up a y-aligned line
			glVertex3i(i, 0, j);														//	start at one end
			glVertex3i(i, yDim-1, j);													//	and go to the other
			} //	end of one y-aligned line
		
	for (i = 0; i < yDim; i++)															//	loop through all y values
		for (j = 0; j < zDim; j++)														//	and all z values
			{ //	set up a vertical line
			glVertex3i(0, i, j);														//	start at the near end
			glVertex3i(xDim-1, i, j);													//	and go to the far end
			} //	end of one vertical line
	glEnd();																			//	end the set of lines
	} // DrawEdges()
	
void HeightField::DrawContourTree()														//	draws the contour tree
	{ // DrawContourTree()
	long datumX, datumY, datumZ;															//	position of the given datum
	long arcTargetX, arcTargetY, arcTargetZ;												//	position of the datum at the other end of arc

	glBegin(GL_LINES);																	//	we will generate a bunch of lines

	for (int i = 0; i < nSuperarcs; i++)													//	walk through the array from low to high
		{ // loop through superarcs
		height.ComputeIndex(supernodes[superarcs[i].topID].value, datumX, datumY, datumZ);			//	compute x, y, z location
		height.ComputeIndex(supernodes[superarcs[i].bottomID].value, arcTargetX, arcTargetY, arcTargetZ);//	for both ends
		glVertex3i(datumX, datumY, datumZ);												//	draw top end of edge
		glVertex3i(arcTargetX, arcTargetY, arcTargetZ);										//	draw bottom end of edge
		} // loop through superarcs
	glEnd();																			//	end the set of lines
	} // DrawContourTree()

void HeightField::DrawPlanarContourTree(bool useColouredTree)		//	draws a planar version of the contour tree
	{ // DrawPlanarContourTree()
	float heightUnit = (MaxHeight() - MinHeight()) * 0.005;	
        //printf("MaxHeight %f\n",MinHeight());								//	compute unit of height for scaling
	float cutSize = 0.02;																//	relative length of cuts
	int arc, whichArc;
	float xDiff, xHeight;

	glColor3fv(edge_colour);															//	set the colour for nodes and arcs
//        printf("nValide %d\n", nValidArcs);
//PrintValidValues();
	glLineWidth(2.0);
	glBegin(GL_LINES);																	//	we will generate a bunch of lines
	for (whichArc = 0; whichArc < nValidArcs; whichArc++)										//	walk through the array from low to high
		{ // loop through superarcs
		arc = valid[whichArc]; //printf("%d ", arc);															//	grab an edge from the list
//		printf("arc %d\n", arc);
		if (differentColouredContours && useColouredTree)
        glColor3fv(surface_colour[superarcs[arc].colour]);
		long topNode = superarcs[arc].topID, bottomNode = superarcs[arc].bottomID;					//	grab the two ends
		xDiff = supernodes[topNode].xPosn - supernodes[bottomNode].xPosn;						//	compute difference in x
		xHeight = (*(supernodes[topNode].value) - *(supernodes[bottomNode].value));				//	compute the x-height
		glVertex2f(supernodes[topNode].xPosn, supernodes[topNode].yPosn);						//	draw top end of edge
//                printf("id %d location %d %f %f %e\n",arc,topNode, supernodes[topNode].xPosn,supernodes[topNode].yPosn, *(supernodes[topNode].value));
		glVertex2f(supernodes[bottomNode].xPosn, supernodes[bottomNode].yPosn);					//	draw bottom end of edge
		} // loop through superarcs
	glEnd();																			//	end the set of lines
	glLineWidth(1.0);

	glPointSize(5.0);																	//	use a point radius of 5
	glBegin(GL_POINTS);																	//	now draw some points

	for (int whichNode = 0; whichNode < nValidNodes; whichNode++)								//	walk through the nodes
		{ // for each node
		int node = validNodes[whichNode];													//	grab an active node from the list
		if (differentColouredContours && useColouredTree)
                    glColor3fv(surface_colour[supernodes[node].colour]);
		glVertex2f(supernodes[node].xPosn, supernodes[node].yPosn);								//	place the point
		} // for each node
	glEnd();																			//	end the set of points
	glPointSize(1.0);																	//	use a point radius of 1
	
	glBegin(GL_QUADS);																	//	one quad per edge
	for (whichArc = 0; whichArc < nActiveArcs; whichArc++)										//	walk through the array from low to high
		{ // loop through superarcs
		arc = active[whichArc];															//	grab an edge from the list
//		printf("arc %d\n", arc);
		if (! superarcs[arc].CheckFlag(Superarc::isValid)) { printf("Yowch! %d \n", arc); continue; }
		if (superarcs[arc].CheckFlag(Superarc::isSuppressed)) continue;							//	skip suppressed edges
		
		long topNode = superarcs[arc].topID, bottomNode = superarcs[arc].bottomID;					//	grab the two ends
		xDiff = supernodes[topNode].xPosn - supernodes[bottomNode].xPosn;						//	compute difference in x
		xHeight = (*(supernodes[topNode].value) - *(supernodes[bottomNode].value));				//	compute the x-height
	        	
		if (differentColouredContours)
			glColor3fv(surface_colour[superarcs[arc].colour]);
		else
			glColor3fv(basic_colour);													//	set the colour for this contour
		float xPosn = supernodes[bottomNode].xPosn + xDiff * (superarcs[arc].seedValue - *(supernodes[bottomNode].value)) / xHeight;	
																					//	compute x position
		glVertex2f(xPosn-cutSize, superarcs[arc].seedValue-2*heightUnit);				
		glVertex2f(xPosn+cutSize, superarcs[arc].seedValue-2*heightUnit);				
		glVertex2f(xPosn+cutSize, superarcs[arc].seedValue+2*heightUnit);				
		glVertex2f(xPosn-cutSize, superarcs[arc].seedValue+2*heightUnit);		
		} // loop through superarcs

	for (whichArc = 0; whichArc < nSelectedArcs; whichArc++)									//	walk through the array from low to high
		{ // loop through superarcs
		arc = selected[whichArc];														//	grab an edge from the list
		long topNode = superarcs[arc].topID, bottomNode = superarcs[arc].bottomID;					//	grab the two ends
		xDiff = supernodes[topNode].xPosn - supernodes[bottomNode].xPosn;						//	compute difference in x
		xHeight = (*(supernodes[topNode].value) - *(supernodes[bottomNode].value));				//	compute the x-height
		
		if (differentColouredContours)
			glColor3fv(basic_colour);								//	set the colour for this contour
		else
			glColor3fv(select_colour);													//	set the colour for this contour
		float xPosn = supernodes[bottomNode].xPosn + xDiff * (currentSelectionValue - *(supernodes[bottomNode].value)) / xHeight;	
																					//	compute x position
		glVertex2f(xPosn-1.5*cutSize, currentSelectionValue-3*heightUnit);				
		glVertex2f(xPosn+1.5*cutSize, currentSelectionValue-3*heightUnit);				
		glVertex2f(xPosn+1.5*cutSize, currentSelectionValue+3*heightUnit);				
		glVertex2f(xPosn-1.5*cutSize, currentSelectionValue+3*heightUnit);		
		} // loop through superarcs

	glEnd();																			//	end the set of quads
	} // DrawPlanarContourTree()

#define N_LOG_COLLAPSE_SAMPLES 120
void HeightField::PlotLogCollapse()														//	plots the log collapse curve
	{ // PlotLogCollapse()
	float logNSuperarcs = log((float)nNonEpsilonArcs);												//	compute the range
	long targetNArcs;																	//	how many arcs we are looking for
	float xPosition, yPosition;															//	position of chosen point
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	//	walk across a fixed number of times
	for (int i = 0; i <= N_LOG_COLLAPSE_SAMPLES; i++)
		{ // for each sample
		xPosition = (float) i / (float) N_LOG_COLLAPSE_SAMPLES;								//	compute the ratio
		targetNArcs = (long) exp(logNSuperarcs * xPosition);									//	and compute the target # of arcs
		yPosition = log((float)collapseBounds[targetNArcs]) / log((float)nVertices);							//	compute y position
//		printf("%2d, %6d: (%7.3f, %7.3f)\n", i, targetNArcs, xPosition, yPosition);
		glVertex2f(xPosition, yPosition);													//	plot the point
		} // for each sample	
	glEnd();
	glLineWidth(1.0);

	//	put a point at the intersection of the lines controlled by sliders
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex2f(logTreeSize, logPriorityBound);
	glEnd();
	glPointSize(1.0);
	
	//	and put cross-hair lines through it
	glBegin(GL_LINES);
	glVertex2f(logTreeSize, 0.0);
	glVertex2f(logTreeSize, 1.0);
	glVertex2f(0.0, logPriorityBound);
	glVertex2f(1.0, logPriorityBound);
	glEnd();
	} // PlotLogCollapse()
