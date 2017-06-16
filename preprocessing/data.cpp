#include <fstream>

using namespace std;

int main()
{
    ifstream infile("qdot_grid_40.txt");

    ofstream outfile("qdot_grid_40.vtk");

    outfile<<"# vtk DataFile Version 2.0"<<endl;
    outfile<<"qdot_grid_40.vtk"<<endl;
    outfile<<"ASCII"<<endl;   
    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;
    outfile<<"POINTS "<<51*101<<" float"<<endl;
   
    for(int i=0;i<101;i++)
    {
          for(int j=0;j<51;j++)
          {
              outfile<<i<<" "<<j<<" "<<0<<endl;
          }
    }
    outfile<<endl;

    outfile<<"SCALARS density float 1"<<endl;
    outfile<<"LOOKUP_TABLE default"<<endl;

    for(int i=0;i<101*51;i++)
    {
          double p;
          infile>>p;
          outfile<<p<<endl;
    }

    outfile.close();

    infile.close();

    return 0;
}
