///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTCheckableBox.cpp
//	------------------------
//	
//	A checkbox  without text
//
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	02/08/01		Initial version
//
///////////////////////////////////////////////////	

#include "GLUTCheckableBox.h"
#include <GL/freeglut.h>
#include <stdio.h>

GLUTCheckableBox::GLUTCheckableBox(GLUTPane *Parent, bool *Target, bool *Dirty, bool initiallyChecked)		
	: GLUTPane(Parent, GLUT_CHECKABLE_BOX_TOTAL_SIZE, GLUT_CHECKABLE_BOX_TOTAL_SIZE, false, false)
	{ // GLUTCheckableBox()
	dragCheck = false;														//	not currently dragging
	checked = initiallyChecked;												//	set initial check status
	dirty = Dirty; target = Target;											//	set the external pointers
	if (target != NULL) *target = initiallyChecked; 								//	set initial value, if relevant
	if (dirty != NULL) *dirty = true;											//	set initial value for this, too
	} // GLUTCheckableBox()
	
GLUTCheckableBox::~GLUTCheckableBox()											//	destructor
	{ // GLUTCheckableBox()
	} // GLUTCheckableBox()
	
void GLUTCheckableBox::Display()												//	display
	{ // Display()
	GLfloat boxColour[3] = { 0.0, 0.0, 0.0 };									//	we want to draw in black
//	GLfloat backgroundColour[3] = { 1.0, 1.0, 1.0 };								//	on white

//	glPushAttrib(GL_ALL_ATTRIB_BITS);											//	save the attribute frame
//	glDisable(GL_LIGHTING);													//	turn off lighting
//	glDisable(GL_DEPTH_TEST);												//	turn off depth test

	glMatrixMode(GL_PROJECTION);												//	set the projection matrix
	glLoadIdentity();														//	revert to identity matrix
	glOrtho(left, left + width, bottom, bottom + height, -1.0, 1.0);					//	set projection to match viewport

//	glMatrixMode(GL_MODELVIEW);												//	switch to modelview matrix
//	glLoadIdentity();														//	and revert to the identity matrix
	
//	glColor3fv(backgroundColour);												//	set the background colour
//	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
//	glBegin(GL_POLYGON);													//	and draw a "square"
//	glVertex2f(left, bottom);
//	glVertex2f(left + width, bottom);
//	glVertex2f(left + width, bottom + height);
//	glVertex2f(left, bottom + height);
//	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
//	glEnd();

	glColor3fv(boxColour);													//	switch to box colour
	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
	glBegin(GL_LINE_LOOP);													//	and draw an empty square													
	glVertex2f(left + GLUT_CHECKABLE_BOX_BOUNDARY, bottom + GLUT_CHECKABLE_BOX_BOUNDARY);	
	glVertex2f(left + width - GLUT_CHECKABLE_BOX_BOUNDARY, bottom + GLUT_CHECKABLE_BOX_BOUNDARY);	
	glVertex2f(left + width - GLUT_CHECKABLE_BOX_BOUNDARY, bottom + height - GLUT_CHECKABLE_BOX_BOUNDARY);	
	glVertex2f(left + GLUT_CHECKABLE_BOX_BOUNDARY, bottom + height - GLUT_CHECKABLE_BOX_BOUNDARY);	
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();
	
	if (checked ^ dragCheck)													//	i.e. dragCheck toggles checked visually
		{ // the check mark
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_LINES);													//	draw an X													
		glVertex2f(left + GLUT_CHECKABLE_BOX_BOUNDARY, bottom + GLUT_CHECKABLE_BOX_BOUNDARY);	
		glVertex2f(left + width - GLUT_CHECKABLE_BOX_BOUNDARY, bottom + height - GLUT_CHECKABLE_BOX_BOUNDARY);	
		glVertex2f(left + width - GLUT_CHECKABLE_BOX_BOUNDARY, bottom + GLUT_CHECKABLE_BOX_BOUNDARY);	
		glVertex2f(left + GLUT_CHECKABLE_BOX_BOUNDARY, bottom + height - GLUT_CHECKABLE_BOX_BOUNDARY);	
		//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
		glEnd();
		} // the check mark
//	glPopAttrib();															//	go back to previous attributes
	} // Display()
	
void GLUTCheckableBox::Mouse(int button, int state, int x, int y)					//	mouse down / up routine 
	{ // Mouse()
//	printf("GLUTCheckableBox::Mouse(): %s button in state %s at (%ld, %ld)\n", (button == GLUT_RIGHT_BUTTON) ? "right" :
//		((button == GLUT_LEFT_BUTTON) ? "left" : "middle"), (state == GLUT_UP) ? "up" : "down", x, y); 
	if ((state == GLUT_DOWN) && (x > GLUT_CHECKABLE_BOX_BOUNDARY) && (x < width - GLUT_CHECKABLE_BOX_BOUNDARY) &&
			(y > GLUT_CHECKABLE_BOX_BOUNDARY) && (y < height - GLUT_CHECKABLE_BOX_BOUNDARY))
		{ // mouse-down in the box proper
//		printf("Mouse down in box.\n");
		AcquireDragFocus(this);												//	acquire focus
		dragCheck = true;													//	set flag to toggle visually
		glutPostRedisplay();												//	force a redisplay
		} // mouse-down in the box proper
	else if ((state == GLUT_UP) && (hasDragFocus))								//	if we were dragging a checkmark, and it's mouse-up
		{ // mouse-up when dragging checkmark
//		printf("mouse up.\n");
		ReleaseDragFocus();													//	release drag focus
		if ((x > GLUT_CHECKABLE_BOX_BOUNDARY) && (x < width - GLUT_CHECKABLE_BOX_BOUNDARY) &&
			(y > GLUT_CHECKABLE_BOX_BOUNDARY) && (y < height - GLUT_CHECKABLE_BOX_BOUNDARY))
			checked = !checked;												//	invert the check flag
		dragCheck = false;													//	turn off the dragCheck for safety		
		if (target != NULL) *target = checked;									//	if the target is non-NULL, report the change
		if (dirty != NULL) *dirty = true;										//	set dirty flag
		glutPostRedisplay();												//	force a redisplay
		} // mouse-up when dragging checkmark
	} // Mouse()
	
void GLUTCheckableBox::Motion(int x, int y)										//	mouse motion routine	
	{ // Motion()
	if ((x > GLUT_CHECKABLE_BOX_BOUNDARY) && (x < width - GLUT_CHECKABLE_BOX_BOUNDARY) &&
		(y > GLUT_CHECKABLE_BOX_BOUNDARY) && (y < height - GLUT_CHECKABLE_BOX_BOUNDARY))
		dragCheck = true;													//	set flag if mouse is inside box
	else
		dragCheck = false;													//	reset flag if mouse is outside box
	glutPostRedisplay();													//	force a redisplay
	} // Motion()

void GLUTCheckableBox::PrintType()												//	prints text string for type
	{ // PrintType()
	printf("GLUTCheckableBox");
	} // PrintType()	
