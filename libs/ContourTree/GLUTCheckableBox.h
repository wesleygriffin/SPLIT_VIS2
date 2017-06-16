///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTCheckableBox.h
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

#ifndef _HEADER_GLUTCheckableBox
#define _HEADER_GLUTCheckableBox

#include <stdlib.h>															//	needed for NULL
#include "GLUTPane.h"

#define GLUT_CHECKABLE_BOX_SIZE 12
#define GLUT_CHECKABLE_BOX_BOUNDARY 4
#define GLUT_CHECKABLE_BOX_TOTAL_SIZE 20

class GLUTCheckableBox : public GLUTPane										//	class to draw the actual box
	{ // class GLUTCheckableBox
	public:
	bool checked;															//	flag for whether it is currently checked
	bool dragCheck;														//	between mouse-down & mouse-up in the box
	bool *target;															//	pointer to a variable elsewhere to update
	bool *dirty;															//	pointer to flag marking whether target was updated
	
	GLUTCheckableBox(GLUTPane *Parent, bool *Target = NULL, bool *Dirty = NULL, bool initiallyChecked = false);
																		//	constructor
	~GLUTCheckableBox();													//	destructor

	virtual void Display();													//	display
	virtual void Mouse(int button, int state, int x, int y);						//	mouse down / up routine 
	virtual void Motion(int x, int y);											//	mouse motion routine	
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTCheckableBox

#endif
