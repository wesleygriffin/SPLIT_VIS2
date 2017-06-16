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


#ifndef _HEADER_GLUTString
#define _HEADER_GLUTString

void PrintString(char *s);										//	call this to print a string

int StringHeight(char *s);										//	returns the height of the string
int StringLength(char *s);										//	returns the length of the string

#endif