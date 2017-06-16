///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	January, 2004
//
//	------------------------
//	ColourSelectionPane.h
//	------------------------
//	
//	A pane displaying the colour choices
//	
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	01/04/04		Initial version
//
///////////////////////////////////////////////////

#include "ColourSelectionPane.h"														//	the ColourSelectionPane class	
#include <GL/freeglut.h>
#include "HeightField.h"

ColourSelectionPane::ColourSelectionPane(GLUTPane *Parent, GLUTPane *Target, long Action, long *TargetInteger)	
	: GLUTPane(Parent, 20 * N_SURFACE_COLOURS, 20, false, false)											//	constructor 
	{ // ColourSelectionPane()
	target = Target;
	action = Action;
	targetInteger = TargetInteger;													//	make sure it points to something
	} // ColourSelectionPane()
	
ColourSelectionPane::~ColourSelectionPane()												//	destructor
	{ // ~ColourSelectionPane()
	} // ~ColourSelectionPane()
													
void ColourSelectionPane::Display()													//	display
	{ // Display()
	glPushAttrib(GL_ALL_ATTRIB_BITS);													//	save the attribute frame
	glDisable(GL_LIGHTING);															//	turn off lighting
	glDisable(GL_DEPTH_TEST);														//	turn off depth test

	glMatrixMode(GL_PROJECTION);														//	set the projection matrix
	glLoadIdentity();																//	revert to identity matrix
	glOrtho(0.0, 5.0 * N_SURFACE_COLOURS, 0.0, 5.0, -1.0, 1.0);											//	set projection

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(0.0, 0.0); glVertex2f(15.0, 0.0); glVertex2f(15.0, 15.0); glVertex2f(0.0, 15.0);
	glEnd();

	for (int colour = 0; colour < N_SURFACE_COLOURS; colour++)
		{ // i,j loop
		if (colour == *targetInteger)
			{ // it's selected
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_QUADS);
			glVertex2f(colour * 5, 0);
			glVertex2f(colour * 5 + 5, 0);
			glVertex2f(colour * 5 + 5, 5);
			glVertex2f(colour * 5, 5);
			glEnd();
			} // it's selected
		
		glColor3fv(surface_colour[colour]);
		
		glBegin(GL_QUADS);
		glVertex2f(colour * 5 + 1, 1);
		glVertex2f(colour * 5 + 4, 1);
		glVertex2f(colour * 5 + 4, 4);
		glVertex2f(colour * 5 + 1, 4);
		glEnd();

		} // i,j loop

	glPopAttrib();															//	go back to previous attributes
	} // Display()

void ColourSelectionPane::Mouse(int button, int state, int x, int y)							//	mouse down / up routine 
	{ // Mouse()
	if (state == GLUT_DOWN)												//	if anywhere in the pane
		{ // mouse-down 
//		printf("%d %d\n", x, y);
		int colour = x / 20; 
		if (colour < 0) 
			colour = 0; 
		else if (colour > N_SURFACE_COLOURS) 
			colour = N_SURFACE_COLOURS;
		*targetInteger = colour;
		target->Action(action);
		} // mouse-down 
	} // Mouse()
	
void ColourSelectionPane::PrintType()													//	prints text string for type
	{ // PrintType()
	printf("ColourSelectionPane");
	} // PrintType()