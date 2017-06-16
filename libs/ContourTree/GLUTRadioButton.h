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
//	A radio button
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

#ifndef _HEADER_GLUTRadioButton
#define _HEADER_GLUTRadioButton

#include <stdlib.h>															//	needed for NULL
#include "GLUTPane.h"	

class GLUTRadioButton;														//	forward declaration

#include "GLUTRadioGroup.h"													//	let it know about radio groups

#define GLUT_RADIO_BUTTON_SIZE 12
#define GLUT_RADIO_BUTTON_BOUNDARY 4
#define GLUT_RADIO_BUTTON_TOTAL_SIZE 20

class GLUTRadioButton : public GLUTPane											//	a radio button class
	{ // class GLUTRadioButton
	public:
	bool chosen;															//	flag for whether it is currently checked
	bool dragCheck;														//	between mouse-down & mouse-up in the box
	int buttonValue;														//	value of the button
	GLUTRadioGroup *theRadioGroup;											//	the radio group to which it belongs
	
	GLUTRadioButton(GLUTPane *Parent, GLUTRadioGroup *theGroup, int ButtonValue, bool initiallyChosen = false);
																		//	constructor
	~GLUTRadioButton();														//	destructor

	virtual void Display();													//	display
	virtual void Mouse(int button, int state, int x, int y);						//	mouse down / up routine 
	virtual void Motion(int x, int y);											//	mouse motion routine	
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTRadioButton

#endif
