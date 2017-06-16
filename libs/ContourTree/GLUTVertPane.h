///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTVertPane.h
//	------------------------
//	
//	A pane that stacks subpanes vertically
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

#ifndef _HEADER_GLUTVertPane
#define _HEADER_GLUTVertPane

#include "GLUTPane.h"														//	we need the base class

#define ALIGN_HORIZONTAL_CENTRE 0
#define ALIGN_HORIZONTAL_LEFT 1
#define ALIGN_HORIZONTAL_RIGHT 2

class GLUTVertPane	: public GLUTPane											//	class for vertical stacking of subpanes
	{ // class GLUTHorizPane
	public:
	int alignment;															//	how to align elements horizontally
	
	GLUTVertPane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz = true, bool GrowVert = true,
			int Alignment = ALIGN_HORIZONTAL_CENTRE);							//	constructor
		
	virtual void Reshape(int Width, int Height);									//	window reshape routine
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTHorizPane

class GLUTVertWhitePane : public GLUTVertPane									//	version that clears to white for placing controls
	{ // class GLUTVertWhitePane
	public:
	GLUTVertWhitePane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz = true, bool GrowVert = true,	
			int Alignment = ALIGN_HORIZONTAL_CENTRE);
																		//	constructor
		
	virtual void Display();													//	display
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTVertWhitePane	
#endif
