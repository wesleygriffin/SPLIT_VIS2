///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	August, 2003
//
//	------------------------
//	LogCollapsePane.cpp
//	------------------------
//	
//	A pane displaying the tree size / collapse
//	bound as a log-log plot
//	
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	08/01/03		Initial version
//
///////////////////////////////////////////////////


#include "LogCollapsePane.h"		
#include <GL/freeglut.h>

LogCollapsePane::LogCollapsePane(GLUTPane *Parent, int Width, int Height, HeightField *newField, 
		bool GrowHoriz, bool GrowVert)
	: GLUTPane(Parent, Width, Height, GrowHoriz, GrowVert)
	{ // LogCollapsePane()												//	constructor 
	field = newField;													//	store the field
	} // LogCollapsePane()
	
LogCollapsePane::~LogCollapsePane()										//	destructor
	{ // ~LogCollapsePane()
	} // ~LogCollapsePane()
																		
void LogCollapsePane::Display()											//	display routine																	
	{ // Display()
	GLfloat backgroundColour[3] = { 0.0, 0.0, 0.0 };							//	on white
	GLfloat edgeColour[3] = { 0.25, 0.25, 0.25 };							//	draw in dark grey

	glPushAttrib(GL_ALL_ATTRIB_BITS);										//	save the attribute frame
	glDisable(GL_LIGHTING);												//	turn off lighting
	glDisable(GL_DEPTH_TEST);											//	turn off depth test

	glMatrixMode(GL_PROJECTION);											//	set the projection matrix
	glLoadIdentity();													//	revert to identity matrix
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);									//	and set a simple projection
		
	glMatrixMode(GL_MODELVIEW);											//	switch to modelview matrix
	glLoadIdentity();													//	and revert to the identity matrix
	
	glColor3fv(backgroundColour);											//	set the background colour
	glBegin(GL_LINE_STRIP);												//	and draw a "square"
	glVertex2f(0.0, 0.999);
	glVertex2f(0.999, 0.999);
	glVertex2f(0.999, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 0.999);
	glEnd();

	glColor3fv(edgeColour);												//	set the edge colour
	field->PlotLogCollapse();

	glPopAttrib();														//	return to previous attributes
	} // Display()
