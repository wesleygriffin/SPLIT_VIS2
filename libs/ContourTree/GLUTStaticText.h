///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTStaticText.h
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

#ifndef _HEADER_GLUTStaticText
#define _HEADER_GLUTStaticText

#include "GLUTPane.h"														//	we need the base class
#define STATIC_TEXT_BOUNDARY 5												//	put a 5 pixel boundary around text

class GLUTStaticText	: public GLUTPane										//	class for a static piece of text
	{ // class GLUTStaticText
	public:
	char *theText;															//	the text in this pane
	
	GLUTStaticText(GLUTPane *Parent, char *text);								//	constructor: always fixed size
	~GLUTStaticText();														//	destructor

	virtual void Display();													//	displays the text
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTStaticText
#endif
