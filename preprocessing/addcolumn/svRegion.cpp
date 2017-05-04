#include <fstream>
#include <iostream>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <string.h>
#include <sys/stat.h>
#include <iomanip>
using namespace std;

int main(int argc, char *argv[])
{
     int type = atof(argv[1]);
     char *str = strdup(argv[2]);
     char *out = strdup(argv[3]);
     ifstream infile(str);
     ofstream outfile(out);
     if(type ==0)
     {
         double p[9];
         while(infile>>p[0]>>p[1]>>p[2]>>p[3]>>p[4]>>p[5]>>p[6]>>p[7]>>p[8])
         {
             outfile<<fixed<<showpoint<<std::setprecision(4)<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3]<<" "<<p[4]<<" "
                 <<std::setprecision(15)<<p[5]<<" "<<p[6]<<" "<<p[7]<<" "<<p[8]<<" "<<std::setprecision(4)<<p[3]<<endl;
         }
     }         
     else
     {
         double p[9];
         while(infile>>p[0]>>p[1]>>p[2]>>p[3]>>p[4]>>p[5]>>p[6]>>p[7])
         {
            if(p[2]<10)
            {
              if(p[3]<2)
             {outfile<<fixed<<showpoint<<std::setprecision(4)<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3]<<" "<<p[4]<<" "
                 <<std::setprecision(15)<<p[5]<<" "<<p[6]<<" "<<p[7]<<" "<<std::setprecision(4)<<p[3]<<endl;
              }
              else
             outfile<<fixed<<showpoint<<std::setprecision(4)<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3]<<" "<<p[4]<<" "
                 <<std::setprecision(15)<<p[5]<<" "<<p[6]<<" "<<p[7]<<" "<<std::setprecision(4)<<p[3]+1<<endl;
            }

            else
            {
                 if(p[3]<2)
             outfile<<fixed<<showpoint<<std::setprecision(4)<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3]<<" "<<p[4]<<" "
                 <<std::setprecision(15)<<p[5]<<" "<<p[6]<<" "<<p[7]<<" "<<std::setprecision(4)<<p[3]+1<<endl;
                else
             outfile<<fixed<<showpoint<<std::setprecision(4)<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3]<<" "<<p[4]<<" "
                 <<std::setprecision(15)<<p[5]<<" "<<p[6]<<" "<<p[7]<<" "<<std::setprecision(4)<<p[3]+1<<endl;
             }
         }
     }
 
}
