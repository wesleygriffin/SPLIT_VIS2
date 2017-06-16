#include "genVTKfromOrg.h"
#include <iostream>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <string.h>
#include <sys/stat.h>

using namespace std;

#define STR_SIZE  1024 

int main(int argc, char *argv[])
{
    //char *in_dir = new char[200];
    //char *in_fname = new char[200];
    //char *out_dir = new char[200];
    int scalar=0;
    
/*
    char *raw_fname = new char[STR_SIZE];
    char *sort_fname = new char[STR_SIZE];
    char *vtk_fname = new char[STR_SIZE];
    char *format_fname = new char[STR_SIZE];
*/
    char *raw_fname = strdup(argv[1]);
    char *sort_fname = strdup(argv[2]);
    char *vtk_fname = strdup(argv[3]);
    char *format_fname = strdup(argv[4]);
    char *density_fname = strdup(argv[5]);
    char *input_fname = strdup(argv[6]);
    int format = atoi(argv[7]);
    
    mkdir(vtk_fname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  

    float pos_x, pos_y, pos_z, dir_x, dir_y, dir_z, distance;

    ifstream infile(input_fname);
    infile>>pos_x>>pos_y>>pos_z>>dir_x>>dir_y>>dir_z>>distance;
//cerr<<distance<<endl;
    infile.close();
//cerr<<density_fname<<endl;
    //scalar = atoi(argv[5]); 
    
   // sprintf(raw_fname, "%s/%s", in_dir, in_fname);
  //  sprintf(sort_fname, "%s/%s_sorted", out_dir, in_fname);
   // sprintf(vtk_fname, "%s/%s", out_dir, in_fname);
   // sprintf(format_fname, "%s/%s_format", out_dir, in_fname);
    
    genVTKfromOrg *qdotData = new genVTKfromOrg();

    qdotData->SetFormat(format);    
    qdotData->ReadRawData(raw_fname, scalar);//cerr<<"read"<<endl;
    qdotData->SaveSortedData(sort_fname);//cerr<<"read"<<endl;
    qdotData->SplitData(pos_x, pos_y, pos_z, dir_x, dir_y, dir_z, distance);
    qdotData->Compute2DData();
    cerr<<"splitdata"<<endl;

    qdotData->SaveSplitData(vtk_fname);
    qdotData->SaveRegiontoFile(vtk_fname);

    cerr<<"region"<<endl; 
    qdotData->SavetoVTK(vtk_fname);cerr<<"save to "<< vtk_fname << endl;
    qdotData->SaveIndex(vtk_fname);cerr<<"index"<<endl;
    qdotData->SaveAllData(vtk_fname);cerr<<"all"<<endl;
    qdotData->SaveLayerFormat(vtk_fname);cerr<<"layer"<<endl;
    qdotData->SaveZFormat(vtk_fname);cerr<<"zformat"<<endl;
    qdotData->SaveDataFormat(format_fname);cerr<<"save to "<< format_fname << endl;
    qdotData->Save2DDensity(vtk_fname);cerr<<"density"<<endl;
    qdotData->SaveDensityBylayer(density_fname);
cerr<<"done"<<endl;
    //delete [] in_dir;
    //delete [] in_fname;
    //delete [] out_dir;
    delete [] raw_fname;
    delete [] sort_fname;
    delete [] vtk_fname;
    delete [] format_fname;
    
//    delete qdotData;
 //   qdotData = NULL;
}
