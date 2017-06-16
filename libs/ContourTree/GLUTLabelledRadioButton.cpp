///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	---------------------------
//	GLUTLabelledRadioButton.cpp
//	---------------------------
//	
//	A LabelledRadioButton with text
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

#include "GLUTLabelledRadioButton.h"
#include "GLUTString.h"
#include <stdio.h>

GLUTLabelledRadioButton::GLUTLabelledRadioButton(GLUTRadioGroup *Parent, char *text, int ButtonValue, bool initiallyChosen)
	: GLUTHorizPane(Parent, GLUT_RADIO_BUTTON_TOTAL_SIZE + StringLength(text) + 2 * STATIC_TEXT_BOUNDARY,
		(GLUT_RADIO_BUTTON_TOTAL_SIZE > (2 * STATIC_TEXT_BOUNDARY + StringHeight(text))) ? GLUT_RADIO_BUTTON_TOTAL_SIZE : 
		(2 * STATIC_TEXT_BOUNDARY + StringHeight(text)), true, false, ALIGN_VERTICAL_CENTRE)
	{ // GLUTLabelledRadioButton()
//	AddChild(new GLUTRadioButton(this, Parent, ButtonValue, initiallyChosen));			//	create the button
//	AddChild(new GLUTStaticText(this, text));									//	and the text label for it
	theButton = new GLUTRadioButton(this, Parent, ButtonValue, initiallyChosen);					//	create the button
	new GLUTStaticText(this, text);											//	and the text label for it
	} // GLUTLabelledRadioButton()
	
void GLUTLabelledRadioButton::PrintType()										//	prints text string for type
	{ // PrintType()
	printf("GLUTLabelledRadioButton");
	} // PrintType()	
	
