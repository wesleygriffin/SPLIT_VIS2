///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTArcBall.h
//	------------------------
//	
//	An arcball
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
//	06/07/01		Added code to allow ball to be manipulated
//				indirectly (allows manipulation from a pane)
//
///////////////////////////////////////////////////	

#ifndef _HEADER_GLUTArcBall
#define _HEADER_GLUTArcBall

#include <stdlib.h>															//	needed for NULL
#include "GLUTPane.h"
#include "Ball.h"

class GLUTArcBall : public GLUTPane											//	class to draw the actual box
	{ // class GLUTArcBall
	public:
	float *target;															//	pointer to a float[16] to write to
	bool *dirty;															//	pointer to flag marking whether target was updated
	BallData theBall;														//	the actual ball data
	
	GLUTArcBall(GLUTPane *Parent, float *Target = NULL, bool *Dirty = NULL);			//	constructor
	~GLUTArcBall();														//	destructor

	void BeginDrag(float x, float y);											//	routine to pass through drag to arcball
	void ContinueDrag(float x, float y);										//	ditto for actual dragging (scaled)
	void EndDrag();														//	routine to end dragging

	virtual void Display();													//	display
	virtual void Mouse(int button, int state, int x, int y);						//	mouse down / up routine 
	virtual void Motion(int x, int y);											//	mouse motion routine	
	virtual void PrintType();												//	prints text string for type
	}; // class GLUTArcBall

#endif
