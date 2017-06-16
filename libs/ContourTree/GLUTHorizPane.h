///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTHorizPane.h
//	------------------------
//	
//	A pane that stacks subpanes horizontally
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

#ifndef _HEADER_GLUTHorizPane
#define _HEADER_GLUTHorizPane

#include "GLUTPane.h"														//	we need the base class

#define ALIGN_VERTICAL_CENTRE 0
#define ALIGN_VERTICAL_TOP 1
#define ALIGN_VERTICAL_BOTTOM 2

class GLUTHorizPane	: public GLUTPane											//	class for horizontal stacking of subpanes
	{ // class GLUTHorizPane
	public:
	int alignment;															//	the vertical alignment policy
	
	GLUTHorizPane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz = true, bool GrowVert = true,	
			int Alignment = ALIGN_VERTICAL_CENTRE);
																		//	constructor
		
	virtual void Reshape(int Width, int Height);									//	window reshape routine
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTHorizPane
	
class GLUTHorizWhitePane : public GLUTHorizPane									//	version that clears to white for placing controls
	{ // class GLUTHorizWhitePane
	public:
	GLUTHorizWhitePane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz = true, bool GrowVert = true,	
			int Alignment = ALIGN_VERTICAL_CENTRE);
																		//	constructor
		
	virtual void Display();													//	display
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTHorizWhitePane	
	
#endif
