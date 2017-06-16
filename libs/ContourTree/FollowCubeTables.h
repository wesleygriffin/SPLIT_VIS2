// FollowCubeTables.h
// Copyright H. Carr 2003
// Marching Cube cases surface-specific contour-following (continuation)
// Generated 16:35:46, 07/07/03
extern int nSurfaces[256];
extern int nTriangles[256][4];
extern int mcFollowTriangles[256][4][15];
extern int seedEdge2Surface[256][24];
extern int nExitEdges[256][4];
extern int surface2exitEdges[256][4][7];
extern int exit2entryEdge[36];
extern int exitDirection[36][3];
extern int entryEdge2Surface[256][36];
extern int vertex2edge[8][8];
extern int mcFollowVertexCoords[8][3];
extern int edgeVertices[24][2];
extern char *caseName[23];
extern int baseCase[256];
