///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	March, 2002
//
//	------------------------
//	ContourTreePane.h
//	------------------------
//	
//	A pane with the contour tree laid out in it
//	
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	03/27/01		Initial version
//
///////////////////////////////////////////////////

#ifndef _HEADER_ContourTreePane
#define _HEADER_ContourTreePane

#include "GLUTPane.h"													//	the GLUTPane class
#include "HeightField.h"													//	the HeightField class
#include "IsosurfacePane.h"

class ContourTreePane : public GLUTPane										//	pane with a contour tree in it
	{ // class ContourTreePane
	public:	
	HeightField *field;													//	the height field
	bool showTree;														//	whether to show tree
	long nodeID;														//	node ID for dragging
	float *isovalue;													//	pointer to where the isovalue is tracked
	bool *isovalueChanged;												//	and pointer to a flag to indicate changes
  bool useColouredTree;                       //  flag for whether tree edges are coloured
	IsosurfacePane *theIsoPane;											//	the corresponding isosurface pane
	
	ContourTreePane(GLUTPane *Parent, int Width, int Height, HeightField *newField, 
		float *Isovalue, bool *IsovalueChanged, IsosurfacePane *TheIsoPane, bool GrowHoriz = false, bool GrowVert = false);
																	//	constructor 
	~ContourTreePane();													//	destructor
																	
	virtual void Display();												//	display routine																	
	
	virtual void Mouse(int button, int state, int x, int y);					//	mouse down / up routine 
	virtual void Motion(int x, int y);										//	mouse motion routine	
	}; // class ContourTreePane

#endif
