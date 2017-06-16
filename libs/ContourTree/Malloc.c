/*	74.225 Intersession 1998 					*/
/*	Debug malloc & free		 					*/
/*	Hamish Carr, 1998							*/

#include "Malloc.h"
#include <stdio.h>
#include <string.h>

#ifdef DEBUG_MALLOC
#undef malloc
#undef free
#undef calloc
#undef realloc
/*	this makes sure that these functions work properly in this unit					*/
#define DEBUG_MALLOC_FILE_NAME_LENGTH 256
typedef struct mem_node											/*	node in memory storage list		*/
	{
	long numBytes;												/*	number of bytes allocated		*/
	long sequence;												/*	sequence number of node			*/
	char fileName[DEBUG_MALLOC_FILE_NAME_LENGTH];					/*	name of file from which allocated	*/
	long fileLine;												/*	line number in file				*/
	struct mem_node *prev, *next;									/*	adjacent mem-nodes in list		*/
	} Mem_node;												/*	declare it as a type			*/
	
static long nextSequenceNo = 1;									/*	next sequence number for node	*/
static Mem_node theList = {0, 0, __FILE__, __LINE__, &theList, &theList};	/*	dummy node in our list			*/
static unsigned long totalMalloced = 0;

void *Malloc(size_t size, char *fileName, long fileLine)				/*	version of malloc() with debug	*/
	{
	size_t newSize = size + sizeof(Mem_node);						/*	compute new (larger) size		*/
	void* newChunk = malloc(newSize);								/*	malloc() it					*/
	Mem_node *newNode;											/*	pointer to a new node			*/

#if DEBUG_MALLOC_VERBOSE
		printf("%40s:%5d:Malloc() :", fileName, fileLine, size);
#endif
	if (newChunk == NULL)										/*	if it fails, 					*/
		{
#ifdef DEBUG_MALLOC_VERBOSE
		printf("Failed malloc() call\n");							/*	print error message				*/
#endif
		return NULL;											/*	return a null pointer			*/
		} /* end of failed case */
	
	newNode = (Mem_node *) newChunk;								/*	point the new node to the front	*/
	newNode->numBytes = size;									/*	set the size of the new node		*/
	newNode->sequence = nextSequenceNo;							/*	set the sequence no				*/
	strncpy(newNode->fileName, fileName, DEBUG_MALLOC_FILE_NAME_LENGTH);	/*	copy the file name				*/
	newNode->fileName[DEBUG_MALLOC_FILE_NAME_LENGTH - 1] = '\0';		/*	and make sure it's null-terminated	*/
	newNode->fileLine = fileLine;									/*	set the line number				*/
	newNode->prev = &theList;									/*	set the prev. node to the list	*/
	theList.next->prev = newNode;									/*	let the next node know			*/
	newNode->next = theList.next;									/*	set the prev. node to old next	*/
	theList.next = newNode;										/*	let "theList" know				*/	
	
	if (nextSequenceNo == 1)										/*	if this is the first malloc()	*/
		atexit(&Malloc_Report);									/*	set Malloc_Report for exit call	*/
	totalMalloced += newSize;

#if DEBUG_MALLOC_VERBOSE
	printf("%4d:%10d bytes at 0x%08X for total of %10d\n", 
		nextSequenceNo, size, newNode + 1, totalMalloced);
															/*	report allocation				*/
#endif
	nextSequenceNo++;											/*	increment the sequence no.		*/
	return (newNode + 1);										/*	return the balance to caller		*/
	} /* end of Malloc() */

void FreeStub(void *ptr, char *fileName, long fileLine)				/*	stub version of free() 			*/
	{
#if DEBUG_MALLOC_VERBOSE
	printf("%40s:%5d:FreeStub():Release %p.\n", fileName, fileLine, ptr);
#endif
	return;													/*	don't do anything				*/
	} /* end of FreeStub() */

void Free(void *ptr, char *fileName, long fileLine)					/*	version of free() with debug		*/
	{
	if (ptr == NULL) 
		{
		printf("%40s:%5d:Free()   :    : Null pointer.\n", fileName, fileLine);
		return;
		}
	Mem_node *origPtr = (Mem_node *)ptr;							/*	the original pointer			*/
	origPtr--;												/*	subtract to get the original ptr	*/
#if DEBUG_MALLOC_VERBOSE
	printf("%40s:%5d:Free()   :%4d:%10d bytes at 0x%08X from %40s:%5d", fileName, fileLine, origPtr->sequence, 
			origPtr->numBytes, ptr, origPtr->fileName, origPtr->fileLine);
#endif
	if (origPtr->sequence == 0)									/*	if it's node 0, return			*/
		{

#if DEBUG_MALLOC_VERBOSE
		printf(" Failed.\n", ptr);								/*	print error message				*/
#endif
		return;												/*	return to caller				*/		
		} /* end of accidental attempt on #0 */

	origPtr->next->prev = origPtr->prev;							/*	link past this node				*/
	origPtr->prev->next = origPtr->next;							/*	both directions				*/

	free(origPtr);												/*	free the original pointer		*/
#if DEBUG_MALLOC_VERBOSE
		printf(" Succeeded.\n", ptr);
															/*	print message					*/
#endif
	} /* end of Free() */

void *Calloc(size_t nelem, size_t size, char *fileName, long fileLine)	/*	version of calloc() with debug	*/
	{
#if DEBUG_MALLOC_VERBOSE
		printf("%40s:%5d:Calloc() :%4d: (%5d @ %5d = %8d).\n", fileName, fileLine, nextSequenceNo, nelem, 
				size, nelem*size);
#endif
	return Malloc(nelem*size, fileName, fileLine);					/*	just pass it into Malloc()		*/	
	} /* end of Calloc() */

void *Realloc(void *ptr, size_t size, char *fileName, long fileLine)		/*	version of realloc() with debug	*/
	{
	Mem_node *origPtr = (Mem_node *)ptr;							/*	the original pointer			*/
	origPtr--;												/*	subtract to get the original ptr	*/
#if DEBUG_MALLOC_VERBOSE
		printf("%40s:%5d:Realloc():%4d:%8d bytes at %p from %40s:%5d.  ", fileName, fileLine, origPtr->sequence, size, 
			ptr, origPtr->fileName, origPtr->fileLine);
#endif
	if (origPtr->sequence == 0)									/*	if it's node 0, return			*/
		{
#if DEBUG_MALLOC_VERBOSE
		printf("Bad Realloc() call on %p\n", ptr);
															/*	print error message				*/
#endif
		return ptr;											/*	return original pointer			*/		
		} /* end of accidental attempt on #0 */
	
	origPtr = (Mem_node *)realloc(origPtr, size + sizeof(Mem_node));		/*	reallocate memory				*/
	if (origPtr == NULL)										/*	if it fails, 					*/
		{	
#if DEBUG_MALLOC_VERBOSE
		printf("Failed realloc() call\n");							/*	print error message				*/
#endif
		return NULL;											/*	return a null pointer			*/
		} /* end of failed case */
												
	origPtr->numBytes = size;									/*	reset the number of bytes		*/
#if DEBUG_MALLOC_VERBOSE
	printf("%4d: %4d bytes now at %p. ", 
		nextSequenceNo, size, origPtr + sizeof(Mem_node));
															/*	report allocation				*/
	printf("Reset location from %s:%5d\n", origPtr->fileName, origPtr->fileLine);
#endif
	return (origPtr + 1);										/*	return the correct pointer		*/
	} /* end of Realloc() */
	
void Malloc_Report()											/*	reports at end of program		*/
	{
	Mem_node *walkNode;											/*	for walking around the list		*/
	
	for (walkNode = theList.next; walkNode != &theList; walkNode = walkNode->next)
		{													/*	walk around the list			*/
		printf("%4d Pointer %p allocated from %s:%5d was never deallocated (size was %d bytes).\n",
			walkNode->sequence, walkNode+1, walkNode->fileName, walkNode->fileLine, walkNode->numBytes);
		} /* end of loop around list */
	} /* end of Malloc_Report() */
#endif
