///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	IsosurfaceWindow.cpp
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

#include "IsosurfaceWindow.h"												//	the GLUTWindow class
#include "IsosurfacePane.h"
#include "GLUTToolbox.h"
#include "ContourTreePane.h"
#include "LogCollapsePane.h"
#include "ColourSelectionPane.h"

#define INITIAL_HEIGHT 600
#define INITIAL_WIDTH (ISO_PANE_WIDTH + CONTOUR_PANEL_WIDTH + LOG_COLLAPSE_PANEL_WIDTH)
#define CONTOUR_PANEL_WIDTH (CONTOUR_PANE_WIDTH + VERT_SLIDER_WIDTH)
#define CONTOUR_PANE_WIDTH 300
#define CONTOUR_PANE_HEIGHT ISO_PANE_HEIGHT
#define VERT_SLIDER_HEIGHT ISO_PANE_HEIGHT - 12
#define VERT_SLIDER_WIDTH 20
#define ISO_PANE_WIDTH 480
#define ISO_PANE_HEIGHT ISO_PANE_WIDTH
#define LABELLED_ROTATOR_HEIGHT 70
#define LABELLED_ROTATOR_WIDTH 50
#define ZOOM_SLIDER_WIDTH (ISO_PANE_WIDTH-2*LABELLED_ROTATOR_WIDTH-120)
#define ZOOM_SLIDER_HEIGHT 20
#define LOG_COLLAPSE_PANEL_WIDTH 200
#define LOG_COLLAPSE_PANEL_HEIGHT (ISO_PANE_HEIGHT - 60 + SLIDER_SIZE)
#define SLIDER_SIZE 35
#define SLIDER_OVERHANG 10
#define LOG_COLLAPSE_PANE_HEIGHT (LOG_COLLAPSE_PANEL_HEIGHT - SLIDER_SIZE - 2*SLIDER_OVERHANG)
#define LOG_COLLAPSE_PANE_WIDTH (LOG_COLLAPSE_PANEL_WIDTH - SLIDER_SIZE - 2*SLIDER_OVERHANG)
#define COLLAPSE_BUTTON_HEIGHT 30
#define LOG_COLLAPSE_TEXT_PANEL_HEIGHT (INITIAL_HEIGHT - LOG_COLLAPSE_PANEL_HEIGHT - COLLAPSE_BUTTON_HEIGHT)

char *priorityNames[] = { "Height", "Volume", "Height * Volume", "Riemann Hypervolume"}; 
int priorityValues[] = { PRIORITY_HEIGHT, PRIORITY_VOLUME, PRIORITY_HYPERVOLUME, PRIORITY_APPROX_HYPERVOLUME};

IsosurfaceWindow::IsosurfaceWindow(int argc, char **argv, char *windowName)
	: GLUTWindow(windowName, INITIAL_WIDTH, INITIAL_HEIGHT, false, false)				//	constructor : everything else should be in the
	{ // IsosurfaceWindow()													//	IsosurfacePane
	GLUTPane *majorVPane, *minorHPane, *minorVPane;								//	temporary pointers to panes
	IsosurfacePane *theIsoPane;												//	pointer to the isosurface pane

	//	first vertical column: isosurface pane plus arcballs
	majorVPane = new GLUTVertWhitePane(this, ISO_PANE_WIDTH, INITIAL_HEIGHT, false, false, ALIGN_HORIZONTAL_CENTRE);
																		//	put a 2d vertical pane in window
	theIsoPane = new IsosurfacePane(majorVPane, ISO_PANE_WIDTH, ISO_PANE_HEIGHT, argc, argv, false, false);	
																		//	put an isosurface pane inside the window

	minorHPane = new GLUTHorizWhitePane(majorVPane, ISO_PANE_WIDTH, LABELLED_ROTATOR_HEIGHT, false, false, ALIGN_VERTICAL_CENTRE);
	minorVPane = new GLUTVertPane(minorHPane, LABELLED_ROTATOR_WIDTH, LABELLED_ROTATOR_HEIGHT, false, false, ALIGN_HORIZONTAL_LEFT);
	GLUTArcBall *rotator = new GLUTArcBall(minorVPane, (theIsoPane->rotMat));			//	put an arcball in it
	new GLUTStaticText(minorVPane, "Rotation");									//	with a suitable label
	minorVPane = new GLUTVertPane(minorHPane, LABELLED_ROTATOR_WIDTH, LABELLED_ROTATOR_HEIGHT, false, false, ALIGN_HORIZONTAL_LEFT);
	GLUTArcBall *lightRotator = new GLUTArcBall(minorVPane, (theIsoPane->lightMat));	//	a second arcball for light position
	new GLUTStaticText(minorVPane, "Light");									//	with a suitable label
	theIsoPane->SetArcBalls(rotator, lightRotator);								//	set the arcballs
	minorVPane = new GLUTVertPane(minorHPane, ZOOM_SLIDER_WIDTH+20, ZOOM_SLIDER_HEIGHT, false, false, ALIGN_HORIZONTAL_CENTRE);
	GLUTPane *minPane = new GLUTHorizPane(minorVPane, ZOOM_SLIDER_WIDTH, ZOOM_SLIDER_HEIGHT, false, false, ALIGN_VERTICAL_CENTRE);
	new GLUTHorizSlider(minPane, -2.0, 2.0, ZOOM_SLIDER_WIDTH, 0.0, &(theIsoPane->scale), NULL);//	add a slider for dealing with zoom 
	new GLUTStaticText(minPane, "Zoom (log scale)");

	minPane = new GLUTHorizPane(minorVPane, ZOOM_SLIDER_WIDTH, ZOOM_SLIDER_HEIGHT, false, false, ALIGN_VERTICAL_CENTRE);
	new GLUTHorizSlider(minPane, 0.1, 4.0, ZOOM_SLIDER_WIDTH, 1.5, &(theIsoPane->zScale), NULL);//	add a slider for dealing with zoom 
	new GLUTStaticText(minPane, "Z - scale");
	new GLUTPane(minorVPane, 1, 1, false, true);								//	put a stretchy pane in to push buttons to top	
	minorHPane = new GLUTHorizWhitePane(majorVPane, ISO_PANE_WIDTH, 50, false, true, ALIGN_VERTICAL_CENTRE);
																		//	add a "stretchy pane" to push them up
	minorVPane = new GLUTVertPane(minorHPane, 120, 50, false, false, ALIGN_HORIZONTAL_LEFT);
	new GLUTCheckBox(minorVPane, "Show Contours", &(theIsoPane->showLevelSet), NULL, false);
	new GLUTCheckBox(minorVPane, "Coloured Contours", &(theIsoPane->field->differentColouredContours), &(theIsoPane->isoSliderChanged), true);
	new GLUTPane(minorVPane, 1, 1, false, true);								//	put a stretchy pane in to push buttons to top
	GLUTPane *secondCheckBoxPane = new GLUTVertPane(minorHPane, 120, 50, false, false, ALIGN_HORIZONTAL_LEFT);

	minorVPane = new GLUTVertPane(minorHPane, 120, 50, false, false, ALIGN_HORIZONTAL_LEFT);
	new GLUTTextButton(minorVPane, "Delete Selection", theIsoPane, IsosurfacePane::Action_Delete_Selection);
	new GLUTTextButton(minorVPane, "Delete Complement", theIsoPane, IsosurfacePane::Action_Delete_Complement);
	new GLUTPane(minorVPane, 1, 1, false, true);								//	put a stretchy pane in to push buttons to top
	minorVPane = new GLUTVertPane(minorHPane, 120, 50, false, false, ALIGN_HORIZONTAL_LEFT);
	new GLUTTextButton(minorVPane, "Restore Deleted", theIsoPane, IsosurfacePane::Action_Restore_Deleted);
	new GLUTTextButton(minorVPane, "Quit", theIsoPane, IsosurfacePane::Action_Quit);
	new GLUTPane(minorVPane, 1, 1, false, true);								//	put a stretchy pane in to push buttons to top	

	//	second vertical column: contour tree pane plus checkboxes / buttons
	majorVPane = new GLUTVertWhitePane(this, CONTOUR_PANEL_WIDTH, INITIAL_HEIGHT, false, false, ALIGN_HORIZONTAL_LEFT);
																		//	put a 2d vertical pane in window

	minorHPane = new GLUTHorizWhitePane(majorVPane, CONTOUR_PANEL_WIDTH, ISO_PANE_HEIGHT, false, false, ALIGN_VERTICAL_CENTRE);
																		//	put a horizontal backdrop inside that
	new GLUTVertSlider(minorHPane, 0.0, 1.0, VERT_SLIDER_HEIGHT, 0.90, &(theIsoPane->isoValue), &(theIsoPane->isoSliderChanged));
	
	ContourTreePane *theCTPane = new ContourTreePane(minorHPane, CONTOUR_PANE_WIDTH, CONTOUR_PANE_HEIGHT, 
		theIsoPane->field, &(theIsoPane->isoValue), &(theIsoPane->isoSliderChanged), theIsoPane, false, true);
																				//	create a contour tree pane	
	minorHPane = new GLUTHorizWhitePane(majorVPane, CONTOUR_PANEL_WIDTH, INITIAL_HEIGHT-CONTOUR_PANE_HEIGHT, false, false, ALIGN_VERTICAL_CENTRE);
	new GLUTVertPane(minorHPane, 40, INITIAL_HEIGHT-CONTOUR_PANE_HEIGHT, false, false, ALIGN_HORIZONTAL_LEFT);
	minorVPane = new GLUTVertPane(minorHPane, CONTOUR_PANEL_WIDTH - 40, INITIAL_HEIGHT-CONTOUR_PANE_HEIGHT, false, false, ALIGN_HORIZONTAL_LEFT);
	minorHPane = new GLUTHorizWhitePane(minorVPane, CONTOUR_PANEL_WIDTH-40, 20, false, false, ALIGN_VERTICAL_CENTRE);
//	new GLUTStaticText(minorHPane, "Contour Tree Display:");
	new GLUTCheckBox(minorHPane, "Show Tree", &(theCTPane->showTree), NULL, true);
	new GLUTCheckBox(minorHPane, "Colour Tree", &(theCTPane->useColouredTree), NULL, false);

	new GLUTStaticText(minorVPane, "Vertical slider: isovalue. Tags colour-coded to match surfaces.");
//	new GLUTStaticText(minorVPane, "Tags are colour-coded to match surfaces in main pane.");
	new ColourSelectionPane(minorVPane, theIsoPane, IsosurfacePane::Action_Select_Colour, &(theIsoPane->selectionColour));
//	new GLUTStaticText(minorVPane, "     Selection if one or more surfaces are selected.");
//	new GLUTStaticText(minorVPane, "     Level Set if no surfaces are selected.");
//	new GLUTStaticText(minorVPane, "     Upper Leaves if local contours are active.");
	minorHPane = new GLUTHorizWhitePane(minorVPane, CONTOUR_PANEL_WIDTH, 25, false, false, ALIGN_VERTICAL_CENTRE);
	new GLUTTextButton(minorHPane, "Dot Layout", theIsoPane, IsosurfacePane::Action_Dot_Layout);
	new GLUTTextButton(minorHPane, "Save Layout", theIsoPane, IsosurfacePane::Action_Save_Layout);
	new GLUTTextButton(minorHPane, "Load Layout", theIsoPane, IsosurfacePane::Action_Load_Layout);
	minorHPane = new GLUTHorizWhitePane(minorVPane, CONTOUR_PANEL_WIDTH, 25, false, false, ALIGN_VERTICAL_CENTRE);
	new GLUTTextButton(minorHPane, "Value++", theIsoPane, IsosurfacePane::Action_Increment);
	new GLUTTextButton(minorHPane, "Value--", theIsoPane, IsosurfacePane::Action_Decrement);
	
	new GLUTPane(minorVPane, 1, 1, false, true);								//	put a stretchy pane in to push buttons to top

	//	these have to go here so that the CTPane is defined already
	new GLUTCheckBox(secondCheckBoxPane, "Local Contours", &(theIsoPane->useLocalContours), &(theIsoPane->isoSliderChanged), false);
	new GLUTCheckBox(secondCheckBoxPane, "Display Lists", &(theIsoPane->useDisplayLists), &(theIsoPane->isoSliderChanged), false);
	new GLUTPane(secondCheckBoxPane, 1, 1, false, true);								//	put a stretchy pane in to push buttons to top
															
	//	third vertical column: log-collapse pane plus buttons
	majorVPane = new GLUTVertWhitePane(this, LOG_COLLAPSE_PANEL_WIDTH, INITIAL_HEIGHT, false, false, ALIGN_HORIZONTAL_LEFT);
																		//	put a 3d vertical pane in window
	minorHPane = new GLUTHorizWhitePane(majorVPane, LOG_COLLAPSE_PANEL_WIDTH, LOG_COLLAPSE_PANEL_HEIGHT - SLIDER_SIZE, false, false, ALIGN_VERTICAL_CENTRE);
																		//	put a horizontal backdrop inside that
	new GLUTPane(minorHPane, SLIDER_OVERHANG, 1, false, false);						//	make room to line up the horizontal slider
	minorVPane = new GLUTVertPane(minorHPane, LOG_COLLAPSE_PANE_WIDTH, LOG_COLLAPSE_PANE_HEIGHT + 2*SLIDER_OVERHANG, false, false, ALIGN_HORIZONTAL_CENTRE);
	new GLUTPane(minorVPane, 1, SLIDER_OVERHANG, false, false);						//	move it over by the amount of the overhang
	LogCollapsePane *theLCPane = new LogCollapsePane(minorVPane, LOG_COLLAPSE_PANE_WIDTH, LOG_COLLAPSE_PANE_HEIGHT, 
		theIsoPane->field, false, true);										//	create a log-collapse pane	
	new GLUTPane(minorVPane, 1, SLIDER_OVERHANG, false, false);						//	move it over by the amount of the overhang
	new GLUTPane(minorHPane, SLIDER_OVERHANG, 1, false, false);						//	move it over by the amount of the overhang
	new GLUTVertSlider(minorHPane, 0.0, 1.0, LOG_COLLAPSE_PANE_HEIGHT, theIsoPane->field->InitialLogPriorityBound(), &(theIsoPane->field->logPriorityBound), &(theIsoPane->updateFromPriorityBound));
	new GLUTHorizSlider(majorVPane, 0.0, 1.0, LOG_COLLAPSE_PANE_WIDTH, 0.0, &(theIsoPane->field->logTreeSize), &(theIsoPane->updateFromTreeSize));	
		
	minorHPane = new GLUTHorizWhitePane(majorVPane, LOG_COLLAPSE_PANEL_WIDTH, COLLAPSE_BUTTON_HEIGHT, false, false, ALIGN_VERTICAL_CENTRE);
	new GLUTPane(minorHPane, 1, 1, true, false);								//	put a stretchy pane in to push buttons to side
	new GLUTTextButton(minorHPane, "Collapse", theIsoPane, IsosurfacePane::Action_Collapse);
	new GLUTPane(minorHPane, 1, 1, true, false);								//	put a stretchy pane in to push buttons to side
	new GLUTTextButton(minorHPane, "Uncollapse", theIsoPane, IsosurfacePane::Action_Uncollapse);
	new GLUTPane(minorHPane, 1, 1, true, false);								//	put a stretchy pane in to push buttons to side
	
	minorHPane = new GLUTHorizWhitePane(majorVPane, LOG_COLLAPSE_PANEL_WIDTH, LOG_COLLAPSE_TEXT_PANEL_HEIGHT, false, false, ALIGN_VERTICAL_CENTRE);
	minorVPane = new GLUTVertPane(minorHPane, 40, LOG_COLLAPSE_TEXT_PANEL_HEIGHT, false, false, ALIGN_HORIZONTAL_LEFT);
//	minorVPane = new GLUTVertPane(minorHPane, LOG_COLLAPSE_PANEL_WIDTH - 40, LOG_COLLAPSE_TEXT_PANEL_HEIGHT, false, false, ALIGN_HORIZONTAL_LEFT);
//	new GLUTStaticText(minorVPane, "Log-log plot of tree collapse:");
//	new GLUTStaticText(minorVPane, "Vertical axis: log (smallest object):");
//	new GLUTStaticText(minorVPane, "Horizontal axis: log (size of tree):");
	GLUTRadioGroup *rGroupField = new GLUTRadioGroup(minorVPane, "Geometric Measure", 4, priorityNames, 
							priorityValues, PRIORITY_HEIGHT, &(theIsoPane->collapsePriority), 
							&(theIsoPane->recollapse));


	new GLUTPane(minorVPane, 1, 1, false, true);								//	put a stretchy pane in to push buttons to top
	} // IsosurfaceWindow()
