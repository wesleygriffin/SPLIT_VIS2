#include "genSymmetry.h"

void genSymmetry()::SetData(char *file, int seed)
{
     this->seed = seed;

     char *str = new char[200];

     for(int i=0;i<seed;i++)
     {
          sprintf(str, "%s/%d.txt", file, i);
          ifstream infile(str);
 
          int n;
          infile>>n;

          for(int j=0;j<n;j++)
          {
              Spin d;
              infile>>d.px>>d.py>>d.pz>>d.vx>>d.vy>>d.vz>>d.den;
 
              data.push_back(d);
          }

          infile.close();
     }

     delete [] str;
}

void genSymmetry()::SetPlane(vector< vector<double> > pos, double *vec)
{
      planevec[0] = vec[0];
      planevec[1] = vec[1];
      planevec[2] = vec[2];

      planepos.resize(pos.size());
      for(int i=0;i<pos.size();i++)
      {
          planepos[i].resize(pos[i].size());
          for(int j=0;j<pos[i].size();j++)
          {
              planepos[i][j] = pos[i][j];
          }
      }
}

void genSymmetry()::ProcessAll()
{
     for(int j=0;j<data.size();j++)
     {
           if(symmetry[j] == -1)
           {
               for(int i=0;i<n;i++)
               {
                  for(int t=0;t<planepos.size();t++)
                  {
                      svVector3 p = 
                  }
               }
           }
     }
      
}
