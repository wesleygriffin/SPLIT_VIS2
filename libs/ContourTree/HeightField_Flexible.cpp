//	IsoSurface II
//	Hamish Carr, 2000

//	HeightField_Flexible_Manipulation.cpp:	routines for flexible isosurface manipulation
//	Palatino-12; 10 n-spaces/tab

#include "HeightField.h"
#include <GL/freeglut.h>
#include <ctime>
#include <math.h>

#define NO_DANCING_SIMPLICES 0
#define DUMP_FRAME_TIMES 1

#define GLErrorString(x) ( ((x) == GL_NO_ERROR) ? "No Error" : ((x) == GL_INVALID_ENUM) ? "Invalid enum" : ((x) == GL_INVALID_VALUE) ? "Invalid Value" : ((x) == GL_INVALID_OPERATION) ? "Invalid Operation" : ((x) == GL_STACK_OVERFLOW) ? "Stack Overflow" : ((x) == GL_STACK_UNDERFLOW) ? "Stack Underflow" : ((x) == GL_OUT_OF_MEMORY) ? "Out of Memory" : "Unknown Error")

void HeightField::FollowHierarchicalPathSeed(int sArc, float ht)			//	generate surface using hierarchical path seeds
	{ // FollowHierarchicalPathSeed()
//	gettimeofday(&thatTime, NULL);
// 	timingBuffer += sprintf(timingBuffer, "Constructing display list for contour %ld.  Starting at time %ld:%ld\n", 
// 		sArc, thatTime.tv_sec % 1000, thatTime.tv_usec);
// 	flushTimingBuffer();

//	printf("Making display list at isovalue %7.3f for superarc %d\n", ht, sArc);
	//	deal with hierarchy of superarcs to find the seed:
	//	this will change later
	long whichArc = sArc;
//        printf("%d %f\n", sArc, ht);
	while (superarcs[whichArc].topArc != NO_SUPERARC)
		{ // loop to find valid seed
//		printf("Arc %3d has non-empty children %3d (%7.3f - %7.3f) and %3d (%7.3f - %7.3f).\n", whichArc, 
//			superarcs[whichArc].bottomArc,  *(supernodes[superarcs[superarcs[whichArc].bottomArc].bottomID].value), 
//								  	*(supernodes[superarcs[superarcs[whichArc].bottomArc].topID].value),
//			superarcs[whichArc].topArc,  *(supernodes[superarcs[superarcs[whichArc].topArc].bottomID].value),
//									*(supernodes[superarcs[superarcs[whichArc].topArc].topID].value)
//			);
		if (*(supernodes[superarcs[superarcs[whichArc].topArc].bottomID].value) <= ht)
			whichArc = superarcs[whichArc].topArc;
		else
			whichArc = superarcs[whichArc].bottomArc;
//		printf("Choosing %3d\n", whichArc);
		} // loop to find valid seed 
		
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glLineWidth(3);
	glBegin(GL_TRIANGLES);											//	start a set of triangles
	FollowContour(ht, superarcs[whichArc]);								//	follow the contour around
	glEnd();
//	glLineWidth(1);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	time(&thisTime);
// 	timingBuffer += sprintf(timingBuffer, "Display list construction finished at  %ld:%ld\n", 
// 		thisTime.tv_sec % 1000, thisTime.tv_usec);
	//extractionTime[sArc] = (float)(thisTime.tv_sec - thatTime.tv_sec) + 0.000001 * (thisTime.tv_usec - thatTime.tv_usec);
	extractionTime[sArc] = difftime(thisTime,thatTime);
	pathLengths[sArc] = pathLength;
	nContourTriangles[sArc] = triangleCount;
// 	timingBuffer += sprintf(timingBuffer, "Display list for contour %ld constructed in %8.5f seconds.\n", 
// 		sArc, extractionTime[sArc]);
//	timingBuffer += sprintf(timingBuffer, "Display list construction took %8.3f seconds.  Path length was %ld.  Triangle count was %ld\n",
//		extractionTime[sArc], pathLength, triangleCount);
// 	flushTimingBuffer();

	} // FollowHierarchicalPathSeed()
	
void HeightField::SetIsosurface(float ht)								//	sets an isosurface at a particular value
	{ // SetIsosurface()
	int sArc, validArc;												//	index for walking through arcs
	
	ClearAllArrays();												//	reset all the arrays

	for (validArc = 0; validArc < nValidArcs; validArc++)					//	walk through valid arcs
		{ // loop through arcs
		sArc = valid[validArc];										//	retrieve the superarc's ID
		if (*(supernodes[superarcs[sArc].topID].value) > ht && *(supernodes[superarcs[sArc].bottomID].value) <= ht)
			AddToActive(sArc, ht);									//	add it to the active set
		} // loop through arcs
	} // SetIsosurface()

void HeightField::SetLocal(float ht)									//	sets an isosurface at a particular value
	{ // SetLocal()
	int sArc, validArc;												//	index for walking through arcs
	float localHeight;												//	height on local edge
	
	ClearAllArrays();												//	reset all the arrays

	for (validArc = 0; validArc < nValidArcs; validArc++)					//	walk through valid arcs
		{ // loop through arcs
		sArc = valid[validArc];										//	retrieve the superarc's ID
		long topNode = superarcs[sArc].topID, bottomNode = superarcs[sArc].bottomID;
		if (!supernodes[topNode].IsUpperLeaf()) 
			continue;												//	ignore everything except upper leaves
		if (*(supernodes[topNode].value) == *(supernodes[bottomNode].value)) 	//	paranoid check to prevent divide by zero
			continue;												//	should be unnecessary, because of CollapseEpsilonEdges()
		localHeight = ht * (*(supernodes[topNode].value)) + (1.0 - ht) * (*(supernodes[bottomNode].value));
		AddToActive(sArc, localHeight);								//	add it to the active list
		} // loop through arcs
	} // SetLocal()

void HeightField::SetComponentHeight(int component, float height)			//	sets the seed value for a specific component
	{ // SetComponentHeight()
	if (height < *(supernodes[superarcs[component].bottomID].value)) height = *(supernodes[superarcs[component].bottomID].value);
	if (height >= *(supernodes[superarcs[component].topID].value)) height = *(supernodes[superarcs[component].topID].value) - 0.00001;
	superarcs[component].seedValue = height;							//	set the height
	} // SetComponentHeight()

void HeightField::SelectComponent(int newSelectionRoot)					//	initializes the selection arrays, using edge's seedvalue
	{ // SelectComponent()
	SelectComponent(newSelectionRoot, superarcs[newSelectionRoot].seedValue);	//	pass it on to the next routine
	} // SelectComponent()
	
void HeightField::SelectComponent(int newSelectionRoot, float selectionValue)	//	initializes the selection arrays
	{ // SelectComponent()
	CommitSelection();												//	transfer any existing selection to active
	SetComponentHeight(newSelectionRoot, selectionValue);					//	make sure the height is set
	AddToSelected(newSelectionRoot);									//	add it to the selection
	selectionRoot = newSelectionRoot;									//	store it for dragging purposes
	selectionRootValue = currentSelectionValue = selectionValue;			//	store the value as well
	} // SelectComponent()

void HeightField::ProportionalUpdateSelection(float height)				//	version of update selection constrained to edge
	{ // ProportionalUpdateSelection()
	float topValue = * (supernodes[superarcs[selectionRoot].topID].value);
	float bottomValue = *(supernodes[superarcs[selectionRoot].bottomID].value);
	float proportionalHeight = bottomValue + height * (topValue - bottomValue);
	UpdateSelection(proportionalHeight);
	} // ProportionalUpdateSelection()
	
void HeightField::UpdateSelection(float height)							//	updates the selection arrays during a drag
	{ // UpdateSelection()
	long *searchQueue = new long[nSuperarcs];							//	queue for our search
	long searchQueueSize = 0, nextToSearch = 0;							//	size of queue, and position on queue
	long sArc;													//	loop index

	long *backSearchQueue = new long[nSuperarcs];						//	queue for reverse search to suppress
	long backSearchQueueSize = 0, nextBackSearch = 0;						//	indices for reverse search
	long theArc;													//	pointer to a superarc
	long theNode;													//	the end node of an edge
		
	currentSelectionValue = height;									//	set the new height
	if (currentSelectionValue < MinHeight()) currentSelectionValue = MinHeight();//	clamp to minimum possible
	if (currentSelectionValue > MaxHeight()) currentSelectionValue = MaxHeight();//	clamp to maximum possible

	ClearSuppressed();												//	convert all suppressed arcs back to active
	ClearBackQueue();												//	clear flags for the back queue
	ClearSelection();												//	clear the selection

	searchQueue[searchQueueSize++] = selectionRoot;						//	initialize the queue

	if (currentSelectionValue < selectionRootValue)						//	if it's lower
		{ // currentSelectionValue < selectionRootValue
//		printf("Searching downwards\n");
		while (nextToSearch < searchQueueSize)							//	until all processed
			{ // while loop to process queue
			sArc = searchQueue[nextToSearch++];						//	retrieve the next component
			if (superarcs[sArc].CheckFlag(Superarc::isActive))			//	if it's active
				superarcs[sArc].SetFlag(Superarc::isSuppressed);			//	if we have checked it, suppress it
			
			if (*(supernodes[superarcs[sArc].bottomID].value) <= currentSelectionValue)
																//	if the low end is lower (i.e. we span)
				{ // arc spans the new value - terminal case for search
//				printf("Arc %ld selected at value %f\n", sArc, currentSelectionValue);
				backSearchQueue[backSearchQueueSize++] = sArc;			//	add to the reverse search queue
				superarcs[sArc].SetFlag(Superarc::isOnBackQueue);			//	set flag for arc
				AddToSelected(sArc);								//	and add it to the selection
				continue;											//	and don't search any further on this branch
				} // arc spans the new value - terminal case for search
				
			theNode = superarcs[sArc].bottomID;						//	grab a pointer to the low end
			theArc = supernodes[theNode].downList;						//	grab the next edge at the vertex
			for (int i = 0; i < supernodes[theNode].downDegree; i++)
				{ // loop to add all downward arcs to queue
				searchQueue[searchQueueSize++] = theArc;				//	put on queue 
				theArc = superarcs[theArc].nextDown;					//	and take next pointer at high end
				} // loop to add all downward arcs to queue
			} // while loop to process queue

		while (nextBackSearch < backSearchQueueSize)						//	until all processed
			{ // while loop to process reverse queue
			sArc = backSearchQueue[nextBackSearch++];					//	retrieve the next component
			if (superarcs[sArc].CheckFlag(Superarc::isActive))			//	if it's active
				superarcs[sArc].SetFlag(Superarc::isSuppressed);			//	if we have checked it, suppress it
			if (*(supernodes[superarcs[sArc].topID].value) > selectionRootValue)//	keep going until we're back at the root value
				continue;											//	i.e. search no further in this direction
			theNode = superarcs[sArc].topID;							//	grab a pointer to the high end
			theArc = supernodes[theNode].upList;						//	grab the next edge at the vertex
			for (int i = 0; i < supernodes[theNode].upDegree; i++)
				{ // loop to add all upward arcs to queue
				if (!superarcs[theArc].CheckFlag(Superarc::isOnBackQueue))//	if !already on queue
					{ // !on queue yet
					backSearchQueue[backSearchQueueSize++] = theArc;		//	put on queue 
					superarcs[theArc].SetFlag(Superarc::isOnBackQueue);	//	set flag
					} // !on queue yet
				theArc = superarcs[theArc].nextUp;						//	and take next arc going upwards
				} // loop to add all upward arcs to queue
			} // while loop to process reverse queue
		} // currentSelectionValue < selectionRootValue
	else
		{ // currentSelectionValue > selectionRootValue
//		printf("Searching upwards\n");
		while (nextToSearch < searchQueueSize)							//	until all processed
			{ // while loop to process queue
			sArc = searchQueue[nextToSearch++];						//	retrieve the next component
			if (superarcs[sArc].CheckFlag(Superarc::isActive))			//	if it's active
				superarcs[sArc].SetFlag(Superarc::isSuppressed);			//	if we have checked it, suppress it
			if (*(supernodes[superarcs[sArc].topID].value) > currentSelectionValue)
																//	if the high end is higher (i.e. we span)
				{ // arc spans the new value - terminal case for search
//				printf("Arc %ld selected\n", sArc);
				backSearchQueue[backSearchQueueSize++] = sArc;			//	add to the reverse search queue
				superarcs[sArc].SetFlag(Superarc::isOnBackQueue);		//	set flag for arc
				AddToSelected(sArc);								//	and add it to the selection
				continue;										//	and don't search any further on this branch
				} // arc spans the new value - terminal case for search
				
			theNode = superarcs[sArc].topID;							//	grab a pointer to the high end
			theArc = supernodes[theNode].upList;						//	grab the next edge at the vertex
			for (int i = 0; i < supernodes[theNode].upDegree; i++)
				{ // loop to add all upward arcs to queue
				searchQueue[searchQueueSize++] = theArc;				//	put on queue 
				theArc = superarcs[theArc].nextUp;						//	and take next pointer at high end
				} // loop to add all upward arcs to queue
			} // while loop to process queue

		while (nextBackSearch < backSearchQueueSize)						//	until all processed
			{ // while loop to process reverse queue
			sArc = backSearchQueue[nextBackSearch++];					//	retrieve the next component
			if (superarcs[sArc].CheckFlag(Superarc::isActive))			//	if it's active
				superarcs[sArc].SetFlag(Superarc::isSuppressed);			//	if we have checked it, suppress it
			if (*(supernodes[superarcs[sArc].bottomID].value) <= selectionRootValue)
																//	keep going until we're back at the root value
				continue;											//	i.e. search no further in this direction
			theNode = superarcs[sArc].bottomID;						//	grab a pointer to the low end
			theArc = supernodes[theNode].downList;						//	grab the next edge at the vertex
			for (int i = 0; i < supernodes[theNode].downDegree; i++)
				{ // loop to add all downward arcs to queue
				if (!superarcs[theArc].CheckFlag(Superarc::isOnBackQueue))//	if !already on queue
					{ // !on queue yet
					backSearchQueue[backSearchQueueSize++] = theArc;		//	put on queue 
					superarcs[theArc].SetFlag(Superarc::isOnBackQueue);	//	set flag
					} // !on queue yet
				theArc = superarcs[theArc].nextDown;					//	and take next arc going downwards
				} // loop to add all downward arcs to queue
			} // while loop to process reverse queue
		} // currentSelectionValue > selectionRootValue

	delete searchQueue; delete backSearchQueue;
	} // UpdateSelection()
	
void HeightField::CommitSelection()									//	commits the selection
	{ // CommitSelection()
	int sArc, activeArc, selectedArc;									//	loop indices

//	printf("CommitSelection()\n");
	for (selectedArc = 0; selectedArc < nSelectedArcs; selectedArc++)		//	for each selected arc
		{ // for each selected arc
		sArc = selected[selectedArc];									//	get its ID
		superarcs[sArc].ClearFlag(Superarc::isSelected);					//	clear the selection flag
		superarcs[sArc].ClearFlag(Superarc::isSuppressed);				//	clear the suppressed flag
		if (superarcs[sArc].CheckFlag(Superarc::isActive))				//	if it's already active
			superarcs[sArc].seedValue = currentSelectionValue;			//	reset the seed value
		else
			AddToActive(sArc, currentSelectionValue);					//	add to the active list
		} // for each selected arc

	for (activeArc = 0; activeArc < nActiveArcs;activeArc++)						//	walk through the arcs
		{ // for each arc
		sArc = active[activeArc];									//	get the next valid one
		if (superarcs[sArc].CheckFlag(Superarc::isSuppressed))				//	if it is suppressed
			{ // if suppressed
			superarcs[sArc].ClearFlag(Superarc::isSuppressed);			//	clear the suppression flag
			RemoveFromActive(sArc);									//	remove from active list
			continue;
			} // if suppressed
		} // for each arc
	nSelectedArcs = 0;												//	reset the logical selection size
	selectionRoot = NO_SUPERARC;										//	reset the root to "none"
	ClearRestorable();												//	invalidates a restorable set
	} // CommitSelection()
	
void HeightField::DeleteSelection()									//	deactivates the selected components
	{ // DeleteSelection()
	int sArc, activeArc, selectedArc;											//	loop indices
	ClearRestorable();												//	get ready to transfer to restorable

//	printf("DeleteSelection: %1d active arcs:", nActiveArcs);
	for (activeArc = 0; activeArc < nActiveArcs; )						//	walk through the arcs
		{ // for each arc
		sArc = active[activeArc];									//	get the next active one
//		printf(" %d", sArc);
		if (superarcs[sArc].CheckFlag(Superarc::isSuppressed))				//	if it is suppressed
			{ // if suppressed
//			printf("(S)");
			superarcs[sArc].ClearFlag(Superarc::isSuppressed);			//	clear the suppression flag
			RemoveFromActive(sArc);									//	remove from active list
			continue;												//	do !increment
			} // if suppressed
		activeArc++;												//	increment at end of loop
		} // for each arc
//	printf("\n");

	restorableOrigin = restorableSelection;								//	set the origin to "selection"
	restorableSelectionRoot = selectionRoot;							//	and save the root
	restorableCurrentValue = currentSelectionValue;						//	isovalue, 
	restorableRootValue = selectionRootValue;							//	&c.

	for (selectedArc = 0; selectedArc < nSelectedArcs; selectedArc++)						//	walk through the arcs
		{ // for each arc
		sArc = selected[selectedArc];									//	get the next active one
		if (superarcs[sArc].CheckFlag(Superarc::isActive))
			RemoveFromActive(sArc);										//	remove the arc from active status
		AddToRestorable(sArc);										//	and add it to restorable status
		} // for each arc

	ClearSelection();												//	clear selected: all we need to store is:
	selectionRoot = NO_SUPERARC;										//	reset the root to "none"
	} // DeleteSelection()
	
void HeightField::DeleteComplement()									//	deactivates the complement of the selection
	{ // DeleteComplement()
	int sArc, activeArc;											//	loop indices
	ClearRestorable();												//	get ready to transfer to restorable

	for (activeArc = 0; activeArc < nActiveArcs; )						//	walk through the arcs
		{ // for each arc
		sArc = active[activeArc];									//	get the next active one
		RemoveFromActive(sArc);										//	remove the arc from active status
		AddToRestorable(sArc);										//	and add it to restorable status
		} // for each arc

	restorableOrigin = restorableActive;								//	set the origin to "complement"
	} // DeleteComplement()
	
void HeightField::RestoreDeleted()										//	restores the last saved set									
	{ // RestoreDeleted()
	int sArc;														//	loop indices

	if (restorableOrigin == restorableSelection)							//	if we are restoring a selection
		{ // restoring selection
		SelectComponent(restorableSelectionRoot, restorableRootValue);		//	reselect it
		UpdateSelection(restorableCurrentValue);						//	and update the value
		} // restoring selection
	else
		{ // restoring complement
		while (nRestorableArcs > 0)									//	as long as they exist
			{ // loop through arcs
			sArc = restorable[0];									//	grab the first one
			RemoveFromRestorable(sArc);								//	delete it from there
			AddToActive(sArc, superarcs[sArc].seedValue);				//	add it back to active with its current isovalue
			} // loop through arcs
		} // restoring complement
	
	} // RestoreDeleted()

int HeightField::PickNode(float x, float y)								//	picks a node: returns -1 if it fails
	{ // PickNode()
	float bestDistanceSquared = 1.0;									//	start off with some ridiculous distance
	long whichNode = NO_SUPERNODE;									//	and start off with no node found
	
	for (int node = 0; node < nValidNodes; node++)						//	walk through nodes
		{ // node loop
		int theNode = validNodes[node];								//	lifting them from the valid list
//		printf("Node: %3d: ", theNode);
		float deltaX = x - supernodes[theNode].xPosn;					//	compute distance in x
		float deltaY = y - (supernodes[theNode].yPosn - MinHeight()) / (MaxHeight() - MinHeight());
																//	and distance in y
		float distSquared = deltaX*deltaX + deltaY*deltaY;				//	and square of distance
//		printf("dx: %7.3f, dy: %7.3f, dsqr: %7.3f\n", deltaX, deltaY, distSquared);
		if ((distSquared < 0.001) && (distSquared < bestDistanceSquared))	//	if it's the best choice so far
			{ whichNode = theNode; bestDistanceSquared = distSquared; }		//	store it and go on
		} // node loop
	return whichNode;												//	return whatever we ended up with	
	} // PickNode()

int HeightField::PickArc(float x, float y)								//	picks an arc: returns -1 if it fails
	{ // PickArc()
	y = MinHeight() + y * (MaxHeight() - MinHeight());					//	convert to the range of heights actually in use
	float bestDeltaX = 1.0;											//	best distance laterally
	long whichArc = NO_SUPERARC;										//	which arc has this distance

	for (int theArc = 0; theArc < nValidArcs; theArc++)					//	again, walk through all the arcs
		{ // arc loop
		int arc = valid[theArc];										//	grab the arc from the active list
		float deltaHt = *(supernodes[superarcs[arc].topID].value) - *(supernodes[superarcs[arc].bottomID].value);
																//	height range of arc
		if (*(supernodes[superarcs[arc].topID].value) <= y) continue;		//	discard arcs outside the range
		if (*(supernodes[superarcs[arc].bottomID].value) > y) continue;						
		if (deltaHt == 0.0) continue;									//	discard horizontal arcs
		float YSlope = (supernodes[superarcs[arc].topID].xPosn - supernodes[superarcs[arc].bottomID].xPosn) / deltaHt;
																//	this way, no divide by zero for vertical arcs
		float arcX =  supernodes[superarcs[arc].bottomID].xPosn + (y - *(supernodes[superarcs[arc].bottomID].value)) * YSlope;	
																//	compute x value at fixed height on arc
		float deltaX = fabs(arcX - x);								//	compute delta from point given	
		if ((deltaX < 0.02) && (deltaX < bestDeltaX))					//	check for tolerance & best
			{ whichArc = arc; bestDeltaX = deltaX; };					//	and update "best" so far
		} // arc loop
	return whichArc;												//	return the one we found (if any)
	} // PickArc()

void HeightField::UpdateNodeXPosn(int node, float x)						//	updates a node's x position
	{ // UpdateNodeXPosn()
	if (x < 0.01) x = 0.01; if (x > 0.99) x = 0.99;						//	clamp
	supernodes[node].xPosn = x;										//	update it
	} // UpdateNodeXPosn()

void HeightField::ComputeStatistics()									//	computes statistics on average path length, &c.
	{ // ComputeStatistics()
	long totalPathLength = 0, nSArcs = 0;								//	variables for capturing statistics
	
	totalTriangles = 0;												//	reset the field's variables
	maxPathLength = 0;
	totalExtractionTime = 0.0;										//	likewise
	for (int theArc = 0; theArc < nValidArcs; theArc++)					//	again, walk through all the arcs
		{ // loop through arcs
		int arc = valid[theArc];										//	grab the arc from the active list
		if (superarcs[arc].CheckFlag(Superarc::isSuppressed)) continue;
		if (	!superarcs[arc].CheckFlag(Superarc::isActive) &&
			!superarcs[arc].CheckFlag(Superarc::isSelected)) continue;
		totalTriangles += nContourTriangles[arc];						//	update statistics
		totalPathLength += pathLengths[arc];
		if (pathLengths[arc] > maxPathLength) maxPathLength = pathLengths[arc];
		totalExtractionTime += extractionTime[arc];
		nSArcs++;
		} // loop through arcs
	
	//gettimeofday(&thisFrameTime, NULL);
	time(&thisFrameTime);
	//float frameTime = thisFrameTime.tv_sec - lastFrameTime.tv_sec + 1E-6 * (thisFrameTime.tv_usec - lastFrameTime.tv_usec);
	float frameTime = difftime(thisFrameTime,lastFrameTime);
	frameRate = 1.0 / frameTime;
	//lastFrameTime.tv_sec = thisFrameTime.tv_sec; 
	//lastFrameTime.tv_usec = thisFrameTime.tv_usec;
	lastFrameTime = thisFrameTime;
	avgPathLength = (float) totalPathLength / nSArcs;
#ifdef DUMP_FRAME_TIMES
	timingBuffer += sprintf(timingBuffer, "------------------------------\n");
	timingBuffer += sprintf(timingBuffer, "Longest Path: %ld\nAverage Path: %8.5f\n", maxPathLength, avgPathLength);
	timingBuffer += sprintf(timingBuffer, "Total Triangles displayed: %ld.  Extracted in %8.5f seconds\n", totalTriangles, totalExtractionTime);
	timingBuffer += sprintf(timingBuffer, "Frame time: %8.5f.  Frame rate: %8.5f Hz.\n", frameTime, frameRate);
	flushTimingBuffer();	
#endif
	} // ComputeStatistics()

bool HeightField::ArcSelected(int arcIndex)								//	accessor for selected flag
	{ // ArcSelected()
	return superarcs[arcIndex].CheckFlag(Superarc::isSelected);				//	just check the flag on the arc
	} // ArcSelected()
	
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

void HeightField::ClearRestorable()									//	clears restorable list
	{ // ClearActive()
	while (nRestorableArcs > 0)										//	as long as there are any
		RemoveFromRestorable(restorable[0]);							//	remove the first one
	} // ClearActive()

void HeightField::ClearSelection()										//	clears selected list
	{ // ClearSelection()
//	printf("ClearSelection: %1d arcs:", nSelectedArcs);
	for (int arc = 0; arc < nSelectedArcs; arc++)						//	walk through the selection
		{
//		printf(" %1d", selected[arc]);
		superarcs[selected[arc]].ClearFlag(Superarc::isSelected);			//	removing from the selection
		}
//	printf("\n");
	nSelectedArcs = 0;												//	and set logical number to 0
	} // ClearSelection()

void HeightField::ClearSuppressed()									//	resets all suppressed arcs to active
	{ // ClearSuppressed()
	for (int arc = 0; arc < nActiveArcs; arc++)							//	walk through array
		superarcs[active[arc]].ClearFlag(Superarc::isSuppressed);			//	clearing it
	} // ClearSuppressed()

void HeightField::ClearBackQueue()										//	removes all vertices from the back queue
	{ // ClearSuppressed()
	for (int arc = 0; arc < nActiveArcs; arc++)							//	walk through array
		superarcs[active[arc]].ClearFlag(Superarc::isOnBackQueue);			//	clearing it
	} // ClearSuppressed()

void HeightField::AddToActive(long arc, float seedValue)					//	adds an arc to the active list		
	{ // AddToActive()
//	printf("Making arc %d active\n", arc);
	if (superarcs[arc].CheckFlag(Superarc::isActive))						//	check to see if already active
		{ // paranoid check
		printf("Error: attempt to activate arc %1d that is already active.\n", arc);	
		return;
		} // paranoid check
	superarcs[arc].index = nActiveArcs;								//	set the index to next available
	superarcs[arc].SetFlag(Superarc::isActive);							//	set the flag
	superarcs[arc].SetFlag(Superarc::isDirty);							//	be conservative: re-extract
	superarcs[arc].seedValue = seedValue;								//	set the seed value
	active[nActiveArcs++] = arc;										//	and add to the array
	} // AddToActive()

void HeightField::RemoveFromActive(long arc)								//	removes arc from the active list
	{ // RemoveFromActive()
	if (!superarcs[arc].CheckFlag(Superarc::isActive))
		{ // paranoid check
		printf("Error: attempt to deactivate arc %1d that is !already active.\n", arc);	
		return;
		} // paranoid check
	long currentIndex = superarcs[arc].index;							//	grab the current index
	long tailEnd = active[--nActiveArcs];								//	the tail end of the array
	active[currentIndex] = tailEnd;									//	swap the last entry into place
	superarcs[tailEnd].index = currentIndex;							//	and reset it's index
	superarcs[arc].index = -1;										//	reset the arc's index
	superarcs[arc].ClearFlag(Superarc::isActive);						//	and clear the flag
	} // RemoveFromActive()

void HeightField::AddToValid(long arc)									//	adds an arc to the valid list		
	{ // AddToValid()
	if (superarcs[arc].CheckFlag(Superarc::isValid))						//	check to see if already valid
		{ // paranoid check
		printf("Error: attempt to make arc %1d valid when it is already valid.\n", arc);	
		return;
		} // paranoid check
	superarcs[arc].valid_index = nValidArcs;							//	set the index to next available
	superarcs[arc].SetFlag(Superarc::isValid);							//	set the flag
	valid[nValidArcs++] = arc;										//	and add to the array
	} // AddToValid()

void HeightField::RemoveFromValid(long arc)								//	removes arc from the valid list
	{ // RemoveFromValid()
	if (!superarcs[arc].CheckFlag(Superarc::isValid))
		{ // paranoid check
		printf("Error: attempt to make arc %1d invalid when it is !already valid.\n", arc);	
		return;
		} // paranoid check
	if (superarcs[arc].CheckFlag(Superarc::isActive)) RemoveFromActive(arc);	//	make sure it is inactive
	if (superarcs[arc].CheckFlag(Superarc::isRestorable)) RemoveFromRestorable(arc);
	long currentIndex = superarcs[arc].valid_index;						//	grab the current index
	long tailEnd = valid[--nValidArcs];								//	the tail end of the array
	valid[currentIndex] = tailEnd;									//	swap the last entry into place
	superarcs[tailEnd].valid_index = currentIndex;						//	and reset it's index
	superarcs[arc].valid_index = -1;									//	reset the arc's index
	superarcs[arc].ClearFlag(Superarc::isValid);							//	and clear the flag
	} // RemoveFromValid()

void HeightField::AddToSelected(long arc)								//	adds an arc to the selected list		
	{ // AddToSelected()
	if (superarcs[arc].CheckFlag(Superarc::isSelected))					//	check to see if already selected
		{ // paranoid check
		printf("Error: attempt to add arc %1d to selection when it is already selected.\n", arc);	
		return;
		} // paranoid check
	superarcs[arc].SetFlag(Superarc::isDirty);							//	force surface construction
	superarcs[arc].SetFlag(Superarc::isSelected);						//	set the flag
	selected[nSelectedArcs++] = arc;									//	and add to the array
	} // AddToSelected()

void HeightField::AddToRestorable(long arc)								//	adds an arc to the restorable list		
	{ // AddToRestorable()
	if (superarcs[arc].CheckFlag(Superarc::isRestorable))					//	check to see if already restorable
		{ // paranoid check
		printf("Error: attempt to make arc %1d restorable when it is already restorable.\n", arc);	
		return;
		} // paranoid check
	superarcs[arc].index = nRestorableArcs;								//	set the index to next available
	superarcs[arc].SetFlag(Superarc::isRestorable);						//	set the flag
	restorable[nRestorableArcs++] = arc;								//	and add to the array
	} // AddToRestorable()

void HeightField::RemoveFromRestorable(long arc)							//	removes arc from the restorable list
	{ // RemoveFromRestorable()
	if (!superarcs[arc].CheckFlag(Superarc::isRestorable))
		{ // paranoid check
		printf("Error: attempt to remove arc %1d from restorable[] when it is !already restorable.\n", arc);	
		return;
		} // paranoid check
	long currentIndex = superarcs[arc].index;							//	grab the current index
	long tailEnd = restorable[--nRestorableArcs];						//	the tail end of the array
	restorable[currentIndex] = tailEnd;								//	swap the last entry into place
	superarcs[tailEnd].index = currentIndex;							//	and reset it's index
	superarcs[arc].index = -1;										//	reset the arc's index
	superarcs[arc].ClearFlag(Superarc::isRestorable);						//	and clear the flag
	} // RemoveFromRestorable()

void HeightField::AddToValidNodes(long node)								//	adds an node to the valid list		
	{ // AddToValidNodes()
//	printf("Adding node %d to validNodes[]\n", node);
	if (supernodes[node].valid_index != -1)								//	check whether already valid
		{ // paranoid check
		printf("Error: attempt to make node %1d valid when it is already valid.\n", node);	
		return;
		} // paranoid check
	supernodes[node].valid_index = nValidNodes;							//	set the index to next available
	validNodes[nValidNodes++] = node;									//	and add to the array
	} // AddToValidNodes()

void HeightField::RemoveFromValidNodes(long node)							//	removes node from the valid list
	{ // RemoveFromValidNodes()
//	printf("Removing node %d from validNodes[]\n", node);
	if (supernodes[node].valid_index == -1)								//	check whether already valid
		{ // paranoid check
		printf("Error: attempt to make node %1d invalid when it is already invalid.\n", node);	
		return;
		} // paranoid check
	long currentIndex = supernodes[node].valid_index;						//	grab the current index
	long tailEnd = validNodes[--nValidNodes];							//	the tail end of the array
	validNodes[currentIndex] = tailEnd;								//	swap the last entry into place
	supernodes[tailEnd].valid_index = currentIndex;						//	and reset it's index
	supernodes[node].valid_index = -1;									//	reset the node's index
	} // RemoveFromValidNodes()

long HeightField::GetSuperarcColour(long theArc)							//	accessor to retrieve arc colour
	{ // GetSuperarcColour()
	if (theArc < 0) return -1;
	if (theArc >= nextSuperarc) return -1;
	return superarcs[theArc].colour;
	} // GetSuperarcColour()
	
void HeightField::SetSuperarcColour(long theArc, long theColour)				//	accessor to set arc colour
	{ // SetSuperarcColour()
	if (theArc < 0) return;
	if (theArc >= nextSuperarc) return;
	if (supernodes[superarcs[theArc].bottomID].colour == superarcs[theArc].colour)		//	update ends if they are the same colour
            supernodes[superarcs[theArc].bottomID].colour = theColour;
	if (supernodes[superarcs[theArc].topID].colour == superarcs[theArc].colour)
            supernodes[superarcs[theArc].topID].colour = theColour;
	superarcs[theArc].colour = theColour;
	} // SetSuperarcColour()
	
	
