///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTStaticText.cpp
//	------------------------
//	
//	A pane that holds a single piece of static text
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

#include "GLUTStaticText.h"													//	the header
#include "GLUTString.h"														//	and the font utilities
#include <string.h>															//	C string handling
#include <stdlib.h>															//	C memory handling, &c.
#include <ctype.h>															//	C character utilities
#include <GL/freeglut.h>															//	and GLUT itself
#include <stdio.h>

GLUTStaticText::GLUTStaticText(GLUTPane *Parent, char *text)
	: GLUTPane(Parent, StringLength(text) + 2*STATIC_TEXT_BOUNDARY, StringHeight(text) + 2*STATIC_TEXT_BOUNDARY, false, false)
																		//	call pane constructor
	{ // GLUTStaticText()
	theText = (char *) malloc(strlen(text) + 1);									//	allocate memory for a copy of it	
	strcpy(theText, text);													//	copy the string
	} // GLUTStaticText()

GLUTStaticText::~GLUTStaticText()												//	destructor
	{ // ~GLUTStaticText()
	free(theText);															//	get rid of the string
	} // ~GLUTStaticText()

void GLUTStaticText::Display()												//	displays the text
	{ // Display()
	GLfloat textColour[3] = { 0.0, 0.0, 0.0 };									//	we want to draw in black
//	GLfloat backgroundColour[3] = { 1.0, 1.0, 1.0 };								//	on white
		
//	glPushAttrib(GL_ALL_ATTRIB_BITS);											//	save the attribute frame
	glDisable(GL_LIGHTING);													//	turn off lighting
	glDisable(GL_DEPTH_TEST);												//	turn off depth test
//	glMatrixMode(GL_PROJECTION);												//	set the projection matrix
//	glLoadIdentity();														//	reset to identity matrix
//	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);									//	project simply
//	glMatrixMode(GL_MODELVIEW);												//	switch to modelview matrix
//	glLoadIdentity();														//	and revert to the identity matrix
	
//	glColor3fv(backgroundColour);												//	set the background colour
//	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
//	glBegin(GL_POLYGON);													//	and draw a "square"
//	glVertex2f(-1.0, -1.0);
//	glVertex2f(-1.0, 1.0);
//	glVertex2f(1.0, 1.0);
//	glVertex2f(1.0, -1.0);
//	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
//	glEnd()

	glColor3fv(textColour);													//	switch to the desired text colour

	glMatrixMode(GL_PROJECTION);												//	set the projection matrix
	glLoadIdentity();														//	revert to identity matrix
	glOrtho(0, WindowWidth(), 0, WindowHeight(), -1.0, 1.0);									//	set projection to match viewport
	glViewport(0,0, WindowWidth(), WindowHeight());

	glRasterPos2i(left + STATIC_TEXT_BOUNDARY, bottom + STATIC_TEXT_BOUNDARY);						//	set the raster position
	PrintString(theText);													//	and print the string

//	glPopAttrib();															//	return to the previous attributes
	} // Display()
		
void GLUTStaticText::PrintType()												//	prints text string for type
	{ // PrintType()
	printf("GLUTStaticText");
	} // PrintType()	
	
