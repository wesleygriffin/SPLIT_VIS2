///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTVertSlider.h
//	------------------------
//	
//	A vertical slider
//
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	01/12/02	Initial version
//
///////////////////////////////////////////////////	

#ifndef _HEADER_GLUTVertSlider
#define _HEADER_GLUTVertSlider

#include <stdlib.h>															//	needed for NULL
#include "GLUTPane.h"

class GLUTVertSlider : public GLUTPane											//	class to draw the actual box
	{ // class GLUTVertSlider
	public:
	float *target;															//	pointer to target's value
	bool *dirty;															//	pointer to flag marking whether target was updated
	float minValue, maxValue;													//	min. & max values of the slider
	float delta;																//	smallest possible increment
	int nSteps;																//	# of steps required
	int thumbLocation;														//	position of thumb: original, and during drag
	
	GLUTVertSlider(GLUTPane *Parent, float MinValue = 0, float MaxValue = 100, int NSteps = 100, 
		float initialValue = 50, float *Target = NULL, bool *Dirty = NULL);
	~GLUTVertSlider();														//	destructor

	virtual void Display();														//	display
	virtual void Mouse(int button, int state, int x, int y);								//	mouse down / up routine 
	virtual void Motion(int x, int y);												//	mouse motion routine	
	virtual void PrintType();													//	prints text string for type
	}; // class GLUTVertSlider

#endif
