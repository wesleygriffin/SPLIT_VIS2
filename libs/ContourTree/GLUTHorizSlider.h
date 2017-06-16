///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTHorizSlider.h
//	------------------------
//	
//	A horizontal slider
//
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	02/09/01		Initial version
//
///////////////////////////////////////////////////	

#ifndef _HEADER_GLUTHorizSlider
#define _HEADER_GLUTHorizSlider

#include <stdlib.h>															//	needed for NULL
#include "GLUTPane.h"

class GLUTHorizSlider : public GLUTPane											//	class to draw the actual box
	{ // class GLUTHorizSlider
	public:
	float *target;															//	pointer to target's value
	bool *dirty;															//	pointer to flag marking whether target was updated
	float minValue, maxValue;												//	min. & max values of the slider
	float delta;															//	smallest possible increment
	int nSteps;															//	# of steps required
	int thumbLocation;														//	position of thumb: original, and during drag
	
	GLUTHorizSlider(GLUTPane *Parent, float MinValue = 0, float MaxValue = 100, int NSteps = 100, 
		float initialValue = 50, float *Target = NULL, bool *Dirty = NULL);
	~GLUTHorizSlider();														//	destructor

	virtual void Display();													//	display
	virtual void Mouse(int button, int state, int x, int y);						//	mouse down / up routine 
	virtual void Motion(int x, int y);											//	mouse motion routine	
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTHorizSlider

#endif
