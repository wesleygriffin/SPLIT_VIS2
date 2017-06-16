//	IsoSurface II
//	Hamish Carr, 2004

//	HeightField:					class representing the height field
//	HeightField_Layout:	routines dealing with layout of contour tree
//	Palatino-12; 10 m-spaces/tab

#include "HeightField.h"																													//	include the header for the definitions
#include<stdio.h>
#include<iostream>

void HeightField::DoDotLayout()																									//	perform dot-based layout at user request
	{ // DoDotLayout()
	if (nValidArcs < nDotLayout)																										//	if we are allowed to layout this many
		UseDotToSetPositions(nDotLayout);																					//	call routine to set x-positions with dot
	} // DoDotLayout()
	
void HeightField::SaveTreeLayout()																								//	save the contour tree's current layout
	{ // SaveTreeLayout()
	FILE *outFile = NULL;
	char fileName[256];

  CommitSelection();                                                            //  make life easy: no selections allowed
  
	cout << "File to which to save tree layout?" << endl;																//	ask user for file name
	cin >> fileName;
	
	if (strcmp(fileName, "stdout") == 0)
		outFile = stdout;
	else	
		outFile = fopen(fileName, "w");																								//	open file with error check
	if (outFile == NULL)
		{ // bad file name
		printf("Unable to open file %s\n", fileName);
		return;
		} // bad file name

	fprintf(outFile, "Nodes in this file: %d\n", nValidNodes);														//	print # of nodes in file
	for (int i = 0; i < nValidNodes; i++)
		{ // for each valid node
		fprintf(outFile, "Node %d: X-Position %8.5f Colour: %d\n", validNodes[i],
      supernodes[validNodes[i]].xPosn, supernodes[validNodes[i]].colour);		//	print each node's ID & x-position
		} // for each valid node
  fprintf(outFile, "Arcs in this file: %d\n", nValidArcs);                            //  print # of arcs in file
  for (int i = 0; i < nValidArcs; i++)
    { // for each valid Arc
    fprintf(outFile, "Arc %d (%d,%d) Colour: %d %s Contour Value: %8.5f\n", valid[i],
                      superarcs[valid[i]].topID, superarcs[valid[i]].bottomID,
                      superarcs[valid[i]].colour,
                      superarcs[valid[i]].CheckFlag(Superarc::isActive) ? "Active" : "Inactive",
                      superarcs[valid[i]].seedValue);
    } // for each valid Arc
	fclose(outFile);
	} // SaveTreeLayout()

void HeightField::LoadTreeLayout()																							//	load the contour tree's current layout
	{ // LoadTreeLayout()
	FILE *inFile = NULL;
	char fileName[256];
	int nNodesToLoad = 0, nArcsToLoad = 0;
	
	cout << "File from which to load tree layout?" << endl;															//	ask user for file name
	cin >> fileName;

	inFile = fopen(fileName, "r");																										//	open file with error check
	if (inFile == NULL)
		{ // bad file name
		printf("Unable to open file %s\n", fileName);
		return;
		} // bad file name

  ClearAllArrays();
  
	fscanf(inFile, "Nodes in this file: %d\n", &nNodesToLoad);													//	read # of nodes in file
	printf("NodesToLoad: %d\n", nNodesToLoad);
  UpdateToFixedSize(nNodesToLoad - 1);
  for (int i = 0; i < nNodesToLoad; i++)
		{ // for each valid node
		int nodeNum, nodeColour;
		float xPosition;
		fscanf(inFile, "Node %d: X-Position %f Colour: %d\n", &nodeNum, &xPosition, &nodeColour);							//	read each node's ID & x-position
		if ((nodeNum < 0) || (nodeNum >= nSupernodes))															//	bad node number
			{ // bad node number
			printf("Bad node number %d\n", nodeNum);
			continue;
			} // bad node number
		printf("nodeNum: %d. X Position: %8.5f Colour: %d\n", nodeNum, xPosition, nodeColour);
    supernodes[nodeNum].colour = nodeColour;
		supernodes[nodeNum].xPosn = xPosition;																			//	set the x position of the node as requested
		} // for each valid node
	fscanf(inFile, "Arcs in this file: %d\n", &nArcsToLoad);													//	read # of nodes in file
  printf("ArcsToLoad: %d\n", nArcsToLoad);
	for (int i = 0; i < nArcsToLoad; i++)
		{ // for each valid node
		int arcNum, arcColour;
    int topID, bottomID;
    char arcActive[64];
		float arcSeedValue;
		fscanf(inFile, "Arc %d (%d,%d) Colour: %d %s Contour Value: %f\n",
        &arcNum, &topID, &bottomID, &arcColour, arcActive, &arcSeedValue);
				//	read each node's ID & x-position
		if ((arcNum < 0) || (arcNum >= nextSuperarc))															//	bad arc number
			{ // bad arc number
			printf("Bad arc number %d\n", arcNum);
			continue;
			} // bad arc number
		printf("Arc %d Colour: %d [%s] Contour Value: %8.5f\n", arcNum, arcColour, arcActive, arcSeedValue);
    superarcs[arcNum].colour = arcColour;
    superarcs[arcNum].seedValue = arcSeedValue;
    if (strcmp(arcActive, "Active") == 0)
      { // arc is active
      printf("Activating arc %d at %8.5f\n", arcNum, arcSeedValue);
      SelectComponent(arcNum, arcSeedValue);
      } // arc is active
   CommitSelection();
   } // for each valid node
  fclose(inFile);
	} // LoadTreeLayout()

void HeightField::SetNodeXPositions()																						//	sets the x-positions for the nodes
	{ // SetNodeXPositions()
	long X, Y, Z;																																	//	coordinates of vertex	

	for (int node = 0; node < nSupernodes; node++) 
		{ // loop through nodes
		float *valuePtr = supernodes[node].value;																		//	pointer to the value at the node
		height.ComputeIndex(valuePtr, X, Y, Z);																			//	find the indices for the high end
		supernodes[node].yPosn = *valuePtr;																					//	save the y position for convenience
		supernodes[node].xPosn = (float) (X + Y + Z) / (xDim + yDim + zDim);						//	this gives us a first approximation in range 0.0 - 1.0
		} // loop through nodes
	} //SetNodeXPositions()

void HeightField::UseDotToSetPositions(int nArcs)																	//	sets the x-positions for nodes by calling dot
	{ // UseDotToSetPositions()
	printf("Calling ContourTreeToDotFile() with %d nodes.\n", nArcs);
	ContourTreeToDotFile("layout", nArcs, false, true);
//	ContourTreeToDotFile("layout", 100, true, true);
//	printf("Exiting ContourTreeToDotFile().\n");
	char fileName[256];																														//	now re-open the output file from dot
	sprintf(fileName, "dot/layout_%04d.txt", nArcs);
	FILE *dotFile = fopen(fileName, "r");
	if (dotFile == NULL)																													//	if the file isn't there
		return;																																		//	default to cruddy x positions
	float dotScale, dotWidth, dotHeight;																						//	dot width & height
	char lineType[64], nodeID[64], nodeLabel[64];																		//	type of line, name and label of a node
	float nodeX, nodeY, nodeXSize, nodeYSize;																			//	x, y position and size of a node
	fscanf(dotFile, "graph %f %f %f\n", &dotScale, &dotWidth, &dotHeight);						//	retrieve the scales from the first line
// 	printf("Got scale %f, width %f, height %f\n", dotScale, dotWidth, dotHeight);
	while (true)
		{ // loop through nodes
 		fscanf(dotFile, "%s", &lineType);																							//	grab the first word (node or edge)
// 		printf("Scanned [%s]\n", lineType);
// 		getc(stdin);
		if (strcmp(lineType, "edge") == 0) break;																				//	if edge, we're done with nodes
		int nScan = fscanf(dotFile, "%s %f %f %f %f %s solid ellipse black lightgrey\n", 
			&nodeID, &nodeX, &nodeY, &nodeXSize, &nodeYSize, &nodeLabel);						//	until we hit a line that is not nodes
// 		printf("%d: Got node %s (%f, %f) / (%f, %f), %s\n", nScan, nodeID, nodeX, nodeY, nodeXSize, nodeYSize, nodeLabel);
		if (nodeID[0] != 'v') continue;																									//	skip the nodes that force the rank
		int nodeNum = atol(nodeID+1);																							//	convert the rest to a node number
		if ((nodeNum < 0) || (nodeNum > nSupernodes))															//	out of bounds number
			{ printf("Dot file included a vertex %d whose ID is invalid\n", nodeNum); continue;}
		if (supernodes[nodeNum].valid_index == -1)
			{ printf("Dot file included a position for a currently invalid supernode %d\n", nodeNum); continue; }
		supernodes[nodeNum].xPosn = nodeX / dotWidth;															//	compute the position in the range 0..1
		} // loop through nodes
	fclose(dotFile);																																//	close the file
	} // UseDotToSetPositions()
	

