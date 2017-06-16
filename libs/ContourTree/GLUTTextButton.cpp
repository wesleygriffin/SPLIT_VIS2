///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTTextButton.cpp
//	------------------------
//	
//	A button with text in it
//
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	02/09/01		Initial version
//
///////////////////////////////////////////////////	

#include <stdlib.h>															//	needed for NULL
#include "GLUTTextButton.h"	
#include <GL/freeglut.h>
#include <string.h>
#include "GLUTString.h"
#include <stdio.h>

GLUTTextButton::GLUTTextButton(GLUTPane *Parent, char *Text, GLUTPane *Target, int Action)//	constructor
	: GLUTPane(Parent, GLUT_TEXT_BUTTON_SIZE + 2 * GLUT_TEXT_BUTTON_BOUNDARY + StringLength(Text),
		GLUT_TEXT_BUTTON_SIZE + 2 * GLUT_TEXT_BUTTON_BOUNDARY, false, false)
	{ // constructor
//	printf("Text: %s\n", Text);fflush(stdout);
	text = (char *) malloc(strlen(Text) + 1);									//	allocate memory for a copy of it	
	strcpy(text, Text);														//	copy the string
//	printf("text: %s\n", text);fflush(stdout);
	textLen = StringLength(text);												//	compute the length of the text
	target = Target;														//	set the target
	invert = false;														//	don't invert colours
	action = Action;														//	set the action
	} // constructor

GLUTTextButton::~GLUTTextButton()												//	destructor
	{ // destructor
	free(text);															//	get rid of the string
	} // destructor

void GLUTTextButton::Display()												//	display
	{ // Display()
	GLfloat buttonColour[3] = { 0.0, 0.0, 0.0 };									//	we want to draw in black
	GLfloat backgroundColour[3] = { 1.0, 1.0, 1.0 };								//	on white
	glPushAttrib(GL_ALL_ATTRIB_BITS);											//	save the attribute frame
	glDisable(GL_LIGHTING);													//	turn off lighting
	glDisable(GL_DEPTH_TEST);												//	turn off depth test

	glMatrixMode(GL_PROJECTION);												//	set the projection matrix
	glLoadIdentity();														//	revert to identity matrix
	glOrtho(left, left + width, bottom, bottom + height, -1.0, 1.0);					//	set projection to match viewport

	glColor3fv(backgroundColour);												//	switch to white
	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
	glBegin(GL_POLYGON);
	glVertex2i(left, bottom);
	glVertex2i(left+width, bottom);
	glVertex2i(left+width, bottom+height);
	glVertex2i(left, bottom+height);
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();

	int butLeft = left + GLUT_TEXT_BUTTON_BOUNDARY;
	int butBottom = bottom + GLUT_TEXT_BUTTON_BOUNDARY;
	int butRight = butLeft + GLUT_TEXT_BUTTON_SIZE + textLen; 
	int butTop = butBottom + GLUT_TEXT_BUTTON_SIZE;
		
	glColor3fv(buttonColour);												//	set the button colour
	if (invert)															//	if it needs to be highlighted
		{ 
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_POLYGON);												//	fill the whole damn thing
		}
	else
		{ 
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_LINE_LOOP);												//	just draw an empty button													
		}
	glVertex2i(butLeft + GLUT_TEXT_BUTTON_CORNER_INSET, butBottom);
	glVertex2i(butRight - GLUT_TEXT_BUTTON_CORNER_INSET, butBottom);
	glVertex2i(butRight, butBottom + GLUT_TEXT_BUTTON_CORNER_INSET);
	glVertex2i(butRight, butTop - GLUT_TEXT_BUTTON_CORNER_INSET);
	glVertex2i(butRight - GLUT_TEXT_BUTTON_CORNER_INSET, butTop);
	glVertex2i(butLeft + GLUT_TEXT_BUTTON_CORNER_INSET, butTop);
	glVertex2i(butLeft, butTop - GLUT_TEXT_BUTTON_CORNER_INSET);
	glVertex2i(butLeft, butBottom + GLUT_TEXT_BUTTON_CORNER_INSET);
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();

	if (invert)															//	if it's inverted, reverse colours
		glColor3fv(backgroundColour);								

	glMatrixMode(GL_PROJECTION);												//	set the projection matrix
	glLoadIdentity();														//	revert to identity matrix
	glOrtho(0, WindowWidth(), 0, WindowHeight(), -1.0, 1.0);									//	set projection to match viewport
	glViewport(0,0, WindowWidth(), WindowHeight());
	glRasterPos2i(butLeft + GLUT_TEXT_BUTTON_RADIUS, butBottom + 4);					//	set the raster position
	PrintString(text);														//	and print the string
	glPopAttrib();															//	go back to previous attributes
	} // Display()

void GLUTTextButton::Mouse(int button, int state, int x, int y)						//	mouse down / up routine 
	{ // Mouse()
	if ((state == GLUT_DOWN) && ClickInButton(x, y))								//	button is down
		{ // button is down
		AcquireDragFocus(this);												//	acquire the drag focus					
		invert = true;														//	set flag to toggle visually
		glutPostRedisplay();												//	force a redisplay
		} // mouse-down in the button proper
	else if ((state == GLUT_UP) && (hasDragFocus))								//	if we were dragging a checkmark, and it's mouse-up
		{ // mouse-up
		ReleaseDragFocus();													//	release drag focus
		if (ClickInButton(x, y))												//	if click is in button
			if (target != NULL) target->Action(action);							//	invoke whatever action we were given
		invert = false;													//	turn off the invert for safety		
		glutPostRedisplay();												//	force a redisplay
		} // mouse-up
	} // Mouse()

void GLUTTextButton::Motion(int x, int y)										//	mouse motion routine	
	{ // Motion()
	if (ClickInButton(x, y))	invert = true; else invert = false;					//	update the invert flag
	} // Motion()
	
bool GLUTTextButton::ClickInButton(int x, int y)									//	tests whether click is in button
	{ // ClickInButton()
	x -= GLUT_TEXT_BUTTON_BOUNDARY; y -= GLUT_TEXT_BUTTON_BOUNDARY;					//	get rid of the boundary
	if ((y < 0) || (y > GLUT_TEXT_BUTTON_SIZE)) return false;						//	ignore clicks in the boundary
	if ((x < 0) || (x > GLUT_TEXT_BUTTON_SIZE + textLen)) return false;				//	same here
	if (x < GLUT_TEXT_BUTTON_RADIUS)											//	check to see whether outside curve
		if ((GLUT_TEXT_BUTTON_RADIUS - x)*(GLUT_TEXT_BUTTON_RADIUS - x) + 
			(GLUT_TEXT_BUTTON_RADIUS - y)*(GLUT_TEXT_BUTTON_RADIUS - y) > GLUT_TEXT_BUTTON_RADIUS*GLUT_TEXT_BUTTON_RADIUS)
			return false;
	if (x > GLUT_TEXT_BUTTON_RADIUS + textLen)									//	check to see whether outside curve	
		if ((GLUT_TEXT_BUTTON_RADIUS + textLen - x)*(GLUT_TEXT_BUTTON_RADIUS + textLen - x) + 
			(GLUT_TEXT_BUTTON_RADIUS - y)*(GLUT_TEXT_BUTTON_RADIUS - y) > GLUT_TEXT_BUTTON_RADIUS*GLUT_TEXT_BUTTON_RADIUS)
			return false;
	return true;
	} // ClickInButton()	

void GLUTTextButton::PrintType()												//	prints text string for type
	{ // PrintType()
	printf("GLUTTextButton");
	} // PrintType()	

