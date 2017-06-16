///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTRadioButton.h
//	------------------------
//	
//	A radio button class
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

#include <stdlib.h>															//	needed for NULL
#include <GL/freeglut.h>
#include "GLUTPane.h"	
#include "GLUTRadioButton.h"													//	include the header
#include <stdio.h>

GLUTRadioButton::GLUTRadioButton(GLUTPane *Parent, GLUTRadioGroup *theGroup, int ButtonValue, bool initiallyChosen)
	: GLUTPane(Parent, GLUT_RADIO_BUTTON_TOTAL_SIZE, GLUT_RADIO_BUTTON_TOTAL_SIZE, false, false)
	{ // constructor
	dragCheck = false;														//	not initially dragging
	chosen = initiallyChosen;												//	set initial "chosen" value	
	theRadioGroup = theGroup;												//	keep a properly typed pointer as well as parent	
	buttonValue = ButtonValue;												//	store the value tied to the button
	if (initiallyChosen) theRadioGroup->Update(this);								//	update the radio group
	} // constructor

GLUTRadioButton::~GLUTRadioButton()											//	destructor
	{ // destructor
	} // destructor

void GLUTRadioButton::Display()												//	display
	{ // Display()
//	GLfloat buttonColour[3] = { 0.0, 0.0, 0.0 };									//	we want to draw in black
//	GLfloat backgroundColour[3] = { 1.0, 1.0, 1.0 };								//	on white
	glPushAttrib(GL_ALL_ATTRIB_BITS);											//	save the attribute frame
	glDisable(GL_LIGHTING);													//	turn off lighting
	glDisable(GL_DEPTH_TEST);												//	turn off depth test

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
//	glColor3fv(buttonColour);												//	switch to box colour

	if (dragCheck && !chosen)												//	if it's yet unchosen
		{ 
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_POLYGON);												//	fill the whole damn thing
		}
	else
		{ 
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_LINE_LOOP);												//	just draw an empty circle													
		}
	glVertex2f(left + 7.5, bottom + 4.5);
	glVertex2f(left + 12.5, bottom + 4.5);
	glVertex2f(left + 15.5, bottom + 7.5);
	glVertex2f(left + 15.5, bottom + 12.5);
	glVertex2f(left + 12.5, bottom + 15.5);
	glVertex2f(left + 7.5, bottom + 15.5);
	glVertex2f(left + 4.5, bottom + 12.5);
	glVertex2f(left + 4.5, bottom + 7.5);
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();

	if (chosen)															//	in this case, we need a smaller circle, filled
		{ // the check mark
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_POLYGON);												//	treat it as a filled polygon
		glVertex2f(left + 8.5, bottom + 6.5);
		glVertex2f(left + 11.5, bottom + 6.5);
		glVertex2f(left + 13.5, bottom + 8.5);
		glVertex2f(left + 13.5, bottom + 11.5);
		glVertex2f(left + 11.5, bottom + 13.5);
		glVertex2f(left + 8.5, bottom + 13.5);
		glVertex2f(left + 6.5, bottom + 11.5);
		glVertex2f(left + 6.5, bottom + 8.5);
		//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
		glEnd();
		} // the check mark

	glPopAttrib();															//	go back to previous attributes
	} // Display()
	
void GLUTRadioButton::Mouse(int button, int state, int x, int y)						//	mouse down / up routine 
	{ // Mouse()
	if ((state == GLUT_DOWN) && (((x-10)*(x-10)+(y-10)*(y-10)) <= 36))				//	if within a radius of 6 of the centre
		{ // mouse-down in the button proper
//		printf("Mouse down in button.\n");
		AcquireDragFocus(this);												//	acquire focus
		dragCheck = true;													//	set flag to toggle visually
		glutPostRedisplay();												//	force a redisplay
		} // mouse-down in the button proper
	else if ((state == GLUT_UP) && (hasDragFocus))								//	if we were dragging a checkmark, and it's mouse-up
		{ // mouse-up when dragging checkmark
//		printf("mouse up.\n");
		ReleaseDragFocus();													//	release drag focus
		if (((x-10)*(x-10)+(y-10)*(y-10)) <= 36)								//	if in radius 6 of centre
			chosen = true;													//	invert the check flag
		dragCheck = false;													//	turn off the dragCheck for safety		
		theRadioGroup->Update(this);											//	call radio group to update anything else
		glutPostRedisplay();												//	force a redisplay
		} // mouse-up when dragging checkmark
	} // Mouse()

void GLUTRadioButton::Motion(int x, int y)										//	mouse motion routine	
	{ // Motion()
	if (((x-10)*(x-10)+(y-10)*(y-10)) <= 36)									//	if within 6 radius of the centre
		dragCheck = true;													//	set flag if mouse is inside box
	else
		dragCheck = false;													//	reset flag if mouse is outside box
	glutPostRedisplay();													//	force a redisplay
	} // Motion()

void GLUTRadioButton::PrintType()												//	prints text string for type
	{ // PrintType()
	printf("GLUTRadioButton");
	} // PrintType()	
