///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTRadioGroup.h
//	------------------------
//	
//	A LabelledRadioButton with text
//
//	Notes:
//	------
//
//
//	Change log:
//	-----------
//	Date:		Changes:
//	-----		--------
//	02/08/01		Initial version
//
///////////////////////////////////////////////////	

#include "GLUTRadioGroup.h"
#include "GLUTStaticText.h"
#include "GLUTString.h"
#include <stdio.h>
#include <GL/freeglut.h>

GLUTRadioGroup::GLUTRadioGroup(GLUTPane *Parent, char *name, int nButtons, char **buttonNames, int *buttonValues, int initialChoice, 
	long *Target, bool *Dirty)
	: GLUTVertWhitePane(Parent, 0, 0, false, false, ALIGN_HORIZONTAL_LEFT)
	{ // constructor
	int button;																	//	loop index
	int maxStrLen = 0;																//	maximum string length
	int stringHeight = 2 * STATIC_TEXT_BOUNDARY + StringHeight(name);						//	compute the string height
	
	for (button = 0; button < nButtons; button++)										//	walk along string
		if (StringLength(buttonNames[button]) > maxStrLen)								//	if the length beats the longest yet
			maxStrLen = StringLength(buttonNames[button]);								//	update the length
			
	width = GLUT_RADIO_BUTTON_TOTAL_SIZE + 2 * STATIC_TEXT_BOUNDARY + maxStrLen;				//	set the width directly
	height = stringHeight + nButtons * ((GLUT_RADIO_BUTTON_TOTAL_SIZE > stringHeight) ? GLUT_RADIO_BUTTON_TOTAL_SIZE : stringHeight);
																				//	and the height

//	AddChild(new GLUTStaticText(this, name));											//	add the name label
	new GLUTStaticText(this, name);													//	add the name label

	target = Target;																//	set the target
	if (target != NULL) 															//	if it exists
		*target = buttonValues[initialChoice];											//	set the initial value
	dirty = Dirty;																	//	set the pointer to the dirty flag
	if (dirty != NULL)																//	if it exists
		*dirty = true;																//	set it

  NButtons = nButtons;
  values = (int *) malloc(sizeof(int) * nButtons);
  buttons = (GLUTLabelledRadioButton **) malloc(sizeof(GLUTRadioButton *) * nButtons);
     
	chosenButton = NULL;															//	be paranoid
	for (button = 0; button < nButtons; button++)										//	walk along string
		{
//		if (button == initialChoice)													//	if it's chosen
//			AddChild(new GLUTLabelledRadioButton(this, buttonNames[button], buttonValues[button], true));
//		else
//			AddChild(new GLUTLabelledRadioButton(this, buttonNames[button], buttonValues[button], false));
    values[button] = buttonValues[button];
    if (button == initialChoice)													//	if it's chosen
			buttons[button] = new GLUTLabelledRadioButton(this, buttonNames[button], buttonValues[button], true);
		else
			buttons[button] = new GLUTLabelledRadioButton(this, buttonNames[button], buttonValues[button], false);
    } // for each button
  } // constructor	
		
GLUTRadioGroup::~GLUTRadioGroup()
  { // destructor
  free(values);  free(buttons);
  } // destructor
    

void GLUTRadioGroup::Update(GLUTRadioButton *newChoice)									//	updates any target
	{ // Update()
	if (chosenButton != NULL)														//	if there is an old choice
		chosenButton->chosen = false;													//	tell it it is not chosen
	chosenButton = newChoice;														//	store the new one
		chosenButton->chosen = true;													//	make sure the new one knows
	if (target != NULL)																//	if there is a target
		*target = newChoice->buttonValue;												//	set it
	if (dirty != NULL)																//	if there is a dirty flag
		*dirty = true;																//	set it, too
	} // Update()

void GLUTRadioGroup::ReflectChange(int newValue)                   //  resets the chosen button to a particular value
   { // ReflectChange()
   for (int button = 0; button < NButtons; button++)
     if (values[button] == newValue)
       { // matched a value
       chosenButton->chosen = false;            //  turn off the old one
       chosenButton = buttons[button]->theButton;          //  select the new one
       chosenButton->chosen = true;              // turn on the new one
       glutPostRedisplay();                     // and force a redisplay
       } // matched a value
   } // ReflectChange()
   	
void GLUTRadioGroup::PrintType()														//	prints text string for type
	{ // PrintType()
	printf("GLUTRadioGroup");
	} // PrintType()	
	
