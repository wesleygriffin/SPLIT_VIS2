/*	74.225 Intersession 1998 					*/
/*	Debug malloc & free		 					*/
/*	Hamish Carr, 1998							*/

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG_MALLOC
#define malloc(x) Malloc(x, __FILE__, __LINE__)
#define free(x) Free(x, __FILE__, __LINE__)
#define calloc(x, y) Calloc(x, y, __FILE__, __LINE__)
#define realloc(x, y) Realloc(x, y, __FILE__, __LINE__)

void *Malloc(size_t size, char *fileName, long fileLine);				/*	version of malloc() with debug	*/
void Free(void *ptr, char *fileName, long fileLine);					/*	version of free() with debug		*/
void *Calloc(size_t nelem, size_t size, char *fileName, long fileLine);	/*	version of calloc() with debug	*/
void *Realloc(void *ptr, size_t size, char *fileName, long fileLine);		/*	version of realloc() with debug	*/
void FreeStub(void *ptr, char *fileName, long fileLine);				/*	stub version of free() 			*/

void Malloc_Report();											/*	reports at end of program		*/
#endif
