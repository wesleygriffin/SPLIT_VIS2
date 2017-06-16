///////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	IsosurfacePane.cpp
//	------------------------
//	
//	A simple pane with an isosurface in it
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
///////////////////////////

#include "IsosurfacePane.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include "sphere.h"
#include "writetofile.h"

static GLfloat light_position[] = {0.4, 1.0, 0.5, 0.0};							//	position of light source (at infinity)
static GLfloat redColour[4] = {1.0, 0.0, 0.0, 0.0};								//	red
static GLfloat greenColour[4] = {0.0, 1.0, 0.0, 0.0};								//	green
static GLfloat blackColour[4] = {0.0, 0.0, 0.0, 0.0};								//	black
static GLfloat whiteColour[4] = {1.0, 1.0, 1.0, 0.0};								//	white

#define N_CIRCLE_STEPS 36
#ifndef PI
#define PI 3.14159265358979
#endif

IsosurfacePane::IsosurfacePane(GLUTPane *Parent, int Width, int Height, int argc, char **argv, bool GrowHoriz, bool GrowVert)									//	constructor 
	: GLUTPane(Parent, Width, Height, GrowHoriz, GrowVert)
	{ // constructor
	field = new HeightField(argc, argv);										//	create a height field with stream
	cellEdgeDisplayList = glGenLists(1);										//	and another for cell edges	
	isoSliderChanged = false;												//	flag to keep track of slider
	showAxes = showCells = false;
	useMarchingCubes = false;
	recomputeCellEdgeDisplayList = true;										//	don't build if we don't need it yet
	for (int i = 0; i < 16; i++) if (i/4 == i%4) rotMat[i] = lightMat[i] = 1.0; else rotMat[i] = lightMat[i] = 0.0;
//	rotMat[5] = rotMat[10] = -1;
	glutPostRedisplay();													//	force a redisplay to start off with
//	field->selectionRoot = noContourSelected;									//	start off with no contours selected
	offset_x = offset_y = dragOld_x = dragOld_y = 0.0;							//	set the initial offset
	scale = -0.2;
	updateFromTreeSize = updateFromPriorityBound = false;							//	set it manually
	selectionColour = -1;
	collapsePriority = PRIORITY_HEIGHT;
	recollapse = false;
	} // constructor

IsosurfacePane::~IsosurfacePane()												//	destructor
	{ // destructor
	delete field;															//	delete the field
	} // destructor																	

void IsosurfacePane::Display()												//	display routine																	
	{ // Display()
	if ((recollapse) && (field != NULL))
		{ // recollapse
		field->ResetCollapsePriority(collapsePriority);
		recollapse = false;
		} // recollapse
	if (recomputeCellEdgeDisplayList) 	MakeCellEdgeDisplayList();					//	ditto for cell edges
	if (updateFromTreeSize) 
		{ // pending update on the degree of collapse in the tree
		field->UpdateFromLogTreeSize();										//	tell the field to do the update
		updateFromTreeSize = false;											//	disable flag
		isoSliderChanged = true;												//	force re-computation
		glutPostRedisplay();												//	force re-display
		} // pending update on the degree of collapse in the tree
	if (updateFromPriorityBound) 
		{ // pending update on the degree of collapse in the tree
		//field->UpdateFromLogPriorityBound();									//	tell the field to do the update
		updateFromPriorityBound = false;										//	disable flag
		isoSliderChanged = true;												//	force re-computation
		glutPostRedisplay();												//	force re-display
		} // pending update on the degree of collapse in the tree
	if (isoSliderChanged && !useMarchingCubes) UpdateHeightFieldComponents();			//	call routine to update components
	
	if (field == NULL) return;												//	for an empty field, do nothing
	if ((width == 0) || (height == 0)) return;									//	no sense rendering a 0x0 pane

	SetUpMatrices();														//	set up projection & modelview matrices	

	if (showAxes) DrawAxes();												//	if we want axes shown

	if (showCells)															//	if cell display is turned on
		{ // showing cells
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blackColour);			//	edges don't reflect
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, greenColour);					//	but they do emit
		glCallList(cellEdgeDisplayList);										//	call the display list
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);					//	turn off emission
		} // showing cells	

	if (showLevelSet)														//	if level set display is turned on
		{ // showing level set
//		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, redColour);			//	surfaces reflect light
//		field->MarchingSimplices(isoValue * (field->MaxHeight() - field->MinHeight()) + field->MinHeight());
		glEnable(GL_NORMALIZE);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
                if (useMarchingCubes)												//	if marching cubes turned on
			field->MarchingCubes(field->MinHeight() + isoValue * (field->MaxHeight() - field->MinHeight()));
		else
			field->FlexibleContours(useDisplayLists);							//	draw the current set of contours
		} // showing level set
	else
		{ // draw sphere instead
		int i, j;
		GLfloat backgroundColour[4] = { 1.0, 1.0, 1.0, 1.0};						//	background is white
		GLfloat ballColour[4] = { 0.0, 0.0, 0.0, 1.0 };							//	ball is in black
	
		glPushAttrib(GL_ALL_ATTRIB_BITS);										//	save the attribute frame
		glEnable(GL_DEPTH_TEST);												//	turn depth buffer back on
		glDisable(GL_LIGHTING);												//	disable lighting
		glMatrixMode(GL_PROJECTION);											//	set the projection matrix
		glLoadIdentity();													//	revert to identity matrix
		glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);								//	set projection to simple 

		glMatrixMode(GL_MODELVIEW);											//	make sure that we are set to the model matrix
		glPushMatrix();													//	push the GL stack to get a new matrix
		glLoadIdentity();													//	load the identity matrix
		glColor3fv(backgroundColour);											//	set colour for polygon (zero-plane)

		//printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
		glBegin(GL_POLYGON);												//	draw a polygon to hide rear edges
		glVertex3f(-1.0, -1.0, 0.0);
		glVertex3f(1.0, -1.0, 0.0);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(-1.0, 1.0, 0.0);
		//printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
		glEnd();

		glColor3fv(ballColour);												//	set the colour for the sphere
		glScalef(0.8, 0.8, 0.8);
// 		printf("Calling glBegin() level %ld at %s:%ld\n", ++glBeginDepth, __FILE__, __LINE__); 
 		glBegin(GL_LINE_LOOP);
 		for (j = 0; j < N_CIRCLE_STEPS; j++)
	 		glVertex3f(sin(2 * PI * j / N_CIRCLE_STEPS), cos(2 * PI * j / N_CIRCLE_STEPS), 0.5);
// 		printf("Calling glEnd()   level %ld at %s:%ld\n", glBeginDepth--, __FILE__, __LINE__);
 		glEnd();
	
		glMultMatrixf(rotMat);												//	multiply by this matrix

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
		glPopMatrix();														//	revert to the old matrix
		glPopAttrib();														//	pop attributes back off stack

		} // draw sphere instead

//	printf("Done with display. . .\n");
	field->ComputeStatistics();fflush(stdout);
//	glutPostRedisplay();
	} // Display()

void IsosurfacePane::MakeCellEdgeDisplayList()									//	makes the display list for cell edges
	{ // MakeCellEdgeDisplayList()
	glNewList(cellEdgeDisplayList, GL_COMPILE);									//	start writing to the list
	if ((showCells) && (field != NULL))										//	and only if we are showing cells
		field->DrawEdges();													//	do we add the edges to the list
	glEndList();															//	stop writing to the list	
	recomputeCellEdgeDisplayList = false;										//	turn it off, for now
	} // MakeCellEdgeDisplayList()

void IsosurfacePane::SetUpMatrices()											//	sets up rendering matrices
	{ // SetUpMatrices()
	glMatrixMode(GL_PROJECTION);												//	first set up matrix for the viewing volume
	glLoadIdentity();														//	reset to the identity matrix
	float powScale = pow(10.0, scale + 0.25);									//	apply exponential scale
	
	if (width > height)
		glOrtho(-width/height, width/height, -1.0, 1.0, 0.001, 4.0);				//	set to orthogonal rendering
	else
		glOrtho(-1.0, 1.0, -height/width, height/width, 0.001, 4.0);				//	set to orthogonal rendering
	glTranslatef(0.0, 0.0, -2.0);												//	translate the projection matrix	

	glScalef((float) powScale / field->Size(), (float) powScale / field->Size(), zScale * (float) powScale / field->Size());
																		//	scale to place things in view
	glMatrixMode(GL_MODELVIEW);												//	switch to matrix for basis vectors at eye
	glLoadIdentity();
	glTranslatef((float) -offset_x * field->Size() / 2, (float) -offset_y*field->Size() / 2, 0);							/*	offset the position									*/
	glMultMatrixf(rotMat);													//	apply the rotation matrix	
	gluLookAt(2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0);
	glShadeModel(GL_SMOOTH);													//	use smooth lighting
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(lightMat);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);							// 	specify the position of the light
	glPopMatrix();
	glEnable(GL_LIGHT0);													// 	switch light #0 on
	glEnable(GL_LIGHTING);													//	switch lighting on
	glEnable(GL_DEPTH_TEST);													//	make sure depth buffer is switched on
	glClear(GL_DEPTH_BUFFER_BIT);												//	clear the depth buffer
	
	glTranslatef((float) -0.5*(field->XDim()-1), (float) -0.5*(field->YDim()-1), -0.5*(field->ZDim()-1));
																		//	place the data set dead centre
	glScalef(zScale, 1.0, 1.0);		/* note that z is actually x in my implementation */
	} // SetUpMatrices()

void IsosurfacePane::SetArcBalls(GLUTArcBall *newRotator, GLUTArcBall *newLightRotator)	//	set the arcballs to use	
	{ // SetArcBalls()
	rotator = newRotator; lightRotator = newLightRotator;							//	remember them
	} // SetArcBalls()
	
void IsosurfacePane::Mouse(int button, int state, int x, int y)							//	mouse down / up routine 
	{ // Mouse()
	if (glutGetModifiers() && GLUT_ACTIVE_SHIFT) button = GLUT_RIGHT_BUTTON;
	if (state == GLUT_DOWN)												//	if anywhere in the pane
		{ // mouse-down 
		dragButton = button;												//	store the button that was clicked
		switch (dragButton)												//	and then choose what to do
			{ // switch on which button
			case GLUT_RIGHT_BUTTON:
//				timingBuffer += sprintf(timingBuffer, "Right button: Picking.");
				if (field->selectionRoot != noContourSelected)					//	if there is already a selection
					{ // selection exists
					field->CommitSelection();								//	commit the selection
					} // selection exists

				selectionColour = -1;										//	set the selection colour to "none"
				field->selectionRoot = PickComponent(x, y);						//	pick the component under the pointer
//				printf("Contour ID: %ld\n", contourID);
				if (field->selectionRoot != noContourSelected)					//	i.e. contour selected
					{ // component selected
					field->SelectComponent(field->selectionRoot);			//	select the component we clicked on
					isoValue = (field->currentSelectionValue - field->MinHeight()) / (field->MaxHeight() - field->MinHeight());
																	//	reset the slider's isovalue
					selectionColour = field->GetSuperarcColour(field->selectionRoot);
					} // component selected
				glutPostRedisplay();										//	force a redisplay
				break;
			case GLUT_MIDDLE_BUTTON:
//				timingBuffer += sprintf(timingBuffer, "Middle button: Translating.");
//				whichArcBall = lightRotator;
				AcquireDragFocus(this);									//	acquire focus
//				whichArcBall->BeginDrag((2.0*x - width)/width, (2.0*y - height)/height);//	scale to arcball pane & start dragging
//				AcquireDragFocus(this);									//	acquire focus
				dragOld_x = x; dragOld_y = y;								//	set the initial coordinates of the drag						
//				printf("Dragging offset: %f %f Starting at: %f, %f\n", offset_x, offset_y, dragOld_x, dragOld_y);
				glutPostRedisplay();
				break;
			case GLUT_LEFT_BUTTON:
//				timingBuffer += sprintf(timingBuffer, "Left button: Rotating.");
				whichArcBall = rotator;
				AcquireDragFocus(this);									//	acquire focus
				whichArcBall->BeginDrag((2.0*x - width)/width, (2.0*y - height)/height);//	scale to arcball pane & start dragging
				break;
			} // switch on which button
		} // mouse-down 
	else if ((state == GLUT_UP) && (hasDragFocus))								//	if we were dragging in the pane, and it's mouse-up
		{ // mouse-up when dragging checkmark
//		timingBuffer += sprintf(timingBuffer, "Mouse released.");
		ReleaseDragFocus();												//	release drag focus
		if (dragButton == GLUT_LEFT_BUTTON)
			{ // left drag
			whichArcBall->EndDrag();										//	and call routine to tidy up
			whichArcBall = NULL;											//	reset the arcball for safety
			} // left drag
		} // mouse-up when dragging checkmark
	} // Mouse()
	
void IsosurfacePane::Motion(int x, int y)										//	mouse motion routine	
	{ // Motion()
	if (dragButton == GLUT_RIGHT_BUTTON) return;							//	if button selects a component, return now
	if (dragButton == GLUT_MIDDLE_BUTTON)
		{ // middle drag
//		timingBuffer += sprintf(timingBuffer, "Motion: translation.");
			float mhw = (height < width) ? height : width;							//	find the smaller of the two values						
			offset_x -= (x - dragOld_x)  * 2.0 / mhw;								//	change the x offset									
			offset_y -= (y - dragOld_y) * 2.0 / mhw;								//	and the y offset										
			dragOld_x = x; dragOld_y = y;										//	and update the stored values							
//		printf("New offset: %f %f New Starting Point: %f, %f\n", offset_x, offset_y, dragOld_x, dragOld_y);
		glutPostRedisplay();												//	force a redisplay
		} // middle drag
	else if (dragButton == GLUT_LEFT_BUTTON)
		{ // left drag
//		timingBuffer += sprintf(timingBuffer, "Motion: rotation.");
		whichArcBall->ContinueDrag((2.0 * x - width)/width, (2.0 * y - height)/height);	//	in both dimensions
		} // left drag
	} // Motion()

void IsosurfacePane::UpdateHeightFieldComponents()								//	updates the components of the height field
	{ // UpdateHeightFieldComponents()

	if (field->selectionRoot == noContourSelected)								//	no contours selected
		{ // no contours selected
//		timingBuffer += sprintf(timingBuffer, "Isovalue change.  Creating %s.", useLocalContours ? "local contours" : "level set");
		if (useLocalContours)
			field->SetLocal(isoValue);										//	create a set of local contours
		else
			field->SetIsosurface(field->MinHeight() + isoValue * (field->MaxHeight() - field->MinHeight()));
		} // no contours selected
	else // i.e. contours are selected
		{
		if (useLocalContours)
			field->ProportionalUpdateSelection(isoValue);
		else
//		timingBuffer += sprintf(timingBuffer, "Isovalue change.  Changing isovalue of selection.");
			field->UpdateSelection(field->MinHeight() + isoValue * (field->MaxHeight() - field->MinHeight()));	
		}
																	//	update the selection	
	isoSliderChanged = false;												//	reset flag controlling slider
	} // UpdateHeightFieldComponents()

void IsosurfacePane::DrawAxes()												//	routine to draw axes
	{ // DrawAxes()
	float fieldSize = field->Size();											//	retrieve the field's size
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blackColour);				//	edges don't reflect
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);						//	but they do emit black
	glLineWidth(2.0);
	glBegin(GL_LINES);														//	start drawing lines
	glVertex3f(0.0, 0.0, 0.0);	glVertex3f(fieldSize + 1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);	glVertex3f(0.0, fieldSize + 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);	glVertex3f(0.0, 0.0, fieldSize + 1.0);
	glEnd();
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);						//	but they do emit
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, whiteColour);				//	set reflective colour
	glMatrixMode(GL_MODELVIEW);												//	switch to modelview
	glPushMatrix();														//	push a matrix on the stack
	glTranslatef(0.0, 0.0, fieldSize + 1.0);									//	move to end of z axis
	glutSolidCone(0.02*fieldSize, 0.1*fieldSize, 10, 2);							//	draw a solid cone for the z-axis
	glRotatef(90.0, 0.0, 1.0, 0.0);											//	rotate to draw in z=0 plane
	glTranslatef(0.0, 0.1*fieldSize, 0.0);										//	move away slightly
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blackColour);				//	edges don't reflect
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);						//	but they do emit
	glScalef(0.001*fieldSize, 0.001*fieldSize, 0.001*fieldSize);					//	scale down
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'Z');									//	and draw the letter Z
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);						//	but they do emit
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, whiteColour);				//	set reflective colour
	glPopMatrix();															//	pop matrix off stack
	glPushMatrix();														//	push a matrix on the stack
	glTranslatef(0.0, fieldSize + 1.0, 0.0);									//	move to end of y axis
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.02*fieldSize, 0.1*fieldSize, 10, 2);							//	draw a solid cone for the z-axis
	glRotatef(90.0, 1.0, 0.0, 0.0);											//	rotate to draw in y=0 plane
	glTranslatef(0.1*fieldSize, 0.0, 0.0);										//	move away slightly
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blackColour);				//	edges don't reflect
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);						//	but they do emit
	glScalef(0.001*fieldSize, 0.001*fieldSize, 0.001*fieldSize);					//	scale down
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'Y');									//	and draw the letter Z
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);						//	but they do emit
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, whiteColour);				//	set reflective colour
	glPopMatrix();															//	pop matrix off stack
	glPushMatrix();														//	push a matrix on the stack
	glTranslatef(fieldSize + 1.0, 0.0, 0.0);									//	move to end of x axis
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.02*fieldSize, 0.1*fieldSize, 10, 2);							//	draw a solid cone for the x-axis
	glRotatef(90.0, 0.0, 1.0, 0.0);											//	rotate to draw in x=0 plane
	glTranslatef(0.0, 0.1*fieldSize, 0.0);										//	move away slightly
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blackColour);				//	edges don't reflect
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);						//	but they do emit
	glScalef(0.001*fieldSize, 0.001*fieldSize, 0.001*fieldSize);					//	scale down
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'X');									//	and draw the letter X
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blackColour);						//	but they do emit
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, whiteColour);				//	set reflective colour
	glPopMatrix();															//	pop matrix off stack
	glLineWidth(1.0);
	} // DrawAxes()

long IsosurfacePane::PickComponent(int x, int y)									//	picks a component	
	{ // PickComponent()
	long componentID = 0;												//	ID of the component
	GLubyte rgbvals[4];													//	local memory in which to place pixel
//	GLfloat mNow[16];													//	local matrix for ball routines

// 	printf("Entering PickComponent()\n"); fflush(stdout);
// 	WriteToFile();
	
	glMatrixMode(GL_PROJECTION);										//	switch to projection matrix
	glPushMatrix();														//	and provide a copy of it
	glMatrixMode(GL_MODELVIEW);										//	switch to modelview matrix
	glPushMatrix();														//	and provide a copy of it
	glViewport(left, bottom, width, height);									//	set the viewport

	glDisable(GL_LIGHTING);												//	disable lighting
	glEnable(GL_DEPTH);
	glClearColor(1.0, 1.0, 1.0, 1.0);											//	set background colour to white
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					//	clear the window to white

	glMatrixMode(GL_PROJECTION);										//	first set up matrix for the viewing volume
	glLoadIdentity();													//	reset to the identity matrix
	float powScale = pow(10.0, scale + 0.25);										//	apply exponential scale
	
	if (width > height)
		glOrtho(-width/height, width/height, -1.0, 1.0, 0.001, 4.0);					//	set to orthogonal rendering
	else	
		glOrtho(-1.0, 1.0, -height/width, height/width, 0.001, 4.0);					//	set to orthogonal rendering
	glTranslatef(0.0, 0.0, -2.0);												//	translate the projection matrix	

	glScalef((float) powScale / field->Size(), (float) powScale / field->Size(), (float) powScale / field->Size());

	glMatrixMode(GL_MODELVIEW);										//	switch to matrix for basis vectors at eye
	glPushMatrix();														//	and provide a copy of it
	glLoadIdentity();
	glMultMatrixf(rotMat);												//	apply the rotation matrix	
	gluLookAt(2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0);
	glTranslatef((float) -0.5*(field->XDim()-1), (float) -0.5*(field->YDim()-1), -0.5*(field->ZDim()-1));
	field->FlexibleContours(useDisplayLists, true);								//	draw the current set of contours for selection
	glPopMatrix();														// 	revert to the old matrix

// 	printf("Dumping to file: \n");
// 	WriteToFile();
// 	
// 	printf("Reading from window coords (%ld, %ld)\n", left + x, bottom + y);
	glReadPixels(left + x, bottom + y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgbvals);		//	read the pixel colour
	componentID = (rgbvals[0] + 256*(rgbvals[1] + 256*rgbvals[2]));					//	compute the component ID
	if (componentID == 16777215) componentID = noContourSelected;					//	i.e. we didn't find any
// 	printf("Component ID is: %ld\n", componentID);
// 	printf("rgbvals:  %8X\n", rgbvals);
	glEnable(GL_LIGHTING);												//	reenable lighting
	glMatrixMode(GL_PROJECTION);										//	switch to projection matrix
	glPopMatrix();														//	and revert to previous copy
	glMatrixMode(GL_MODELVIEW);										//	switch to modelview matrix
	glPopMatrix();														//	and revert to previous copy
	return componentID;													//	and return the ID
	} // PickComponent()

void IsosurfacePane::Action(int action)											//	invoked by buttons, with the desired action value
	{ // Action()
	switch(action)
		{ // switch on action
		case Action_Quit:												//	quit the ugly way
			exit(0);
			break;
		case Action_Delete_Selection:
//			timingBuffer += sprintf(timingBuffer, "Deleting selection.");
			field->DeleteSelection();
			break;
		case Action_Delete_Complement:
//			timingBuffer += sprintf(timingBuffer, "Deleting complement.");
			field->DeleteComplement();
			break;
		case Action_Restore_Deleted:
//			timingBuffer += sprintf(timingBuffer, "Restoring deleted contours.");
			field->RestoreDeleted();
			break;
		case Action_Collapse:
			field->SingleCollapse();
			break;
		case Action_Uncollapse:
			field->SingleUnCollapse();
			break;
		case Action_Select_Colour:
			field->SetSuperarcColour(field->selectionRoot, selectionColour);
//			printf("Colour %d chosen.\n", selectionColour);
			break;
		case Action_Dot_Layout:
			field->DoDotLayout();
			break;
		case Action_Save_Layout:
			field->SaveTreeLayout();
			break;
		case Action_Load_Layout:
			field->LoadTreeLayout();
			break;			
		case Action_Increment:
		  //printf("isovalue was: %f (%f)\n", isoValue, field->MinHeight() + isoValue * (field->MaxHeight() - field->MinHeight()));
 		        isoValue += 1.0 / (field->MaxHeight() - field->MinHeight());
			//printf("isovalue was: %f (%f)\n", isoValue, field->MinHeight() + isoValue * (field->MaxHeight() - field->MinHeight()));
		        isoSliderChanged = true;	
                        break;			
		case Action_Decrement:
		  //printf("isovalue was: %f (%f)\n", isoValue, field->MinHeight() + isoValue * (field->MaxHeight() - field->MinHeight()));

		        isoValue -= 1.0 / (field->MaxHeight() - field->MinHeight());
			//printf("isovalue was: %f (%f)\n", isoValue, field->MinHeight() + isoValue * (field->MaxHeight() - field->MinHeight()));
		        isoSliderChanged = true;	
                        break;			

		} // switch on action
	glutPostRedisplay();
	} // Action()
