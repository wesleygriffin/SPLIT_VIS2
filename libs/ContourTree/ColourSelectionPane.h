///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	January, 2004
//
//	------------------------
//	ColourSelectionPane.h
//	------------------------
//	
//	A pane displaying the colour choices
//	
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	01/04/04		Initial version
//
///////////////////////////////////////////////////

#ifndef _HEADER_COLOUR_SELECTION_PANE
#define _HEADER_COLOUR_SELECTION_PANE

#include "GLUTPane.h"													//	the GLUTPane class

class ColourSelectionPane : public GLUTPane									//	pane with colour choices
	{ // class ColourSelectionPane
	private:
	GLUTPane *target;													//	the pane that gets invoked
	long action;														//	code to pass to target
	long *targetInteger;												//	the variable that stores the "current" colour
	
	public:	
	ColourSelectionPane(GLUTPane *Parent, GLUTPane *Target, long Action, long *TargetInteger);
																	//	constructor 
	~ColourSelectionPane();												//	destructor
																	
	virtual void Display();												//	display
	virtual void Mouse(int button, int state, int x, int y);					//	mouse down / up routine 
	virtual void PrintType();											//	prints text string for type
	}; // class ColourSelectionPane

#endif
