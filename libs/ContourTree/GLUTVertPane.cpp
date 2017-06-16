///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTVertPane.cpp
//	------------------------
//	
//	A pane that stacks subpanes vertically
//
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	02/07/01		Initial version
//
///////////////////////////////////////////////////	

#include "GLUTVertPane.h"														//	the header
#include <GL/freeglut.h>															//	and GLUT
#include <stdlib.h>																//	needed for NULL, of all things
#include <stdio.h>																//	C I/O

//int paneDepth = 0;

GLUTVertPane::GLUTVertPane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz, bool GrowVert, int Alignment)
	: GLUTPane(Parent, Width, Height, GrowHoriz, GrowVert)								//	call inherited constructor
	{ // GLUTVertPane()															//	constructor
	alignment = Alignment;														//	store the alignment policy
	} // GLUTVertPane()

void GLUTVertPane::Reshape(int Width, int Height)										//	window reshape routine
	{ // Reshape()
	int nFixed = 0, nStretchy = 0;												//	counters for the different types of children
	int horizSpaceRequired = 0;													//	horizontal space required by fixed children
	int vertSpaceRequired = 0;													//	how much vertical space is required
	int whichChild;															//	loop index
	GLUTPane *child;															//	local pointer to a child
	int vertSpaceAvailable;														//	space to be divided among stretchy panes	
	int spacePerStretchyPane;													//	how much space per stretchy pane
	int bottomEdge;															//	bottom edge of a given pane

//	for (int i = 0; i < paneDepth; i++) printf("|");
//	printf("Entering GLUTVertPane::Reshape() with %ld x %ld (%ld x %ld)\n", Width, Height, width, height);
//	paneDepth++;
	for (whichChild = 0; whichChild < nChildren; whichChild++)							//	loop through child panes
		{ // loop through child panes
		child = children[whichChild];												//	grab a pointer to the child
//		printf("Child %ld of %ld is %ld wide x %ld high and is %s vertically and %s horizontally\n", 
//			whichChild, nChildren, child->width, child->height, child->growVert ? "stretchy": "fixed",
//			child->growHoriz ? "stretchy" : "fixed");
		if (child->growVert)													//	if the child is allowed to grow vertically
			nStretchy++;														//	then add one to the number of stretchy children
		else																	//	i.e. fixed size
			{ // fixed-size child
			nFixed++;															//	add one to the count of fixed children
			vertSpaceRequired += child->height;									//	add the child's height to the space required
			} // fixed-size child
		if (!child->growHoriz)													//	if the child is fixed horizontally
			if (child->width > horizSpaceRequired) horizSpaceRequired = child->width;		//	take the max. of child widths
		} // loop through child panes

	if (horizSpaceRequired > Width)												//	if this happens, we have a problem
		Width = horizSpaceRequired;												//	so reset it: eventually, the window will notice

	if (vertSpaceRequired > Height)												//	if this happens, we also have a problem
		Height = vertSpaceRequired;												//	again, assume the window will eventually notice

	if ((!growHoriz) & (width > horizSpaceRequired))									//	if the pane itself needs more room
		Width = width;															//	use this as the minimum

	if ((!growVert) & (height > vertSpaceRequired))									//	if the pane itself needs more room
		Height = height;														//	use this as the minimum

	width = Width; height = Height;												//	reset the pane's width and height	
		
	vertSpaceAvailable = Height - vertSpaceRequired;									//	compute how much spare space there is
	if (nStretchy > 0)	spacePerStretchyPane = vertSpaceAvailable / nStretchy;				//	compute the amount of space per stretchy pane
	else	spacePerStretchyPane = 0;												//	be paranoid
	bottomEdge = bottom;														//	start at bottom
		
	for (whichChild = nChildren - 1; whichChild >= 0; whichChild--)						//	loop through child panes
		{ // loop through child panes
		int childWidth, childHeight;												//	new width & height for the child
		child = children[whichChild];												//	grab a pointer to the child
//		printf("Child %ld of %ld is %ld wide x %ld high and is %s vertically and %s horizontally\n", 
//			whichChild, nChildren, child->width, child->height, child->growVert ? "stretchy": "fixed",
//			child->growHoriz ? "stretchy" : "fixed");
//	for (int i = 0; i < paneDepth; i++) printf("|");
//		printf("Bottom edge now %ld, vertSpaceAvailable = %ld\n", bottomEdge, vertSpaceAvailable);
		child->bottom = bottomEdge;												//	set the new left edge of the child
		if (child->growVert)													//	if the child is allowed to grow vertically
			{ // a stretchy pane
			if (nStretchy-- == 1)												//	if this is the last stretchy child
				spacePerStretchyPane = vertSpaceAvailable;							//	make sure it get any spare pixels	
			childHeight = spacePerStretchyPane;									//	set the width
			vertSpaceAvailable -= childHeight;										//	and update the available space
			} // a stretchy pane
		else																	//	i.e. fixed size
			{ // fixed-size child
			childHeight = child->height;											//	set the height
			} // fixed-size child
		if (child->growHoriz)													//	if the child grows vertically
			{ // stretchy horizontally
//			printf("left %ld width %ld\n", left, width);
			child->left = left;													//	set it to the same left as this pane
			childWidth = Width;													//	and set the width
			} // stretchy horizontally
		else // i.e. fixed horizontally
			{ // fixed horizontally
			switch (alignment)													//	depending on the pane's alignment policy
				{ // switch on alignment
				case ALIGN_HORIZONTAL_LEFT:										//	sticking to left edge
					child->left = left;
					break;
				case ALIGN_HORIZONTAL_RIGHT:										//	sticking to right edge
					child->left = left + Width - child->width;	
					break;
				case ALIGN_HORIZONTAL_CENTRE:										//	vertically centred
				default:														//	and (from paranoia), anything else
//					printf("Child left: %ld, left: %ld, Width: %ld, child->width: %ld\n", child->left, left, Width, child->width);
					child->left = left + (Width - child->width)/2;	
					break;
				} // switch on alignment
			childWidth = child->width;											//	retrieve the child's width
			} // fixed horizontally
		child->Reshape(childWidth, childHeight);									//	ask the child to reshape accordingly
		bottomEdge += childHeight;												//	update the bottom edge position
//		for (int i = 0; i < paneDepth; i++) printf("|");
//			printf("Child %X (", child); 
//		child->PrintType();
//		printf(") now has position %ld %ld, size %ld x %ld\n", child->left, child->bottom, child->width, child->height);
		} // loop through child panes

//	paneDepth--;
//	for (int i = 0; i < paneDepth; i++) printf("|");
//	printf("Exiting GLUTVertPane::Reshape() with %ld x %ld (%ld x %ld)\n", Width, Height, width, height);
	} // Reshape()

GLUTVertWhitePane::GLUTVertWhitePane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz, bool GrowVert,	
	int Alignment)																//	constructor
	: GLUTVertPane(Parent, Width, Height, GrowHoriz, GrowVert, Alignment)
	{ // GLUTVertWhitePane()
	} // GLUTVertWhitePane()
	
void GLUTVertWhitePane::Display()													//	display
	{ // Display()
	GLfloat backgroundColour[3] = { 1.0, 1.0, 1.0 };									//	on white

	glPushAttrib(GL_ALL_ATTRIB_BITS);												//	save the attribute frame
	glDisable(GL_LIGHTING);														//	turn off lighting
	glDisable(GL_DEPTH_TEST);													//	turn off depth test

	glMatrixMode(GL_PROJECTION);													//	set the projection matrix
	glLoadIdentity();															//	revert to identity matrix
	glOrtho(left, left + width, bottom, bottom + height, -1.0, 1.0);						//	set projection to match viewport

	glMatrixMode(GL_MODELVIEW);													//	switch to modelview matrix
	glLoadIdentity();															//	and revert to the identity matrix
	
	glColor3fv(backgroundColour);													//	set the background colour
	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
	glBegin(GL_POLYGON);														//	and draw a "square"
	glVertex2f(left, bottom);
	glVertex2f(left + width, bottom);
	glVertex2f(left + width, bottom + height);
	glVertex2f(left, bottom + height);
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();

	GLUTPane::Display();														//	call the inherited version	
	glPopAttrib();																//	return to previous attributes
	} // Display()	
	
void GLUTVertPane::PrintType()													//	prints text string for type
	{ // PrintType()
	printf("GLUTVertPane");
	} // PrintType()	

void GLUTVertWhitePane::PrintType()												//	prints text string for type
	{ // PrintType()
	printf("GLUTVertWhitePane");
	} // PrintType()	
