#include "../ann_1.1.1/include/ANN/ANN.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

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

using namespace std;

namespace __svl_lib {

class svDistance
{
  public:
    svDistance(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe,
		svInt size);
    ~svDistance(){cleanup();}

      void SetData(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe,
		svInt size);
	void SetSelectedLayer(int n);
      void SetDirection(vector< svVector3 > v, 
			vector<svScalar> z);

	void DecideSelectedPoint(){decide_layer = select_layer;}
      void CosDistance();
      void ANN_Montecarlo();

	void getDistance(svScalar *dis);

  protected:
	void cleanup();

	svVector3 * dataPos;
	svVector3 * dataVel;
	svScalar * dataDen;
	svScalar * dataExp;
	svScalar * dataCoe;
	int dataSize;
	vector<int> Z_format;

	svScalar *dataDistance;
	
	int select_layer;
	int decide_layer;
	
	vector<double> direction_z;
	vector<svVector3> direction_vel;
	vector<int> select_index;
	

};
}

