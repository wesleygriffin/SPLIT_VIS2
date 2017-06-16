//	IsoSurface II
//	Hamish Carr, 2000

//	HeightField:			class representing the height field
//	HeightField_Collapse:	routines dealing with collapsing the contour tree
//	Palatino-12; 10 m-spaces/tab

#include "HeightField.h"													//	include the header for the definitions
#include <stdlib.h>
#include <queue>

void HeightField::CollapseEpsilonEdges()									//	collapses the epsilon-height edges
	{ // CollapseEpsilonEdges()
	bool *wasChecked = (bool *) calloc(nSupernodes, sizeof(bool));				//	array recording which nodes we've checked (init. to zero)
	int *supernodeQueue = (int *) malloc(nSupernodes * sizeof(int));				//	queue for supernodes
	int qNext, qSize;													//	keep track of logical next item & queue size

	int dotFileNo = 1;
//	PrintContourTree();
//	ContourTreeToDotFile("uncollapsed", 0);
	//	loop through all supernodes: test each to see if it has any zero-height edges
	for (int whichSupernode = 0; whichSupernode < nSupernodes; whichSupernode++)
		{ // loop through supernodes
		double *whichVertex;											//	the spatial vertex corresponding to a supernode
		if (wasChecked[whichSupernode]) continue;							//	skip nodes we've already checked
		wasChecked[whichSupernode] = true;									//	and mark this one as checked
		
//		printf("Checking supernode %d\n", whichSupernode);
		qNext = qSize = 0;												//	reset the queue
		//	now, walk around the node
		Superarc *walkArc = nodeArcLists[whichSupernode];						//	grab the first arc
		if (walkArc == NULL) continue;									//	skip past any node with no edges

		int nArcsAtNode = 0;											//	counter for number of arcs here
		do															//	loop to count number of arcs here
			{ // count loop
			nArcsAtNode++;
//			printf("At superarc %d\n", walkArc->superArcID);
//			if (nArcsAtNode > 5) 
//				printf("%d ", nArcsAtNode);
			if (walkArc->hiID == whichSupernode) walkArc = walkArc->nextHi;		//	step round
			else walkArc = walkArc->nextLo;
			} // count loop
		while (walkArc != nodeArcLists[whichSupernode]);						//	until we return to start
//		if (nArcsAtNode > 5) 
//			printf("\n");
		
		//	now that we have counted the nodes, walk around them
		for (int whichArcAtNode = 0; whichArcAtNode < nArcsAtNode; whichArcAtNode++)
			{ // loop through arcs at node
//			printf("On arc %d of %d\n", whichArcAtNode, nArcsAtNode);
//			printf("Considering superarc %d\n", walkArc->superArcID);
			if (walkArc->hiID == whichSupernode)							//	match at high end
				{ // node at high end
				whichVertex = walkArc->hiEnd;								//	grab a pointer to the underlying vertex
				if (*(walkArc->hiEnd) == *(walkArc->loEnd))					//	if the isovalues match, epsilon height
					{ // found an epsilon-height edge
//					printf("Found epsilon-height edge %d - %d\n", walkArc->hiID, walkArc->loID);
					Superarc *nextWalkArc = walkArc->nextHi;				//	grab the next "high" end
//					printf("Queueing %d\n", walkArc->loID); 
					supernodeQueue[qSize++] = walkArc->loID;				//	add the "low end" to the queue
					walkArc->SetFlag(Superarc::isCollapsed);				//	and mark the edge as collapsed
					RemoveArc(walkArc);									//	and remove the arc from the contour tree
					walkArc = nextWalkArc;								//	and walk to the next one					
//					ContourTreeToDotFile("collapsed", dotFileNo++);
//					PrintContourTree();
					} // found an epsilon-height edge
				else
					walkArc = walkArc->nextHi;							//	step round at the high end
				} // node at high end
			else
				{ // node at low end
				whichVertex = walkArc->loEnd;								//	grab a pointer to the underlying vertex
				if (*(walkArc->hiEnd) == *(walkArc->loEnd))					//	if the isovalues match, epsilon height
					{ // found an epsilon-height edge
//					printf("Found epsilon-height edge %d - %d\n", walkArc->hiID, walkArc->loID);
					Superarc *nextWalkArc = walkArc->nextLo;				//	grab the next "low" end
//					printf("Queueing %d\n", walkArc->hiID); 
					supernodeQueue[qSize++] = walkArc->hiID;				//	add the "high end" to the queue
					walkArc->SetFlag(Superarc::isCollapsed);				//	and mark the edge as collapsed
					RemoveArc(walkArc);									//	and remove the arc from the contour tree
					walkArc = nextWalkArc;								//	and walk to the next one					
//					ContourTreeToDotFile("collapsed", dotFileNo++);
//					PrintContourTree();
					} // found an epsilon-height edge
				else
					walkArc = walkArc->nextLo;							//	step round at the high end
				} // node at low end

			//	now, as long as there is stuff on the queue
			while(qNext < qSize)								
				{ //  loop through nodes on the queue
//				printf("Checking supernode %d (%d on queue)\n", supernodeQueue[qNext], qNext);
				int nextNode = supernodeQueue[qNext++];						//	grab the next node on the queue
				wasChecked[nextNode] = true;								//	mark it as checked				

				Superarc *walkUpdateArc = nodeArcLists[nextNode];				//	walking arc for a loop similar to the above
				if (walkUpdateArc == NULL) 
					{
//					printf("Skipping (NULL list => nothing to transfer)\n");
					continue;											//	skip past any node with no edges
					}
					
				int nArcsAtUpdateNode = 0;								//	counter for number of arcs here
				do													//	loop to count number of arcs here
					{ // count loop
					nArcsAtUpdateNode++;
//					printf("At superarc %d\n", walkUpdateArc->superArcID);
//					if (nArcsAtUpdateNode > 5) 
//						printf("%d ", nArcsAtUpdateNode);
					if (walkUpdateArc->hiID == nextNode) 
						walkUpdateArc = walkUpdateArc->nextHi;				//	step round
					else walkUpdateArc = walkUpdateArc->nextLo;
					} // count loop
				while (walkUpdateArc != nodeArcLists[nextNode]);				//	until we return to start
//				if (nArcsAtUpdateNode > 5) 
//					printf("\n");

				for (int whichArcAtUpdateNode = 0; whichArcAtUpdateNode < nArcsAtUpdateNode; whichArcAtUpdateNode++)
					{ // walk around node
//					printf("On arc %d of %d\n", whichArcAtUpdateNode, nArcsAtUpdateNode);
//					printf("Considering superarc %d\n", walkUpdateArc->superArcID);
					if (walkUpdateArc->hiID == nextNode)					//	if we are at the high end
						if (*(walkUpdateArc->hiEnd) == *(walkUpdateArc->loEnd))//	if the isovalues match, epsilon height
							{ // found an epsilon-height edge
//							printf("Found epsilon-height edge %d - %d\n", walkUpdateArc->hiID, walkUpdateArc->loID);
//							printf("Queueing %d\n", walkUpdateArc->loID); 
							supernodeQueue[qSize++] = walkUpdateArc->loID;	//	add the "low end" to the queue
							Superarc *nextWalkArc = walkUpdateArc->nextHi;	//	grab the next "high" end
							walkUpdateArc->SetFlag(Superarc::isCollapsed);	//	and mark the edge as collapsed
							RemoveArc(walkUpdateArc);					//	and remove the arc from the contour tree
							walkUpdateArc = nextWalkArc;					//	and walk to the next one					
//							PrintContourTree();
//							ContourTreeToDotFile("collapsed", dotFileNo++);
							} // found an epsilon-height edge
						else
							{ // a "real" edge
							walkUpdateArc->hiID = whichSupernode;			//	attach the top end to the collapsed supernode
							walkUpdateArc->hiEnd = whichVertex;			//	and make it point to the corresponding vertex
							walkUpdateArc = walkUpdateArc->nextHi;			//	step round at the high end
							} // a "real" edge
					else
						if (*(walkUpdateArc->hiEnd) == *(walkUpdateArc->loEnd))//	if the isovalues match, epsilon height
							{ // found an epsilon-height edge
//							printf("Found epsilon-height edge %d - %d\n", walkUpdateArc->hiID, walkUpdateArc->loID);
//							printf("Queueing %d\n", walkUpdateArc->hiID); 
							supernodeQueue[qSize++] = walkUpdateArc->hiID;	//	add the "high end" to the queue
							Superarc *nextWalkArc = walkUpdateArc->nextLo;	//	grab the next "low" end
							walkUpdateArc->SetFlag(Superarc::isCollapsed);	//	and mark the edge as collapsed
							RemoveArc(walkUpdateArc);					//	and remove the arc from the contour tree
							walkUpdateArc = nextWalkArc;					//	and walk to the next one					
//							PrintContourTree();
//							ContourTreeToDotFile("collapsed", dotFileNo++);
							} // found an epsilon-height edge
						else
							{ // a "real" edge
							walkUpdateArc->loID = whichSupernode;			//	attach the bottom end to the collapsed supernode
							walkUpdateArc->loEnd = whichVertex;			//	and make it point to the corresponding vertex
							walkUpdateArc = walkUpdateArc->nextLo;			//	step round at the low end
							} // a "real" edge					
					} // walk around node
				TransferArcList(whichSupernode, nextNode);					//	merge nextNode's arcs onto whichSupernode's
//				ContourTreeToDotFile("collapsed", dotFileNo++);
//				PrintContourTree();
				} // loop through nodes on the queue				
			} // while loop around node
		} // loop through supernodes

//	ContourTreeToDotFile("collapsed", dotFileNo++);
	// release temporary memory
	free(wasChecked); free(supernodeQueue);
	} // CollapseEpsilonEdges()
	
void HeightField::RemoveArc(Superarc *whichArc)								//	routine to remove an arc from the tree
	{ // RemoveArc()
	//	first, remove it at the high end
	//	check to see if there are any other edges at this end
	if (whichArc->nextHi == whichArc)										//	if it's the only edge at this node
		nodeArcLists[whichArc->hiID] = NULL;								//	make it point to NULL
	else
		{ // not the only edge
		SetLast(whichArc->hiID, whichArc->nextHi, whichArc->lastHi);			//	reset the "next high" at shared end
		SetNext(whichArc->hiID, whichArc->lastHi, whichArc->nextHi);
		if (nodeArcLists[whichArc->hiID] == whichArc)						//	make sure the entry pointer stays valid
			nodeArcLists[whichArc->hiID] = whichArc->nextHi;
		} // not the only edge
	// now do the same at the low end
	if (whichArc->nextLo == whichArc)										//	if it's the only edge at this node
		nodeArcLists[whichArc->loID] = NULL;								//	make it point to NULL
	else
		{ // not the only edge
		SetLast(whichArc->loID, whichArc->nextLo, whichArc->lastLo);			//	reset the "next high" at shared end
		SetNext(whichArc->loID, whichArc->lastLo, whichArc->nextLo);
		if (nodeArcLists[whichArc->loID] == whichArc)						//	make sure the entry pointer stays valid
			nodeArcLists[whichArc->loID] = whichArc->nextLo;
		} // not the only edge
	} // RemoveArc()

// *************	
// ** WARNING **
// *************	
//
// This routine assumes that the fromNode's arcs have already had the hiID/loID and hiEnd/loEnd correctly reset to the onto node
//
void HeightField::TransferArcList(int ontoNode, int fromNode)					//	transfer's arcs from one node to another
	{ // TransferArcList()
//	printf("Transferring arcs from %3d to %3d\n", fromNode, ontoNode);
	if (nodeArcLists[ontoNode] == NULL) 									//	easy case: first one is empty
		nodeArcLists[ontoNode] = nodeArcLists[fromNode];						//	just shift the pointer
	else if (nodeArcLists[fromNode] != NULL)								//	if the second one is non-empty
		{ // two non-empty lists
		Superarc *fromArc = nodeArcLists[fromNode];
		Superarc *ontoArc = nodeArcLists[ontoNode];
		Superarc *fromLast, *ontoNext;									//	pointers to the two nodes needing fixing
		if (ontoArc->hiID == ontoNode)									//	if we're at the high end
			ontoNext = ontoArc->nextHi;
		else
			ontoNext = ontoArc->nextLo;
		if (fromArc->hiID == ontoNode)									//	if also at the high end here (note comparison to onto hi end)
			fromLast = fromArc->lastHi;
		else
			fromLast = fromArc->lastLo;
		SetLast(ontoNode, ontoNext, fromLast);								//	set the last pointer on the ontoNext
		SetNext(ontoNode, fromLast, ontoNext);								//	and reciprocally
		SetLast(ontoNode, fromArc, ontoArc);								//	ditto on the from and to arcs
		SetNext(ontoNode, ontoArc, fromArc);
		} // two non-empty lists
	nodeArcLists[fromNode] = NULL;										//	make sure the from node now has no arcs
	} // TransferArcList()
	
//	resets the "next" pointer at a particular node
void HeightField::SetNext(int atNode, Superarc *target, Superarc *newNext)
	{ // SetNext()
	if (target->hiID == atNode)
		target->nextHi = newNext;
	else
		target->nextLo = newNext;
	} // SetNext()	

//	resets the "last" pointer at a particular node 
void HeightField::SetLast(int atNode, Superarc *target, Superarc *newLast)
	{ // SetLast()
	if (target->hiID == atNode)
		target->lastHi = newLast;
	else
		target->lastLo = newLast;
	} // SetLast()
