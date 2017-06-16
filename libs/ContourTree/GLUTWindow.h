///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTWindow.h
//	------------------------
//	
//	Both GLUI & GLOW rely on subwindows, which 
//	neither Mac OS 9 or OS X is happy with.  Frankly,
//	neither am I: I prefer the logical separation of
//	Windows and Panes, so I am rolling my own
//
//	A window should consist of the necessary callbacks
//	for GLUT, and a mechanism to retrieve the window
//	so that the global callback works correctly
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
//	02/06/01		Initial version
//
///////////////////////////////////////////////////

#ifndef _HEADER_GLUTWindow
#define _HEADER_GLUTWindow

#include <GL/freeglut.h>															//	GLUT
#include "GLUTHorizPane.h"													//	GLUTHorizPane

#define DEFAULT_GLUT_WINDOW_NAME "GLUT Window"

void InitGLUTWindows(int *argc, char **argv);									//	sets up all global data to track windows

class GLUTWindow : public GLUTHorizPane											//	encapsulates window behaviour
	{ // class GLUTWindow()
	private:
	//	width & height are inherited from the Pane class

	//	the window ID according to GLUT
	int GLUTWindowID;														//	result returned by GLUTCreateWindow()
	GLUTWindow *nextGLUTWindow;												//	pointer to allow a linked list of windows

	//	stuff relating to the pane hierarchy
	GLUTPane *dragFocus;													//	pane in which dragging is active
	GLUTPane *keyFocus;														//	pane in which keyboard focus is active
	
	bool focusFollowsMouse;													//	whether to use "focus follows mouse"
	bool doubleBuffered;													//	whether the window is double-buffered
	bool hasDepthBuffer;													//	whether the window is depth-buffered

	GLfloat bufClearRed, bufClearGreen, bufClearBlue, bufClearAlpha;					//	the colour to use for clearing buffers

	public:
	//	initial constants
//	static const char defaultWindowName[] = "GLUT Window";							//	the name of the window
	static const int defaultWidth = 512;										//	default width for the window
	static const int defaultHeight = 512;										//	default height for the window
	static const unsigned int defaultGLUTmode = GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;	//	default window type

	//	constructor / destructor
	GLUTWindow(const char *name = DEFAULT_GLUT_WINDOW_NAME, int width = defaultWidth, int height = defaultHeight, 
					bool GrowHoriz = true, bool GrowVert = true, unsigned int GLUTmode = defaultGLUTmode, 
					GLfloat redComp = 1.0, GLfloat greenComp = 1.0, GLfloat blueComp = 1.0, GLfloat alphaComp = 0.0, 
					bool FocusFollowsMouse = false);	
																		//	constructor
	~GLUTWindow();															//	destructor

	virtual void Display();													//	main display routine
	virtual void Reshape(int Width, int Height);									//	window reshape routine
	virtual void Mouse(int button, int state, int x, int y);						//	mouse down / up routine 
	virtual void Motion(int x, int y);											//	mouse motion routine	
	virtual void KeyDown(unsigned char key, int x, int y);							//	routine for ordinary characters
	virtual void SpecialKeyDown(int key, int x, int y);							//	routine for special characters (e.g. arrow keys)

	virtual void AcquireDragFocus(GLUTPane *focusPane);							//	acquires drag focus on behalf of focusPane
	virtual void ReleaseDragFocus();											//	releases current drag focus
	virtual void AcquireKeyboardFocus(GLUTPane *focusPane);						//	acquires keyboard focus on behalf of focusPane
	virtual void ReleaseKeyboardFocus();										//	releases keyboard focus on behalf of focusPane
	virtual void PrintType();												//	prints text string for type

	int GetGLUTWindowID();													//	returns the window ID
	friend class GLUTWindowList;												//	give the GLUTWindowList class access
	void ChangeClearColour(GLfloat redComp, GLfloat greenComp, GLfloat blueComp, GLfloat alphaComp);
																		//	change the colour used to clear the buffer
	}; // class GLUTWindow()

class GLUTWindowList														//	utility class for tracking windows
	{ // class GLUTWindowList
	private:
	GLUTWindow *topWindow;													//	the top window on the list
	
	public:
	GLUTWindowList();														//	constructor
	~GLUTWindowList();														//	destructor

	void AddWindow(GLUTWindow *theWindow);										//	adds a window to the list
	void RemoveWindow(GLUTWindow *theWindow);									//	removes a window from the list
	GLUTWindow *GetGLUTWindow(int theWindowID);									//	retrieves a window by ID
	}; // class GLUTWindowList

//	standard GLUT callbacks

void Display();															//	main display routine
void Reshape(int width, int height);											//	window reshape routine
void Mouse(int button, int state, int x, int y);									//	mouse down / up routine 
void Motion(int x, int y);													//	mouse motion routine	
void PassiveMotion(int x, int y);												//	routine for passive (mouse-up) motion
void KeyDown(unsigned char key, int x, int y);									//	routine for ordinary characters
void SpecialKeyDown(int key, int x, int y);										//	routine for special characters (e.g. arrow keys)

#endif
