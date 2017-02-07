/**
 * ivLine
 */

#ifndef __IV_LINE_H
#define __IV_LINE_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
namespace __svl_lib {

class ivLine : public svPrimitive
{
 public:
  ivLine(svChar *inf, svInt _seed_num);
  ivLine(svVector3 _lbbox, svVector3 _rbbox, svChar *inf, svInt _seed_num);
/*
  ivLine(svVectorField* f, svParticle* sd, 
               svInt sd_num, svUint mode=SV_DISPLAYLIST);
  ivLine(svVectorField* f, 
               svVector3 _lbbox, svVector3 _rbbox,
               svParticle* sd, 
               svInt sd_num, svUint mode=SV_DISPLAYLIST);
*/

  virtual ~ivLine();
  void   cleanStreamLine(); // clean memory

  svVector3ArrayPtr GetVector3Array() const 
  {return streamline;}

  virtual void Print();

  virtual void SetBBox(svVector3 &new_lbbox, svVector3 &new_rbbox){};

  virtual void GenerateFromFile(); //read in for direct ren
  virtual void Generate(){};
  // Generate streamlines
  //virtual void Generate(svVector3 pos, svInt streamlineIndex); 
  // Not implemented: Generate streamline from a position

  virtual void Render();
  virtual void RenderGroups(); // render grouped_streamline
  // Rendering

  virtual void RemovePoint(svInt lineIndex);
  // Not implemented: remove a streamline
  virtual void SetPoint(svVector3 newp); 

  void EnableColor();
  virtual void EnableColor(svInt colorModel){}; // colorModel: SV_GRAY, SV_LOCS
  virtual void EnableLineWidth(svFloat minlw, svFloat maxlw){};

  //virtual void SetLut(const svLut & newlut)
  //{}; // Not implemented
  //virtual void GetColor(svVector4Array *colorArray) const 
  //{}; // Not implemented
  //virtual void GetLineWidth(svScalarArray *lineWidth) const 
  //{}; // Not implemented
  virtual void GetPosition(svVector3Array *streamlinepos) const 
  {};// Not implemented
  virtual svScalar  CalcLineWidth(const svVector3 &pos) 
  {return -1;};
  virtual svVector4 CalcLineColor(const svVector3 &pos) 
  {return svVector4(0,0,0,0);}

  virtual void Replace(svUint streamline_num, svScalar x, svScalar y, svScalar z)
  {}; // Not imeplemented


/*
  virtual void   Advance(svInt &dir, svInt &lineIndex, svParticle &start);
  virtual svParticle AdvanceStepOnly(svInt &dir, svParticle &start);
*/

  svDouble streamlineSegLengthSq(svVector3 p, svInt i)
  {
    int lastIndex = streamline[i].size()-1;
    if(lastIndex >= 0)
    {
      svDouble l = ((p[0]-streamline[i][lastIndex][0])*
            (p[0]-streamline[i][lastIndex][0])+
            (p[1]-streamline[i][lastIndex][1])* 
	    (p[1]-streamline[i][lastIndex][1])+
            (p[2]-streamline[i][lastIndex][2])* 
	    (p[2]-streamline[i][lastIndex][2]));
      return l;
    };
    return 9999; // don't check = pass the first 
  }

  virtual svVector4Array * GetColor4Array() const {return streamlineColors;}
  void  SetSeparationBy(int _ng, int *numfg, float _dis);
  void  SetSeparationBy(int _ng, int *numfg, int num_in_one_line, float _dis);
  void  SetDividingLines(int nfiles, int *frameIndex);
  void  SetSpeciesLables(int nfiles, char **lables);
  void  SetLineLength(float l) {length=l;}
  void  SetCoordAndLabels(int nf, float *_locx, float *_locy, float sx, float sy, char **_labels);
  void  SetCoordAndLabels(int nf, float *_locx, float *_locy, float sx, float sy, char **_labels, char ** _speed, char ** _mass);
 private:
  void RenderStore() const;     // using vertex array

 protected:
  virtual void GroupStreamlines();
  virtual void RenderStoreDirect() const;
  virtual void  BuildDisplayListFromStore();

  svVector3Array *grouped_streamline; // save updated positions
  svVector3Array *streamline;     // save the caliberated positions
  svVector4Array *streamlineColors;
  svScalarArray  *streamlineWidth;

  svChar  *infile;

  // this is bad coding style
  //------------------------------------------
  // render according the a rule
  float    dis; // distance between the groups
  int      ng;  // number of grouping
  int     *nfg;  // num of files in a group
  int      num_in_line; // number of groups in each line



  // shoudl use this one
  float *locx, *locy;
  int num_of_items;
  float   *cenx, *ceny, *cenz; 
  // the bookstein coords are not necessarily located on the spot.
 

  // 
  bool LABEL_SET;
  bool FULL_LABEL_SET;
  bool DOWNSTROKE_SET;

  char **label, **speed, **mass;
  char **full_label_set;

  int   *num_of_downstroke;
  float length;
};

}

#endif // __IV_LINE_H
