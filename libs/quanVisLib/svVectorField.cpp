/**
 *  This file implements the class: svVectorField and svAdvect
 */

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "svException.h"
#include "svVectorField.h"

using namespace std;

#define DEFAULT_DELTA_T      0.001
#define DEFAULT_MIN_DELTA_T  0.001
#define DEFAULT_MAX_DELTA_T  0.08
#define DEFAULT_ERROR_TOLERANCE  0.0001

#define DEFAULT_MIN_BBOX  999
#define DEFAULT_MAX_BBOX  -999

namespace __svl_lib {

/*contour generation*/
svContour::svContour(svVectorField *inputField)
{
  field = inputField;
}

void svContour::ComputeContours(char *vtkfName, char *contourfName, float contourValue)
{
//#ifdef _VTK
//  char *outfilename = new char[1024];
//  sprintf(outfilename, "%s/preprocessing/generateContours/input", LIB_DIR);

//cerr<<outfilename<<endl;

 // ofstream outfile(outfilename);

 //int num = contourValues.size();
 //outfile<<num<<endl;

 //for(int i=0;i<num;i++)
 //{
/*	if(contourValues[i] == SV_CONTOUR_RATIO)
	{
			outfile<<"1 "<<contourValues[i+1]<<endl;
	}
	else if(contourValues[i] == SV_CONTOUR_ABSOLUTE)
	{
			outfile<<"0 "<<contourValues[i+1]<<endl;
	}
	i = i + 2;
*/
	//outfile<<contourValues[i]<<endl;
 //}
 //outfile.close();

 /*------------call the explicit prorams------------------------------*/
      char *exe = new char[2048];
	  //char *contourname = new char[400];
	  //sprintf(contourname, "%s/%s/%s", field->dataDir, field->dataFile, outContourfname); //contour.txt
      sprintf(exe, "./Contour %s %s %g", 
	      vtkfName, contourfName, contourValue);
cerr<<exe<<endl;		  
      system(exe); 
 //    cerr<<exe<<endl;
/*----------------------------------------------------------------*/
//delete [] outfilename;
  //delete [] contourname; 
  delete [] exe;

//#endif
}
/*contour end*/


/*KMeans clustering*/

svKmeans::svKmeans()
{
}

	

void svKmeans::Normalization(char *datafName, char *normalizefName, 
				int dim, int dataSize, svScalarArray weight)
{
	vector< vector<double> >  data;

	vector<double> mean(dim);
	vector<double> stdev(dim);

	for(int i=0;i<dim;i++)
	{
		mean[i] = 0;	
		stdev[i] = 0;
	}

	ifstream infile(datafName);
	ofstream outfile(normalizefName);
		
	for(int i=0;i<dataSize;i++)
	{
		vector<double> temp_data(dim);
		
		for(int j=0;j<dim;j++)
		{
			infile>>temp_data[j];
			mean[j] = mean[j] + temp_data[j];
		}
		data.push_back(temp_data);
		
		temp_data.clear();
	}
	
	infile.close();
	

	if(data.size()>0)
	{
		for(int i=0;i<dim;i++)
		{
			mean[i] = mean[i]/data.size();
		}		
		for(int i=0;i<data.size();i++)
		{
			for(int j=0;j<dim;j++)
			{
				stdev[j] = stdev[j] + (data[i][j]-mean[j])*(data[i][j]-mean[j]);
			}
		}
		for(int i=0;i<dim;i++)
		{
			stdev[i] = sqrt(stdev[i]/(data.size()-1));
		//	cerr<<mean[i]<<" "<<stdev[i]<<" "<<data.size()<<endl;
			if(stdev[i]==0) stdev[i] = 1;
		}
	}
	
	for(int i=0;i<data.size();i++)
	{
		for(int j=0;j<dim;j++)
		{
	//		if(j==dim-1)
	//		cerr<<mean[j]<<" "<<stdev[j]<<" "<<data[i][j]<<" "<<(data[i][j] - mean[j])/stdev[j] * weight[j]<<endl;
			outfile<<(data[i][j] - mean[j])/stdev[j] * weight[j]<<" ";
		}
		outfile<<endl;
	}		
	outfile.close();

	for(int i=0;i<data.size();i++)
	{
		data[i].clear();
	}
	data.clear();
	mean.clear();
	stdev.clear();
}

void svKmeans::ComputeClusters(char *datafName, char *clusterfName, 
								int dim, int dataSize, int numCluster)
{
	
	
	//ifstream infile(infname);
	//ofstream output(outfname);	

	//int num;
	//infile>>num;
	//int dim;
	//output<<num<<endl;
	//infile>>dim;
	//for(int i=0;i<num;i++)
	//{
		//int dataNum;
		

		//infile>>dataNum;	
		//output<<dataNum<<endl;

		//char *clusterFile = new char[400];
		//sprintf(clusterFile, "%s/%s/cluster_input_temp.txt", 
		//	field->GetDataDir(), field->GetDataFile());
		//char *clusteroutFile = new char[400];
        //        sprintf(clusteroutFile, "%s/%s/cluster_output_temp.txt", 
        //                field->GetDataDir(), field->GetDataFile());

		//ofstream outfile(clusterFile);
		//for(int j=0;j<dataNum;j++)
		//{
			//for(int t=0;t<dim;t++)
			//{
				//float tmp;
				//infile>>tmp;
				//outfile<<tmp<<" ";
			//}outfile<<endl;
		//}
		//outfile.close();

		//if(dataNum > 0)
		//{	
			char *exe = new char[2048];
			
			sprintf(exe, "./kmlsample -d %d -k %d -max %d -df %s > %s 2>&1",  
			dim,
			numCluster,
			dataSize,
			datafName,
			clusterfName);
			
			system(exe);
			
//		cerr<<exe<<endl;
		
			//ifstream readcluster(clusteroutFile);
			//float c;
			//for(int ii=0;ii<dataNum;ii++)
			//{
				//readcluster>>c;
				//output<<c<<endl;
			//}
			//readcluster.close();

			delete [] exe;
		//}

		//delete [] clusteroutFile;
		//delete [] clusterFile; 
	//}

	//output.close();
	//infile.close();
}

/*KMeans end*/

/*Symmetry*/
svSymmetry::svSymmetry(svVectorField *inputfield)
{
    field = inputfield;
}

//constrain: only allow symmetry patterns 
void svSymmetry::ComputeSymmetry(SymmetryProperty &property)
{
   vector<int> index;
   svVector3Array pos;

   ifstream infile(property.datafile);
//cerr<<property.datafile<<endl;
   int n;
   infile>>n;
//   pos.resize(n);
   for(int i=0;i<n;i++)
   {//cerr<<i<<" "<<pos[i][0]<<" "<<n<<endl;
       double tmp[4];
       svVector3 p;
       infile>>p[0]>>p[1]>>p[2]
             >>tmp[0]>>tmp[1]>>tmp[2]>>tmp[3];
       index.push_back(-1);
       pos.add(p);

      // cerr<<i<<" "<<pos[i][0]<<" "<<pos[i][1]<<" "<<pos[i][2]<<endl;
   }
   
   infile.close();
//cerr<<property.datafile<<endl;
   int count = -1;
   for(int i=0;i<n;i++)
   { //cerr<<i<<endl;
        vector<int> symmetryindex;
        if(index[i] == -1)
        {
            // #pragma omp for
             for(int j=0;j<n;j++)
             {//cerr<<"==="<<endl;
                for(int t =0;t<property.pos.size();t++)
                {
                 //    svVector3 p1 = pos[i] - property.pos[t];
                 //    svVector3 p2 = pos[j] - property.pos[t];
                     svVector3 p = normalize(pos[i] - pos[j]);
                     svScalar d1 = (pos[i][0] - property.pos[t][0])*(pos[i][0] - property.pos[t][0])
                      +(pos[i][1] - property.pos[t][1])*(pos[i][1] - property.pos[t][1])
                      +(pos[i][2] - property.pos[t][2])*(pos[i][2] - property.pos[t][2]);
                     svScalar d2 = (pos[j][0] - property.pos[t][0])*(pos[j][0] - property.pos[t][0])
                      +(pos[j][1] - property.pos[t][1])*(pos[j][1] - property.pos[t][1])
                      +(pos[j][2] - property.pos[t][2])*(pos[j][2] - property.pos[t][2]);

                /*     if(pos[i][0] == 11 && pos[i][1] == 18
                        && pos[j][0] == -18 && pos[j][1] == -11)
                     {
                          cerr<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<fabs(dot(p, property.dir[t]))<<" "<<fabs(dot(normalize(pos[i] - pos[j]), property.dir[t]))<<endl;
                     }
*/
                     if(fabs(fabs(dot(p, property.dir[t]))-1.) <0.000001
                    && fabs(d1-d2)<0.000001)
                   // &&fabs(dot(normalize(pos[i] - pos[j]), property.dir[t]))<0.0000001)
                     {
                        symmetryindex.push_back(j);
                        if(index[j] > -1)
                        {
                              index[i] = index[j];
                        }
                     }

                }
             }
             if(index[i] == -1)
             {
                  count++;
                  index[i] = count;
             }
             for(int j=0;j<symmetryindex.size();j++)
             {
                  index[symmetryindex[j]] = index[i];
             }
      //       cerr<<symmetryindex.size()<<endl;
        }
        symmetryindex.clear();
   }
//cerr<<property.outputfile<<endl;
   ofstream outfile(property.outputfile);
   outfile<<count+1<<endl;
   for(int i=0;i<n;i++)
   //      outfile<<pos[i][0]<<" "<<pos[i][1]<<" "<<pos[i][2]<<" "<<index[i]<<endl;
            outfile<<index[i]<<endl;
   outfile.close();

   pos.free();
   index.clear();
}

/*Symmetry end*/

//
// svVectorField Methods
//
svVectorField::svVectorField() 
{
  lbbox = svVector3(DEFAULT_MIN_BBOX,DEFAULT_MIN_BBOX,DEFAULT_MIN_BBOX);
  rbbox = svVector3(DEFAULT_MAX_BBOX,DEFAULT_MAX_BBOX,DEFAULT_MAX_BBOX);

  //JC: advect = new svAdvect(this); 

  max_strength = 0;
  min_strength = 9999;
}

svVectorField & svVectorField::operator=(const svVectorField & vef)
{
    if(this == &vef)
        return *this;

    dimension = vef.dimension;
    lbbox = vef.lbbox;
    rbbox = vef.rbbox;
	
	//dataFile = vef.dataFile;
	//dataDir = vef.dataDir;

    return *this;
}


}
