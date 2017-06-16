//	IsoSurface II
//	Hamish Carr, 2000

//	HeightField_Flexible_Manipulation.cpp:	routines for flexible isosurface manipulation
//	Palatino-12; 10 n-spaces/tab

#include "HeightField.h"
#include <GL/freeglut.h>
#include <ctime>

#define NO_DANCING_SIMPLICES 0

#define GLErrorString(x) ( ((x) == GL_NO_ERROR) ? "No Error" : ((x) == GL_INVALID_ENUM) ? "Invalid enum" : ((x) == GL_INVALID_VALUE) ? "Invalid Value" : ((x) == GL_INVALID_OPERATION) ? "Invalid Operation" : ((x) == GL_STACK_OVERFLOW) ? "Stack Overflow" : ((x) == GL_STACK_UNDERFLOW) ? "Stack Underflow" : ((x) == GL_OUT_OF_MEMORY) ? "Out of Memory" : "Unknown Error")

void HeightField::MakeArcDisplayList(int sArc, double ht)					//	make the display list for an arc
	{ // MakeArcDisplayList()
	int errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] before making Display List %ld for superarc %ld at height %8.5f \n", __FILE__, __LINE__, errNum, GLErrorString(errNum), baseDisplayListID + sArc, sArc, ht);
		}
	glNewList(baseDisplayListID + sArc, GL_COMPILE_AND_EXECUTE);		//	start creating its display list
	errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] when allocating Display List %ld for superarc %ld at height %8.5f \n", __FILE__, __LINE__, errNum, GLErrorString(errNum), baseDisplayListID + sArc, sArc, ht);
		}
	
	gettimeofday(&thatTime, NULL);
// 	timingBuffer += sprintf(timingBuffer, "Constructing display list for contour %ld.  Starting at time %ld:%ld\n", 
// 		sArc, thatTime.tv_sec % 1000, thatTime.tv_usec);
// 	flushTimingBuffer();
	
	FollowContour(ht, contourTree[sArc]);								//	follow the contour around

	gettimeofday(&thisTime, NULL);
// 	timingBuffer += sprintf(timingBuffer, "Display list construction finished at  %ld:%ld\n", 
// 		thisTime.tv_sec % 1000, thisTime.tv_usec);
	extractionTime[sArc] = (float) (thisTime.tv_sec - thatTime.tv_sec) + 0.000001 * (thisTime.tv_usec - thatTime.tv_usec);
	pathLengths[sArc] = pathLength;
	nContourTriangles[sArc] = triangleCount;
// 	timingBuffer += sprintf(timingBuffer, "Display list for contour %ld constructed in %8.5f seconds.\n", 
// 		sArc, extractionTime[sArc]);
//	timingBuffer += sprintf(timingBuffer, "Display list construction took %8.3f seconds.  Path length was %ld.  Triangle count was %ld\n",
//		extractionTime[sArc], pathLength, triangleCount);
// 	flushTimingBuffer();

//	printf("\n");
	errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] when describing Display List %ld for superarc %ld at height %8.5f \n", __FILE__, __LINE__, errNum, GLErrorString(errNum), baseDisplayListID + sArc, sArc, ht);
		}
	glEndList();												//	end the list
	errNum = glGetError();
	if (errNum != GL_NO_ERROR)
		{
		printf("%s(%ld): GL Error %ld [%s] when finalizing Display List %ld for superarc %ld at height %8.5f \n", __FILE__, __LINE__, errNum, GLErrorString(errNum), baseDisplayListID + sArc, sArc, ht);
		}
	} // MakeArcDisplayList()

void HeightField::ClearAllArrays()										//	clears active[], selected[], restorable[]
	{ // ClearAllArrays()
	ClearActive();
	ClearSelection();
	ClearRestorable();
	} // ClearAllArrays()

void HeightField::ClearActive()										//	clears active list
	{ // ClearActive()
	while (nActiveArcs > 0)											//	as long as there are any
		RemoveFromActive(active[0]);									//	remove the first one
	} // ClearActive()

void HeightField::ClearSelection()										//	clears selected list
	{ // ClearSelection()
	while (nSelectedArcs > 0)										//	as long as there are any
		RemoveFromSelected(selected[0]);								//	remove the first one
	} // ClearSelection()

void HeightField::ClearRestorable()									//	clears restorable list
	{ // ClearActive()
	while (nRestorableArcs > 0)										//	as long as there are any
		RemoveFromRestorable(restorable[0]);							//	remove the first one
	} // ClearActive()

void HeightField::AddToActive(long arc, double seedValue)					//	adds an arc to the active list		
	{ // AddToActive()
	if (contourTree[arc].CheckFlag(Superarc::isActive))					//	check to see if already active
		{ // paranoid check
		printf("Error: attempt to activate arc %1d that is already active.\n", arc);	
		return;
		} // paranoid check
	contourTree[arc].index = nActiveArcs;								//	set the index to next available
	contourTree[arc].SetFlag(Superarc::isActive);						//	set the flag
	contourTree[arc].SetFlag(Superarc::isDirty);							//	be conservative: re-extract
	contourTree[arc].seedValue = seedValue;								//	set the seed value
	active[nActiveArcs++] = arc;										//	and add to the array
	} // AddToActive()

void HeightField::RemoveFromActive(long arc)								//	removes arc from the active list
	{ // RemoveFromActive()
	if (not contourTree[arc].CheckFlag(Superarc::isActive))
		{ // paranoid check
		printf("Error: attempt to deactivate arc %1d that is not already active.\n", arc);	
		return;
		} // paranoid check
	active[contourTree[arc].index] = active[--nActiveArcs];				//	swap the last entry into place
	contourTree[active[arc.index]].index = arc.index;						//	and reset it's index
	arc.index = -1;												//	reset the arc's index
	contourTree[arc].ClearFlag(Superarc::isActive);						//	and clear the flag
	} // RemoveFromActive()

void HeightField::AddToSelection(long arc, double seedValue)				//	adds an arc to the selected list		
	{ // AddToSelection()
	if (contourTree[arc].CheckFlag(Superarc::isSelected))					//	check to see if already selected
		{ // paranoid check
		printf("Error: attempt to add arc %1d to selection when it is already selected.\n", arc);	
		return;
		} // paranoid check
	if (contourTree[arc].CheckFlag(Superarc::isActive))					//	if the arc is already active
		RemoveFromActive(arc);										//	remove it from the active list
	else															//	otherwise, it's a brand new one
		contourTree[arc].SetFlag(Superarc::isDirty);						//	force surface construction

	contourTree[arc].index = nSelectedArcs;								//	set the index to next available
	contourTree[arc].SetFlag(Superarc::isSelected);						//	set the flag
	contourTree[arc].seedValue = seedValue;								//	set the seed value
	selected[nSelectedArcs++] = arc;									//	and add to the array
	} // AddToSelection()

void HeightField::RemoveFromSelection(long arc)							//	removes arc from the selected list
	{ // RemoveFromSelection()
	if (not contourTree[arc].CheckFlag(Superarc::isSelected))
		{ // paranoid check
		printf("Error: attempt to remove arc %1d from selection when it is not already selected.\n", arc);	
		return;
		} // paranoid check
	selected[contourTree[arc].index] = selected[--nSelectedArcs];			//	swap the last entry into place
	contourTree[selected[arc.index]].index = arc.index;					//	and reset it's index
	arc.index = -1;												//	reset the arc's index
	contourTree[arc].ClearFlag(Superarc::isSelected);						//	and clear the flag
	} // RemoveFromSelection()

void HeightField::AddToRestorable(long arc)								//	adds an arc to the restorable list		
	{ // AddToRestorable()
	if (contourTree[arc].CheckFlag(Superarc::isRestorable))				//	check to see if already restorable
		{ // paranoid check
		printf("Error: attempt to make arc %1d restorable when it is already restorable.\n", arc);	
		return;
		} // paranoid check
	contourTree[arc].index = nRestorableArcs;							//	set the index to next available
	contourTree[arc].SetFlag(Superarc::isRestorable);						//	set the flag
	restorable[nRestorableArcs++] = arc;								//	and add to the array
	} // AddToRestorable()

void HeightField::RemoveFromRestorable(long arc)							//	removes arc from the restorable list
	{ // RemoveFromRestorable()
	if (not contourTree[arc].CheckFlag(Superarc::isRestorable))
		{ // paranoid check
		printf("Error: attempt to remove arc %1d from restorable[] when it is not already restorable.\n", arc);	
		return;
		} // paranoid check
	restorable[contourTree[arc].index] = restorable[--nRestorableArcs];		//	swap the last entry into place
	contourTree[restorable[arc.index]].index = arc.index;					//	and reset it's index
	arc.index = -1;												//	reset the arc's index
	contourTree[arc].ClearFlag(Superarc::isRestorable);					//	and clear the flag
	} // RemoveFromRestorable()
	
void HeightField::SetIsosurface(double ht)								//	sets an isosurface at a particular value
	{ // SetIsosurface()
	int sArc, validArc;												//	index for walking through arcs
	
	ClearAllArrays();												//	reset all the arrays

	for (validArc = 0; validArc < nValidArcs; validArc++)					//	walk through valid arcs
		{ // loop through arcs
		sArc = valid[validArc];										//	retrieve the superarc's ID
		if (*(contourTree[sArc].hiEnd) > ht && *(contourTree[sArc].loEnd) <= ht)
																//	if the superarc spans the value
			AddArcToActive(sArc, ht);								//	add it to the active set
		} // loop through arcs
	} // SetIsosurface()

void HeightField::SetLocal(double ht)									//	sets an isosurface at a particular value
	{ // SetLocal()
	int sArc, validArc;												//	index for walking through arcs
	double localHeight;												//	height on local edge
	
	ClearAllArrays();												//	reset all the arrays

	for (validArc = 0; validArc < nValidArcs; validArc++)					//	walk through valid arcs
		{ // loop through arcs
		sArc = valid[validArc];										//	retrieve the superarc's ID
		if (contourTree[sArc].nextHi != &(contourTree[sArc])) continue;		//	skip arcs which aren't local maxima
		localHeight = ht * (*(contourTree[sArc].hiEnd)) + (1.0 - ht) * (*(contourTree[sArc].loEnd));
		AddToActive(sArc, localHeight);								//	add it to the active set
		} // loop through arcs
	} // SetLocal()

void HeightField::SetComponentHeight(int component, float height)			//	sets the seed value for a specific component
	{ // SetComponentHeight()
	if (height < *(contourTree[component].loEnd)) height = *(contourTree[component].loEnd);
	if (height >= *(contourTree[component].hiEnd)) height = *(contourTree[component].hiEnd) - 0.00001;
	seedValue[component] = height;									//	set the height
	} // SetComponentHeight()
	
void HeightField::SelectComponent(int newSelectionRoot, double selectionValue)	//	initializes the selection arrays
	{ // SelectComponent()
	CommitSelection();												//	transfer any existing selection to active
	AddToSelection(newSelectionRoot,
	contourTree[newSelectionRoot].SetFlag(Superarc::isSelected);			//	select it
	selectionRoot = newSelectionRoot;									//	store it for dragging purposes
	selectionRootValue = seedValue[newSelectionRoot];						//	store the value as well
	currentSelectionValue = seedValue[newSelectionRoot];					//	and set it as the current value as well
	} // SelectComponent()
	
void HeightField::UpdateSelection(double height)							//	updates the selection arrays during a drag
	{ // UpdateSelection()
	int *searchQueue = new int[nValidArcs];								//	queue for our search
	int searchQueueSize = 0, nextToSearch = 0;							//	size of queue, and position on queue
	int sArc;														//	loop index

	int *backSearchQueue = new int[nValidArcs];							//	queue for reverse search to suppress
	bool *onBackQueue = new bool[nSuperarcs];							//	flags for visited arcs
	int backSearchQueueSize = 0, nextBackSearch = 0;						//	indices for reverse search
	Superarc *theArc;												//	pointer to a superarc
	double *theNode;												//	the end node of an edge
		
//	currentSelectionValue = selectionRootValue + heightDelta *(MaxHeight() - MinHeight());
																//	compute the final height
	currentSelectionValue = height;									//	set the new height
	if (currentSelectionValue < MinHeight()) currentSelectionValue = MinHeight();	//	clamp to minimum possible
	if (currentSelectionValue > MaxHeight()) currentSelectionValue = MaxHeight();//	clamp to maximum possible

//	printf("value: %f\n", currentSelectionValue);
	if (currentSelectionValue == selectionRootValue)						//	special case - exactly equal
		{ // exactly on root value
		SelectComponent(selectionRoot, true);							//	re-select to reset the selection
		delete searchQueue; delete backSearchQueue; delete onBackQueue;
		return;												//	we're done
		} // exactly on root value

	searchQueue[searchQueueSize++] = selectionRoot;						//	initialize the queue

	for (sArc = 0; sArc < nSuperarcs; sArc++)							//	walk through the arcs
		{
		if (contourTree[sArc].CheckFlag(Superarc::isCollapsed)) continue;	//	skip collapsed edges
		contourTree[sArc].ClearFlag(Superarc::isSelected);
		contourTree[sArc].ClearFlag(Superarc::isSuppressed);
		onBackQueue[sArc] = false;									//	and set to unselected
		}
	if (currentSelectionValue < selectionRootValue)						//	if it's lower
		{ // currentSelectionValue < selectionRootValue
//		printf("Searching downwards\n");
		while (nextToSearch < searchQueueSize)						//	until all processed
			{ // while loop to process queue
			sArc = searchQueue[nextToSearch++];						//	retrieve the next component
			if (*(contourTree[sArc].loEnd) <= currentSelectionValue)		//	if the low end is lower (i.e. we span)
				{ // arc spans the new value - terminal case for search
//				printf("Arc %ld selected at value %f\n", sArc, currentSelectionValue);
				backSearchQueue[backSearchQueueSize++] = sArc;		//	add to the reverse search queue
				onBackQueue[sArc] = true;							//	set flag for arc
				contourTree[sArc].SetFlag(Superarc::isSelected);
				MakeArcDisplayList(sArc, currentSelectionValue);			//	make the corresponding display list
				contourTree[sArc].SetFlag(Superarc::isDirty);
				continue;										//	and don't search any further on this branch
				} // arc spans the new value - terminal case for search
				
			contourTree[sArc].SetFlag(Superarc::isSuppressed);
			theArc = contourTree[sArc].nextLo;						//	grab the next edge at the vertex
			theNode = contourTree[sArc].loEnd;						//	grab a pointer to the low end
			do
				{ // loop to add all downward arcs to queue
				if (theArc->hiEnd == theNode)						//	in this case, it's a downward arc
					{ // downward arc
					searchQueue[searchQueueSize++] = theArc->superArcID;//	put on queue 
					theArc = theArc->nextHi;						//	and take next pointer at high end
					} // downward arc
				else // upward arc
					theArc = theArc->nextLo;						//	advance to next pointer
				} // loop to add all downward arcs to queue
			while (theArc != contourTree[sArc].nextLo);					//	until we come back round to where we started
			} // while loop to process queue

		while (nextBackSearch < backSearchQueueSize)					//	until all processed
			{ // while loop to process reverse queue
			sArc = backSearchQueue[nextBackSearch++];				//	retrieve the next component
			if (not contourTree[sArc].CheckFlag(Superarc::isSelected))	//	if not selected
				contourTree[sArc].SetFlag(Superarc::isSuppressed);
			if (*(contourTree[sArc].hiEnd) > selectionRootValue)			//	keep going until we're back at the root value
				continue;										//	i.e. search no further in this direction
			theArc = contourTree[sArc].nextHi;						//	grab the next edge at the vertex
			theNode = contourTree[sArc].hiEnd;						//	grab a pointer to the high end
			do
				{ // loop to add all upward arcs to queue
				if (theArc->loEnd == theNode)						//	in this case, it's an upward arc
					{ // upward arc
					if (!onBackQueue[theArc->superArcID])			//	if not already on queue
						{ // not on queue yet
						backSearchQueue[backSearchQueueSize++] = theArc->superArcID;
															//	put on queue 
						onBackQueue[theArc->superArcID] = true;		//	set flag
						} // not on queue yet
					theArc = theArc->nextLo;						//	and take next pointer at high end
					} // upward arc
				else // downward arc
					theArc = theArc->nextHi;						//	advance to next pointer
				} // loop to add all upward arcs to queue
			while (theArc != contourTree[sArc].nextHi);					//	until we come back round to where we started
			} // while loop to process reverse queue
		} // currentSelectionValue < selectionRootValue
	else
		{ // currentSelectionValue > selectionRootValue
//		printf("Searching upwards\n");
		while (nextToSearch < searchQueueSize)						//	until all processed
			{ // while loop to process queue
			sArc = searchQueue[nextToSearch++];						//	retrieve the next component
			if (*(contourTree[sArc].hiEnd) > currentSelectionValue)			//	if the low end is lower (i.e. we span)
				{ // arc spans the new value - terminal case for search
//				printf("Arc %ld selected\n", sArc);
				backSearchQueue[backSearchQueueSize++] = sArc;		//	add to the reverse search queue
				onBackQueue[sArc] = true;							//	set flag for arc
				contourTree[sArc].SetFlag(Superarc::isSelected);
				MakeArcDisplayList(sArc, currentSelectionValue);			//	make the corresponding display list
				contourTree[sArc].SetFlag(Superarc::isDirty);
				continue;										//	and don't search any further on this branch
				} // arc spans the new value - terminal case for search
				
			contourTree[sArc].SetFlag(Superarc::isSuppressed);
			theArc = contourTree[sArc].nextHi;						//	grab the next edge at the vertex
			theNode = contourTree[sArc].hiEnd;						//	grab a pointer to the high end
			do
				{ // loop to add all upward arcs to queue
				if (theArc->loEnd == theNode)						//	in this case, it's a upward arc
					{ // upward arc
					searchQueue[searchQueueSize++] = theArc->superArcID;//	put on queue
					theArc = theArc->nextLo;						//	and take next pointer at low end
					} // upward arc
				else // downward arc
					theArc = theArc->nextHi;						//	advance to next pointer
				} // loop to add all upward arcs to queue
			while (theArc != contourTree[sArc].nextHi);					//	until we come back round to where we started
			} // while loop to process queue

		while (nextBackSearch < backSearchQueueSize)					//	until all processed
			{ // while loop to process reverse queue
			sArc = backSearchQueue[nextBackSearch++];				//	retrieve the next component
			if (not contourTree[sArc].CheckFlag(Superarc::isSelected))	//	if not selected
				contourTree[sArc].SetFlag(Superarc::isSuppressed);
			if (*(contourTree[sArc].loEnd) <= selectionRootValue)			//	keep going until we're back at the root value
				continue;										//	i.e. search no further in this direction
			theArc = contourTree[sArc].nextLo;						//	grab the next edge at the vertex
			theNode = contourTree[sArc].loEnd;						//	grab a pointer to the low end
			do
				{ // loop to add all downward arcs to queue
				if (theArc->hiEnd == theNode)						//	in this case, it's a downward arc
					{ // downward arc
					if (!onBackQueue[theArc->superArcID])			//	if not already on queue
						{ // not on queue yet
						backSearchQueue[backSearchQueueSize++] = theArc->superArcID;
															//	put on queue 
						onBackQueue[theArc->superArcID] = true;		//	set flag
						} // not on queue yet
					theArc = theArc->nextHi;						//	and take next pointer at high end
					} // downward arc
				else // upward arc
					theArc = theArc->nextLo;						//	advance to next pointer
				} // loop to add all downward arcs to queue
			while (theArc != contourTree[sArc].nextLo);					//	until we come back round to where we started
			} // while loop to process reverse queue
		} // currentSelectionValue > selectionRootValue

	for (sArc = 0; sArc < nSuperarcs; sArc++)							//	walk through the arcs
		if (	contourTree[sArc].CheckFlag(Superarc::isActive) and			//	big test to see if we need to redo the display list
			contourTree[sArc].CheckFlag(Superarc::isDirty) and
			not contourTree[sArc].CheckFlag(Superarc::isSelected) and
			not contourTree[sArc].CheckFlag(Superarc::isSuppressed) and
			not contourTree[sArc].CheckFlag(Superarc::isCollapsed)
		    )
			{ // arc active, but display list dirty				
			MakeArcDisplayList(sArc, seedValue[sArc]);					//	make the display list again
			contourTree[sArc].ClearFlag(Superarc::isDirty);
			} // arc active, but display list dirty

	delete searchQueue; delete backSearchQueue; delete onBackQueue;
	} // UpdateSelection()
	
void HeightField::CommitSelection(double height)						//	commits the selection
	{ // CommitSelection()
	int sArc;													//	loop index

	for (sArc = 0; sArc < nSuperarcs; sArc++)							//	walk through the arcs
		{ // for each arc
		if (contourTree[sArc].CheckFlag(Superarc::isCollapsed)) continue;
		if (contourTree[sArc].CheckFlag(Superarc::isSelected)) 
			{ // arcSelected
			contourTree[sArc].SetFlag(Superarc::isActive);
			seedValue[sArc] = currentSelectionValue;					//	setting the seed value
			} // arcSelected	
		if (contourTree[sArc].CheckFlag(Superarc::isSuppressed)) 			//	if the arc was suppressed during a drag
			contourTree[sArc].ClearFlag(Superarc::isActive);
		contourTree[sArc].ClearFlag(Superarc::isSuppressed);
		contourTree[sArc].ClearFlag(Superarc::isRestorable);
		} // for each arc
	} // CommitSelection()
	
void HeightField::DeleteSelection()									//	deactivates the selected components
	{ // DeleteSelection()
	int sArc;														//	loop index
	for (sArc = 0; sArc < nSuperarcs; sArc++)							//	walk through the arcs
		{ // for each arc
		if (contourTree[sArc].CheckFlag(Superarc::isCollapsed)) continue;
		if (contourTree[sArc].CheckFlag(Superarc::isSelected))				//	if selected, transfer to restorable list
			{ // if it was selected
			contourTree[sArc].ClearFlag(Superarc::isActive);				//	turn off active flag
			contourTree[sArc].ClearFlag(Superarc::isSelected);			//	set to unselected
			contourTree[sArc].ClearFlag(Superarc::isDirty);				//	and turn off 'wasSelected' flag
			contourTree[sArc].SetFlag(Superarc::isRestorable);
			} // if it was selected
		else
			contourTree[sArc].ClearFlag(Superarc::isRestorable);
		} // for each arc
	} // DeleteSelection()
	
void HeightField::DeleteComplement()									//	deactivates the complement of the selection
	{ // DeleteComplement()
	int sArc;														//	loop index
	for (sArc = 0; sArc < nSuperarcs; sArc++)							//	walk through the arcs
		{ // for each arc
		if (contourTree[sArc].CheckFlag(Superarc::isCollapsed)) continue;

		if (contourTree[sArc].CheckFlag(Superarc::isActive))				//	if selected, transfer to restorable list
			contourTree[sArc].SetFlag(Superarc::isRestorable);
		else
			contourTree[sArc].ClearFlag(Superarc::isRestorable);
		if (contourTree[sArc].CheckFlag(Superarc::isSelected)) continue;
		if (contourTree[sArc].CheckFlag(Superarc::isActive))				//	if selected
			{ // if it was active
			contourTree[sArc].ClearFlag(Superarc::isActive);				//	turn off active flag
			contourTree[sArc].ClearFlag(Superarc::isSelected);			//	set to unselected
			contourTree[sArc].ClearFlag(Superarc::isDirty);				//	and turn off 'wasSelected' flag
			} // if it was active
		} // for each arc
	} // DeleteComplement()
	
void HeightField::RestoreDeleted()									//	restores the last saved set									
	{ // RestoreDeleted()
	int sArc;													//	loop index
	for (sArc = 0; sArc < nSuperarcs; sArc++)							//	walk through the arcs
		{ // for each arc
		if (contourTree[sArc].CheckFlag(Superarc::isCollapsed)) continue;

		if (	contourTree[sArc].CheckFlag(Superarc::isRestorable) and not 
			contourTree[sArc].CheckFlag(Superarc::isSelected))
			{ // if restorable
			contourTree[sArc].SetFlag(Superarc::isActive);				//	set to active
			MakeArcDisplayList(sArc, seedValue[sArc]);					//	recompute the display list
			} // if restorable
		} // for each arc
	} // RestoreDeleted()

void HeightField::Unselect()											//	clears selection, transferring to active status
	{ // Unselect()
	int sArc;														//	loop index
	for (sArc = 0; sArc < nSuperarcs; sArc++)							//	walk through the arcs
		{ // for each arc
		if (contourTree[sArc].CheckFlag(Superarc::isCollapsed)) continue;
		if (contourTree[sArc].CheckFlag(Superarc::isSelected))				//	stored, but not also selected
		contourTree[sArc].SetFlag(Superarc::isActive);					//	set to active
		contourTree[sArc].ClearFlag(Superarc::isSelected);				//	set to active
		contourTree[sArc].SetFlag(Superarc::isDirty);					//	set to active
		contourTree[sArc].ClearFlag(Superarc::isSuppressed);				//	set to active
		} // for each arc	
	} // Unselect()

void HeightField::DeactivateAll()										//	deactivates all components
	{ // DeactivateAll()
	int sArc;													//	loop index
	for (sArc = 0; sArc < nSuperarcs; sArc++)							//	walk through the arcs
		{ // for each arc
		if (contourTree[sArc].CheckFlag(Superarc::isCollapsed)) continue;
		contourTree[sArc].ClearFlag(Superarc::isActive);
		contourTree[sArc].ClearFlag(Superarc::isSelected);	
		contourTree[sArc].SetFlag(Superarc::isDirty);			
		contourTree[sArc].ClearFlag(Superarc::isRestorable);			
		contourTree[sArc].ClearFlag(Superarc::isSuppressed);			
		}  // for each arc
	} // DeactivateAll()

int HeightField::PickNode(float x, float y)								//	picks a node: returns -1 if it fails
	{ // PickNode()
//	x = -0.1 + (x * 1.2);											//	because we display in range -0.1 - 1.1 to provide a border
//	y = -0.1 + (y * 1.2);											//	same thing for vertical height
	float bestDistanceSquared = 1.0;									//	start off with some ridiculous distance
	long whichNode = -1;											//	and start off with no node found
	
//	printf("x: %4.2f. y: %4.2f\n", x, y);	
	for (int node = 0; node < nSupernodes; node++)						//	walk through nodes
		{ // node loop
		float deltaX = x - nodeXPosn[node];							//	compute distance in x
		float deltaY = y - (nodeYPosn[node] - MinHeight()) / (MaxHeight() - MinHeight());
															//	and distance in y
		float distSquared = deltaX*deltaX + deltaY*deltaY;					//	and square of distance
//		printf("nodeX: %4.2f, nodeY: %4.2f\n", nodeXPosn[node], (nodeYPosn[node] - MinHeight()) / (MaxHeight() - MinHeight()));
//		printf("dx: %4.2f, dy: %4.2f\n", deltaX, deltaY);
		if ((distSquared < 0.001) && (distSquared < bestDistanceSquared))		//	if it's the best choice so far
			{ whichNode = node; bestDistanceSquared = distSquared; }		//	store it and go on
		} // node loop
	return whichNode;											//	return whatever we ended up with	
	} // PickNode()

int HeightField::PickArc(float x, float y)									//	picks an arc: returns -1 if it fails
	{ // PickArc()
//	x = -0.1 + (x * 1.2); y = -0.1 + (y *1.2);								//	scale to allow for borders
	y = MinHeight() + y * (MaxHeight() - MinHeight());					//	convert to the range of heights actually in use
	float bestDeltaX = 1.0;											//	best distance laterally
	long whichArc = -1;											//	which arc has this distance

//	printf("x: %4.2f. y: %4.2f\n", x, y);	
	for (int arc = 0; arc < nSuperarcs; arc++)								//	again, walk through all the arcs
		{ // arc loop
		if (contourTree[arc].CheckFlag(Superarc::isCollapsed)) continue;
//		printf("Hi: (%4.2f, %4.2f). Lo: (%4.2f, %4.2f)\n", 
//			nodeXPosn[contourTree[arc].hiID], *(contourTree[arc].hiEnd), 
//			nodeXPosn[contourTree[arc].loID], *(contourTree[arc].loEnd));
		float deltaHt = *(contourTree[arc].hiEnd) - *(contourTree[arc].loEnd);	//	height range of arc
		if (*(contourTree[arc].hiEnd) <= y) continue;						//	discard arcs outside the range
		if (*(contourTree[arc].loEnd) > y) continue;						
		if (deltaHt == 0.0) continue;									//	discard horizontal arcs
		float YSlope = (nodeXPosn[contourTree[arc].hiID] - nodeXPosn[contourTree[arc].loID]) / deltaHt;
															//	this way, no divide by zero for vertical arcs
		float arcX =  nodeXPosn[contourTree[arc].loID] + (y - *(contourTree[arc].loEnd)) * YSlope;	
															//	compute x value at fixed height on arc
		float deltaX = fabs(arcX - x);									//	compute delta from point given	
//		printf("deltaH: %6.3f YSlope: %6.3f arcX: %6.3f deltaX: %6.3f\n", deltaHt, YSlope, arcX, deltaX);
		if ((deltaX < 0.02) && (deltaX < bestDeltaX))						//	check for tolerance & best
			{ whichArc = arc; bestDeltaX = deltaX; };					//	and update "best" so far
		} // arc loop
	return whichArc;											//	return the one we found (if any)
	} // PickArc()

void HeightField::UpdateNodeXPosn(int node, float x)						//	updates a node's x position
	{ // UpdateNodeXPosn()
	if (x < 0.01) x = 0.01; if (x > 0.99) x = 0.99;							//	clamp
	nodeXPosn[node] = x;											//	update it
	} // UpdateNodeXPosn()

void HeightField::ComputeStatistics()									//	computes statistics on average path length, &c.
	{ // ComputeStatistics()
	long totalPathLength = 0, nSArcs = 0;								//	variables for capturing statistics
	
	totalTriangles = 0;											//	reset the field's variables
	maxPathLength = 0;
	totalExtractionTime = 0.0;										//	likewise
	for (int arc = 0; arc < nSuperarcs; arc++)								//	walk through all the arcs
		{ // loop through arcs
		if (contourTree[arc].CheckFlag(Superarc::isCollapsed)) continue;
		if (contourTree[arc].CheckFlag(Superarc::isSuppressed)) continue;
		if (	not contourTree[arc].CheckFlag(Superarc::isActive) and
			not contourTree[arc].CheckFlag(Superarc::isSelected)) continue;
		totalTriangles += nContourTriangles[arc];						//	update statistics
		totalPathLength += pathLengths[arc];
		if (pathLengths[arc] > maxPathLength) maxPathLength = pathLengths[arc];
		totalExtractionTime += extractionTime[arc];
		nSArcs++;
		} // loop through arcs
	
	gettimeofday(&thisFrameTime, NULL);
	float frameTime = thisFrameTime.tv_sec - lastFrameTime.tv_sec + 1E-6 * (thisFrameTime.tv_usec - lastFrameTime.tv_usec);
	frameRate = 1.0 / frameTime;
	lastFrameTime.tv_sec = thisFrameTime.tv_sec; 
	lastFrameTime.tv_usec = thisFrameTime.tv_usec;
	avgPathLength = (float) totalPathLength / nSArcs;
	timingBuffer += sprintf(timingBuffer, "------------------------------\n");
	timingBuffer += sprintf(timingBuffer, "Longest Path: %ld\nAverage Path: %8.5f\n", maxPathLength, avgPathLength);
	timingBuffer += sprintf(timingBuffer, "Total Triangles displayed: %ld.  Extracted in %8.5f seconds\n", totalTriangles, totalExtractionTime);
	timingBuffer += sprintf(timingBuffer, "Frame time: %8.5f.  Frame rate: %8.5f Hz.\n", frameTime, frameRate);
	flushTimingBuffer();	
	} // ComputeStatistics()

bool HeightField::ArcSelected(int arcIndex)									//	accessor for selected flag
	{ // ArcSelected()
	return contourTree[arcIndex].CheckFlag(Superarc::isSelected);				//	just check the flag on the arc
	} // ArcSelected()