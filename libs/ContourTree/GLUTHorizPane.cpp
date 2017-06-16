///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTHorizPane.cpp
//	------------------------
//	
//	A pane that stacks subpanes horizontally
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

#include "GLUTHorizPane.h"														//	the header
#include <GL/freeglut.h>																//	GLUT itself
#include <stdlib.h>																//	needed for NULL, of all things
#include <stdio.h>																//	C I/O

//extern int paneDepth;															//	in GLUTVertPane.cpp

GLUTHorizPane::GLUTHorizPane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz, bool GrowVert, int Alignment)
	: GLUTPane(Parent, Width, Height, GrowHoriz, GrowVert)								//	call inherited constructor
	{ // GLUTHorizPane()														//	constructor
	alignment = Alignment;														//	set alignment
	} // GLUTHorizPane()

void GLUTHorizPane::Reshape(int Width, int Height)									//	window reshape routine
	{ // Reshape()
	int nFixed = 0, nStretchy = 0;												//	counters for the different types of children
	int horizSpaceRequired = 0;													//	horizontal space required by fixed children
	int vertSpaceRequired = 0;													//	how much vertical space is required
	int whichChild;															//	loop index
	GLUTPane *child;															//	local pointer to a child
	int horizSpaceAvailable;														//	space to be divided among stretchy panes	
	int spacePerStretchyPane;													//	how much space per stretchy pane
	int leftEdge;																//	left-hand edge of a given pane

//	for (int i = 0; i < paneDepth; i++) printf("|");
//	printf("Entering GLUTHorizPane::Reshape() with %ld x %ld\n", Width, Height);
//	paneDepth++;
	for (whichChild = 0; whichChild < nChildren; whichChild++)							//	loop through child panes
		{ // loop through child panes
		child = children[whichChild];												//	grab a pointer to the child
		if (child->growHoriz)													//	if the child is allowed to grow horizontally
			nStretchy++;														//	then add one to the number of stretchy children
		else																	//	i.e. fixed size
			{ // fixed-size child
			nFixed++;															//	add one to the count of fixed children
			horizSpaceRequired += child->width;									//	add the child's width to the space required
			} // fixed-size child
		if (!child->growVert)													//	if the child is fixed vertically
			if (child->height > vertSpaceRequired) vertSpaceRequired = child->height;		//	take the max. of child heights
		} // loop through child panes
//	for (int i = 0; i < paneDepth; i++) printf("|");
//	printf("Space required (v,h): %ld, %ld\n", vertSpaceRequired, horizSpaceRequired);
	
	if (horizSpaceRequired > Width)												//	if this happens, we have a problem
		Width = horizSpaceRequired;												//	so reset it: eventually, the window will notice

	if (vertSpaceRequired > Height)												//	if this happens, we also have a problem
		Height = vertSpaceRequired;												//	again, assume the window will eventually notice

	if ((!growHoriz) & (width > horizSpaceRequired))									//	if the pane itself needs more room
		Width = width;															//	use this as the minimum

	if ((!growVert) & (height > vertSpaceRequired))									//	if the pane itself needs more room
		Height = height;														//	use this as the minimum

	width = Width; height = Height;												//	reset the pane's width and height	

//	for (int i = 0; i < paneDepth; i++) printf("|");
//	printf("Size (w,h): %ld, %ld\n", width, height);
		
	horizSpaceAvailable = Width - horizSpaceRequired;									//	compute how much spare space there is
	if (nStretchy > 0)	spacePerStretchyPane = horizSpaceAvailable / nStretchy;			//	compute the amount of space per stretchy pane
	else	spacePerStretchyPane = 0;												//	be paranoid
	leftEdge = left;															//	start at left
		
	for (whichChild = 0; whichChild < nChildren; whichChild++)							//	loop through child panes
		{ // loop through child panes
		int childWidth, childHeight;												//	new width & height for the child
		child = children[whichChild];												//	grab a pointer to the child

//		for (int i = 0; i < paneDepth; i++) printf("|");
//		printf("Child %ld: %X. LeftEdge: %ld\n", whichChild, child, leftEdge);

		child->left = leftEdge;													//	set the new left edge of the child
		if (child->growHoriz)													//	if the child is allowed to grow horizontally
			{ // a stretchy pane
//			for (int i = 0; i < paneDepth; i++) printf("|");
			if (nStretchy-- == 1)												//	if this is the last stretchy child
				spacePerStretchyPane = horizSpaceAvailable;							//	make sure it get any spare pixels	
			childWidth = spacePerStretchyPane;										//	set the width
			horizSpaceAvailable -= childWidth;										//	and update the available space
			} // a stretchy pane
		else																	//	i.e. fixed size
			{ // fixed-size child
			childWidth = child->width;											//	set the width
			} // fixed-size child
		if (child->growVert)													//	if the child grows vertically
			{ // stretchy vertically
			child->bottom = bottom;												//	set it to the same bottom as this pane
			childHeight = Height;												//	and set the height
			} // stretchy vertically
		else // i.e. fixed vertically
			{ // fixed vertically
			switch (alignment)													//	depending on the pane's alignment policy
				{ // switch on alignment
				case ALIGN_VERTICAL_BOTTOM:										//	sticking to bottom edge
					child->bottom = bottom;
					break;
				case ALIGN_VERTICAL_TOP:											//	sticking to top edge
					child->bottom = bottom + Height - child->height;	
					break;
				case ALIGN_VERTICAL_CENTRE:										//	vertically centred
				default:														//	and (from paranoia), anything else
					child->bottom = bottom + (Height - child->height)/2;	
					break;
				} // switch on alignment
			childHeight = child->height;											//	retrieve the child's height
			} // fixed vertically
//		for (int i = 0; i < paneDepth; i++) printf("|");
//		printf("Child width, height: %ld, %ld\n", childWidth, childHeight);
		child->Reshape(childWidth, childHeight);									//	ask the child to reshape accordingly
//		for (int i = 0; i < paneDepth; i++) printf("|");
//			printf("Child %X (", child); 
//		child->PrintType();
//		printf(") now has position %ld %ld, size %ld x %ld\n", child->left, child->bottom, child->width, child->height);
//		printf("Child %X now has position %ld %ld, size %ld x %ld\n", child, child->left, child->bottom, child->width, child->height);
		leftEdge += childWidth;													//	update the left edge position
		} // loop through child panes

//	paneDepth--;
//	for (int i = 0; i < paneDepth; i++) printf("|");
//	printf("Exiting GLUTHorizPane::Reshape() with %ld x %ld\n", width, height);
	} // Reshape()
	
GLUTHorizWhitePane::GLUTHorizWhitePane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz, bool GrowVert,	
	int Alignment)											//	constructor
	: GLUTHorizPane(Parent, Width, Height, GrowHoriz, GrowVert)
	{ // GLUTHorizWhitePane()
	} // GLUTHorizWhitePane()
	
void GLUTHorizWhitePane::Display()													//	display
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

void GLUTHorizPane::PrintType()													//	prints text string for type
	{ // PrintType()
	printf("GLUTHorizPane");
	} // PrintType()	

void GLUTHorizWhitePane::PrintType()												//	prints text string for type
	{ // PrintType()
	printf("GLUTHorizWhitePane");
	} // PrintType()	
	
