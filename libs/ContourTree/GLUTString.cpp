///////////////////////////////////////////////////
//
//	Contour-tree based isosurfaces
//	version 3.0
//	copyright Hamish Carr
//	February, 2001
//
//	------------------------
//	GLUTString.h
//	------------------------
//	
//	Routines for printing strings
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

#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>
#include "GLUTString.h"											//	the corresponding header

#define theBitmap GLUT_BITMAP_TIMES_ROMAN_10
#define theStroke GLUT_STROKE_ROMAN

void PrintString(char *s)										//	call this to print a string
	{ // PrintString()
	for (unsigned int i = 0; i < strlen(s); i++)							//	loop through the characters
		glutBitmapCharacter(theBitmap, s[i]);						//	drawing them
	} // PrintString()

int StringHeight(char *s)										//	returns the height of the string
	{ // StringHeight()
	return 10;
	} // StringHeight()

int StringLength(char *s)										//	returns the length of the string
	{ // StringLength()
	int slen = 0;												//	total length of string
	for (unsigned int i = 0; i < strlen(s); i++)							//	loop through the characters
		slen += glutBitmapWidth(theBitmap, s[i]);					//	summing the widths
	return slen;
	} // StringLength()


