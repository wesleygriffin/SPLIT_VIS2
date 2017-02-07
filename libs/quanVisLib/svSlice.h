#ifndef __SV_SLICE_H
#define __SV_SLICE_H

#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "svUtil.h"

#include "../jcUtil/MGL.h"
#include "../jcUtil/MArray.h"
#include "../jcUtil/MVectorMatrix.h"

using namespace std;

namespace __svl_lib {

class svSlice 
{
 public:

	svSlice(svVector3 l, svVector3 r);
  svSlice(svVector3 l, svVector3 r, 
	vector<svVector3> pv, vector<svVector3> pn);  
  ~svSlice(){cleanup();}

  void SetData(svVector3 l, svVector3 r);  
  
  //void SetMinDistance(svVector3 d){mindistance = d;}

  void RenderSlice();
  void RenderSlice(bool two);
  void RenderSlice(vector<int> plane);

  bool pointinpolygon(float x,float y);

  void SetRegularGrid();
  void Intersection();

  vector<svVector3> GetDir(){return plane_normal;}
  vector<svVector3> GetPoint(){return plane_vector;}

  void Reset();

  void SetPlane(vector<svVector3> w, vector<svVector3> v); 
  void SetPlane(svVector3 w, svVector3 v, double d);

  void move_ctrlpoint(int x, int y, bool tilted, svScalar precision, int width, GLfloat *tm);
  void Find_Nearest(svVector3 rayPos, svVector3 rayDir, GLfloat *tm);
  void Click(svVector3 rayPos, svVector3 rayDir, GLfloat *tm);

  void SetFontColor(svVector3 c){font_color =c ;}

  vector<bool> getSelected(){return click_selection;}
  void setSelected(vector<int> s, vector< vector<int> > s2);

  int getCurrentPlane(){return current_plane;}
  svVector3 getCurrentPlanePos(){return plane_vector[current_plane];}
  void setCurrentPlane(float d);
  void setCurrentPlane();
  int getPlaneSize(){return plane_vector.size();}

  vector<svVector3> getPlanePos(){return plane_vector;}

  bool SELECTED;
  vector<bool> NEAREST;
  
  GLfloat tb_transform[4][4];
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

 private:
  vector<svVector3> widgetp;
  float widgetl;

  vector<svVector3> plane_vector;
  vector<svVector3> plane_normal;

  GLdouble wx;
  GLdouble wy;
  GLdouble wz;
  int ni;
  int nj;

  svVector3 lb;	
  svVector3 rb;
  svVector3 rayInter;
  svVector3 start_plane_vector;

  vector< vector<svVector3> > intersectionp;
  vector<int> intersectionnum;
  vector<bool> click_selection;

  svVector3 font_color;
	int current_plane;
  
  
 protected:
	void cleanup();
};

}
#endif // __SV_SLICE_H
