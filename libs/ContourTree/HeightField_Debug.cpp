//	IsoSurface II
//	Hamish Carr, 1999

//	HeightField.h:		class representing the height field
//	Palatino-12; 5 n-spaces/tab

#include "HeightField.h"
#ifdef HEIGHTFIELD_DEBUG_PRINT										//	check to see if this stuff should be compiled

//	set up flags to enable / disable each routine
#define ENABLE_PRINT_FIELD 1
#define ENABLE_PRINT_PARAMETERS 1
#define ENABLE_PRINT_SORT_ORDER 1
#define ENABLE_PRINT_VISIT_FLAGS 1
#define ENABLE_PRINT_JOIN_COMPONENTS 1
#define ENABLE_PRINT_JOIN_TREE 1
#define ENABLE_PRINT_SPLIT_COMPONENTS 1
#define ENABLE_PRINT_SPLIT_TREE 1
#define ENABLE_PRINT_CONTOUR_COMPONENTS 1
#define ENABLE_PRINT_CONTOUR_TREE 1
#define ENABLE_PRINT_NEIGHBOUR_QUEUE 1
#define ENABLE_PRINT_LEAF_QUEUE 1
#define USE_SHORT_COMPONENTS 1

#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

void HeightField::PrintPointerIndices(float *Vertex)								//	prints out the pointer indices
	{ // PrintPointerIndices()
	long x, y, z;
	if (Vertex == NULL) printf("NULL");
	else if (Vertex == &minus_infinity) printf("minus infinity");
	else if (Vertex == &plus_infinity) printf("plus infinity");
	else { // should be valid pointer 
		height.ComputeIndex(Vertex, x, y, z);
		printf("(%2ld, %2ld, %2ld): %6.2f", x, y, z, *Vertex);
		} // should be valid pointer
	} // PrintPointerIndices()

void HeightField::PrintValidValues(char *fname)
{
    ofstream outfile(fname);
    outfile<<nValidArcs<<endl;

    for(int i=0;i<nValidArcs;i++)
    {
       int arc = valid[i];
       if(superarcs[arc].seedFromLo!=NULL)
       {
//           printf("values %d %e ", i, *superarcs[arc].seedToLo); 
             outfile<<setprecision(20)<<*superarcs[arc].seedToLo<<" ";
       }
       else
       {
             outfile<<setprecision(20)<<*supernodes[superarcs[arc].bottomID].value<<" ";
//           printf("values %d %e ", i, *supernodes[superarcs[arc].bottomID].value); 
       }
       if(superarcs[arc].seedFromHi!=NULL)
       {
             outfile<<setprecision(20)<<*superarcs[arc].seedToHi<<" ";
//           printf("%e \n", *superarcs[arc].seedToHi);  
       }
       else
       {
             outfile<<setprecision(20)<<*supernodes[superarcs[arc].topID].value<<" ";
//           printf("%e \n", *supernodes[superarcs[arc].topID].value);   
       }
       outfile<<endl;
    }
    outfile.close();
}

void HeightField::PrintField()												//	prints out the contents of the height field
	{ // PrintField()
#ifdef ENABLE_PRINT_FIELD
	printf("Dimensions: %ld x %ld x %ld = %ld vertices.\n", xDim, yDim, zDim, nVertices);
	printf("Min & Max Height: %f, %f\n", minHeight, maxHeight);
	printf("Height Array is %ld x %ld x %ld, with rowsize %ld, slicesize %ld and nElements %ld\n", height.XDim(), height.YDim(), height.ZDim(), 
			height.RowSize(), height.SliceSize(), height.NElements());
	for (long i = 0; i < height.XDim(); i++)
		{ // i
		for (long j = 0; j < height.YDim(); j++)
			{ // ij
			for (long k = 0; k < height.ZDim(); k++)
				{ // ijk
				printf("%8.5f ", height(i, j, k));			
				} // ijk
			printf("\n");
			} // ij
		printf("\n");
		} // i
//	getc(stdin);
#undef ENABLE_PRINT_FIELD
#endif // ENABLE_PRINT_FIELD
	} // PrintField()
	
void HeightField::PrintParameters()								//	prints out the dimensions, offset, &c.
	{ // PrintParameters()
#ifdef ENABLE_PRINT_PARAMETERS
	printf("Recursion Depth: %ld\n", recursionDepth);
	printf("Max. Recursion Depth: %ld\n", maxRecursionDepth);
	printf("nCellsVisited: %ld\n", nCellsVisited);
	printf("nTrianglesGenerated: %ld\n", nTrianglesGenerated);
	printf("nCellsIntersected: %ld\n", nCellsIntersected);
	printf("nSimplicesToDraw: %ld\n", nSimplicesToDraw);
//	getc(stdin);
#undef ENABLE_PRINT_PARAMETERS
#endif // ENABLE_PRINT_PARAMETERS
	} // PrintParameters()

void HeightField::PrintSortOrder()											//	prints out the heights in sorted order
	{ // PrintSortOrder()
#ifdef ENABLE_PRINT_SORT_ORDER
	for (long i = 0; i < nVertices; i++)
		{ // i
		long x, y, z;
		height.ComputeIndex(heightSort[i], x, y, z);
		printf("(%2ld, %2ld, %2ld): %8.5f\n", x, y, z, *(heightSort[i]));
		} // i
//	getc(stdin);
#undef ENABLE_PRINT_SORT_ORDER
#endif // ENABLE_PRINT_SORT_ORDER
	} // PrintSortOrder()

void HeightField::PrintVisitFlags()											//	prints out the "visited" flags
	{ // PrintVisitFlags()
#ifdef ENABLE_PRINT_VISIT_FLAGS
	for (long i = 0; i < xDim; i++)
		{ // i
		for (long j = 0; j < yDim; j++)
			{ // ij
			for (long k = 0; k < zDim; k++)
				{ // ijk
				unsigned long flags = visitFlags(i, j, k);
				printf("%ld%ld%ld%ld%ld ", flags & 0x10, flags & 0x08, flags & 0x04, flags & 0x02, flags & 0x01);
				} // ijk
			printf("\n");
			} // ij
		printf("\n");
		} // i
	getc(stdin);
#undef ENABLE_PRINT_VISIT_FLAGS
#endif // ENABLE_PRINT_VISIT_FLAGS
	} // PrintVisitFlags()

void HeightField::PrintJoinComponents()										//	prints out the Union-Find info for the join sweep
	{ // PrintJoinComponents()
#ifdef ENABLE_PRINT_JOIN_COMPONENTS
	for (long i = 0; i < xDim; i++)
		{ // i
		for (long j = 0; j < yDim; j++)
			{ // ij
			for (long k = 0; k < zDim; k++)
				{ // ijk
				printf("(%2ld, %2ld, %2ld): %8.5f: pointer: %8X\n", i, j, k, height(i, j, k), joinComponent(i, j, k));
#ifndef USE_SHORT_COMPONENTS
				if (joinComponent(i, j, k) == NULL) continue;
				printf("hiEnd: "); PrintPointerIndices(joinComponent(i, j, k)->hiEnd); printf(", ");
				printf("loEnd: "); PrintPointerIndices(joinComponent(i, j, k)->loEnd); printf(", ");
				printf("seed: "); PrintPointerIndices(joinComponent(i, j, k)->seed); printf("\n");
				printf("nextHi: %8X, lastHi: %8X, nextLo: %8X, lastLo: %8X\n",  
						joinComponent(i, j, k)->nextHi, joinComponent(i, j, k)->lastHi, 
						joinComponent(i, j, k)->nextLo, joinComponent(i, j, k)->lastLo);  
#endif
				} // ijk
			printf("\n");
			} // ij
		printf("\n");
		} // i
//	getc(stdin);
#undef ENABLE_PRINT_JOIN_COMPONENTS
#endif // ENABLE_PRINT_JOIN_COMPONENTS
	} // PrintJoinComponents()

void HeightField::PrintJoinTree()											//	prints out the join tree (i.e. join components, with duplicates suppressed)
	{ // PrintJoinTree()
#ifdef ENABLE_PRINT_JOIN_TREE
	for (long i = 0; i < xDim; i++)
		{ // i
		for (long j = 0; j < yDim; j++)
			{ // ij
			for (long k = 0; k < zDim; k++)
				{ // ijk
				if (joinComponent(i, j, k) == NULL) continue;
				if (joinComponent(i, j, k)->hiEnd != &height(i, j, k)) continue;
				printf("hiEnd: "); PrintPointerIndices(joinComponent(i, j, k)->hiEnd); printf(", ");
				printf("loEnd: "); PrintPointerIndices(joinComponent(i, j, k)->loEnd); printf(", ");
//				printf("seed: "); PrintPointerIndices(joinComponent(i, j, k)->seed); printf("\n");
//				printf("pointer: %8X, nextHi: %8X, lastHi: %8X, nextLo: %8X, lastLo: %8X\n",  joinComponent(i, j, k), 
//						joinComponent(i, j, k)->nextHi, joinComponent(i, j, k)->lastHi, 
//						joinComponent(i, j, k)->nextLo, joinComponent(i, j, k)->lastLo);  
				} // ijk
			} // ij
		} // i
//	getc(stdin);
#undef ENABLE_PRINT_JOIN_TREE
#endif // ENABLE_PRINT_JOIN_TREE
	} // PrintJoinTree()

void HeightField::PrintSplitComponents()										//	prints out the Union-Find info for the split sweep
	{ // PrintSplitComponents()
#ifdef ENABLE_PRINT_SPLIT_COMPONENTS
	for (long i = 0; i < xDim; i++)
		{ // i
		for (long j = 0; j < yDim; j++)
			{ // ij
			for (long k = 0; k < zDim; k++)
				{ // ijk
				printf("(%2ld, %2ld, %2ld): %8.5f: pointer: %8X\n", i, j, k, height(i, j, k), splitComponent(i, j, k));
#ifndef USE_SHORT_COMPONENTS
				if (splitComponent(i, j, k) == NULL) continue;
				printf("hiEnd: "); PrintPointerIndices(splitComponent(i, j, k)->hiEnd); printf(", ");
				printf("loEnd: "); PrintPointerIndices(splitComponent(i, j, k)->loEnd); printf(", ");
				printf("seed: "); PrintPointerIndices(splitComponent(i, j, k)->seed); printf("\n");
//				printf("nextHi: %8X, lastHi: %8X, nextLo: %8X, lastLo: %8X\n",  
//						splitComponent(i, j, k)->nextHi, splitComponent(i, j, k)->lastHi, 
//						splitComponent(i, j, k)->nextLo, splitComponent(i, j, k)->lastLo);  
#endif
				} // ijk
			printf("\n");
			} // ij
		printf("\n");
		} // i
//	getc(stdin);
#undef ENABLE_PRINT_SPLIT_COMPONENTS
#endif // ENABLE_PRINT_SPLIT_COMPONENTS
	} // PrintSplitComponents()

void HeightField::PrintSplitTree()											//	prints out the split tree (i.e. split components, with duplicates suppressed)
	{ // PrintSplitTree()
#ifdef ENABLE_PRINT_SPLIT_TREE
	for (long i = 0; i < xDim; i++)
		{ // i
		for (long j = 0; j < yDim; j++)
			{ // ij
			for (long k = 0; k < zDim; k++)
				{ // ijk
				if (splitComponent(i, j, k) == NULL) continue;
				if (splitComponent(i, j, k)->loEnd != &height(i, j, k)) continue;
				printf("hiEnd: "); PrintPointerIndices(splitComponent(i, j, k)->hiEnd); printf(", ");
				printf("loEnd: "); PrintPointerIndices(splitComponent(i, j, k)->loEnd); printf(", ");
//				printf("seed: "); PrintPointerIndices(splitComponent(i, j, k)->seed); printf("\n");
				printf("pointer: %8X, nextHi: %8X, lastHi: %8X, nextLo: %8X, lastLo: %8X\n",  splitComponent(i, j, k),
						splitComponent(i, j, k)->nextHi, splitComponent(i, j, k)->lastHi, 
						splitComponent(i, j, k)->nextLo, splitComponent(i, j, k)->lastLo);  
				} // ijk
			} // ij
		} // i
//	getc(stdin);
#undef ENABLE_PRINT_SPLIT_TREE
#endif // ENABLE_PRINT_SPLIT_TREE
	} // PrintSplitTree()

void HeightField::PrintContourTree()											//	prints out the contour tree, once it is fully computed
	{ // PrintContourTree()

#ifdef ENABLE_PRINT_CONTOUR_TREE
	printf("nSuperNodes: %ld, nSuperArcs: %ld, nextSuperarc: %ld\n", nSupernodes, nSuperarcs, nextSuperarc);
	printf("Supernodes:\n");
//        printf("ActiveAc %d\n", nActiveArcs);
/*	for (long i = 0; i < nSupernodes; i++)
		{
		printf("Vertex %2d: ", i);
		PrintPointerIndices(supernodes[i].value);
		printf(", Value: %e\n", *(supernodes[i].value));//7.2f
		printf("Up: %2d, Down: %2d, Updegree: %2d, Downdegree: %2d, Position: (%5.2f,%5.2f)\n", 
			supernodes[i].upList, supernodes[i].downList, supernodes[i].upDegree, supernodes[i].downDegree,
			supernodes[i].xPosn, supernodes[i].yPosn);
		}
	printf("Superarcs:\n");
	for (long i = 0; i < nSuperarcs; i++)
		{ // i
		if (! superarcs[i].CheckFlag(Superarc::isValid)) 
			printf("%3d: I ", i);
		else
			printf("%3d:   ", i);
		printf("bottom: %3d, ", superarcs[i].bottomID);
		printf("top   : %3d, ", superarcs[i].topID);
//		printf("%ld:\n", i);
//		printf("seedHi: "); PrintPointerIndices(contourTree[i].seedHi); printf(", ");
//		printf("seedLo: "); PrintPointerIndices(contourTree[i].seedLo); printf("\n");
//		printf("pointer: %8X, nextHi: %8X, lastHi: %8X, nextLo: %8X, lastLo: %8X\n",  &(contourTree[i]),  
//		printf("ptrs: %3X, %3X, %3X, %3X, %3X\n",  (long) &(contourTree[i]) & 0xFFF,  
//			(long) contourTree[i].nextHi & 0xFFF, (long) contourTree[i].lastHi & 0xFFF, 
//			(long) contourTree[i].nextLo & 0xFFF, (long) contourTree[i].lastLo & 0xFFF);  
		printf("ptrs: up: %3d, %3d, down: %3d, %3d\n", 
			superarcs[i].nextUp, superarcs[i].lastUp,
			superarcs[i].nextDown, superarcs[i].lastDown);
		printf("ssTop: %8.5f, ssOn: %8.5f, ssBot: %8.5f\n", superarcs[i].sampleSumTop, superarcs[i].sampleSumOnArc, superarcs[i].sampleSumBottom);
		} // i
*/
//	getc(stdin);
#undef ENABLE_PRINT_CONTOUR_TREE
#endif // ENABLE_PRINT_CONTOUR_TREE

	} // PrintContourTree()

void HeightField::PrintNeighbourQueue()										//	prints off the queue that identifies the neighbours of a vertex
	{ // PrintNeighbourQueue()
#ifdef ENABLE_PRINT_NEIGHBOUR_QUEUE
	printf("nNeighbours: %ld\n", nNeighbours);
	for (long i = 0; i < nNeighbours; i++)
		printf("(%2ld, %2ld, %2ld): %8.5f\n", neighbourQueue[i][0], neighbourQueue[i][1], neighbourQueue[i][2], height(neighbourQueue[i][0], neighbourQueue[i][1],
				neighbourQueue[i][2]));
//	getc(stdin);
#undef ENABLE_PRINT_NEIGHBOUR_QUEUE
#endif // ENABLE_PRINT_NEIGHBOUR_QUEUE
	} // PrintNeighbourQueue()

void HeightField::PrintLeafQueue()											//	prints off the leaf queue
	{ // PrintLeafQueue()
#ifdef ENABLE_PRINT_LEAF_QUEUE
	printf("leafQSize: %ld\n", leafQSize);
	for (long i = 0; i < leafQSize; i++)
		{ // i
		long x, y, z;
		height.ComputeIndex(leafQueue[i], x, y, z);
		printf("(%2ld, %2ld, %2ld): %8.5f\n", x, y, z, height(x, y, z));
		} // i
//	getc(stdin);
#undef ENABLE_PRINT_LEAF_QUEUE
#endif // ENABLE_PRINT_LEAF_QUEUE
	} // PrintLeafQueue()

void HeightField::CheckJoinTree() {}											//	checks join tree for consistency (empty for now)
void HeightField::CheckSplitTree() {}											//	checks split tree for consistency (empty for now)
void HeightField::CheckContourTree() {}											//	checks contour tree for consistency (empty for now)

void HeightField::CheckVisitFlags()											//	checks the "visited" flags
	{ // CheckVisitFlags()
	for (long i = 0; i < xDim-1; i++)
		{ // i
		for (long j = 0; j < yDim-1; j++)
			{ // ij
			for (long k = 0; k < zDim-1; k++)
				{ // ijk
				unsigned long flags = visitFlags(i, j, k);
				if (flags != 0) 
					{
					printf("Bad flags at (%ld, %ld, %ld): %X\n", i, j, k, flags);
					visitFlags(i, j, k) = 0;
					}
				} // ijk
			} // ij
		} // i
	} // CheckVisitFlags()

//	this routine generates a dot file for generating graph layouts
//	filename is: basefilename_serialNum.dot
void HeightField::ContourTreeToDotFile(char *basefilename, int serialNum, bool postscriptOutput, bool forceYPosition, long highlightEdge)
																//	prints out the contour tree, once it is fully computed
	{ // ContourTreeToDotFile()
	char filenamePrefix[1024];
// 	printf("Entering ContourTreeToDotFile with parameters %s, %d, %s, %s, %d.\n", basefilename, serialNum, 
// 				postscriptOutput ? "Postscript" : "Dot", forceYPosition ? "Ranked" : "Unranked", highlightEdge);
	sprintf(filenamePrefix, "dot/%s_%04d", basefilename, serialNum);
	char filename[1024];
	sprintf(filename, "%s.dot", filenamePrefix);
	FILE *dotFile = fopen(filename, "w");
	if (dotFile == NULL) { printf("Unable to write to dotfile %s\n", filename); return; }

	fprintf(dotFile, "digraph G {\n");
	fprintf(dotFile, "\tsize=\"6.5, 9\"\n\tratio=\"fill\"\n");
	//	put a list down the side of isovalues
	if (forceYPosition)
		{ // ranked positions
		for (int i = 255; i > 0; i--)
			fprintf(dotFile, "\t%d -> %d\n", i, i-1);
		for (int arc = 0; arc < nValidArcs; arc++)
			{
			int i = valid[arc];
			fprintf(dotFile, "\tv%1d -> v%1d [label=\"e%1d\"]\n", superarcs[i].topID, superarcs[i].bottomID, i);
			}
 		for (int i = 255; i >= 0; i--)
 			{
 			fprintf(dotFile, "\t{ rank = same; %d; ", i);
 			for (int valid = 0; valid < nValidNodes; valid++)
 				{
 				int node = validNodes[valid];
				int nodeVal = (int) (255 * (*(supernodes[node].value) - MinHeight()) / (MaxHeight() - MinHeight())); 
 				if (nodeVal == i)
 					fprintf(dotFile, "v%1d; ", node);
 				}			
 			fprintf(dotFile, "}\n");
 			}
//		for (int valid = 0; valid < nValidNodes; valid++)
//			{
//			int node = validNodes[valid];
//			int nodeVal = (int) (255 * (*(supernodes[node].value) - MinHeight()) / (MaxHeight() - MinHeight())); 
//			printf("Node %d. Value %8.5f (%3d)\n", node, *(supernodes[node].value), nodeVal);
//			fprintf(dotFile, "\t{ rank = same; %d; v%1d; }\n", nodeVal, node);
//			}			
		} // ranked positions
	else
		{ // unranked positions
		if (highlightEdge != -1) 
			fprintf(dotFile, "\t{ node [color = red, style = filled];\n\tv%1d;\n\tv%1d;}\n", 
				superarcs[highlightEdge].topID, superarcs[highlightEdge].bottomID);
		for (int arc = 0; arc < nValidArcs; arc++)
			{
			int i = valid[arc];
			long x1, y1, z1, x2, y2, z2;
			height.ComputeIndex(supernodes[superarcs[i].topID].value, x1, y1, z1);
			height.ComputeIndex(supernodes[superarcs[i].bottomID].value, x2, y2, z2);
//			fprintf(dotFile, "\t\"(%1d,%1d,%1d)\" -> \"(%1d,%1d,%1d)\" [label=\"%1d\", taillabel = \"%1d\", headlabel=\"%1d\"];\n", 
//				x1, y1, z1, x2, y2, z2, superarcs[i].nodesOnArc, superarcs[i].nodesThisSideOfTop, superarcs[i].nodesThisSideOfBottom);
	 		fprintf(dotFile, "\tv%1d -> v%1d [label=\"e%1d[%1d]%s\", taillabel = \"%1d\", headlabel=\"%1d\"%s]\n", 
				superarcs[i].topID, superarcs[i].bottomID, i, superarcs[i].nodesOnArc,
				(*(supernodes[superarcs[i].topID].value) == *(supernodes[superarcs[i].bottomID].value)) ? "(E)" : "",
				superarcs[i].nodesThisSideOfTop, superarcs[i].nodesThisSideOfBottom,
				i == highlightEdge ? ", color = red, style = bold" : ""
				);
			}
		} // unranked positions
	fprintf(dotFile, "}\n");
	fclose(dotFile);

	char systemCall[4096];
	if (postscriptOutput)
		sprintf(systemCall, "dot -Tps2 %s.dot -o %s.ps; ps2pdf %s.ps %s.pdf; open %s.pdf", 
			filenamePrefix, filenamePrefix, filenamePrefix, filenamePrefix, filenamePrefix);
	else
		sprintf(systemCall, "dot -Tplain %s.dot -o %s.txt", filenamePrefix, filenamePrefix);	
	system(systemCall);
//	getc(stdin);
	} // ContourTreeToDotFile()
		
void HeightField::JoinArcsToDotFile(char *basefilename, int serialNum)								//		prints out the join arcs
	{ // JoinArcsToDotFile()
	char filenamePrefix[1024];
	sprintf(filenamePrefix, "dot/%s_%04d", basefilename, serialNum);
	char filename[1024];
	sprintf(filename, "%s.dot", filenamePrefix);
	FILE *dotFile = fopen(filename, "w");
	if (dotFile == NULL) { printf("Unable to write to dotfile %s\n", filename); return; }

	fprintf(dotFile, "digraph G {\n");
	fprintf(dotFile, "\tsize=\"6.5, 9\"\n\tratio=\"fill\"\n");
	//	put a list down the side of isovalues


	for (int x = 0; x < xDim; x++)
		for (int y = 0; y < yDim; y++)
			for (int z = 0; z < zDim; z++)
				{ // for each sample
				if (joinArcs(x, y, z) == NULL) continue;
				if (joinArcs(x, y, z) == &minus_infinity) continue;
				long xFar, yFar, zFar;
				height.ComputeIndex(joinArcs(x, y, z), xFar, yFar, zFar);
		 		fprintf(dotFile, "\t\"(%1d,%1d,%1d)\" -> \"(%1d,%1d,%1d)\";\n", x, y, z, xFar, yFar, zFar);
				} // for each sample
	fprintf(dotFile, "}\n");
	fclose(dotFile);

	char systemCall[4096];
	sprintf(systemCall, "~/dot -Tps2 %s.dot -o %s.ps; ps2pdf %s.ps %s.pdf; open %s.pdf", 
		filenamePrefix, filenamePrefix, filenamePrefix, filenamePrefix, filenamePrefix);
	system(systemCall);
//	getc(stdin);
	} // JoinArcsToDotFile()

void HeightField::SplitArcsToDotFile(char *basefilename, int serialNum)								//	prints out the split arcs
	{ // SplitArcsToDotFile()
	char filenamePrefix[1024];
	sprintf(filenamePrefix, "dot/%s_%04d", basefilename, serialNum);
	char filename[1024];
	sprintf(filename, "%s.dot", filenamePrefix);
	FILE *dotFile = fopen(filename, "w");
	if (dotFile == NULL) { printf("Unable to write to dotfile %s\n", filename); return; }

	fprintf(dotFile, "digraph G {\n");
	fprintf(dotFile, "\tsize=\"6.5, 9\"\n\tratio=\"fill\"\n");
	//	put a list down the side of isovalues


	for (int x = 0; x < xDim; x++)
		for (int y = 0; y < yDim; y++)
			for (int z = 0; z < zDim; z++)
				{ // for each sample
				if (splitArcs(x, y, z) == NULL) continue;
				if (splitArcs(x, y, z) == &plus_infinity) continue;
				long xFar, yFar, zFar;
				height.ComputeIndex(splitArcs(x, y, z), xFar, yFar, zFar);
		 		fprintf(dotFile, "\t\"(%1d,%1d,%1d)\" -> \"(%1d,%1d,%1d)\";\n", xFar, yFar, zFar, x, y, z);
				} // for each sample
	fprintf(dotFile, "}\n");
	fclose(dotFile);

	char systemCall[4096];
	sprintf(systemCall, "~/dot -Tps2 %s.dot -o %s.ps; ps2pdf %s.ps %s.pdf; open %s.pdf", 
		filenamePrefix, filenamePrefix, filenamePrefix, filenamePrefix, filenamePrefix);
	system(systemCall);
//	getc(stdin);
	} // SplitArcsToDotFile()

void HeightField::CheckNodeCounts()												//	checks consistency of node counts on arcs
	{ // CheckNodeCounts()
	long nodeCount = 0;
	for (long arc = 0; arc < nValidArcs; arc++)
		nodeCount += superarcs[valid[arc]].nodesOnArc;
	nodeCount += nValidNodes;
	printf("Node count: %7d.  Expected: %7d\n", nodeCount, nVertices);
	} // CheckNodeCounts()

void HeightField::PrintCollapseRecord()												//	prints the array storing collapse details
	{ // PrintCollapseRecord()
	printf("Collapse Record\n");
	for (int i = 0; i < nNonEpsilonArcs; i++)
		{ // i
		printf("%3d ", collapseRecord[i]);
		if (i % 32 == 31) printf("\n");
		else if (i % 4 == 3) printf(" ");	
		} // i
	printf("\n");
	} // PrintCollapseRecord()

#undef HEIGHTFIELD_DEBUG_PRINT
#endif

