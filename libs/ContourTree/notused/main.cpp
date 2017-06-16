///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	main.cpp
//	------------------------
//	
//	Simple main routine to test GLUTWindow
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

#include "GLUTWindow.h"
#include "IsosurfaceWindow.h"
#include <stdio.h>

int main(int argc, char **argv)
	{ // main()
	InitGLUTWindows(&argc, argv);										//	initialize the GLUTWindows package

	if (argc < 2) { printf("Usage: %s filename [options]\n", argv[0]); exit (0);};		//	check to see that there is a file name to use

	try
		{
		GLUTWindow *aWindow = new IsosurfaceWindow(argc, argv, argv[1]);						//	create a window
		}
	catch (int i)
		{
		printf("Construction failed.\n"); 
		return 0;
		}
	glutMainLoop();												//	start up the main loop
	} // main()
