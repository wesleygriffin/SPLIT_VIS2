///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTVertSlider.cpp
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
//	01/12/01	Initial version
//
///////////////////////////////////////////////////	

#include <stdlib.h>															//	needed for NULL
#include <stdio.h>															//	for sprintf()
#include <GL/freeglut.h>															//	and GLUT
#include "GLUTVertSlider.h"
#include "GLUTString.h"

#define VERT_SLIDER_BOUNDARY 5
#define VERT_SLIDER_THUMB_OVERHANG 5
#define VERT_SLIDER_THUMB_HEIGHT (2*VERT_SLIDER_THUMB_OVERHANG)
#define VERT_SLIDER_THUMB_WIDTH 10
#define VERT_SLIDER_TICK_WIDTH 3
#define VERT_SLIDER_TICK_LENGTH 5
#define VERT_SLIDER_TEXT_WIDTH 0

GLUTVertSlider::GLUTVertSlider(GLUTPane *Parent, float MinValue, float MaxValue, int NSteps, 
		float initialValue, float *Target, bool *Dirty)
	: GLUTPane(Parent, 	2 * VERT_SLIDER_BOUNDARY + VERT_SLIDER_THUMB_WIDTH + VERT_SLIDER_TICK_WIDTH 
					+ VERT_SLIDER_TEXT_WIDTH,
					NSteps + 2 * VERT_SLIDER_BOUNDARY + VERT_SLIDER_THUMB_HEIGHT+ 1, 
		 			false, false)
	{ // constructor
	minValue = MinValue; maxValue = MaxValue; nSteps = NSteps;						//	copy the parameters
	float currentValue = initialValue;												//	compute the initial value
	if (currentValue < minValue) currentValue = minValue;								//	clamp at minimum
	if (currentValue > maxValue) currentValue = maxValue;								//	and at maximum

	delta = (maxValue - minValue) / nSteps;										//	compute the "size" of a step
	thumbLocation = int(0.5 + (currentValue - minValue) / delta);							//	compute thumb location
	if (thumbLocation == nSteps) currentValue = maxValue; 							//	avoid round-off error at top end
	else currentValue = minValue + thumbLocation * delta;								//	otherwise compute current value

	target = Target; if (target != NULL) *target = currentValue;							//	set the target
	dirty = Dirty; if (dirty != NULL) *dirty = true;										//	and the dirty flag
	} // constructor

GLUTVertSlider::~GLUTVertSlider()												//	destructor
	{ // destructor
	} // destructor

void GLUTVertSlider::Display()													//	display
	{ // Display()
	GLfloat sliderColour[3] = { 0.0, 0.0, 0.0 };										//	we want to draw in black

	glMatrixMode(GL_PROJECTION);											//	set the projection matrix
	glLoadIdentity();														//	revert to identity matrix
	glOrtho(left, left + width, bottom, bottom + height, -1.0, 1.0);							//	set projection to match viewport

//	printf("Height of slider: %ld\n", height);

	float currentValue = *target; 												//	retrieve current value
	if (currentValue < minValue) currentValue = minValue;								//	and clamp
	if (currentValue > maxValue) currentValue = maxValue;

	thumbLocation = (int) ((currentValue - minValue) / delta);							//	compute thumb location

	int sliderBottom= bottom + VERT_SLIDER_BOUNDARY + VERT_SLIDER_THUMB_OVERHANG;//	compute bottom end of slider
	int sliderTop = sliderBottom + nSteps;											//	and top end
	int sliderLeftLine = left + VERT_SLIDER_BOUNDARY + VERT_SLIDER_TICK_WIDTH + VERT_SLIDER_TEXT_WIDTH + 4;
																		//	compute position of vertical lines
	int sliderRightLine = sliderLeftLine + 3; 				

	int thumbBottom = thumbLocation + bottom;									//	compute location of bottom edge
	int thumbTop = thumbBottom + VERT_SLIDER_THUMB_HEIGHT;					//	and top edge 
	int thumbLeft = left + VERT_SLIDER_BOUNDARY + VERT_SLIDER_TICK_WIDTH + VERT_SLIDER_TEXT_WIDTH;
	int thumbRight = thumbLeft + VERT_SLIDER_THUMB_WIDTH;
	
	glColor3fv(sliderColour);													//	switch to box colour
	if (thumbLocation > VERT_SLIDER_THUMB_OVERHANG)							//	if it doesn't overlap at bottom
		{ // no bottom overlap
		glBegin(GL_LINE_STRIP);												//	use a strip of lines
		glVertex2i(sliderRightLine, thumbBottom);	glVertex2i(sliderRightLine, sliderBottom);
		glVertex2i(sliderLeftLine, sliderBottom);	glVertex2i(sliderLeftLine, thumbBottom);
		glEnd();
		} // no bottom overlap
		
	if (thumbLocation + VERT_SLIDER_THUMB_OVERHANG < nSteps)					//	if it doesn't overlap at top
		{ // no top overlap
		glBegin(GL_LINE_STRIP);												//	use a strip of lines
		glVertex2i(sliderRightLine, thumbTop);	glVertex2i(sliderRightLine, sliderTop);	
		glVertex2i(sliderLeftLine, sliderTop);		glVertex2i(sliderLeftLine, thumbTop);
		glEnd();
		} // no top overlap

	glBegin(GL_LINE_LOOP);													//	use a strip of lines
	glVertex2i(thumbLeft, thumbBottom);		glVertex2i(thumbRight, thumbBottom);
	glVertex2i(thumbRight, thumbTop);		glVertex2i(thumbLeft, thumbTop);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(thumbLeft + 2, thumbBottom + 3);	glVertex2i(thumbRight -2, thumbBottom + 3);
	glVertex2i(thumbLeft + 2, thumbBottom + 5);	glVertex2i(thumbRight -2, thumbBottom + 5);
	glVertex2i(thumbLeft + 2, thumbBottom + 7);	glVertex2i(thumbRight -2, thumbBottom + 7);
	glEnd();
	
	int tickLeft = sliderRightLine;
	int tickRight = tickLeft + VERT_SLIDER_TICK_LENGTH;
	glBegin(GL_LINES);
	for (int i = 0; i <= 10; i++)
		{ // loop through ticks
		int tickLocation = bottom + VERT_SLIDER_THUMB_OVERHANG + VERT_SLIDER_BOUNDARY + (i*nSteps) / 10;
		if ((tickLocation >= thumbBottom - 1) && (tickLocation <= thumbTop + 1))
			continue;
		glVertex2i(tickLeft, tickLocation);
		glVertex2i(tickRight, tickLocation);
		} // loop through ticks
	glEnd();
	
	int stringLeft = left + VERT_SLIDER_BOUNDARY;
	char labelString[10];
	sprintf(labelString, "%5.3f", minValue);
	int stringBottom = bottom + VERT_SLIDER_BOUNDARY + VERT_SLIDER_THUMB_OVERHANG - StringLength(labelString) / 2;
	if (stringLeft < 0) stringLeft = 0;
	glRasterPos2i(stringLeft, stringBottom);
	PrintString(labelString);

	sprintf(labelString, "%5.2f", minValue + (float)((int)(nSteps / 2) * delta));
	stringLeft = left + VERT_SLIDER_BOUNDARY + VERT_SLIDER_THUMB_OVERHANG + nSteps / 2 - StringLength(labelString) / 2;
	glRasterPos2i(stringLeft, stringBottom);
	PrintString(labelString);

	sprintf(labelString, "%4.2f", maxValue);
	stringLeft = left + VERT_SLIDER_BOUNDARY + VERT_SLIDER_THUMB_OVERHANG + nSteps - StringLength(labelString) / 2;
	glRasterPos2i(stringLeft, stringBottom);
	PrintString(labelString);
	
	} // Display()

void GLUTVertSlider::Mouse(int button, int state, int x, int y)								//	mouse down / up routine 
	{ // Mouse()
	if (state == GLUT_DOWN)													//	if anywhere in the pane
		{ // mouse-down 
		AcquireDragFocus(this);												//	acquire focus
		Motion(x, y);														//	snap to it
		} // mouse-down 
	else if ((state == GLUT_UP) && (hasDragFocus))									//	if we were dragging a checkmark, and it's mouse-up
		{ // mouse-up when dragging checkmark
		ReleaseDragFocus();													//	release drag focus
		} // mouse-up when dragging checkmark
	} // Mouse()
	
void GLUTVertSlider::Motion(int x, int y)											//	mouse motion routine	
	{ // Motion()
//	printf("X, Y: %ld, %ld\n", x, y);
//	x = x - VERT_SLIDER_BOUNDARY - VERT_SLIDER_THUMB_OVERHANG;			//	snap to useful range
//	y = y - VERT_SLIDER_BOUNDARY;											//	in both dimensions
	y = y - VERT_SLIDER_THUMB_OVERHANG;
//	printf("X, Y: %ld, %ld\n", x, y);
	thumbLocation = y;														//	reset the thumb location
	if (thumbLocation < 0) thumbLocation = 0;										//	clamp left
	if (thumbLocation > nSteps) thumbLocation = nSteps;								//	clamp right

	float currentValue;	
	if (thumbLocation == nSteps) currentValue = maxValue;								//	avoid round-off errors at top end
	else currentValue = minValue + (float)(delta * thumbLocation);
	
//	printf("thumb is at %ld: nSteps = %ld, maxValue = %f, minValue = %f, delta = %f, value = %f\n",
//			thumbLocation, nSteps, maxValue, minValue, delta, currentValue);
			
	if (target != NULL)														//	if there is a target
		*target = currentValue;												//	update the associated variable
	if (dirty != NULL)														//	if there is a dirty flag
		*dirty = true;														//	set it		
	glutPostRedisplay();														//	force a redisplay
	} // Motion()

void GLUTVertSlider::PrintType()												//	prints text string for type
	{ // PrintType()
	printf("GLUTVertSlider");
	} // PrintType()	
