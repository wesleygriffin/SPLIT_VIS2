/**
 summary disc to show abstract information of each cluster   

Input parameters:
	pos: n*3 array
	vel: n*3 array
	den: n*1 array
	exp: n*1 array
	cluster: n array
	symmetric plane
	# of points
	# of clusters
	# of symmetric planes 	
Mid parameters:
	# of discs
	centric pos
	mean vel
	max/min density
	distribution of density
Others:
	color (diverging color)
Output:
	3D discs
**/

/**
07/03/2016 update the program 
		todo: update the symmetric considerations
**/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <fstream>
#include <vector>
#include <cmath>


#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"
#include "../jcUtil/MGL.h"
#include "../jcUtil/MArray.h"
#include "../jcUtil/MVectorMatrix.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif

#define radius 0.15//0.2
//#define LINE_LENGTH 1
#define MIN_LENGTH 0.1

using namespace std;

namespace __svl_lib{
class svDisc
{
 public:
  svDisc();  
  ~svDisc(){cleanup();}

	/**
	read data
	**/
	void SetData(vector< vector<svVector3> > pos, vector< vector<svVector3> > vel,
			vector< vector<svScalar> > den, vector< vector<svScalar> > exp, vector< vector<svScalar> > coe,
			int size);
	void SetData(svVector3 *pos, svVector3 *vel, svScalar *den, 
			svScalar *exp, svScalar *coe, int size);
	void SetClusters(svInt *cluster, int num, bool layer, int zlayer, double zmin, double zdistance); //num: #ofclusters
	void SetClusters(svInt *cluster, int num); //num: #ofclusters
	void SetClusters(vector< vector<int> > cluster, int num); //num: #ofclusters
	void SetSymmetry(vector<svVector3> v);

	/**
	calculate mid parameters
	**/

	void computeMean(int *cluster, int num);
	void computeMean(vector< vector<int> > cluster2, int num);
	void setColors(svVector4 *color, svVector4 *ccolor, int num, bool layer, int zlayer);//num is the range of magnitudes
	/**
	render
	**/
	void renderAnnulus(svVector3 pos, svVector3 vel,
				svScalar x,
				svScalar H, svScalar r);
	void renderDisc(bool layer, float zvalue, float LINE_LENGTH);
	void renderDisc(float LINE_LENGTH);
	/**
	picking
	**/
  	void Find_Nearest(int x, int y);
    	void Find_Nearest(svVector3 rayPos, svVector3 rayDir, GLfloat *tm, bool layer, float zvalue);
	bool *GetROI(){return dataIn;}

  	int width;
  	bool SELECTED;
  
  	GLfloat tb_transform[4][4];
    	GLint viewport[4];
    	GLdouble mvmatrix[16], projmatrix[16];

protected:
	void cleanup();
private:
	svVector3 *dataPos;
	svVector3 *dataVel;
	svScalar *dataDen;
	svScalar *dataExp;
	svScalar *dataCoe;
	svInt dataSize;
	svInt *dataCluster;
	bool *dataIn;

	svInt *countCluster;
	svScalar *splitCount;	

	bool *selected_disc;


	vector<svVector3> centricPos;
	vector<svVector3> centricVel;
	vector<svScalar>centricDen;
	vector<vector<svScalar> > distributeDen;

	vector<svVector4> dataColor;
	vector<svVector4> clusterColor;

	int numCluster;
	int numSymmetry;
	int numDisc;
};
}
//#endif // __SV_DISCs
