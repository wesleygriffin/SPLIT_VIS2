///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTHorizSlider.cpp
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

#include <stdlib.h>															//	needed for NULL
#include <stdio.h>															//	for sprintf()
#include <GL/freeglut.h>
#include "GLUTHorizSlider.h"
#include "GLUTString.h"

#define HORIZ_SLIDER_BOUNDARY 5
#define HORIZ_SLIDER_THUMB_OVERHANG 5
#define HORIZ_SLIDER_THUMB_WIDTH (2*HORIZ_SLIDER_THUMB_OVERHANG)
#define HORIZ_SLIDER_THUMB_HEIGHT 10
#define HORIZ_SLIDER_TICK_HEIGHT 3
#define HORIZ_SLIDER_TICK_LENGTH 5
//#define HORIZ_SLIDER_TEXT_HEIGHT 12
#define HORIZ_SLIDER_TEXT_HEIGHT 0

GLUTHorizSlider::GLUTHorizSlider(GLUTPane *Parent, float MinValue, float MaxValue, int NSteps, 
		float initialValue, float *Target, bool *Dirty)
	: GLUTPane(Parent, NSteps + 2 * HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_THUMB_WIDTH + 1, 
		2 * HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_THUMB_HEIGHT + HORIZ_SLIDER_TICK_HEIGHT + HORIZ_SLIDER_TEXT_HEIGHT, 
		false, false)
	{ // constructor
	minValue = MinValue; maxValue = MaxValue; nSteps = NSteps;						//	copy the parameters
	float currentValue = initialValue;										//	compute the initial value
	if (currentValue < minValue) currentValue = minValue;							//	clamp at minimum
	if (currentValue > maxValue) currentValue = maxValue;							//	and at maximum

	delta = (maxValue - minValue) / nSteps;										//	compute the "size" of a step
	thumbLocation = int(0.5 + (currentValue - minValue) / delta);					//	compute thumb location
	if (thumbLocation == nSteps) currentValue = maxValue; 							//	avoid round-off error at top end
	else currentValue = minValue + thumbLocation * delta;							//	otherwise compute current value

	target = Target; if (target != NULL) *target = currentValue;					//	set the target
	dirty = Dirty; if (dirty != NULL) *dirty = true;								//	and the dirty flag
	} // constructor

GLUTHorizSlider::~GLUTHorizSlider()											//	destructor
	{ // destructor
	} // destructor

void GLUTHorizSlider::Display()												//	display
	{ // Display()
	GLfloat sliderColour[3] = { 0.0, 0.0, 0.0 };									//	we want to draw in black

	glMatrixMode(GL_PROJECTION);												//	set the projection matrix
	glLoadIdentity();														//	revert to identity matrix
	glOrtho(left, left + width, bottom, bottom + height, -1.0, 1.0);					//	set projection to match viewport

	float currentValue = *target; 											//	retrieve current value
	if (currentValue < minValue) currentValue = minValue;							//	and clamp
	if (currentValue > maxValue) currentValue = maxValue;

	thumbLocation = (int) ((currentValue - minValue) / delta);						//	compute thumb location

	int sliderLeft = left + HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_THUMB_OVERHANG;		//	compute left end of slider
	int sliderRight = sliderLeft + nSteps;										//	and right end
	int sliderBottomLine = bottom + HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_TICK_HEIGHT + HORIZ_SLIDER_TEXT_HEIGHT + 4;
																		//	compute position of horizontal lines
	int sliderTopLine = sliderBottomLine + 2; 				

	int thumbLeft = thumbLocation + left;										//	compute location of left-hand edge
	int thumbRight = thumbLeft + HORIZ_SLIDER_THUMB_WIDTH;							//	and right-hand edge 
	int thumbBottom = bottom + HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_TICK_HEIGHT + HORIZ_SLIDER_TEXT_HEIGHT;
	int thumbTop = thumbBottom + HORIZ_SLIDER_THUMB_HEIGHT;
	
	glColor3fv(sliderColour);												//	switch to box colour
	if (thumbLocation > HORIZ_SLIDER_THUMB_OVERHANG)								//	if it doesn't overlap at left
		{ // no left overlap
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_LINE_STRIP);												//	use a strip of lines
		glVertex2i(thumbLeft, sliderTopLine);		glVertex2i(sliderLeft, sliderTopLine);
		glVertex2i(sliderLeft, sliderBottomLine);	glVertex2i(thumbLeft, sliderBottomLine);
		//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
		glEnd();
		} // no left overlap
		
	if (thumbLocation + HORIZ_SLIDER_THUMB_OVERHANG < nSteps)						//	if it doesn't overlap at right
		{ // no right overlap
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_LINE_STRIP);												//	use a strip of lines
		glVertex2i(thumbRight, sliderTopLine); 		glVertex2i(sliderRight, sliderTopLine);	
		glVertex2i(sliderRight, sliderBottomLine);	glVertex2i(thumbRight, sliderBottomLine);
		//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
		glEnd();
		} // no right overlap

	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
	glBegin(GL_LINE_LOOP);													//	use a strip of lines
	glVertex2i(thumbLeft, thumbBottom);	glVertex2i(thumbRight, thumbBottom);
	glVertex2i(thumbRight, thumbTop);		glVertex2i(thumbLeft, thumbTop);
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();

	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
	glBegin(GL_LINES);
	glVertex2i(thumbLeft + 3, thumbBottom + 2);	glVertex2i(thumbLeft + 3, thumbTop - 1);
	glVertex2i(thumbLeft + 5, thumbBottom + 2);	glVertex2i(thumbLeft + 5, thumbTop - 1);
	glVertex2i(thumbLeft + 7, thumbBottom + 2);	glVertex2i(thumbLeft + 7, thumbTop - 1);
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();
	
	int tickBottom = bottom + HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_TEXT_HEIGHT;
	int tickTop = tickBottom + HORIZ_SLIDER_TICK_LENGTH;
//	printf("TickBottom: %ld tickTop: %ld\n", tickBottom, tickTop);
	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
	glBegin(GL_LINES);
	for (int i = 0; i <= 10; i++)
		{ // loop through ticks
		int tickLocation = left + HORIZ_SLIDER_THUMB_OVERHANG + HORIZ_SLIDER_BOUNDARY + (i*nSteps) / 10;
		if ((tickLocation >= thumbLeft - 1) && (tickLocation <= thumbRight + 1))
			continue;
		glVertex2i(tickLocation, tickBottom);
		glVertex2i(tickLocation, tickTop);
		} // loop through ticks
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();
	
//	int stringBottom = bottom + HORIZ_SLIDER_BOUNDARY;
//	char labelString[10];
//	sprintf(labelString, "%5.3f", minValue);
//	int stringLeft = left + HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_THUMB_OVERHANG - StringLength(labelString) / 2;
//	if (stringLeft < 0) stringLeft = 0;
//	glRasterPos2i(stringLeft, stringBottom);
//	PrintString(labelString);
//
//	sprintf(labelString, "%5.2f", minValue + (float)((int)(nSteps / 2) * delta));
//	stringLeft = left + HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_THUMB_OVERHANG + nSteps / 2 - StringLength(labelString) / 2;
//	glRasterPos2i(stringLeft, stringBottom);
//	PrintString(labelString);
//
//	sprintf(labelString, "%4.2f", maxValue);
//	stringLeft = left + HORIZ_SLIDER_BOUNDARY + HORIZ_SLIDER_THUMB_OVERHANG + nSteps - StringLength(labelString) / 2;
//	glRasterPos2i(stringLeft, stringBottom);
//	PrintString(labelString);
	
	} // Display()

void GLUTHorizSlider::Mouse(int button, int state, int x, int y)						//	mouse down / up routine 
	{ // Mouse()
	if (state == GLUT_DOWN)													//	if anywhere in the pane
		{ // mouse-down 
		AcquireDragFocus(this);												//	acquire focus
		Motion(x, y);														//	snap to it
		} // mouse-down 
	else if ((state == GLUT_UP) && (hasDragFocus))								//	if we were dragging a checkmark, and it's mouse-up
		{ // mouse-up when dragging checkmark
		ReleaseDragFocus();													//	release drag focus
		} // mouse-up when dragging checkmark
	} // Mouse()
	
void GLUTHorizSlider::Motion(int x, int y)										//	mouse motion routine	
	{ // Motion()
//	printf("X, Y: %ld, %ld\n", x, y);
//	x = x - HORIZ_SLIDER_BOUNDARY - HORIZ_SLIDER_THUMB_OVERHANG;					//	snap to useful range
//	y = y - HORIZ_SLIDER_BOUNDARY;											//	in both dimensions
	x = x - HORIZ_SLIDER_THUMB_OVERHANG;
//	printf("X, Y: %ld, %ld\n", x, y);
	thumbLocation = x;														//	reset the thumb location
	if (thumbLocation < 0) thumbLocation = 0;									//	clamp left
	if (thumbLocation > nSteps) thumbLocation = nSteps;							//	clamp right

	float currentValue;
	if (thumbLocation == nSteps) currentValue = maxValue;							//	avoid round-off errors at top end
	else currentValue = minValue + (float)(delta * thumbLocation);
	
//	printf("thumb is at %ld: nSteps = %ld, maxValue = %f, minValue = %f, delta = %f, value = %f\n",
//			thumbLocation, nSteps, maxValue, minValue, delta, currentValue);
			
	if (target != NULL)														//	if there is a target
		*target = currentValue;												//	update the associated variable
	if (dirty != NULL)														//	if there is a dirty flag
		*dirty = true;														//	set it		
	glutPostRedisplay();													//	force a redisplay
	} // Motion()

void GLUTHorizSlider::PrintType()												//	prints text string for type
	{ // PrintType()
	printf("GLUTHorizSlider");
	} // PrintType()	
