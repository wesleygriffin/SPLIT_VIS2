///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTPane.h
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

#ifndef _HEADER_GLUTPane
#define _HEADER_GLUTPane

class GLUTPane																//	abstract class encapsulating pane behaviour
	{ // class GLUTPane
	public:
	GLUTPane *parent;														//	the enclosing GLUTPane (NULL for a window)
	int nChildren;															//	number of child panes
	GLUTPane **children;													//	array of children
	
	int bottom, left;														//	position of pane in window coordinates
	int width, height;														//	width & height of the pane
	bool growHoriz, growVert;												//	whether pane can grow horizontally & vertically
	bool hasDragFocus, hasKeyFocus;											//	flags for whether it has key / drag focus
		
	GLUTPane(GLUTPane *Parent, int Width, int Height, bool GrowHoriz = true, bool GrowVert = true);
																		//	constructor
	virtual ~GLUTPane();													//	destructor
		
	virtual void Display();													//	main display routine
	virtual void Reshape(int Width, int Height);									//	window reshape routine
	
	virtual void Mouse(int button, int state, int x, int y);						//	mouse down / up routine 
	virtual void Motion(int x, int y);											//	mouse motion routine	
	virtual void PassiveMotion(int x, int y);									//	routine for passive (mouse-up) motion
	
	virtual void KeyDown(unsigned char key, int x, int y);							//	routine for ordinary characters
	virtual void SpecialKeyDown(int key, int x, int y);							//	routine for special characters (e.g. arrow keys)

	virtual void AddChild(GLUTPane *child);										//	adds a child pane

	virtual void AcquireDragFocus(GLUTPane *focusPane);							//	acquires drag focus on behalf of focusPane
	virtual void ReleaseDragFocus();											//	releases drag focus on behalf of focusPane
	virtual void AcquireKeyboardFocus(GLUTPane *focusPane);						//	acquires keyboard focus on behalf of focusPane
	virtual void ReleaseKeyboardFocus();										//	releases keyboard focus on behalf of focusPane

	virtual void Action(int action);											//	invoked by buttons, with the desired action value
	virtual void PrintType();												//	prints text string for type

	int WindowWidth();														//	retrieves the width of the window
	int WindowHeight();														//	retrieves the height of the window

	}; // class GLUTPane
#endif
