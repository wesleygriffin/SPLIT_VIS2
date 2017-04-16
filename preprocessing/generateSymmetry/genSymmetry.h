
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

struct Spin
{
	double px;
	double py;
	double pz;
	
	double vx;
	double vy;
	double vz;
	
	double den;
};

class genSymmetry
{
  public:
    genSymmetry(){}
    virtual ~genSymmetry(){cleanup();}
    
    int getNumOfIntegerDigits(double num);
    
    void SetData(char *file);
    void SetPlane(vector< vector<double> > pos, double *vec); 
    void Process();   
 
    void SaveToFile();
    
  protected:
    void cleanup();
    
  private:
    vector<Spin> data;
    vector< vector<double> > planepos;
    double planevec[3];
};
