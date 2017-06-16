///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTPane.cpp
//	------------------------
//	
//	A pane should be a drawing context, which gets invoked
//	by the window.  Controls, &c. should inherit from the
//	pane.
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

#include "GLUTPane.h"															//	the header
#include <GL/freeglut.h>																//	GLUT itself
#include <stdlib.h>																//	needed for NULL, of all things
#include <stdio.h>																//	C I/O
#define GLErrorString(x) ( ((x) == GL_NO_ERROR) ? "No Error" : ((x) == GL_INVALID_ENUM) ? "Invalid enum" : ((x) == GL_INVALID_VALUE) ? "Invalid Value" : ((x) == GL_INVALID_OPERATION) ? "Invalid Operation" : ((x) == GL_STACK_OVERFLOW) ? "Stack Overflow" : ((x) == GL_STACK_UNDERFLOW) ? "Stack Underflow" : ((x) == GL_OUT_OF_MEMORY) ? "Out of Memory" : "Unknown Error")

GLUTPane::GLUTPane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz, bool GrowVert)	//	constructor
	{ //	GLUTPane()															
//	printf("Creating pane %X with dimensions %d x %d\n", this, Width, Height);
	parent = Parent;															//	set pointer to parent
	children = NULL; nChildren = 0;												//	set child information
	
	height = Height; width = Width; 												//	set initial size
	growVert = GrowVert; growHoriz = GrowHoriz;										//	set the growth flags

	bottom = 0; left = 0;														//	set initial (default position)

	hasDragFocus = hasKeyFocus = false;											//	set flags to mark no focus
	if (parent != NULL) parent->AddChild(this);										//	automatically add to parent
	} // GLUTPane()

GLUTPane::~GLUTPane()															//	destructor
	{ // ~GLUTPane()
	for (int child = 0; child < nChildren; child++)									//	loop through children
		delete children[child];													//	deleting them
	free(children);															//	get rid of the array of children
//	if (hasDragFocus) ReleaseDragFocus();											//	release the drag focus
//	if (hasKeyFocus) ReleaseKeyboardFocus();										//	and the keyboard focus
	} // ~GLUTPane()
			
void GLUTPane::Display()															//	main display routine
	{ // Display()
//	printf("In GLUTPane::Display()\n");
	int errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%d): GL Error %d [%s] at beginning of GLUTPane::Display()\n", __FILE__, __LINE__, errNum, GLErrorString(errNum));
		}
	for (int whichChild = 0; whichChild < nChildren; whichChild++)						//	loop through children
		{ // for each child
		GLUTPane *child = children[whichChild];										//	grab a pointer to the child
//		printf("Child #%2ld\n", whichChild);
		errNum = glGetError();
		if (errNum != GL_NO_ERROR)
			{
			printf("%s(%d): GL Error %d [%s] at beginning of GLUTPane::Display(), child #%d\n", __FILE__, __LINE__, errNum, GLErrorString(errNum), whichChild);
			}
//		if (errNum == GL_STACK_OVERFLOW)
//			{
//			long paramValue[8];
//			glGetIntegerv(GL_ATTRIB_STACK_DEPTH, paramValue);
//			printf("Attribute Stack Depth: %d\n", paramValue[0]);
//			glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, paramValue);
//			printf("Maximum Attribute Stack Depth: %d\n", paramValue[0]);
//			glGetIntegerv(GL_CLIENT_ATTRIB_STACK_DEPTH, paramValue);
//			printf("Client Attribute Stack Depth: %d\n", paramValue[0]);
//			glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, paramValue);
//			printf("Maximum Client Attribute Stack Depth: %d\n", paramValue[0]);
//			glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, paramValue);
//			printf("Modelview Matrix Stack Depth: %d\n", paramValue[0]);
//			glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, paramValue);
//			printf("Maximum Modelview Matrix Stack Depth: %d\n", paramValue[0]);
//			glGetIntegerv(GL_PROJECTION_STACK_DEPTH, paramValue);
//			printf("Projection Matrix Stack Depth: %d\n", paramValue[0]);
//			glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, paramValue);
//			printf("Maximum Projection Matrix Stack Depth: %d\n", paramValue[0]);
//			}
//		glMatrixMode(GL_PROJECTION);												//	switch to projection matrix
//		glPushMatrix();														//	and provide a copy of it
		glMatrixMode(GL_MODELVIEW);												//	switch to modelview matrix
		glPushMatrix();														//	and provide a copy of it
		glViewport(child->left, child->bottom, child->width, child->height);				//	set the viewport, so that the child doesn't have to.
		child->Display();														//	and call the child to do the display
//		glMatrixMode(GL_PROJECTION);												//	switch to projection matrix
//		glPopMatrix();															//	and revert to previous copy
		glMatrixMode(GL_MODELVIEW);												//	switch to modelview matrix
		glPopMatrix();															//	and revert to previous copy
		} // for each child
	errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%d): GL Error %d [%s] at end of GLUTPane::Display()\n", __FILE__, __LINE__, errNum, GLErrorString(errNum));
		}
	} // Display()

void GLUTPane::Reshape(int Width, int Height)										//	window reshape routine
	{ // Reshape()
	//	default reshape policy is just to reset the pane's width & height: it's the parent's job to reset the position
	//	exception here is that if growVert or growHoriz is false, then the size is fixed in that dimension
	
	if (growVert) width = Width;													//	reset the width
	if (growHoriz) height = Height;												//	and the height
	} // Reshape()
	
void GLUTPane::Mouse(int button, int state, int x, int y)								//	mouse down / up routine 
	{ // Mouse()																
	//	default behaviour is to pass it to all children that the point intersects, 
	//	when passing to the child, convert coordinates to child-local coordinates first
//	printf("GLUTPane::Mouse(): %s button in state %s at (%d, %d)\n", (button == GLUT_RIGHT_BUTTON) ? "right" :
//		((button == GLUT_LEFT_BUTTON) ? "left" : "middle"), (state == GLUT_UP) ? "up" : "down", x, y); 
//	printf("Pane is %d by %d\n", width, height);

	for (int whichChild = 0; whichChild < nChildren; whichChild++)						//	loop through children
		{ // for each child
		GLUTPane *child = children[whichChild];										//	grab a pointer to the child
//		printf("Child %X: position %d %d, dimensions %d x %d\n", child, child->left, child->bottom, child->width, child->height);
		int x1 = x + left - child->left, y1 = y + bottom - child->bottom;				//	temporary copies of x, y
//		printf("Child-local coordinates: %d %d\n", x1, y1);
		if ((x1 < 0) || (x1 > child->width) || (y1 < 0) || (y1 > child->height))			//	if the event is outside child's frame
			continue;															//	don't pass it to child
		child->Mouse(button, state, x1, y1);										//	and call child's routine
		} // for each child
	} // Mouse()

void GLUTPane::Motion(int x, int y)												//	mouse motion routine	
	{ // Motion()
	//	default behaviour is to invoke LeftMouseMotion(), RightMouseMotion() or MiddleMouseMotion()
	} // Motion()

void GLUTPane::PassiveMotion(int x, int y)											//	routine for passive (mouse-up) motion
	{ // PassiveMotion()
	for (int whichChild = 0; whichChild < nChildren; whichChild++)						//	loop through children
		{ // for each child
		GLUTPane *child = children[whichChild];										//	grab a pointer to the child
		int x1 = x + left - child->left, y1 = y + bottom - child->bottom;				//	temporary copies of x, y
		if ((x1 < 0) || (x1 > child->width) || (y1 < 0) || (y1 > child->height))			//	if the event is outside child's frame
			continue;															//	don't pass it to child
		child->PassiveMotion(x1, y1);												//	and call child's routine
		} // for each child
	} // PassiveMotion()

void GLUTPane::KeyDown(unsigned char key, int x, int y)								//	routine for ordinary characters
	{ // KeyDown()
	//	this function can get called one of two ways:
	//	if the window uses "focus follows mouse", it is called hierarchically
	//	if, on the other hand, the window uses explicit focus, then a mouse-down event in a pane should
	//	cause that pane to acquire keyboard focus where relevant: e.g. a trackball should never acquire
	//	keyboard focus, but a text field should always do so.  
	//	the window should keep a pointer to the current "focus pane", which is set hierarchically by AcquireKeyboardFocus()
	//	in any event, the default implementation of KeyDown is the hierarchical version: the 
	//	direct version will use pane-specific code anyway
	
	//	so this default implementation just passes the key on to all children that intersect (x, y)
	for (int whichChild = 0; whichChild < nChildren; whichChild++)						//	loop through children
		{ // for each child
		GLUTPane *child = children[whichChild];										//	grab a pointer to the child
		int x1 = x + left - child->left, y1 = y + bottom - child->bottom;				//	temporary copies of x, y
		if ((x1 < 0) || (x1 > child->width) || (y1 < 0) || (y1 > child->height))			//	if the event is outside child's frame
			continue;															//	don't pass it to child
		child->KeyDown(key, x1, y1);												//	and call child's routine
		} // for each child
	} // KeyDown()

void GLUTPane::SpecialKeyDown(int key, int x, int y)									//	routine for special characters (e.g. arrow keys)
	{ // SpecialKeyDown()
	//	see KeyDown() for comments
	//	so this default implementation just passes the key on to all children that intersect (x, y)
	for (int whichChild = 0; whichChild < nChildren; whichChild++)						//	loop through children
		{ // for each child
		GLUTPane *child = children[whichChild];										//	grab a pointer to the child
		int x1 = x + left - child->left, y1 = y + bottom - child->bottom;				//	temporary copies of x, y
		if ((x1 < 0) || (x1 > child->width) || (y1 < 0) || (y1 > child->height))			//	if the event is outside child's frame
			continue;															//	don't pass it to child
		child->SpecialKeyDown(key, x1, y1);										//	and call child's routine
		} // for each child
	} // SpecialKeyDown()

void GLUTPane::AddChild(GLUTPane *child)											//	adds a child pane
	{ // AddChild()
	if (nChildren == 0)															//	it's the first child pane
		children = (GLUTPane **) malloc (sizeof(GLUTPane *));							//	allocate space for it
	else																		//	otherwise, there is an array already
		children = (GLUTPane **) realloc (children, sizeof(GLUTPane *) * (nChildren+1));	//	reallocate space & copy		
	children[nChildren++] = child;												//	add the child to the array
	} // AddChild()

void GLUTPane::AcquireDragFocus(GLUTPane *focusPane)									//	acquires drag focus on behalf of focusPane
	{ // AcquireDragFocus()
//	printf("Entering GLUTPane::AcquireDragFocus\n");
	parent->AcquireDragFocus(focusPane);											//	pass the request up the chain
	//	note that the hasDragFocus flag will be modified by the window
	} // AcquireDragFocus()

void GLUTPane::ReleaseDragFocus()													//	releases drag focus on behalf of focusPane
	{ // ReleaseDragFocus()
	parent->ReleaseDragFocus();													//	pass request to parent
	//	note that the hasDragFocus flag will be modified by the window
	} // ReleaseDragFocus()
	
void GLUTPane::AcquireKeyboardFocus(GLUTPane *focusPane)								//	acquires keyboard focus on behalf of focusPane
	{ // AcquireKeyboardFocus()
	parent->AcquireKeyboardFocus(focusPane);										//	pass the request up the chain
	//	note that the hasKeyFocus flag will be modified by the window
	} // AcquireKeyboardFocus()

void GLUTPane::ReleaseKeyboardFocus()												//	releases keyboard focus on behalf of focusPane
	{ // ReleaseKeyboardFocus()
	parent->AcquireKeyboardFocus(parent);											//	pass request to parent
	//	note that the hasKeyFocus flag will be modified by the window
	} // ReleaseKeyboardFocus()

int GLUTPane::WindowWidth()														//	retrieves the width of the window
	{ // WindowWidth()
	if (parent == NULL)	return width; else return parent->WindowWidth();					//	pass the buck up to the top
	} // WindowWidth()
	
int GLUTPane::WindowHeight()														//	retrieves the height of the window
	{ // WindowHeight()
	if (parent == NULL)	return height; else return parent->WindowHeight();				//	pass the buck up to the top
	} // WindowHeight()

void GLUTPane::Action(int action)													//	invoked by buttons, with the desired action value
	{ // Action()
	//	by default, do nothing
	} // Action()

void GLUTPane::PrintType()														//	prints text string for type
	{ // PrintType()
	printf("GLUTPane");
	} // PrintType()	
