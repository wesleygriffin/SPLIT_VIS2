///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTLabelledRadioButton.h
//	------------------------
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

#ifndef _HEADER_GLUTLabelledRadioButton
#define _HEADER_GLUTLabelledRadioButton

class GLUTLabelledRadioButton;												//	forward class declaration

#include "GLUTHorizPane.h"
#include "GLUTRadioButton.h"
#include "GLUTStaticText.h"

class GLUTLabelledRadioButton : public GLUTHorizPane								//	class for a LabelledRadioButton with string
	{ // class GLUTLabelledRadioButton
	public:
  GLUTRadioButton *theButton;
	GLUTLabelledRadioButton(GLUTRadioGroup *Parent, char *text, int ButtonValue, bool initiallyChosen = false);
																		//	constructor
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTLabelledRadioButton

#endif
