
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

	double region;
	double atom;
};

bool Sort_Spin_Z(const Spin &lhs, const Spin &rhs);
bool Sort_Spin_Y(const Spin &lhs, const Spin &rhs);
bool Sort_Spin_X(const Spin &lhs, const Spin &rhs);

class genVTKfromOrg
{
  public:
    genVTKfromOrg();
    virtual ~genVTKfromOrg(){cleanup();}
    
    int getNumOfIntegerDigits(double num);
    int CheckFormat(char *file);
     
    void ReadRawData(char *file, int DEN_SCALAR);
    void SplitData(); // only split by z layer; not used anymore
    void SplitData(double pos_x, double pos_y, double pos_z,
			double dir_x, double dir_y, double dir_z,double distance);

    void Compute2DData();

    void SetFormat(int f){format = f;}
 
    void SaveSortedData(char *file);
    void SaveAllData(char *file);
    void SavetoVTK(char *file);
    void SaveSplitData(char *file);
    void SaveIndex(char *file);
    void SaveRegiontoFile(char *file);
    void SaveDataFormat(char *file); 
    void SaveDensityBylayer(char *file);
    void Save2DDensity(char *file);
    void SaveLayerFormat(char *file);
    void SaveZFormat(char *file);
    
  protected:
    void GenerateCoordinates(Spin p1, Spin p2, double *z, double *xAxis, double *yAxis);
    void SetNewCoordinates(Spin p, Spin origin, double *xAxis, double *yAxis,
                           double &x, double &y);
    void cleanup();
    
  private:
    vector<Spin> data;
    vector<int> splitReverseIndex[2];
    vector< vector<int> > splitIndex;
    vector< vector<Spin> > splitData;
    vector< vector<Spin> > split2dData;
    vector<double> xCoor[3];
    vector<double> yCoor[3];
    vector<int> Zlayer_Num;
    double lbbox[3];
    double rbbox[3];
    
    int scaling;
    int dataSize;
    double min_den;
    double min_den_zero;
    double max_den;
	vector<double> layerDensity;

    vector<int> unique_region;
    vector<int> region_count;
    vector<int> xDim;
    vector<int> yDim;

    int format;

	vector<double> plane_min;
	vector<double> plane_dir;
	double plane_distance;

    int min_region;
};
