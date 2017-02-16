
#include <math.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <assert.h>
#include <ctime>
#include <unistd.h>
#include <time.h>       /* time_t, struct tm, time, localtime */
//#include <X11/Xlib.h>
#include <sys/stat.h>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"

#include "svAnalyticField.h"
#include "svDistance.h"

//#include "svANNcluster.h"
//#include "svCuttingPlane.h"
//#include "svROI.h"

#include "MGL.h"
#include "MArray.h"
#include "MVectorMatrix.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif

using namespace std;

#ifndef __SV_QDOT_DATA_H
#define __SV_QDOT_DATA_H

namespace __svl_lib {

class svQDOT : public svAnalyticField
{
  public:
    svQDOT(){};
    svQDOT(char *dir, char *file, char *outdir);

    virtual ~svQDOT(){cleanup();}
    
    int getNumOfIntegerDigits(double num);

    void sortData(char *dir, char *file, char *outdir); //individual program
    void setCellRegion(int index);//need to be implemented
    void setData();
    void setFormat();

    //contour
    void contourGeneration(vector< vector<double> > locationR, vector<double> lr,
				vector< vector<double> > locationA, vector<double> la,
				vector<double> gr, vector<double> ga); //individual program
    void layerContourGeneration(vector< vector<double> > locationR, vector<double> lr,
				vector< vector<double> > locationA, vector<double> la,
				vector<double> gr, vector<double> ga, vector<bool> selected_plane); //individual program
	//cluster
	// JC
// original data clustering
	void layerClustering(int *numCluster, double *mag1, double *mag2, 
				double *ws, double *wv, double *wd,
				vector<bool> selected_plane,double plane_d);

// contour data clustering
	void layerContourClustering(double *mag1, double *mag2, 
				double *ws, double *wv, double *wd, vector<bool> selected_plane);

	//distance
	void distanceGeneration(int select_layer, vector<svVector3> v, vector<svScalar> z);
	//plane
	void getPlane(bool toggleData, bool toggleRaw, 
		      svVector3 plane_center, svVector3 plane_vector);
	//void setPlaneVTK();
	void getPlane(svVector3 plane_center, svVector3 plane_vector, svScalar plane_d, vector<bool> selected_plane);
	void getPlane(vector<svVector3> plane_center, svVector3 plane_vector);
	void getPlane(svVector3 plane_center, svVector3 plane_vector, int index);
	void planeClustering(bool toggleAttribute, bool toggleROI);

	//get
	vector< vector<svVector3> > getLayerContourPos(){return layerContourPos;}
	vector< vector<svVector3> > getLayerContourVel(){return layerContourVel;}
	vector< vector<svScalar> > getLayerContourDen(){return layerContourDen;}
	vector< vector<svScalar> > getLayerContourCoe(){return layerContourCoe;}
	vector< vector<svScalar> > getLayerContourExp(){return layerContourExp;}
	vector< vector<svInt> > getLayerContourAtom(){return layerContourAtom;}
	vector< vector<svInt> > getLayerContourCluster(){return layerContourCluster;}
	vector< vector<svInt> > getLayerDenContourCluster(){return layerDenContourCluster;}
	int getLayerContourSize(){return layerContourSize;}

	vector< vector<svVector3> > getLayerPosVec(){return layerPos;}
	vector< vector<svVector3> > getLayerVelVec(){return layerVel;}
	vector< vector<svScalar> > getLayerDenVec(){return layerDen;}
	vector< vector<svVector3> > getCellPosVec(){return cellPos;}
	vector< vector<svVector3> > getCellVelVec(){return cellVel;}
	vector< vector<double> > getCellDenVec(){return cellDen;}

	int *getRawCluster(){return dataCluster;}

//JC
	//vector< vector<double> > getLayermag(double z);
	svVector3 *getRawPos(){return dataPos;}
	svVector3 *getRawVel(){return dataVel;}
	svVector3 *getLayerRawVel(float z);
	svScalar *getRawDen(){return dataDen;}
	svScalar *getRawExp(){return dataExp;}
	svScalar *getRawCoe(){return dataCoe;}
	svScalar *getRawDis(){return dataDis;}
	svInt *getRawAtom(){return dataAtom;}
	svBool *getRawInbox(){return dataInbox;}
	int getMaglevel(){return magLevel;}

	int getRawSize(){return dataSize;}
	int getRawLayerSize(float z);
	svVector3 getLbbox(){return lbbox;}
	svVector3 getRbbox(){return rbbox;}
	vector< vector< vector<int> > > getFormat(){return dataFormat;}

	double getZmin(){return zmin;}
	double getZdistance(){return zdistance;}
	double getXdistance(){return xdistance;}
	double getYdistance(){return ydistance;}
	double getXl(){return xl;}
	double getYl(){return yl;}
	double getXd(){return xd;}
	double getYd(){return yd;}
	int getXnum(){return xnum;}
	int getYnum(){return ynum;}
	int getZlayer(){return zlayer;}
	double getScalar(){return scaling;}
	double getMax(){return max_density;}
	double getMin(){return min_density;}
	double getMax(float z);
	double getMin(float z);
	double getMinNozero(){return min_density_nonzero;}

//JC
	//vector< vector<double> > getMagrange(){return range_mag_cluster;}
	//vector< vector< vector<double> > > getLayerMagrange(){return range_layer_mag_cluster;}
	
	svVector3 * getContourPos(){return contourPos;}
	svVector3 * getContourVel(){return contourVel;}
	svScalar * getContourDen(){return contourDen;}
	svScalar * getContourExp(){return contourExp;}
	svScalar * getContourCoe(){return contourCoe;}
	int getContourSize(){return contourSize;}
	
	svVector3 *getLayerPos();
	svVector3 *getLayerVel();
	svScalar *getLayerDen();
	svScalar *getLayerExp();
	svScalar *getLayerCoe();
	svInt getLayerSize();
	int * getLayerCluster();
	int * getLayerDenCluster();
	int getlayer(){return layerPos.size();}
	vector<int> getRegionIndex(){return regionIndex;}
	vector< vector< vector<int> > > getCellIndex(){return cellIndex;}

  protected:
	void cleanup();

	char *dataDir;
	char *dataFile;

	svVector3 * dataPos;
	svVector3 * dataVel;
	svScalar * dataDen;
	svScalar * dataExp;
	svScalar * dataCoe;
	svScalar * dataDis;
	svInt *dataAtom;
	svBool *dataInbox;
	svInt *dataRegion;
	svInt *dataCluster;

	vector<int> regionIndex;
	vector< vector<svVector3> > cellPos;
	vector< vector<svVector3> > cellVel;
	vector< vector<double> > cellDen;
	vector< vector< vector<int> > > cellIndex;

	svVector3 * contourPos;
	svVector3 * contourVel;
	svScalar * contourDen;
	svScalar * contourExp;
	svScalar * contourCoe;
	svInt *contourAtom;

	int contourSize;
	
	int dataSize;	
	svVector3 lbbox;
	svVector3 rbbox;
   	vector< vector< vector<int> > > dataFormat;
	int magLevel;

	vector<svVector3> planeCenter;
	vector<svVector3> planeNormal;

	vector< vector<svVector3> > layerPos;
	vector< vector<svVector3> >  layerVel;
	vector< vector<svScalar> >  layerDen;
	vector< vector<svScalar> >  layerExp;
	vector< vector<svScalar> >  layerCoe;	
	vector< vector<svInt> >  layerAtom;
	vector< vector<int> > layerIndex;
	
	svInt layerSize;
	vector< vector<svInt> >layerCluster;
	vector< vector<svInt> > layerDenCluster;
        vector< vector<svBool> > layerInbox;

	vector< vector<svVector3> > layerContourPos;
	vector< vector<svVector3> >  layerContourVel;
	vector< vector<svScalar> >  layerContourDen;
	vector< vector<svScalar> >  layerContourExp;
	vector< vector<svScalar> >  layerContourCoe;	
	vector< vector<svInt> >  layerContourAtom;
	svInt layerContourSize;
	vector< vector<svInt> >layerContourCluster;
	vector< vector<svInt> > layerDenContourCluster;

	double zmin;
   	double zdistance;
   	double xdistance;
   	double ydistance;
   	double xl;
   	double xd;
   	double yl;
  	double yd;
  	int xnum;
   	int ynum;
   	int zlayer; 
   	double scaling;

   	double max_density;
   	double min_density;
   	double min_density_nonzero;
	vector<double> layer_density;
};

}
#endif
