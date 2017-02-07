#ifndef __SV_QDOT_GLYPH_H
#define __SV_QDOT_GLYPH_H

#include "svQDOT.h"
#include "svGlyph.h"

namespace __svl_lib {

class svQDOTGlyph : public svGlyph
{
 public:
  svQDOTGlyph(svVectorField* f, int numPlane);
		   
 // Update
	virtual void New(int numPlane);
	virtual void New(svVectorField* f, int numPlane);
 //
 
 //Computation
	virtual void SetROI();
 	virtual void SetROI(double mag1, double mag2);
	virtual void SetIsRaw(int i){isRaw = i;}
	virtual void SetData(svScalarArray * cV);
	
	virtual void SetColorsbyCluster(int isWhole, int color_model, svIntArray &layer, 
								svScalarArray &clusterParameters, int numCluster);
	
  virtual void SetAtom(int a){};

  virtual ~svQDOTGlyph();
  void   cleanQDOTGlyph(); // clean memory

 private:

 protected:
  svScalarArray  *digit;   //digit term
  svScalarArray  *exp; //exponent term
  
  svIntArray *atom; // atom type
  
  
  
  svInt isRaw;
  
  virtual void SetContours(svScalarArray * cV);
  virtual void SetRaw();

};

}

#endif // __SV_QDOT_GLYPH_H
