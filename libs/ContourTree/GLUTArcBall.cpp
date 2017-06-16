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

#include "GLUTArcBall.h"
#include <GL/freeglut.h>
#include <stdio.h>

#define ARCBALL_PANE_SIZE 50
#define ARCBALL_SIZE 40

#include "sphere.h"

GLUTArcBall::GLUTArcBall(GLUTPane *Parent, float *Target, bool *Dirty)				//	constructor
	: GLUTPane(Parent, ARCBALL_PANE_SIZE, ARCBALL_PANE_SIZE, false, false)
	{ // GLUTArcBall()
	target = Target; dirty = Dirty;											//	set the target & dirty pointers
	Ball_Init(&theBall);													//	initialize the arcball
	Ball_Place(&theBall, qOne, 0.80);											//	place the arcball (at 3/4 of window size)
	} // GLUTArcBall()

GLUTArcBall::~GLUTArcBall()													//	destructor
	{ // ~GLUTArcBall()
	} // ~GLUTArcBall()

void GLUTArcBall::Display()													//	display
	{ // Display()
	int i, j;																//	loop indices
	GLfloat backgroundColour[4] = { 1.0, 1.0, 1.0, 1.0};							//	background is white
	GLfloat ballColour[4] = { 0.0, 0.0, 0.0, 1.0 };								//	ball is in black
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);											//	save the attribute frame
	glEnable(GL_DEPTH_TEST);													//	turn depth buffer back on
	glDisable(GL_LIGHTING);													//	disable lighting
	glMatrixMode(GL_PROJECTION);												//	set the projection matrix
	glLoadIdentity();														//	revert to identity matrix
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);									//	set projection to simple projection
	GLfloat mNow[16];														//	local matrix for ball routines
	Ball_Value(&theBall, mNow);												//	retrieve the ball's position as a matrix

	glMatrixMode(GL_MODELVIEW);												//	make sure that we are set to the model matrix
	glPushMatrix();														//	push the GL stack to get a new matrix
	glLoadIdentity();														//	load the identity matrix
	glColor3fv(backgroundColour);												//	set colour for polygon (zero-plane)

	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
	glBegin(GL_POLYGON);													//	draw a polygon to hide rear edges
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, 0.0);
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();

	glColor3fv(ballColour);													//	set the colour for the sphere
	glScalef(0.8, 0.8, 0.8);
	//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
	glBegin(GL_LINE_LOOP);
	for (j = 3; j < 84; j += 7)
		glVertex3f(sphereVert[j][0], sphereVert[j][1], 0.5);
	//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
	glEnd();
	
	glMultMatrixf(mNow);													//	multiply by this matrix

	for (i = 0; i < 12; i++)
		{ // loop drawing verticals
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_LINE_STRIP);
		for (j = i*7; j < 7+i*7; j++)
			glVertex3fv(sphereVert[j]);
		//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
		glEnd();		
		} // loop drawing verticals
	for (i = 1; i < 6; i++)
		{ // loop for horizontals
		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_LINE_LOOP);
		for (j = i; j < 84; j += 7)
			glVertex3fv(sphereVert[j]);
		//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
		glEnd();
		} // loop for horizontals
	glPopMatrix();															//	revert to the old matrix
	glPopAttrib();															//	pop attributes back off stack
	} // Display()
	
void GLUTArcBall::Mouse(int button, int state, int x, int y)						//	mouse down / up routine 
	{ // Mouse()
	if (state == GLUT_DOWN)													//	if anywhere in the pane
		{ // mouse-down 
		AcquireDragFocus(this);												//	acquire focus
		BeginDrag((2.0*x - width)/width, (2.0*y - height)/height);						//	scale to arcball pane & start dragging
		} // mouse-down 
	else if ((state == GLUT_UP) && (hasDragFocus))								//	if we were dragging a checkmark, and it's mouse-up
		{ // mouse-up when dragging checkmark
		ReleaseDragFocus();													//	release drag focus
		EndDrag();															//	and call routine to tidy up
		} // mouse-up when dragging checkmark
	} // Mouse()
	
void GLUTArcBall::Motion(int x, int y)											//	mouse motion routine	
	{ // Motion()
	ContinueDrag((2.0 * x - width)/width, (2.0 * y - height)/height);					//	in both dimensions
	} // Motion()

void GLUTArcBall::BeginDrag(float x, float y)									//	routine to pass through drag to arcball
	{ // BeginDrag()
	HVect vNow;														//	vector type for passing to ball functions
	vNow.x = x; vNow.y = y;												//	set the vector
	Ball_Mouse(&theBall, vNow);											//	and pass it to the Ball functions
	Ball_BeginDrag(&theBall);												//	start dragging
	glutPostRedisplay();													//	force a redisplay
	} // Begin Drag()

void GLUTArcBall::ContinueDrag(float x, float y)									//	ditto for actual dragging (scaled)
	{ // ContinueDrag()
	HVect vNow;														//	vector type for passing to ball functions
	vNow.x = x; vNow.y = y;												//	set the vector
	Ball_Mouse(&theBall, vNow);											//	and pass it to the Ball functions
	Ball_Update(&theBall);												//	update the data for the ball
	if (target != NULL)													//	if there is a target
		Ball_Value(&theBall, target);										//	retrieve the ball's position
	if (dirty != NULL)													//	if there is a dirty flag
		*dirty = true;													//	set it		
	glutPostRedisplay();													//	force a redisplay
	} // ContinueDrag()

void GLUTArcBall::EndDrag()												//	routine to end dragging
	{ // EndDrag()
	Ball_EndDrag(&theBall);												//	stop dragging
	Ball_Update(&theBall);												//	update the data for the ball
	if (target != NULL)													//	if there is a target
		Ball_Value(&theBall, target);										//	retrieve the ball's position
	if (dirty != NULL)													//	if there is a dirty flag
		*dirty = true;													//	set it		
	glutPostRedisplay();													//	force a redisplay
	} // EndDrag()

void GLUTArcBall::PrintType()													//	prints text string for type
	{ // PrintType()
	printf("GLUTArcBall");
	} // PrintType()	
