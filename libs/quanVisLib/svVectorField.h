/*  svVectorField
 */

#ifndef __SV_VECTORFIELD_H
#define __SV_VECTORFIELD_H
#include <vector>
#include "svType.h"
#include "svParticle.h"
#include "svArray.h"

#ifdef __APPLE__
#define  LIB_DIR  "/Users/jian/Documents/MyProjects/SPLIT_VIS2"
#else
#define  LIB_DIR  "/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2"
#endif

using namespace std;

namespace __svl_lib {

typedef struct _KmeansProperty {
	
   svBool isNormalize;
   svInt dimension;
   svInt numCluster;
   svIntArray clusterLayer;   
   svScalarArray clusterWeight;
   svChar *file1;
   svChar *file2;
   
} KmeansProperty;

typedef struct _ContourProperty {
	
   svInt seed_num;
   svScalarArray *contourValues;
   svIntArray isUpdate; //detect if we already have the contours;   
   svChar *outputfile;
   svChar *vtkdir;
   
} ContourProperty; 

typedef struct _SymmetryProperty{

   svChar *datafile;
   svChar *outputfile;
   svVector3Array pos;
   svVector3Array dir;

} SymmetryProperty;

/*
typedef enum {
  //SV_CONTOUR_LOCAL = 0,
  //SV_CONTOUR_GLOBAL = 1,
  SV_CONTOUR_RATIO = 1,
  SV_CONTOUR_ABSOLUTE = 0,
} svContourType;
*/
class svVectorField {
 public:
  svVectorField();
  //svVectorField(svChar *dir, svChar *fname){dataFile = fname; dataDir = dir;}
  svVectorField(svVector3 lb, svVector3 rb); // set the bounding box
  svVectorField(const svVectorField& vef) {*this=vef;}
  svVectorField & operator=(const svVectorField & vef);

  //virtual void New(svChar *dir, svChar *fname){dataFile = fname; dataDir = dir;}
  
  virtual ~svVectorField(){}//dataDir=NULL;dataFile=NULL;};
  
  //JC: virtual void CalculateRoughStrength(int num_seeds);
  virtual svVector3 GetVector(const svParticle & p){}// const =0;
  virtual svVector3 GetVector(const svVector3& pos, svScalar t){};// const =0;

  virtual void Update(){};// const = 0;

  svBool pointOutOfBound(svVector3 p) const 
  {
    if((p[0] < lbbox[0] && p[0]> rbbox[0]) && 
       (p[1] < lbbox[1] && p[1]> rbbox[1]) && 
       (p[2] < lbbox[2] && p[2]> rbbox[2])) {
    return true;
    };
    return false;
  }

  virtual void Print()
  {
    cerr << "-------------- svVectorField Print() ------------------" << endl;
    cerr << "Bounding box:  (" << lbbox[0]<<", " << lbbox[1]<<", " << lbbox[2]<<"); ("<< rbbox[0] <<", "<<rbbox[1]<<", " << rbbox[2]<<")" << endl;
    cerr << "=-----------end of svVectorField Print() -------------=" << endl;
  }
  
  // for feature extraction
  //virtual void jacobianMatrix(double x, double y, double z, svMatrix3 &jac){};

  // Get and Set
  // XXX: this is ambiguious
  virtual void GetPhysicalDimension(svScalar* sx, svScalar* sy, svScalar* sz) const
  {  
     *sx = rbbox[0]-lbbox[0]; *sy = rbbox[1]-lbbox[1]; 
     *sz = rbbox[2]-lbbox[2]; 
  };
  virtual void GetPhysicalDimension(svVector3 * lb, svVector3 * rb) const 
  {
      *lb= lbbox; *rb= rbbox;
  };
 //input file: "split by layer" file; output file: input file for kmeans; 
 //parameters: layer or whole;magnitude range;weights 

//move to svQDOT; set one label like isWhole  
  virtual void SetPhysicalDimension(svVector3  _lbbox,
				    svVector3  _rbbox)
  {lbbox=_lbbox; rbbox = _rbbox;  }

  virtual svVector3 GetCenter() const
  { return ((lbbox+rbbox)*0.5); }
  

  virtual svVector3 GetBoundingBoxSize() const
  { return (rbbox - lbbox);};

  virtual void GetBoundingBox(svVector3 *lb, svVector3 *rb) const
  { 
    GetPhysicalDimension(lb, rb);
 // *lb = lbbox; *rb = rbbox; 
  };

  virtual void GetBoundingBox(svScalar *minx,svScalar *maxx, 
                      svScalar *miny,svScalar *maxy,
                      svScalar *minz,svScalar *maxz) const
  {*minx=lbbox[0];*maxx=rbbox[0];*miny=lbbox[1];*maxy=rbbox[1];
   *minz=lbbox[2];*maxz=rbbox[2];
  };

  virtual svUint GetDimNum() const
    {  return dimension;  }

  virtual void SetDimNum(svUint d)
    {  dimension = d;  }

  svScalar GetMaxStrength()
    {  return max_strength; }
  svScalar GetMinStrength()
    {  return min_strength; }
    
    //svChar *GetDataFile(){return dataFile;}
   // svChar *GetDataDir(){return dataDir;}

	
 protected:
  //svContourType contour_type;

  /* general info for the flow volume */
  svUint   dimension;            // the dimension of field, 2, 3, 4, ...
  svScalar longest_side;           // the longest side edge of field
  svScalar max_strength; 
  svScalar min_strength;
  
  svVector3 lbbox, rbbox; //bounding box of the field


 public:
  struct  svContour {
    svVectorField *field;
    svContour(svVectorField *inputField);

    //void Contour(svFloat scale);

   //private: 
    void ComputeContours(char *vtkfName, char *contourfName, float contourValue);

  } *contour;
  friend struct svContour;

  struct svKmeans{
   //  svVectorField *field;
   //  svKmeans(svVectorField *inputField);
       svKmeans();
//     KmeansProperpety kmeansproperty;
//		void GetClusters(char *infname, char *outfname,
//					svScalarArray & clusterParameters);
//                void SetWeight(svScalarArray weightValue);
//		void EnableNormalization(){ifNormalize = true;}
//		void DiableNormalization(){ifNormalize = false;}
		void Normalization(char *datafName, char *normalizefName, int dim, int dataSize, svScalarArray weight);
		void ComputeClusters(char *datafName, char *clusterfName, int dim, int dataSize,int numCluster);
//		private:
//
//			void Clustering(svInt numCluster, 
//					svInt dataSize, svInt dimensions, 
//					char *infile, char *outfile);
	} *kmeans;
	friend struct svKmeans;

    struct svSymmetry{
       svVectorField *field;
       svSymmetry(svVectorField *inputfield);

       void ComputeSymmetry(SymmetryProperty &property);

    } *symmetry;
    friend struct svSymmetry;
   
};

typedef svVectorField::svKmeans svKmeans;
typedef svVectorField::svContour svContour;
typedef svVectorField::svSymmetry svSymmetry;
}
#endif // __SV_VECTORFIELD_H
