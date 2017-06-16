///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	IsosurfacePane.h
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
///////////////////////////////////////////////////

#ifndef _HEADER_IsosurfacePane
#define _HEADER_IsosurfacePane

#include "GLUTWindow.h"													//	the GLUTWindow class
#include "HeightField.h"													//	the HeightField class
#include "GLUTArcBall.h"

class IsosurfacePane : public GLUTPane										//	pane with an isosurface
	{ // class IsosurfacePane
	public:	
	enum {Action_Quit, Action_Delete_Selection, Action_Delete_Complement, Action_Restore_Deleted,
		Action_Collapse, Action_Uncollapse, Action_Select_Colour, Action_Dot_Layout, Action_Save_Layout, Action_Load_Layout,
	        Action_Increment, Action_Decrement};
	HeightField *field;													//	the height field
	float scale;														//	viewing scale
	float zScale;														//	scale of z-dimension (slices)
	float isoValue;													//	value of the isosurface in view
	bool recomputeCellEdgeDisplayList;										//	flag set by controls when necessary to rebuild
	bool showCells;													//	whether to show the cells
	bool showLevelSet;													//	whether to show the level set
	bool showAxes;														//	whether to show the XYZ axes
	bool isoSliderChanged;												//	whether the position of the isovalue slider changed
	bool updateFromPriorityBound;											//	used to flag pending changes to the tree
	bool updateFromTreeSize;												//	likewise
	bool useLocalContours;												//	flag for what the slider controls
	bool useMarchingCubes;												//	flag for marching cubes instead of following
	bool useDisplayLists;												//	whether to use display lists
	int dragButton;													//	which button is active for dragging
	float offset_x, offset_y;											//	allows us to move the object around laterally				
	float dragOld_x, dragOld_y;											//	saved position for dragging
	bool dragOrScale;													//	flag for what middle button does
	long selectionColour;												//	used for choosing colours
	long collapsePriority;												//	collapse priority
	bool recollapse;													//	flag to force recollapse
  
  	GLfloat rotMat[16], lightMat[16];										//	rotation matrices for object, light, respectively
	GLUTArcBall *rotator, *lightRotator;									//	pointers to two rotation arcballs (default NULL)
	GLUTArcBall *whichArcBall;											//	which arcball is actively being manipulated
	
	int cellEdgeDisplayList;												//	ID's of the display lists

	IsosurfacePane(GLUTPane *Parent, int Width, int Height, int argc, char **argv, bool GrowHoriz = false, bool GrowVert = false);
																	//	constructor 
	~IsosurfacePane();													//	destructor
																	
	virtual void Action(int action);										//	function invoked by buttons																
	void UpdateHeightFieldComponents();									//	updates the components of the height field

	virtual void Display();												//	display routine																	
	void DrawAxes();													//	routine to draw axes
	void DrawCells();													//	routine to draw cells
	void DrawSurfaces();												//	routine to draw surfaces
	long PickComponent(int x, int y);										//	picks a component	
	
	void MakeCellEdgeDisplayList();										//	makes the display list for cell edges
	void SetUpMatrices();												//	sets up rendering matrices
	void SetArcBalls(GLUTArcBall *newRotator, GLUTArcBall *newLightRotator);		//	set the arcballs to use	
	virtual void Mouse(int button, int state, int x, int y);					//	mouse down / up routine 
	virtual void Motion(int x, int y);										//	mouse motion routine	
	}; // class IsosurfacePane

#endif
