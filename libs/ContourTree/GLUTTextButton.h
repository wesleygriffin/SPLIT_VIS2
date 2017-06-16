///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTTextButton.h
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

#ifndef _HEADER_GLUTTextButton
#define _HEADER_GLUTTextButton

#include <stdlib.h>															//	needed for NULL
#include "GLUTPane.h"	

#define GLUT_TEXT_BUTTON_CORNER_INSET 4
#define GLUT_TEXT_BUTTON_RADIUS 8
#define GLUT_TEXT_BUTTON_SIZE (2*GLUT_TEXT_BUTTON_RADIUS)							//	i.e. the height
#define GLUT_TEXT_BUTTON_BOUNDARY 2

class GLUTTextButton : public GLUTPane											//	a radio button class
	{ // class GLUTTextButton
	public:
	GLUTPane *target;														//	the pane that gets invoked
	char *text;															//	the text that goes into it
	bool invert;															//	whether to invert the colours
	int textLen;															//	text length
	int action;															//	code to pass to target

	GLUTTextButton(GLUTPane *Parent, char *Text, GLUTPane *Target = NULL, int Action = 0);
																					//	constructor
	~GLUTTextButton();														//	destructor

	virtual void Display();													//	display
	virtual void Mouse(int button, int state, int x, int y);						//	mouse down / up routine 
	virtual void Motion(int x, int y);											//	mouse motion routine	
	bool ClickInButton(int x, int y);											//	tests whether a click is inside
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTTextButton

#endif
