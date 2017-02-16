
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

#include "svUtil.h"
#include "svAnalyticField.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif

using namespace std;

#ifndef __SV_QDOT_DATA_H
#define __SV_QDOT_DATA_H

namespace __svl_lib {

class svQDOT : public svVectorField
{
  public:
        svQDOT(){};
	svQDOT(svChar *formatfName);
    //svQDOT(char *dir, char *file, char *outdir);
	
	//void SetIsWhole(svInt i){Cluster_isWhole = i;}
	
	//virtual void SetClusterInput(svInt isRaw, svIntArray & layer, svIntArray *roi, 
	//							char *outfname, svScalarArray &parameters);
								
	virtual void SetVTK(svChar *rawdir, svChar *rawfile, svChar *dir,
                            svChar *sortFile, svChar *formatFile,
                            svChar *densityFile,
                            svVector3 ppos,
					svVector3 pdir, svScalar pdis);
	virtual void New(char *formatfName);
	
	int GetPlaneIndex(svVector3 p);
	svScalar GetMaxDensitybyLayer(char *densityfName, int layer);

        svVector3 GetPlaneDir(){return plane_dir;}
	virtual svInt GetPlaneNum(){return plane_num;}
        svInt GetNumPower(){return numPower;}
        svScalar GetScaling(){return scaling;}	
    virtual ~svQDOT(){}
	
protected:

	//svInt Cluster_isWhole;
	
	svInt plane_num;
	svVector3 min_plane;
	svVector3 plane_dir;
	svScalar plane_distance;
	svScalar xdistance;
	svScalar ydistance;
	svScalar zdistance;
	svScalar scaling;
        svScalar min_den;
        svScalar max_den;
        svInt numPower;
        svScalar min_den_zero;
	//
	//virtual void SetClusterFile(svInt isRaw, svIntArray & layer, svIntArray *roi, 
	//							char *outfname, svScalarArray &parameters);
	//virtual void SetClusterFilebyLayer(svInt isRaw, svIntArray & layer, svIntArray *roi, 
	//							char *outfname, svScalarArray &parameters);
	//virtual void SetClusterFilebyWhole(svInt isRaw, svIntArray & layer, svIntArray *roi, 
	//							char *outfname, svScalarArray &parameters);
};

}
#endif
