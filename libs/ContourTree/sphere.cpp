#include <GL/freeglut.h>

GLfloat sphereVert[84][3] = {	{	0,	0,	-1	},	
	{	0.5,	0,	-0.86603	},	
	{	0.86603,	0,	-0.5	},	
	{	1,	0,	0	},	
	{	0.86603,	0,	0.5	},	
	{	0.5,	0,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	0.43301,	0.25,	-0.86603	},	
	{	0.75,	0.43301,	-0.5	},	
	{	0.86603,	0.5,	0	},	
	{	0.75,	0.43301,	0.5	},	
	{	0.43301,	0.25,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	0.25,	0.43301,	-0.86603	},	
	{	0.43301,	0.75,	-0.5	},	
	{	0.5,	0.86603,	0	},	
	{	0.43301,	0.75,	0.5	},	
	{	0.25,	0.43301,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	0,	0.5,	-0.86603	},	
	{	0,	0.86603,	-0.5	},	
	{	0,	1,	0	},	
	{	0,	0.86603,	0.5	},	
	{	0,	0.5,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	-0.25,	0.43301,	-0.86603	},	
	{	-0.43301,	0.75,	-0.5	},	
	{	-0.5,	0.86603,	0	},	
	{	-0.43301,	0.75,	0.5	},	
	{	-0.25,	0.43301,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	-0.43301,	0.25,	-0.86603	},	
	{	-0.75,	0.43301,	-0.5	},	
	{	-0.86603,	0.5,	0	},	
	{	-0.75,	0.43301,	0.5	},	
	{	-0.43301,	0.25,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	-0.5,	0,	-0.86603	},	
	{	-0.86603,	0,	-0.5	},	
	{	-1,	0,	0	},	
	{	-0.86603,	0,	0.5	},	
	{	-0.5,	0,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	-0.43301,	-0.25,	-0.86603	},	
	{	-0.75,	-0.43301,	-0.5	},	
	{	-0.86603,	-0.5,	0	},	
	{	-0.75,	-0.43301,	0.5	},	
	{	-0.43301,	-0.25,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	-0.25,	-0.43301,	-0.86603	},	
	{	-0.43301,	-0.75,	-0.5	},	
	{	-0.5,	-0.86603,	0	},	
	{	-0.43301,	-0.75,	0.5	},	
	{	-0.25,	-0.43301,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	0,	-0.5,	-0.86603	},	
	{	0,	-0.86603,	-0.5	},	
	{	0,	-1,	0	},	
	{	0,	-0.86603,	0.5	},	
	{	0,	-0.5,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	0.25,	-0.43301,	-0.86603	},	
	{	0.43301,	-0.75,	-0.5	},	
	{	0.5,	-0.86603,	0	},	
	{	0.43301,	-0.75,	0.5	},	
	{	0.25,	-0.43301,	0.86603	},	
	{	0,	0,	1	},	
	{	0,	0,	-1	},	
	{	0.43301,	-0.25,	-0.86603	},	
	{	0.75,	-0.43301,	-0.5	},	
	{	0.86603,	-0.5,	0	},	
	{	0.75,	-0.43301,	0.5	},	
	{	0.43301,	-0.25,	0.86603	},	
	{	0,	0,	1	}	
	};
