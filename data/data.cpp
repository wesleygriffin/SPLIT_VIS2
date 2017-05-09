#include <fstream>

using namespace std;

int main(int argv, char **argc)
{

   ifstream infile(strdup(argc[1]));
   
   int type = atoi(argc[2]);

   if(type == 1)
   {
      double max, min;
      max = -1;
      min = 1;
      double p[9];
      while(infile>>p[0]>>p[1]>>p[2]>>p[3]>>p[4]>>p[5]>>p[6]>>p[7]>>p[8])
      {
           
      }
   }  
   else
   {
      double p[8];
      while(infile>>p[0]>>p[1]>>p[2]>>p[3]>>p[4]>>p[5]>>p[6]>>p[7])
      {

      }

   } 

   infile.close();

   return 0;
}
