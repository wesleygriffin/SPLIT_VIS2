#ifndef __SV_CONTOUR_H
#define __SV_CONTOUR_H

#ifdef _VTK

#include "vtkContourFilter.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkDelaunay2D.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkUnstructuredGrid.h"
//#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkSmartPointer.h"
#include "vtkPoints.h"
#include "vtkPolygon.h"

#endif


#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;




class svContour
{
public:
  svContour();
  virtual ~svContour(){cleanup();}

  void Get2DContour(char *infile, double density);

  int getNumOfIntegerDigits(double num);
  void SavePointtoFile(char *file);
  void SavePointtoVTK(char *file);
  void SavePointtoOSG(char *file);

  void cleanup();

protected:

private:
        vector< vector<double> > isoPointPos;
        vector< vector<double> > isoPointDir;
        vector<double> isoPointDen;
		//vector<double> isoPointExp;
		//vector<double> isoPointCoe;

        int pointnum;
		int zLayer;//actually existing z layers
		//int DenScalar;
};

#endif
