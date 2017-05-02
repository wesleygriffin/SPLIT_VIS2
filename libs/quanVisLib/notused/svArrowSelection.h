/**
 *  svLineGlyphQDOT
 *    plot line glyph on an irregular grid
 *    
 */

#ifndef __SV_ARROW_SELECTION_H
#define __SV_ARROW_SELECTION_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <algorithm>
#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svException.h"
#include "svUtil.h"
#include "svArrowGlyph.h"
//#include "./ann_1.1.1/include/ANN/ANN.h"

using namespace std;

namespace __svl_lib {

class svArrowSelection : public svArrowGlyph {

 public:
  svArrowSelection(char *data_file, char *format_file);  

  svArrowSelection(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe,svInt *atom,
		svInt size, vector<vector<vector<int> > > f);
  svArrowSelection(vector< vector<svVector3> > pos, vector< vector<svVector3> > vel,
		vector< vector<svScalar> > den, vector< vector<svScalar> > exp, vector< vector<svScalar> > coe,
		vector< vector<svInt> > atom, 
		svInt size, vector<vector<vector<int> > > f);
  ~svArrowSelection(){cleanup();}

  void Init();
 
    void Find_Nearest(svVector3 rayPos, svVector3 rayDir, GLfloat *tm);
    void DecideSelection();
    void Find_Current_Nearest(svVector3 rayPos, svVector3 rayDir, GLfloat *tm);

    svScalar GetSelectedPointMag(){return glyphMag[current_index];}
    svVector3 GetSelectedPointVel(){return glyphDir[0][current_index];}
    svVector3 GetSelectedPoint(){return glyphPos[0][current_index];}
    svVector3 *GetPoints(){svVector3 *c = new svVector3[selected_index.size()];for(int i=0;i<selected_index.size();i++)
					c[i] = glyphPos[0][selected_index[i]];return c;}
    int GetCurrentSelectedIndex(){return current_index;}
    int GetCurrentIndex(){return current_selected_index;}

	int GetVectorNum(){return selected_index.size();}
	vector<int> GetAllVector(){return selected_index;}

    double GetSelectedZ(){return acos(glyphDir[0][current_index][2])/M_PI * 180.;}
    double GetSelectedX(){return glyphDir[0][current_index][0];}
    double GetSelectedY(){return glyphDir[0][current_index][1];}
    double GetSelectedMag(){return glyphMag[current_index];}
    svVector3 GetSelected2DPos(GLfloat *tm);
    svVector3 * GetSelected2DAllPos(GLfloat *tm);

	bool SELECTED;
	bool SELECTED_CURRENT;

	void SetSelectedMarker(svVector3 c);
	void AddMarker(svVector3 c);
	void AddCurrentIndex(svVector3 c);
	void DeleteMarker(svVector3 c);
	void DeleteMarker();
	void Reset();

 private:
  vector<int> selected_index;
  int current_index;
  int current_selected_index;


 protected:
	void cleanup();
};
}
#endif // __SV_ARROW_GLYPH_H
