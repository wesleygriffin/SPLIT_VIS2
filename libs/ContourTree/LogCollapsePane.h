///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	August, 2003
//
//	------------------------
//	LogCollapsePane.h
//	------------------------
//	
//	A pane displaying the tree size / collapse
//	bound as a log-log plot
//	
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	08/01/03		Initial version
//
///////////////////////////////////////////////////

#ifndef _HEADER_LOG_COLLAPSE_PANE
#define _HEADER_LOG_COLLAPSE_PANE

#include "GLUTPane.h"													//	the GLUTPane class
#include "HeightField.h"													//	the HeightField class

class LogCollapsePane : public GLUTPane										//	pane with a contour tree in it
	{ // class LogCollapsePane
	public:	
	HeightField *field;													//	the height field	

	LogCollapsePane(GLUTPane *Parent, int Width, int Height, HeightField *newField, 
		bool GrowHoriz = false, bool GrowVert = false);						//	constructor 
	~LogCollapsePane();													//	destructor
																	
	virtual void Display();												//	display routine																	
	}; // class LogCollapsePane

#endif
