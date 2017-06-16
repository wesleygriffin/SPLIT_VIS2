///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTWindow.cpp
//	------------------------
//	
//	Implementation of GLUTWindow base class
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

#include "GLUTWindow.h"														//	GLUTWindow
#include <GL/freeglut.h>
#include <GL/freeglut.h>															//	and GLUT
#include <stdlib.h>															//	needed for NULL, of all things
#include <stdio.h>															//	for error messages
#define GLErrorString(x) ( ((x) == GL_NO_ERROR) ? "No Error" : ((x) == GL_INVALID_ENUM) ? "Invalid enum" : ((x) == GL_INVALID_VALUE) ? "Invalid Value" : ((x) == GL_INVALID_OPERATION) ? "Invalid Operation" : ((x) == GL_STACK_OVERFLOW) ? "Stack Overflow" : ((x) == GL_STACK_UNDERFLOW) ? "Stack Underflow" : ((x) == GL_OUT_OF_MEMORY) ? "Out of Memory" : "Unknown Error")
#include "writetofile.h"

static GLUTWindowList MasterGLUTWindowList;										//	the master list of all GLUT windows

void InitGLUTWindows(int *argc, char **argv)										//	sets up all global data to track windows
	{ // InitGLUTWindows()
	glutInit(argc, argv);														//	initialize GLUT
	} // InitGLUTWindows()

GLUTWindow::GLUTWindow(const char *name, int width, int height, bool GrowHoriz, bool GrowVert, unsigned int GLUTmode,  
		GLfloat redComp, GLfloat greenComp, GLfloat blueComp, GLfloat alphaComp, bool FocusFollowsMouse)	
	: GLUTHorizPane(NULL, width, height, GrowHoriz, GrowVert)						//	call inherited constructor
	{ // GLUTWindow constructor
	glutInitDisplayMode(GLUTmode);											//	set the display mode
	doubleBuffered = GLUTmode & GLUT_DOUBLE ? true : false;						//	set float-buffering flag
	hasDepthBuffer = GLUTmode & GLUT_DEPTH ? true : false;							//	and depth-buffer flag0
	glutInitWindowSize(width, height);											//	and the window size
	GLUTWindowID = glutCreateWindow(name);										//	create the GLUT window

//	GLint params[1] = { 1};
//	CGLSetParameter(CGLGetCurrentContext(),  kCGLCPSwapInterval, params);

	MasterGLUTWindowList.AddWindow(this);										//	add the window to the list
	dragFocus = NULL;														//	initially, no dragging occurring
	keyFocus = this;														//	start with key focus in the window proper
	hasKeyFocus = true;														//	and set the flag to indicate we have focus
	
	ChangeClearColour(redComp, greenComp, blueComp, alphaComp);						//	reset the buffer-clear colour
		
	glutDisplayFunc(::Display);												//	set the callbacks
	glutMouseFunc(::Mouse);
	glutMotionFunc(::Motion);
	glutPassiveMotionFunc(::PassiveMotion);
	glutReshapeFunc(::Reshape);	
	glutKeyboardFunc(::KeyDown);
	glutSpecialFunc(::SpecialKeyDown);

	} // GLUTWindow constructor
	
GLUTWindow::~GLUTWindow()													//	destructor
	{ // GLUTWindow destructor
	MasterGLUTWindowList.RemoveWindow(this);									//	remove the window from the list
	glutDestroyWindow(GLUTWindowID);											//	ask GLUT to destroy it
	} // GLUTWindow destructor

void GLUTWindow::Display()													//	main display routine
	{ // Display()
	int errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] on entry to GLUTWindow::Display()\n", __FILE__, __LINE__, errNum, GLErrorString(errNum));
		}
	glClearColor(bufClearRed, bufClearGreen, bufClearBlue, bufClearAlpha);			//	set the colour to clear the buffer with
//	printf("Clearing with %f %f %f %f\n", bufClearRed, bufClearGreen, bufClearBlue, bufClearAlpha);
	glClear(GL_COLOR_BUFFER_BIT | (hasDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0));			//	and clear the buffer																	
	errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] in GLUTWindow::Display() before calling GLUTPane::Display()\n", __FILE__, __LINE__, errNum, GLErrorString(errNum));
		}
	GLUTPane::Display();													//	call the inherited routine
	errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] in GLUTWindow::Display() after calling GLUTPane::Display()\n", __FILE__, __LINE__, errNum, GLErrorString(errNum));
		}
	if (doubleBuffered) glutSwapBuffers();										//	then swap buffers if needed
	errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] in GLUTWindow::Display() after calling glutSwapBuffers()\n", __FILE__, __LINE__, errNum, GLErrorString(errNum));
		}
	} // Display()

void GLUTWindow::Reshape(int Width, int Height)									//	window reshape routine
	{ // Reshape()
	GLUTHorizPane::Reshape(Width, Height);										//	call the inherited function
	if ((width != Width) || (height != Height))									//	if we don't end up with the desired size
		glutReshapeWindow(width, height);										//	get GLUT to try again with the computed size
	glutPostRedisplay();													//	when we are done, redisplay
	} // Reshape()

void GLUTWindow::Mouse(int button, int state, int x, int y)							//	mouse down / up routine 
	{ // Mouse()
	y = height - y;														//	invert coordinates to match OpenGL
//	printf("GLUTWindow::Mouse(): %s button in state %s at (%ld, %ld)\n", (button == GLUT_RIGHT_BUTTON) ? "right" :
//		((button == GLUT_LEFT_BUTTON) ? "left" : "middle"), (state == GLUT_UP) ? "up" : "down", x, y); 
	
	if ((state == GLUT_UP) && (dragFocus != NULL))								//	if there is a dragfocus, and a mouse-up
		dragFocus->Mouse(button, state, x - dragFocus->left, y - dragFocus->bottom);
																		//	pass directly to the drag focus pane
	else																	//	otherwise
		GLUTPane::Mouse(button, state, x, y);									//	use the inherited pane routine	
	} // Mouse()

void GLUTWindow::Motion(int x, int y)											//	mouse motion routine	
	{ // Motion()
	y = height - y;														//	invert coordinates to match OpenGL
	if (dragFocus != NULL)													//	if there is a drag focus
		dragFocus->Motion(x - dragFocus->left, y - dragFocus->bottom);				//	pass the message to it
	} // Motion()

void GLUTWindow::KeyDown(unsigned char key, int x, int y)							//	routine for ordinary characters
	{ // KeyDown()
	y = height - y;														//	invert coordinates to match OpenGL
	if (focusFollowsMouse)													//	if the programmer wants it this way
		GLUTPane::KeyDown(key, x, y);											//	call inherited routine
	else if (keyFocus == this)												//	if the top-level pane is the key focus
		GLUTPane::KeyDown(key, x, y);											//	this has the same effect
	else	keyFocus->KeyDown(key, x - keyFocus->left, y - keyFocus->bottom);			//	otherwise, pass directly to focus
	} // KeyDown()

void GLUTWindow::SpecialKeyDown(int key, int x, int y)								//	routine for special characters (e.g. arrow keys)
	{ // SpecialKeyDown()
	y = height - y;														//	invert coordinates to match OpenGL
	if (focusFollowsMouse)													//	if the programmer wants it this way
		GLUTPane::SpecialKeyDown(key, x, y);									//	call inherited routine
	else if (keyFocus == this)												//	if the top-level pane is the key focus
		GLUTPane::SpecialKeyDown(key, x, y);									//	this has the same effect
	else	keyFocus->SpecialKeyDown(key, x - keyFocus->left, y - keyFocus->bottom);		//	otherwise, pass directly to focus
	} // SpecialKeyDown()
	
void GLUTWindow::AcquireDragFocus(GLUTPane *focusPane)								//	acquires drag focus on behalf of focusPane
	{ // AcquireDragFocus()
//	printf("Entering GLUTWindow::AcquireDragFocus()\n");
	if (dragFocus != NULL)													//	if there is an old drag focus
		dragFocus->hasDragFocus = false;										//	tell the old focus pane it no longer has focus
	dragFocus = focusPane;													//	save it for future reference
	dragFocus->hasDragFocus = true;											//	tell the new focus pane it has focus
	} // AcquireDragFocus()

void GLUTWindow::ReleaseDragFocus()											//	releases drag focus 
	{ // ReleaseDragFocus()
	dragFocus->hasDragFocus = false;											//	tell the old focus pane it no longer has focus
	dragFocus = NULL;														//	pass request to parent
	} // ReleaseDragFocus()

void GLUTWindow::AcquireKeyboardFocus(GLUTPane *focusPane)							//	acquires keyboard focus on behalf of focusPane
	{ // AcquireKeyboardFocus()
	keyFocus->hasKeyFocus = false;											//	tell the old focus pane it no longer has focus
	keyFocus = focusPane;													//	save it for future reference
	keyFocus->hasKeyFocus = true;												//	tell the new focus pane it has focus
	} // AcquireKeyboardFocus()

void GLUTWindow::ReleaseKeyboardFocus()											//	releases keyboard focus 
	{ // ReleaseKeyboardFocus()
	//	do nothing														//	the buck stops here: the window must have it if no one else does
	} // ReleaseKeyboardFocus()
	
int GLUTWindow::GetGLUTWindowID()												//	returns the window ID
	{ // GetGLUTWindowID()
	return GLUTWindowID;													//	gee, that was easy
	} // GetGLUTWindowID()

void GLUTWindow::PrintType()													//	prints text string for type
	{ // PrintType()
	printf("GLUTWindow");
	} // PrintType()	


void GLUTWindow::ChangeClearColour(GLfloat redComp, GLfloat greenComp, GLfloat blueComp, GLfloat alphaComp)
																		//	change the colour used to clear the buffer
	{ // ChangeClearColour()
	bufClearRed = redComp; bufClearGreen = greenComp; bufClearBlue = blueComp; bufClearAlpha = alphaComp;
														 				//	set the colour for clearing the buffer
	} // ChangeClearColour()

GLUTWindowList::GLUTWindowList()												//	constructor
	{ // constructor
	topWindow = NULL;														//	initialize the list to empty
	} // constructor

GLUTWindowList::~GLUTWindowList()												//	destructor
	{ // destructor
	GLUTWindow *destroyer;													//	an element about to be destroyed
	while (topWindow != NULL)												//	and walk to the bottom
		{ // valid nodes
		destroyer = topWindow;												//	grab a pointer to top window
		topWindow = topWindow->nextGLUTWindow;									//	step to next window
		delete destroyer;													//	delete the top element
		} // valid nodes
	} // destructor

void GLUTWindowList::AddWindow(GLUTWindow *theWindow)								//	adds a window to the list
	{ // AddWindow()
	theWindow->nextGLUTWindow = topWindow;										//	attach old list to bottom
	topWindow = theWindow;													//	and put new window at top
	} // AddWindow()

void GLUTWindowList::RemoveWindow(GLUTWindow *theWindow)							//	removes a window from the list
	{ // RemoveWindow()
	GLUTWindow *walkWindow, *nextWindow;										//	pointers for walking down the list
	
	if (topWindow == theWindow)												//	if the one we want is at the top
		{ // theWindow is at the top
		topWindow = topWindow->nextGLUTWindow;									//	just reset the list
		return;															//	and leave
		} // theWindow is at the top

	walkWindow = topWindow;													//	start at the top of the list
	while (walkWindow != NULL)												//	keep going until we run off the list
		{ // walkWindow is not NULL
		nextWindow = walkWindow->nextGLUTWindow;								//	grab the next window
		if (nextWindow == theWindow)											//	if it matches the one we seek
			{ // nextWindow == theWindow
			walkWindow->nextGLUTWindow = nextWindow->nextGLUTWindow;				//	connect the list around it
			return;														//	and leave
			} // nextWindow == theWindow
		walkWindow = nextWindow;												//	advance to next window in list		
		} // walkWindow is not NULL
	} // RemoveWindow

GLUTWindow *GLUTWindowList::GetGLUTWindow(int theWindowID)							//	retrieves a window by ID
	{ // GetGLUTWindow()
	GLUTWindow *walkWindow = topWindow;										//	start at top of list
	
	for (; walkWindow != NULL; walkWindow = walkWindow->nextGLUTWindow)				//	walk to bottom of list
		if (walkWindow->GLUTWindowID == theWindowID)								//	if we find a match
			return walkWindow;												//	return it
	return NULL;															//	we failed.  Oops!
	} // GetGLUTWindow()

//	standard GLUT callbacks

void Display()																//	main display routine
	{ // Display()
	int errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] on entry to ::Display() \n", __FILE__, __LINE__, errNum, GLErrorString(errNum));
		}
	GLUTWindow *target = MasterGLUTWindowList.GetGLUTWindow(glutGetWindow());			//	retrieve the target window
	errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] after using glutGetWindow() \n", __FILE__, __LINE__, errNum, GLErrorString(errNum));
		}
	if (target == NULL)														//	if it failed
		printf("Unable to display window %ld.\n", glutGetWindow());					//	report the error
	else																	//	otherwise
		target->Display();													//	tell it to display
#ifdef GENERATE_VIDEO
	WriteToFile();
#endif
	} // Display()

void Reshape(int width, int height)											//	window reshape routine
	{ // Reshape()
	GLUTWindow *target = MasterGLUTWindowList.GetGLUTWindow(glutGetWindow());			//	retrieve the target window
	if (target == NULL)														//	if it failed
		printf("Unable to reshape window %ld.\n", glutGetWindow());					//	report the error
	else																	//	otherwise
		target->Reshape(width, height);										//	tell it to reshape
	} // Reshape()

void Mouse(int button, int state, int x, int y)									//	mouse down / up routine 
	{ // Mouse()
	GLUTWindow *target = MasterGLUTWindowList.GetGLUTWindow(glutGetWindow());			//	retrieve the target window
	if (target == NULL)														//	if it failed
		printf("Unable to pass mouse event to  window %ld.\n", glutGetWindow());		//	report the error
	else																	//	otherwise
		target->Mouse(button, state, x, y);									//	pass mouse event on
	} // Mouse()

void Motion(int x, int y)													//	mouse motion routine	
	{ // Motion()
	GLUTWindow *target = MasterGLUTWindowList.GetGLUTWindow(glutGetWindow());			//	retrieve the target window
	if (target == NULL)														//	if it failed
		printf("Unable to drag in window %ld.\n", glutGetWindow());					//	report the error
	else																	//	otherwise
		target->Motion(x, y);												//	tell it to display
	} // Motion()

void PassiveMotion(int x, int y)												//	routine for passive (mouse-up) motion
	{ // PassiveMotion()
	GLUTWindow *target = MasterGLUTWindowList.GetGLUTWindow(glutGetWindow());			//	retrieve the target window
	if (target == NULL)														//	if it failed
		printf("Unable to perform passive motion in window %ld.\n", glutGetWindow());	//	report the error
	else																	//	otherwise
		target->PassiveMotion(x, y);											//	pass the buck
	} // PassiveMotion()

void KeyDown(unsigned char key, int x, int y)									//	routine for ordinary characters
	{ // KeyDown()
	GLUTWindow *target = MasterGLUTWindowList.GetGLUTWindow(glutGetWindow());			//	retrieve the target window
	if (target == NULL)														//	if it failed
		printf("Unable to pass keystroke to window %ld.\n", glutGetWindow());			//	report the error
	else																	//	otherwise
		target->KeyDown(key, x, y);											//	pass it on
	} // KeyDown()

void SpecialKeyDown(int key, int x, int y)										//	routine for special characters (e.g. arrow keys)
	{ // SpecialKeyDown()
	GLUTWindow *target = MasterGLUTWindowList.GetGLUTWindow(glutGetWindow());			//	retrieve the target window
	if (target == NULL)														//	if it failed
		printf("Unable to pass special keystroke to window %ld.\n", glutGetWindow());	//	report the error
	else																	//	otherwise
		target->SpecialKeyDown(key, x, y);										//	pass the keycode on
	} // SpecialKeyDown()

