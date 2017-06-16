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

#ifndef _HEADER_GLUTRadioGroup
#define _HEADER_GLUTRadioGroup

class GLUTRadioGroup;														//	forward declaration

#include "GLUTVertPane.h"
#include "GLUTLabelledRadioButton.h"

class GLUTRadioGroup : public GLUTVertWhitePane									//	class for a RadioGroup with string
	{ // class GLUTRadioGroup
	public:
	long *target;															//	pointer to a variable elsewhere to update
	bool *dirty;															//	pointer to flag marking whether target was updated
	GLUTRadioButton *chosenButton;											//	pointer to the chosen button

  int *values;                                        //  the values used to set the buttons initially
  GLUTLabelledRadioButton **buttons;                           //  pointer to the buttons
  int NButtons;                                       // count of how many
  
  GLUTRadioGroup(GLUTPane *Parent, char *name, int nButtons, char **names, int *values,
		int initialChoice = 0, long *Target = NULL, bool *Dirty = NULL);				//	constructor
  ~GLUTRadioGroup();  // destructor

    
  void Update(GLUTRadioButton *newChoice);									//	updates any target
  void ReflectChange(int newValue);         //  resets the chosen button to a particular value
  virtual void PrintType();												//	prints text string for type
	}; // class GLUTRadioGroup

#endif
