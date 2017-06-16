///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTCheckBox.h
//	------------------------
//	
//	A CheckBox with text
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

#ifndef _HEADER_GLUTCheckBox
#define _HEADER_GLUTCheckBox

#include "GLUTHorizPane.h"
#include "GLUTCheckableBox.h"
#include "GLUTStaticText.h"

class GLUTCheckBox : public GLUTHorizWhitePane									//	class for a CheckBox with string
	{ // class GLUTCheckBox
	public:
	GLUTCheckBox(GLUTPane *Parent, char *text, bool *Target = NULL, bool *Dirty = NULL, bool initiallyChecked = false);
																		//	constructor
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTCheckBox

#endif
