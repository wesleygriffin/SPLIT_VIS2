///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	IsosurfaceWindow.h
//	------------------------
//	
//	A simple window with an isosurface & controls
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

#ifndef _HEADER_IsosurfaceWindow
#define _HEADER_IsosurfaceWindow

#include <stdlib.h>
#include "GLUTWindow.h"														//	the GLUTWindow class
#include <fstream>
using namespace std;

class IsosurfaceWindow : public GLUTWindow									//	window that displays an isosurface
	{ // class IsosurfaceWindow
	public:	
	IsosurfaceWindow(int argc, char **argv, char *windowName = "Isosurface");				//	constructor : everything else should be in the
																			//	IsosurfacePane
																	
	}; // class IsosurfaceWindow

#endif
