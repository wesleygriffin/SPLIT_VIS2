//	IsoSurface II
//	Hamish Carr, 2003

//	HeightField:			class representing the height field
//	HeightField_Collapse:	routines dealing with collapsing the contour tree

#include "HeightField.h"													//	include the header for the definitions
#include "PriorityIndex.h"
#include <stdlib.h>
#include <queue>
#include <vector>
#include <math.h>

//#define PRIORITY_VOLUME

using namespace std;
static int dotFileNo = 1;												//	index number for generating dot files

void HeightField::CollapseEpsilonEdges()									//	collapses the epsilon-height edges
	{ // CollapseEpsilonEdges()
	timingBuffer += sprintf(timingBuffer, "Before collapse: %d arcs.\n", nSuperarcs);
	time(&thatTime);
	wasChecked = (bool *) calloc(nSupernodes, sizeof(bool));					//	array recording which nodes we've checked (init. to zero)
	supernodeQueue = (long *) malloc(nSupernodes * sizeof(long));				//	queue for supernodes
	long qNext;														//	keep track of logical next item
	long upArc, downArc, nArcs;											//	used for walking around nodes

//	CheckNodeCounts();
//	PrintContourTree();
//	ContourTreeToDotFile("uncollapsed", 0, true);
	//	loop through all supernodes: test each to see if it has any zero-height edges
	for (int node = 0; node < nSupernodes; node++)
		{ // loop through supernodes
		if (wasChecked[node]) continue;									//	skip nodes we've already checked
		wasChecked[node] = true;											//	and mark this one as checked
		
//		printf("Checking supernode %d\n", node);
		qNext = supernodeQueueSize = 0;									//	reset the queue
		//	now, walk around the uparcs at the node
		upArc = supernodes[node].upList;									//	find the first ascending arc
		nArcs = supernodes[node].upDegree;									//	we need this, because RemoveArc() decrements degree
		for (int i = 0; i < nArcs; i++)									//	loop through arcs at node
			{ // loop through up arcs at node
//			printf("Checking superarc %d\n", upArc);
			long nextUpArc = superarcs[upArc].nextUp;						//	find the next arc
			RemoveEdgeIfEpsilonHeight(upArc);								//	check the edge
			upArc = nextUpArc;											//	and walk to the next one					
			} // loop through up arcs at node

		//	now, walk around the downarcs at the node
		downArc = supernodes[node].downList;								//	find the first descending arc
		nArcs = supernodes[node].downDegree;								//	we need this, because RemoveArc() decrements degree
		for (int i = 0; i < nArcs; i++)									//	loop through arcs at node
			{ // loop through down arcs at node
//			printf("Checking superarc %d\n", downArc);
			long nextDownArc = superarcs[downArc].nextDown;					//	find the next arc
			RemoveEdgeIfEpsilonHeight(downArc);							//	check the edge
			downArc = nextDownArc;										//	and walk to the next one					
			} // loop through down arcs at node

		//	now, as long as there is stuff on the queue
		while(qNext < supernodeQueueSize)								
			{ //  loop through nodes on the queue
//			printf("Checking supernode %d (%d on queue)\n", supernodeQueue[qNext], qNext);
			int nextNode = supernodeQueue[qNext++];							//	grab the next node on the queue
			wasChecked[nextNode] = true;									//	mark it as checked				

			//	now, walk around the uparcs at the node
			upArc = supernodes[nextNode].upList;							//	find the first ascending arc
			nArcs = supernodes[nextNode].upDegree;							//	we need this, because RemoveArc() decrements degree
			for (int i = 0; i < nArcs; i++)								//	loop through arcs at node
				{ // loop through up arcs at node
				long nextUpArc = superarcs[upArc].nextUp;					//	find the next arc
//				printf("Checking superarc %d\n", upArc);
				if (! RemoveEdgeIfEpsilonHeight(upArc))					//	check the edge
					superarcs[upArc].bottomID = node;						//	attach the top end to the collapsed supernode
				upArc = nextUpArc;										//	and walk to the next one					
				} // loop through up arcs at node
			
			//	now, walk around the downarcs at the node
			downArc = supernodes[nextNode].downList;						//	find the first descending arc
			nArcs = supernodes[nextNode].downDegree;						//	we need this, because RemoveArc() decrements degree
			for (int i = 0; i < nArcs; i++)								//	loop through arcs at node
				{ // loop through down arcs at node
//				printf("Checking superarc %d\n", downArc);
				long nextDownArc = superarcs[downArc].nextDown;				//	find the next arc
				if (! RemoveEdgeIfEpsilonHeight(downArc))					//	check the edge
					superarcs[downArc].topID = node;						//	attach the top end to the collapsed supernode
				downArc = nextDownArc;									//	and walk to the next one					
				} // loop through down arcs at node

			TransferArcLists(node, nextNode);								//	merge nextNode's arcs onto node's
			RemoveFromValidNodes(nextNode);								//	remove nextNode from the valid list
//			ContourTreeToDotFile("collapsed", dotFileNo++);
//			PrintContourTree();
			} // loop through nodes on the queue				
		
		//	now check to see if the combined node is now a regular node
		if (supernodes[node].IsRegular())
			{ // its now a regular node
//			printf("Node %d now a regular point.  Collapsing.\n", node);
//			ContourTreeToDotFile("collapsed", dotFileNo++);
			CollapseVertex(node);										//	collapse the node itself
//			ContourTreeToDotFile("collapsed", dotFileNo++);
			} // its now a regular node
		} // loop through supernodes

	nNonEpsilonArcs = nValidArcs;											//	store the number of non-epsilon arcs
//	printf("Nodes: \n");
//	for (int i = 0; i < nValidNodes; i++)
//		printf("%3d ", validNodes[i]);
//	printf("\n");
//	printf("Arcs:\n");
//	for (int i = 0; i < nValidArcs; i++)
//		printf("%3d ", valid[i]);
//	printf("\n");
	// release temporary memory
	free(wasChecked); free(supernodeQueue);
//	ContourTreeToDotFile("collapsed", dotFileNo++);
	time(&thisTime);
	timingBuffer += sprintf(timingBuffer, "Epsilon-collapsed tree to %d arcs.\n", nValidArcs);
	//timingBuffer += sprintf(timingBuffer, "Collapse took %8.5f seconds\n", (float)(thisTime.tv_sec - thatTime.tv_sec) + 1E-6 * (thisTime.tv_usec - thatTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Collapse took %8.5f seconds\n", difftime(thisTime,thatTime));
//	CheckNodeCounts();
	} // CollapseEpsilonEdges()
	
bool HeightField::RemoveEdgeIfEpsilonHeight(long theArc)						//	routine to check an arc, and remove from tree if needed
	{ // RemoveEdgeIfEpsilonHeight()
	long topNode = superarcs[theArc].topID, bottomNode = superarcs[theArc].bottomID;
	if (*(supernodes[topNode].value) == *(supernodes[bottomNode].value))
		{ // found an epsilon-height edge
//		printf("Found epsilon-height edge %d - %d\n", superarcs[theArc].topID, superarcs[theArc].bottomID);
		if (! wasChecked[topNode])
			supernodeQueue[supernodeQueueSize++] = topNode;					//	add the "high end" to the queue
		if (! wasChecked[bottomNode])
			supernodeQueue[supernodeQueueSize++] = bottomNode;				//	or the bottom one
		RemoveArc(theArc);												//	and remove the arc from the contour tree
//		ContourTreeToDotFile("collapsed", dotFileNo++);
//		PrintContourTree();
		return true;
		} // found an epsilon-height edge
	return false;	
	} // RemoveEdgeIfEpsilonHeight()
	
void HeightField::RemoveArc(long theArc)									//	routine to remove an arc from the tree
	{ // RemoveArc()
	long topNode = superarcs[theArc].topID, bottomNode = superarcs[theArc].bottomID;//	retrieve top & bottom of arc
	long NextDown = superarcs[theArc].nextDown, LastDown = superarcs[theArc].lastDown;
	long NextUp = superarcs[theArc].nextUp, LastUp = superarcs[theArc].lastUp;
	
	//	first, remove it at the high end
	//	check to see if there are any other edges at this end
	if (NextDown == theArc)												//	if it's the only down edge at this node
		supernodes[topNode].downList = NO_SUPERARC;							//	make it point to NO_SUPERARC
	else
		{ // not the only edge
		if (supernodes[topNode].downList == theArc)							//	make sure the entry pointer stays valid
			supernodes[topNode].downList = NextDown;
		superarcs[LastDown].nextDown = NextDown;
		superarcs[NextDown].lastDown = LastDown;
		} // not the only edge
	// now do the same at the low end
	if (NextUp == theArc)												//	if it's the only up edge at this node
		supernodes[bottomNode].upList = NO_SUPERARC;							//	make it point to NO_SUPERARC
	else
		{ // not the only edge
		if (supernodes[bottomNode].upList == theArc)							//	make sure the entry pointer stays valid
			supernodes[bottomNode].upList = NextUp;
		superarcs[LastUp].nextUp = NextUp;
		superarcs[NextUp].lastUp = LastUp;
		} // not the only edge
	supernodes[topNode].downDegree--;
	supernodes[bottomNode].upDegree--;
	//	and remove the arc from the list of valid arcs
	RemoveFromValid(theArc);
	} // RemoveArc()

// *************	
// ** WARNING **
// *************	
//
// This routine assumes that the fromNode's arcs have already had the hiID/loID and hiEnd/loEnd correctly reset to the onto node
//
void HeightField::TransferArcLists(int ontoNode, int fromNode)					//	transfer's arcs from one node to another
	{ // TransferArcLists()
//	printf("Transferring arcs from %3d to %3d\n", fromNode, ontoNode);
	//	transfer the up list
	if (supernodes[ontoNode].upDegree == 0)									//	simple case: onto node is empty 
		{ // empty list
		supernodes[ontoNode].upDegree = supernodes[fromNode].upDegree;			//	just copy across
		supernodes[ontoNode].upList = supernodes[fromNode].upList;
		} // empty list
	else if (supernodes[fromNode].upDegree != 0)								//	if the second one is non-empty
		{ // two non-empty lists
		long fromFirst = supernodes[fromNode].upList;
		long ontoFirst = supernodes[ontoNode].upList;
		long fromLast, ontoNext;											//	pointers to the two nodes needing fixing
		fromLast = superarcs[fromFirst].lastUp;
		ontoNext = superarcs[ontoFirst].nextUp;
		superarcs[ontoNext].lastUp = fromLast;
		superarcs[fromLast].nextUp = ontoNext;
		superarcs[fromFirst].lastUp = ontoFirst;
		superarcs[ontoFirst].nextUp = fromFirst;
		supernodes[ontoNode].upDegree += supernodes[fromNode].upDegree;
		} // two non-empty lists

	//	transfer the down list
	if (supernodes[ontoNode].downDegree == 0)								//	simple case: onto node is empty 
		{ // empty list
		supernodes[ontoNode].downDegree = supernodes[fromNode].downDegree;		//	just copy across
		supernodes[ontoNode].downList = supernodes[fromNode].downList;
		} // empty list
	else if (supernodes[fromNode].downDegree != 0)							//	if the second one is non-empty
		{ // two non-empty lists
		long fromFirst = supernodes[fromNode].downList;
		long ontoFirst = supernodes[ontoNode].downList;
		long fromLast, ontoNext;											//	pointers to the two nodes needing fixing
		fromLast = superarcs[fromFirst].lastDown;
		ontoNext = superarcs[ontoFirst].nextDown;
		superarcs[ontoNext].lastDown = fromLast;
		superarcs[fromLast].nextDown = ontoNext;
		superarcs[fromFirst].lastDown = ontoFirst;
		superarcs[ontoFirst].nextDown = fromFirst;
		supernodes[ontoNode].downDegree += supernodes[fromNode].downDegree;
		} // two non-empty lists
	supernodes[fromNode].upDegree = 0;										//	and reset the from node				
	supernodes[fromNode].upList = NO_SUPERARC;
	supernodes[fromNode].downDegree = 0;						
	supernodes[fromNode].downList = NO_SUPERARC;
	} // TransferArcLists()
	
long HeightField::CollapseVertex(long whichSupernode)							//	collapses a vertex: returns the new superarcs ID
	{ // CollapseVertex()
	if (! supernodes[whichSupernode].IsRegular())
		{ // not a regular point
		printf("Major error: attempt to collapse a vertex with up-degree %1d and down-degree %1d. (both should be 1).\n",
			supernodes[whichSupernode].upDegree, supernodes[whichSupernode].downDegree);
		return NO_SUPERARC;
		} // not a regular point

	//	grab the existing arcs & nodes
	long topArc = supernodes[whichSupernode].upList, bottomArc = supernodes[whichSupernode].downList;
	long topNode = superarcs[topArc].topID, bottomNode = superarcs[bottomArc].bottomID;

//	printf("Collapsing edges %d and %d\n", topArc, bottomArc);

	//	first remove the existing arcs
	RemoveArc(topArc);	RemoveArc(bottomArc);
	
	//	and remove the vertex
	RemoveFromValidNodes(whichSupernode);
	
	//	now add the new arc
	long newSArc = AddSuperarc(topNode, bottomNode, NULL, NULL, NULL, NULL);

	//	now fill in the hierarchical fields
	superarcs[newSArc].topArc = topArc; superarcs[newSArc].bottomArc = bottomArc;
	superarcs[newSArc].nodesThisSideOfTop = superarcs[topArc].nodesThisSideOfTop;
	superarcs[newSArc].nodesThisSideOfBottom = superarcs[bottomArc].nodesThisSideOfBottom;
	//	this one could equally be the sum of the two parts, but this value is also defensible
	superarcs[newSArc].nodesOnArc = superarcs[newSArc].nodesThisSideOfTop - (nVertices - superarcs[newSArc].nodesThisSideOfBottom);

	//	now do the same for the Riemann sum
	superarcs[newSArc].sampleSumTop = superarcs[topArc].sampleSumTop;
	superarcs[newSArc].sampleSumBottom = superarcs[bottomArc].sampleSumBottom;
	superarcs[newSArc].sampleSumOnArc = superarcs[newSArc].sampleSumTop + superarcs[newSArc].sampleSumBottom - sampleSum;
	
//	printf("top: %8.1f, bot: %8.1f, on: %8.1f\n", superarcs[newSArc].sampleSumTop, superarcs[newSArc].sampleSumBottom, superarcs[newSArc].sampleSumOnArc);
	
	return newSArc;
	} // CollapseVertex()

void HeightField::CollapseToSuperarc(long whichSuperarc)						//	collapses to a known superarc						
	{ // CollapseToSuperarc()
	long topArc = superarcs[whichSuperarc].topArc, bottomArc = superarcs[whichSuperarc].bottomArc;
	long topID = superarcs[whichSuperarc].topID, middleID = superarcs[topArc].bottomID, bottomID = superarcs[whichSuperarc].bottomID;
	bool topWasActive = superarcs[topArc].CheckFlag(Superarc::isActive), bottomWasActive = superarcs[bottomArc].CheckFlag(Superarc::isActive);
	//	first remove the existing arcs
	if (topWasActive) RemoveFromActive(topArc);
	if (bottomWasActive) RemoveFromActive(bottomArc);
	RemoveArc(topArc);	RemoveArc(bottomArc);
	
	//	and remove the vertex
	RemoveFromValidNodes(middleID);
	
	//	now insert the arc into the existing tree
	AddDownArc(topID, whichSuperarc);										//	insert at top end
	AddUpArc(bottomID, whichSuperarc);										//	and at bottom end

	//	and set the valid flag
	AddToValid(whichSuperarc);

	//	now make sure that the active flag is properly set: give priority to top arc
	if (topWasActive)
		AddToActive(whichSuperarc, superarcs[topArc].seedValue);				//	add, transferring seed value
	else if (bottomWasActive)		
		AddToActive(whichSuperarc, superarcs[bottomArc].seedValue);				//	likewise

	//	and check for "selected" status
	if ((selectionRoot == topArc) || (selectionRoot == bottomArc))				//	if either was the root of the selection
		selectionRoot = whichSuperarc;									//	use the collapsed edge instead

	} // CollapseToSuperarc()

long HeightField::Priority(long arc, bool upSweep)							//.	computes priority for a given arc
	{ // Priority()
//	printf("In Priority()\n");
	long volume;
	double riemannSum;
	if (upSweep)
		{ // up sweep
		volume = superarcs[arc].nodesThisSideOfTop;				//	set priority to volume on edge
//		printf("Arc %2d. Raw riemann: %12.1f.  Volume: %8d. Value: %12.1f. V*V: %12.1f\n", arc, superarcs[arc].sampleSumTop, 
//			volume, *(supernodes[superarcs[arc].topID].value), volume * *(supernodes[superarcs[arc].topID].value));
		riemannSum = - superarcs[arc].sampleSumTop + volume * *(supernodes[superarcs[arc].topID].value);
		} // up sweep
	else
		{ // down sweep
		volume = superarcs[arc].nodesThisSideOfBottom;
//		printf("Arc %2d. Raw riemann: %12.1f.  Volume: %8d. Value: %12.1f. V*V: %12.1f\n", arc, superarcs[arc].sampleSumBottom, 
//			volume, *(supernodes[superarcs[arc].bottomID].value), volume * *(supernodes[superarcs[arc].bottomID].value));
		riemannSum = superarcs[arc].sampleSumBottom - volume * *(supernodes[superarcs[arc].bottomID].value);
		} // down sweep
	
	double height = *(supernodes[superarcs[arc].topID].value) - *(supernodes[superarcs[arc].bottomID].value);
	double approxhypervolume = (((double) volume) * height);

//	printf("Approx: %12.1f.  Riemann: %12.1f.  Ratio: %5.3f\n", approxhypervolume, riemannSum, approxhypervolume/riemannSum);

	double scaledhypervolume = ((double) 1E6) * (riemannSum / (((double) nVertices) * (MaxHeight() - MinHeight())));

	switch(collapsePriority)
		{ // collapsePriority
		case PRIORITY_HEIGHT:
			return (long) height;
		case PRIORITY_VOLUME:
			return volume;
		case PRIORITY_APPROX_HYPERVOLUME:
			return (long) (1E6 * approxhypervolume / (double) (nVertices * (MaxHeight() - MinHeight())));
		case PRIORITY_HYPERVOLUME:
		default:
			return (long) scaledhypervolume;
		} // collapsePriority
	} // Priority()

void HeightField::CollapseLeafPruning(int targetSize, long prunedVolumeBound)		//	leaf-prunes until one of the bounds is reached
	{ // CollapseLeafPruning()
	time(&thatTime);
	priority_queue<PriorityIndex, vector<PriorityIndex>, PriorityCompare> pQueue;	//	the priority queue
	//ContourTreeToDotFile("collapsed", dotFileNo++, true);
			
	//	load all the leaves up on the queue
	for (long theNode = 0; theNode < nValidNodes; theNode++)
		{ // for each node
		long node = validNodes[theNode];
//		printf("Node %d is %d with degrees %d up and %d down\n", theNode, node, supernodes[node].upDegree, supernodes[node].downDegree);
		if (supernodes[node].IsUpperLeaf())
			{ // upper leaf
			long theArc = supernodes[node].downList;						//	the edge to queue up is the sole down edge
			pQueue.push(PriorityIndex(theArc, Priority(theArc, false)));		//	add to priority queue
//			printf("Adding upper leaf %d to queue with priority %d\n", theArc, priority);
			} // upper leaf
		else if (supernodes[node].IsLowerLeaf())
			{ // lower leaf
			long theArc = supernodes[node].upList;							//	the edge to queue up is the sole up edge
			pQueue.push(PriorityIndex(theArc, Priority(theArc, true)));			//	add to priority queue
//			printf("Adding lower leaf %d to queue with priority %d\n", theArc, priority);
			} // lower leaf
		} // for each node
		
//	PrintCollapseRecord();
	collapseBounds[nValidArcs-1] = 1;										//	this is the only one we don't know
	//	now process leaves one at a time, until we hit our bounds
	while((! pQueue.empty()) && (nValidArcs > targetSize))					//	stop when we have the target size in # of arcs
		{ // loop through priority queue
		PriorityIndex theIndex = pQueue.top();								//	retrieve the first arc on the priority queue
//		printf("Arc %d retrieved from queue with volume %d:\n", theIndex.index, theIndex.priority);
		pQueue.pop();													//	and remove it from the priority queue
//		ContourTreeToDotFile("collapsed", dotFileNo++, theIndex.index);
		if (theIndex.priority > prunedVolumeBound)							//	if the leaf being pruned is bigger than the target
			{ // too big
//			printf("Priority %d greater than volume bound %d\n", theIndex.priority, prunedVolumeBound);
			break;													//	then we're done, since all others are bigger
			} // too big
		long theArc = theIndex.index;										//	retrieve the arc's ID
		if (! superarcs[theArc].CheckFlag(Superarc::isValid))				//	this arc was collapsed in a regular point reduction
			{ // arc invalid
//			printf("Arc is invalid.\n");
			continue;													//	so we ignore it and continue
			} // arc invalid
		//	grab the nodes at each end
		long topNode = superarcs[theArc].topID, bottomNode = superarcs[theArc].bottomID;
		long interiorNode;
			
//		printf("Pruning arc %d\n", theArc);
		if (supernodes[topNode].IsUpperLeaf())
			{ // top leaf
//			printf("upper leaf.\n");
			if (supernodes[bottomNode].upDegree == 1)						//	this is the only up-leaf at that vertex
				continue;												//	then we want to leave it in the tree
			RemoveArc(theArc);											//	delete the superarc
			RemoveFromValidNodes(topNode);								//	delete the leaf node
			collapseRecord[nValidArcs] = theArc;							//	store the arc
			interiorNode = bottomNode;									//	and set the interior node
			} // top leaf
		else if (supernodes[bottomNode].IsLowerLeaf())
			{ // top leaf
//			printf("lower leaf.\n");
			if (supernodes[topNode].downDegree == 1)						//	this is the only down-leaf at that vertex
				continue;												//	then we want to leave it in the tree
			RemoveArc(theArc);											//	delete the superarc
			RemoveFromValidNodes(bottomNode);								//	delete the leaf node
			collapseRecord[nValidArcs] = theArc;							//	store the arc
			interiorNode = topNode;										//	and set the interior node
			} // top leaf
		else	
			{ // bad edge
			printf("Major error: attempt to prune edge %d which is not actually a leaf.\n");
			break;
			} // bad edge
		collapseBounds[nValidArcs - 1] = theIndex.priority;					//	store the priority bound (after decrement)

//		PrintCollapseRecord();

		//	now check for regular-point collapse
		if (supernodes[interiorNode].IsRegular())
			{ // regular point
			long theArc = CollapseVertex(interiorNode);						//	collapse it
			collapseBounds[nValidArcs - 1] = theIndex.priority;				//	store the priority bound (after decrement)
//			printf("Collapsing regular point %3d to get new arc %3d\n", interiorNode, theArc);
//			ContourTreeToDotFile("collapsed", dotFileNo++, theArc);
			topNode = superarcs[theArc].topID;								//	grab the top/bottom ID's
			bottomNode = superarcs[theArc].bottomID;
			if (supernodes[topNode].IsUpperLeaf())
				{ // upper leaf
				pQueue.push(PriorityIndex(theArc, Priority(theArc, false)));
//				printf("Adding upper leaf %d to queue with priority %d\n", theArc, priority);
				} // upper leaf
			else if (supernodes[bottomNode].IsLowerLeaf())
				{ // lower leaf
				pQueue.push(PriorityIndex(theArc, Priority(theArc, true)));		//	add to priority queue
//				printf("Adding upper leaf %d to queue with priority %d\n", theArc, priority);
				} // lower leaf
			collapseRecord[nValidArcs] = theArc;							//	store the arc
//			PrintCollapseRecord();
			} // regular point
		else if (supernodes[interiorNode].IsUpperLeaf())
			{ // upper leaf
			long theArc = supernodes[interiorNode].downList;					//	the edge to queue up is the sole down edge
			pQueue.push(PriorityIndex(theArc, Priority(theArc, false)));		//	add to priority queue
//			printf("Adding upper leaf %d to queue with priority %d\n", theArc, priority);
			} // upper leaf
		else if (supernodes[interiorNode].IsLowerLeaf())
			{ // lower leaf
			long theArc = supernodes[interiorNode].upList;					//	the edge to queue up is the sole up edge
			pQueue.push(PriorityIndex(theArc, Priority(theArc, true)));			//	add to priority queue
//			printf("Adding lower leaf %d to queue with priority %d\n", theArc, priority);
			} // lower leaf
//		CheckNodeCounts();
		} // loop through priority queue

//	printf("Collapse Bounds:\n");
//	for (long whichCollapseBound = 0; whichCollapseBound < nNonEpsilonArcs; whichCollapseBound++)
//		printf("%2d %6d %7.5f\n", whichCollapseBound, collapseBounds[whichCollapseBound], 
//			log((float)collapseBounds[whichCollapseBound])/log((float)nNonEpsilonArcs)); 

//	printf("Contents of priority queue:\n");
//	while (not pQueue.empty())
//		{ // emptying queue
//		PriorityIndex theIndex = pQueue.top();								//	retrieve the first arc on the priority queue
//		pQueue.pop();													//	and remove it from the priority queue
//		printf("Arc %d retrieved from queue with volume %d:\n", theIndex.index, theIndex.priority);
//		} // emptying queue	

//	ContourTreeToDotFile("collapsed", dotFileNo++);
	time(&thisTime);
	timingBuffer += sprintf(timingBuffer, "Leaf-pruned tree to %d arcs with priority bound of %d.\n", nValidArcs, prunedVolumeBound);
	//timingBuffer += sprintf(timingBuffer, "Pruning took %8.5f seconds\n", (float)(thisTime.tv_sec - thatTime.tv_sec) + 1E-6 * (thisTime.tv_usec - thatTime.tv_usec));
	timingBuffer += sprintf(timingBuffer, "Pruning took %8.5f seconds\n", (float)difftime(thisTime,thatTime));
//	CheckNodeCounts();
//	PrintCollapseRecord();
	} // CollapseLeafPruning()

void HeightField::SingleCollapse()											//	collapses a single node after constructing collapse tree
	{ // SingleCollapse()
	for (int arc = 0; arc < nActiveArcs; arc++)
		if (! superarcs[active[arc]].CheckFlag(Superarc::isActive))
			printf("%20s:%5d: At collapse step %5d, arc %3d is on active list but active flag is not set\n", 
				__FILE__, __LINE__, nextSuperarc-savedNextSuperarc, active[arc]);
		else if (! superarcs[active[arc]].CheckFlag(Superarc::isValid))
			printf("%20s:%5d: At collapse step %5d, arc %3d is on active list but valid flag is not set\n", 
				__FILE__, __LINE__, nextSuperarc-savedNextSuperarc, active[arc]);
	if (nValidArcs <= 1) return;
	ClearRestorable();													//	simplify life by having no "restorables"
	long whichArc = collapseRecord[nValidArcs - 1];							//	find out which arc we will be dealing with
	if (superarcs[whichArc].CheckFlag(Superarc::isValid))						//	if the next to collapse is currently valid
		{ // leaf-prune
		if (selectionRoot == whichArc)									//	it's the selection root
			CommitSelection();											//	so transfer the selection to active status
		else
			ClearSelection();											//	otherwise, clear selection (for later recomputation)
		if (superarcs[whichArc].CheckFlag(Superarc::isActive))					//	if it is active
			{ // active arc
			RemoveFromActive(whichArc);									//	make it inactive
			} // active arc
		if (supernodes[superarcs[whichArc].topID].IsUpperLeaf())
			{ // upper leaf
			RemoveArc(whichArc);										//	and remove it from the tree
			RemoveFromValidNodes(superarcs[whichArc].topID);					//	remove the bottom node as well
			} // upper leaf
		else
			{ // lower leaf
			RemoveArc(whichArc);										//	and remove it from the tree
			RemoveFromValidNodes(superarcs[whichArc].bottomID);				//	remove the bottom node as well
			} // lower leaf						
		} // leaf-prune
	else
		CollapseToSuperarc(whichArc);										//	collapse known top & bottom arcs to it
	if (selectionRoot != NO_SUPERARC)										//	if there is a selection
		UpdateSelection(currentSelectionValue);								//	reset it
	logTreeSize = log((float)nValidArcs) / log((float)nNonEpsilonArcs);			//	reset the tree size
	logPriorityBound = log((float)collapseBounds[nValidArcs]) / log((float)nVertices);//	and the priority bound
	} // SingleCollapse()
	
void HeightField::SingleUnCollapse()										//	similarly, uncollapses a single node
	{ // SingleUnCollapse()
	if (nValidArcs >= nNonEpsilonArcs) return;
	ClearRestorable();													//	simplify life by having no "restorables"
	ClearSelection();													//	clear selection array and flags
	long whichArc = collapseRecord[nValidArcs];								//	find out which arc we will be dealing with
	if (superarcs[whichArc].CheckFlag(Superarc::isValid))						//	if the last to collapse is currently valid
		{ // regular point
		//	find out which arcs / nodes are involved
		long topArc = superarcs[whichArc].topArc, bottomArc = superarcs[whichArc].bottomArc;
		long topID = superarcs[whichArc].topID, middleID = superarcs[topArc].bottomID, bottomID = superarcs[whichArc].bottomID;
		//	find out whether the arc is active, and if so, remove it
		long wasActive = superarcs[whichArc].CheckFlag(Superarc::isActive);
		if (wasActive) RemoveFromActive(whichArc);
		//	now remove the arc, and re-insert the children
 		RemoveArc(whichArc);
		AddToValidNodes(middleID);
		AddDownArc(topID, topArc); AddUpArc(middleID, topArc);
		AddDownArc(middleID, bottomArc); AddUpArc(bottomID, bottomArc);
		AddToValid(topArc);
		AddToValid(bottomArc);

		//	set the colours of the children and the middle node
		superarcs[topArc].colour = superarcs[bottomArc].colour = supernodes[middleID].colour = superarcs[whichArc].colour;
		
		//	if the arc was active, activate the appropriate child
		if (wasActive)
			if (superarcs[whichArc].seedValue > *(supernodes[middleID].value))
				AddToActive(topArc, superarcs[whichArc].seedValue);
			else
				AddToActive(bottomArc, superarcs[whichArc].seedValue);
		//	if the arc was the selection root, update it
		if (selectionRoot == whichArc)
			if (*(supernodes[middleID].value) < selectionRootValue)
				selectionRoot = topArc;
			else
				selectionRoot = bottomArc;
		if (nValidArcs >= nDotLayout)
		supernodes[middleID].xPosn = supernodes[bottomID].xPosn + (supernodes[topID].xPosn - supernodes[bottomID].xPosn) * 
								(*(supernodes[middleID].value) - *(supernodes[bottomID].value)) /
								(*(supernodes[topID].value) - *(supernodes[bottomID].value));
		} // regular point
	else
		{ // leaf-prune
		long topID = superarcs[whichArc].topID, bottomID = superarcs[whichArc].bottomID;
		//	set colour
		superarcs[whichArc].colour = whichArc % N_SURFACE_COLOURS;
		//	add to the list of valid arcs
		AddToValid(whichArc);
		//	and reconnect
		AddDownArc(topID, whichArc);
		AddUpArc(bottomID, whichArc);
		if (supernodes[topID].IsUpperLeaf())
			{ // upper leaf
			AddToValidNodes(topID);
			if (superarcs[whichArc].colour == supernodes[bottomID].colour)						//	colour collision
				superarcs[whichArc].colour = (superarcs[whichArc].colour + 1) % N_SURFACE_COLOURS;	//	cycle to next one
               supernodes[topID].colour = superarcs[whichArc].colour; 
			} // upper leaf
		else
			{ // lower leaf
			AddToValidNodes(bottomID);
			if (superarcs[whichArc].colour == supernodes[topID].colour)							//	colour collision
				superarcs[whichArc].colour = (superarcs[whichArc].colour + N_SURFACE_COLOURS - 1) % N_SURFACE_COLOURS;	//	cycle to previous one
               supernodes[bottomID].colour = superarcs[whichArc].colour; 
			} // lower leaf
		} // leaf-prune
	if (selectionRoot != NO_SUPERARC)										//	if there is a selection
		UpdateSelection(currentSelectionValue);								//	reset it
	logTreeSize = log((float)nValidArcs) / log((float)nNonEpsilonArcs);			//	reset the tree size
	logPriorityBound = log((float)collapseBounds[nValidArcs]) / log((float)nVertices);//	and the priority bound
	} // SingleUnCollapse()

void HeightField::UpdateFromLogPriorityBound()								//	updates the size of the tree
	{ // UpdateFromLogPriorityBound()
	long targetBound = FindLogBound(logPriorityBound);						//	convert log to a desired bound
	while (targetBound < collapseBounds[nValidArcs]) SingleUnCollapse();			//	uncollapse until we reach it
	while (targetBound > collapseBounds[nValidArcs-1]) SingleCollapse();			//	or collapse until we reach it
	printf("Updated to size %d\n", nValidArcs);
	} // UpdateFromLogPriorityBound()
	
void HeightField::UpdateFromLogTreeSize()									//	ditto, but from tree size
	{ // UpdateFromLogTreeSize()
	UpdateToFixedSize(FindLogTreeSize(logTreeSize));							//	convert the log to a desired tree size
	} // UpdateFromLogTreeSize()

void HeightField::UpdateToFixedSize(long targetSize)									//	ditto, but from tree size
	{ // UpdateFromTreeSize()
	while (targetSize > nValidArcs) SingleUnCollapse();						//	repeatedly increase until we reach it
	while (targetSize < nValidArcs) SingleCollapse();							//	likewise decrease until we reach it
	printf("Updated to size %d\n", nValidArcs);
	} // UpdateFromTreeSize()

long HeightField::FindLogBound(float logBound)								//	given a fixed tree size, finds the corresponding bound
	{ // FindLogBound()
	long targetBound = (long) pow(nVertices, logBound);						//	since we normalized to [0,1], this computes it
	if (targetBound < 1) targetBound = 1;									//	be paranoid and check
	else if (targetBound > collapseBounds[1]) targetBound = collapseBounds[1];		//	for overflows in each direction
	return targetBound;													//	return the desired size
	} // FindLogBound()
	
long HeightField::FindLogTreeSize(float logSize)								//	and vice versa
	{ // FindLogTreeSize()
	long targetSize = (long) pow(nNonEpsilonArcs, logSize);					//	since we normalized to [0,1], this computes it
	if (targetSize < 1) targetSize = 1;									//	be paranoid and check
	else if (targetSize > nNonEpsilonArcs) targetSize = nNonEpsilonArcs;			//	for overflows in each direction
	return targetSize;													//	return the desired size
	} // FindLogTreeSize()
	
float HeightField::InitialLogPriorityBound()									//	utility routine for initializing slider
	{ // InitialLogPriorityBound()
	return log((float)collapseBounds[1]) / log((float) nVertices);				//	compute it and return
	} // InitialLogPriorityBound()	

void HeightField::ResetCollapsePriority(long whichCollapsePriority)			//	resets the collapse priority & recollapses
	{ // ResetCollapsePriority()
	long savedNActive = nActiveArcs;
	long *savedActive = new long[nActiveArcs];
	float *savedSeeds = new float [nActiveArcs];
	long targetSize = nValidArcs;											//	save the old size
	CommitSelection();														//	commit selection to active
	UpdateToFixedSize(nNonEpsilonArcs);										//	do a complete uncollapse
	for (long arc = 0; arc < nActiveArcs; arc++)
		{ // loop through active arcs
//		long whichArc = active[arc];
//		printf("Saving %3d.\n", active[arc]);
		savedActive[arc] = active[arc];
		savedSeeds[arc] = superarcs[active[arc]].seedValue;
//		printf("Active:  "); for (int a = 0; a < nActiveArcs; a++) printf("%3d ", active[a]); printf("\n");
//		printf("Indices: "); for (int a = 0; a < nActiveArcs; a++) printf("%3d ", superarcs[active[a]].index); printf("\n");
//		printf("Removing %d\n", active[arc]);
//		RemoveFromActive(whichArc);
//		AddToRestorable(whichArc);
//		printf("116: %d\n", superarcs[116].index);
//		printf("116: %d\n", superarcs[116].index);
		} // loop through active arcs
//	printf("Restorable:  "); for (int a = 0; a < nRestorableArcs; a++) printf("%3d ", restorable[a]); printf("\n");
	collapsePriority = whichCollapsePriority;								//	reset the collapse priority
	nextSuperarc = savedNextSuperarc;										//	revert saved starting point
//	printf("Next superarc: %3d\n", nextSuperarc);
	CollapseLeafPruning(1, nVertices);										//	perform the collapse again
//	for (long arc = 0; arc < savedNActive; arc++)
//		{ // loop through active arcs
//		AddToActive(savedActive[arc], savedSeeds[arc]);
//		} // loop through active arcs
//	printf("Restorable:  "); for (int a = 0; a < nRestorableArcs; a++) printf("%3d ", restorable[a]); printf("\n");
//	for (int arc = 0; arc < nRestorableArcs;)
//		{ // loop through active arcs
//		long whichArc = restorable[arc];
//		printf("Transferring %3d from restorable.\n", whichArc);
//		RemoveFromRestorable(whichArc);
//		AddToActive(whichArc, superarcs[whichArc].seedValue);
//		} // loop through active arcs
	SetInitialColours();
	UpdateToFixedSize(targetSize);											//	and return to old size
	delete[] savedActive;
	delete[] savedSeeds;
	} // ResetCollapsePriority()
