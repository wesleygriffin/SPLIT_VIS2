///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTCheckBox.cpp
//	------------------------
//	
//	A checkbox with text
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

#include "GLUTCheckBox.h"
#include "GLUTString.h"
#include <stdio.h>

GLUTCheckBox::GLUTCheckBox(GLUTPane *Parent, char *text, bool *Target, bool *Dirty, bool initiallyChecked)
	: GLUTHorizWhitePane(Parent, GLUT_CHECKABLE_BOX_TOTAL_SIZE + StringLength(text) + 2 * STATIC_TEXT_BOUNDARY,
		(GLUT_CHECKABLE_BOX_TOTAL_SIZE > (2 * STATIC_TEXT_BOUNDARY + StringHeight(text))) ? GLUT_CHECKABLE_BOX_TOTAL_SIZE : 
		(2 * STATIC_TEXT_BOUNDARY + StringHeight(text)), true, false, ALIGN_VERTICAL_CENTRE) 
	{ // GLUTCheckBox()
//	AddChild(new GLUTCheckableBox(this, Target, Dirty, initiallyChecked));		//	create the box
//	AddChild(new GLUTStaticText(this, text));								//	and the text label for it
	new GLUTCheckableBox(this, Target, Dirty, initiallyChecked);				//	create the box
	new GLUTStaticText(this, text);										//	and the text label for it
	} // GLUTCheckBox()
	
void GLUTCheckBox::PrintType()											//	prints text string for type
	{ // PrintType()
	printf("GLUTCheckBox");
	} // PrintType()	
