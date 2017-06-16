///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	March, 2002
//
//	------------------------
//	ContourTreePane.cpp
//	------------------------
//	
//	A pane with the contour tree laid out in it
//	
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	03/27/01		Initial version
//
///////////////////////////////////////////////////

#include "ContourTreePane.h"												//	the ContourTreePane class	
#include <GL/freeglut.h>

ContourTreePane::ContourTreePane(GLUTPane *Parent, int Width, int Height, HeightField *newField, 
		float *Isovalue, bool *IsovalueChanged, IsosurfacePane *TheIsoPane, bool GrowHoriz, bool GrowVert)
	: GLUTPane(Parent, Width, Height, GrowHoriz, GrowVert)
	{ // ContourTreePane()												//	constructor 
	field = newField;													//	store the field
	nodeID = -1;														//	no node to drag initially
	isovalue = Isovalue;												//	and store the pointers
	isovalueChanged = IsovalueChanged;
	theIsoPane = TheIsoPane;
	} // ContourTreePane()
	
ContourTreePane::~ContourTreePane()										//	destructor
	{ // ~ContourTreePane()
	} // ~ContourTreePane()
																		
void ContourTreePane::Display()												//	display routine																	
	{ // Display()
	GLfloat backgroundColour[3] = { 0.0, 0.0, 0.0 };								//	on white
	GLfloat edgeColour[3] = { 0.25, 0.25, 0.25 };									//	draw in black

	glPushAttrib(GL_ALL_ATTRIB_BITS);										//	save the attribute frame
	glDisable(GL_LIGHTING);												//	turn off lighting
	glDisable(GL_DEPTH_TEST);											//	turn off depth test

	glMatrixMode(GL_PROJECTION);										//	set the projection matrix
	glLoadIdentity();													//	revert to identity matrix
//	glOrtho(left, left + width, bottom, bottom + height, -1.0, 1.0);						//	set projection to match viewport
	glOrtho(-0.02, 1.02, 1.02 * field->MinHeight() - 0.02* field->MaxHeight(),1.02 * field->MaxHeight() - 0.02* field->MinHeight(), -1.0, 1.0);
	
//	printf("Ortho projection: -0.1 1.1 %8.5f %8.5f\n", 1.1 * field->MinHeight() - 0.1* field->MaxHeight(),1.1 * field->MaxHeight() - 0.1* field->MinHeight());
	
	glMatrixMode(GL_MODELVIEW);										//	switch to modelview matrix
	glLoadIdentity();													//	and revert to the identity matrix
	
	glColor3fv(backgroundColour);											//	set the background colour
	glBegin(GL_LINE_STRIP);												//	and draw a "square"
	glVertex2f(-0.019, 1.019 * field->MinHeight() - 0.019* field->MaxHeight());
	glVertex2f(1.019, 1.019 * field->MinHeight() - 0.019* field->MaxHeight());
	glVertex2f(1.019, -0.019 * field->MinHeight() + 1.019* field->MaxHeight());
	glVertex2f(-0.019, -0.019 * field->MinHeight() + 1.019* field->MaxHeight());
	glVertex2f(-0.019, 1.019 * field->MinHeight() - 0.019* field->MaxHeight());
	glEnd();

	glColor3fv(edgeColour);												//	set the edge colour
	if (showTree)
		field->DrawPlanarContourTree(useColouredTree);	//	draw a planar contour tree
	glPopAttrib();														//	return to previous attributes
	} // Display()

void ContourTreePane::Mouse(int button, int state, int x, int y)						//	mouse down / up routine 
	{ // Mouse()
	if (button == GLUT_RIGHT_BUTTON)										//	right button to drag nodes
		{ // dragging nodes
		if (state == GLUT_DOWN)											//	if anywhere in the pane
			{ // mouse-down 
			AcquireDragFocus(this);										//	acquire focus
//			printf("x: %ld left: %ld width: %ld computed: %f\n", x, left, width, (float) x / width);
			nodeID = field->PickNode(	-0.02 + 1.04 * ((float) x / width), 
								(-0.02 + 1.04 * ((float) y / height)));			//	ask the field which node was hit (if any)
//			printf("nodeID: %ld\n", nodeID);
			} // mouse-down 
		else if ((state == GLUT_UP) && (hasDragFocus))							//	if we were dragging in the pane, and it's mouse-up
			{ // mouse-up when dragging node
			if (nodeID != -1) nodeID = -1;									//	reset drag ID's if required
			ReleaseDragFocus();											//	release drag focus
			} // mouse-up when dragging node
		} // dragging nodes
	else if (button == GLUT_LEFT_BUTTON)									//	left button to drag on arcs
		{ // dragging arcs
		if (state == GLUT_DOWN)											//	if anywhere in the pane
			{ // mouse-down 
			AcquireDragFocus(this);										//	acquire focus
			if (field->selectionRoot != noContourSelected)						//	if there is a selection
				field->CommitSelection();								//	commit it
			theIsoPane->selectionColour = -1;
			long arcID = field->PickArc(	-0.02 + 1.04 * ((float) x / width), 
								(-0.02 + 1.04 * ((float) y / height)));			//	ask the field which arc was hit (if any)
			if (arcID == -1) return;											//	do nothing further if we missed
			float newHt = (-0.02 + ((float) y / height) * 1.04) * (field->MaxHeight() - field->MinHeight()) + field->MinHeight();
			field->SelectComponent(arcID, newHt);								//	and select it for dragging
			theIsoPane->selectionColour = field->GetSuperarcColour(arcID);
			glutPostRedisplay();											//	and force a redraw
			} // mouse-down 
		else if ((state == GLUT_UP) && (hasDragFocus))							//	if we were dragging in the pane, and it's mouse-up
			{ // mouse-up when dragging arc
			ReleaseDragFocus();											//	release drag focus
			} // mouse-up when dragging arc
		} // dragging arcs
	} // Mouse()
	
void ContourTreePane::Motion(int x, int y)										//	mouse motion routine	
	{ // Motion()
	if (nodeID != -1)														//	if a node is actually being dragged
		{ // node being dragged
		field->UpdateNodeXPosn(nodeID, -0.02 + 1.04 * ((float) x / width));				//	update the position in the height field / CT
		glutPostRedisplay();												//	and force a redraw
		} // node being dragged
	else if (field->selectionRoot != noContourSelected)								//	if there is a drag selection
		{ // arc being dragged
		float newHt = (-0.02 + ((float) y / height) * 1.04);
		*isovalue = newHt;													//	update it: someone else will redisplay
		*isovalueChanged = true;
		glutPostRedisplay();												//	force a redisplay
		} // arc being dragged
	} // Motion()

