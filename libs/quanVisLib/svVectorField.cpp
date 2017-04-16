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
#include "svConfig.h"


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
      sprintf(exe, "%s/Contour %s %s %g",  BIN_DIR,
	      vtkfName, contourfName, contourValue);
//cerr<<exe<<endl;		  
      system(exe); 
 //    cerr<<exe<<endl;
/*----------------------------------------------------------------*/
//delete [] outfilename;
  //delete [] contourname; 
  delete [] exe;

//#endif
}
/*contour end*/

void svVectorField::ProcessContour(char *contourfile, char *dir, int layer)
{
     char *tmp = new char[200];
     sprintf(tmp, "%s/coor%d.txt", dir, layer);

     svVector3 pos;
     svVector3 x;
        svVector3 y;
        svVector3 z;

     ifstream infile(tmp);
        infile>>pos[0]>>pos[1]>>pos[2];
        infile>>x[0]>>x[1]>>x[2];
        infile>>y[0]>>y[1]>>y[2];
        infile>>z[0]>>z[1]>>z[2];
     infile.close();

        delete [] tmp;
        infile.open(contourfile);
        int n;
        infile>>n;
        svVector3Array ppp;
        svVector3Array vvv;
        svScalarArray mmm;
        for(int i=0;i<n;i++)
        {
                double p[3]; svVector3 v; svScalar m;
                infile>>p[0]>>p[1]>>p[2]>>v[0]>>v[1]>>v[2]>>m;

                svVector3 pp;
                pp[0] = pos[0] + p[0] * x[0];
                pp[1] = pos[1] + p[0] * x[1];
                pp[2] = pos[2] + p[0] * x[2];
                pp[0] = pp[0] + p[1] * y[0];
                pp[1] = pp[1] + p[1] * y[1];
                pp[2] = pp[2] + p[1] * y[2];
                ppp.add(pp);
                vvv.add(v);
                mmm.add(m);
        }
        infile.close();

        ofstream outfile(contourfile);
        outfile<<n<<endl;
        for(int i=0;i<n;i++)
        {
                double p[7];
                outfile<<ppp[i][0]<<" "<<ppp[i][1]<<" "<<ppp[i][2]<<" "
                       <<vvv[i][0]<<" "<<vvv[i][1]<<" "<<vvv[i][2]<<" "
                       <<mmm[i]<<endl;
        }
        outfile.close() ;

        ppp.free();vvv.free();mmm.free();
}

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
			
			sprintf(exe, "%s/kmlsample -d %d -k %d -max %d -df %s > %s 2>&1",  BIN_DIR,
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

int svSymmetry::SymmetryPair(SymmetryProperty &property, svVector3 pos, svVector3 dir,
                    svVector3 *pair, svVector3 *pairdir,
                    SYMMETRYTYPE type)
{
            int count = 0;
            for(int t=0;t<property.pos.size();t++)
            {
                 svVector3 d = property.pos[t] - pos;
                 svVector3 norm;
                 if(dot(d, property.dir[t])<0)
                 {
                          norm = - property.dir[t];
                 }
                 else norm = property.dir[t];
                 svScalar distance = dot(d, norm)*2;
                 svVector3 p = pos + norm * distance;
                 
                 svVector3 d2 = property.pos[t] - (pos+dir);
                 svScalar distance2 = dot(d2, norm) * 2;
                 svVector3 p2 = (pos + dir) + norm * distance2;

                 svVector3 dir2 = normalize(p2 - p);

               if(!(fabs(p[0]-pos[0])<0.001
                    && fabs(p[1]-pos[1])<0.001
                    && fabs(p[2]-pos[2])<0.001))
               {
                  if(type == _ANTISYMMETRY)
                  {
                      pairdir[count]=-dir2;
                      pair[count] = p;
                      count++;  
                  } 
                  else if(type == _POSITIVE)
                  {
                       pairdir[count] = dir2;
                       pair[count] = p;
                       count++;
                  }
                  else if(type == _NEGATIVENEAR)
                  {
                        if(dot(dir, norm)>0)
                        {
                              pairdir[count] = dir2;
                              pair[count] = p;
                              count++;
                        }
                  }
                  else if(type == _NEGATIVEFAR)
                  {
                        if(dot(dir, norm)<0)
                        {                                 
                              pairdir[count] = dir2;
                              pair[count] = p;
                              count++;
                        }
                  }
               }
            }
          
            return count;
}

void svSymmetry::ComputeAntiSymmetry(SymmetryProperty &property)
{
      ComputeSymmetry(property, _ANTISYMMETRY);
}

void svSymmetry::ComputePositiveSymmetry(SymmetryProperty &property)
{
      ComputeSymmetry(property, _POSITIVE);
}

void svSymmetry::ComputeNegativeNearSymmetry(SymmetryProperty &property)
{
      ComputeSymmetry(property, _NEGATIVENEAR);
}

void svSymmetry::ComputeNegativeFarSymmetry(SymmetryProperty &property)
{
      ComputeSymmetry(property, _NEGATIVEFAR);
}


void svSymmetry::ComputeSymmetry(SymmetryProperty &property, SYMMETRYTYPE type)
{

cout<<"Symmetry processing ..."<<endl;
   ifstream test(property.outputfile);

   if(!test.is_open())
  {
   svIntArray * index;
   svVector3Array *pos;
   svVector3Array *dir;

   ifstream infile(property.inputfile);
   int n;
   infile>>n;
   pos = new svVector3Array[n];
   dir = new svVector3Array[n];
   index = new svIntArray[n];
   int N = 0;
   for(int i=0;i<n;i++)
   {
       int m;
       infile>>m;
       for(int j=0;j<m;j++)
       {
       double tmp[4];
       svVector3 p,d;
       infile>>p[0]>>p[1]>>p[2]
             >>d[0]>>d[1]>>d[2]>>tmp[3];
       index[i].add(-1);
       pos[i].add(p);
       dir[i].add(d);
       N++;
       }
   }
   vector< vector<int> > symmetry[2];
   symmetry[0].resize(N);
   symmetry[1].resize(N);   

   infile.close();
   int count = -1;
   int ii=0;
cerr<<property.inputfile<<n<<endl;
   for(int i=0;i<n;i++)
   { 
      for(int j=0;j<pos[i].size();j++)
      {
        vector<int> symmetryindex[2];
        if(index[i][j] == -1)
        {
             vector<svVector3> pair;
             vector<svVector3> pairdir;
             vector<int> planeindex;
/*            for(int t=0;t<property.pos.size();t++)
            {
                 svVector3 d = property.pos[t] - pos[i][j];
                 svVector3 norm;
                 if(dot(d, property.dir[t])<0)
                 {
                          norm = - property.dir[t];
                 }
                 else norm = property.dir[t];
                 svScalar distance = dot(d, norm)*2;
                 svVector3 p = pos[i][j] + norm * distance;
               if(!(fabs(p[0]-pos[i][j][0])<0.001
                    && fabs(p[1]-pos[i][j][1])<0.001
                    && fabs(p[2]-pos[i][j][2])<0.001))
                 pair.push_back(p);
            }  
*/

           svVector3 *pp = new svVector3[property.pos.size()];
           svVector3 *dd = new svVector3[property.pos.size()];
           int size = SymmetryPair(property, pos[i][j], dir[i][j],
                    pp, dd,
                    type);
           // cerr<<type<<" "<<size<<" ";
           if(type == _POSITIVE)
           {
              for(int t=0;t<size;t++)
              {
                if(fabs(dd[t][0]-dir[i][j][0])<1e-3
                 && fabs(dd[t][1]-dir[i][j][1])<1e-3
                 && fabs(dd[t][2]-dir[i][j][2])<1e-3)
                 {
                     pair.push_back(pp[t]);
                     pairdir.push_back(dd[t]);
                 }
              }
           }
           else 
           {
              for(int t=0;t<size;t++)
              {
                  pair.push_back(pp[t]);
                  pairdir.push_back(dd[t]);
                 // cerr<<pp[t][0]<<" "<<pp[t][1]<<" "<<pp[t][2]<<" "<<dd[t][0]<<" "<<dd[t][1]<<" "<<dd[t][2]<<" ";
              }//cerr<<endl;
           }
           delete [] pp;
           delete [] dd;

           for(int t=0;t<pair.size();t++)
           {
                svScalar distance = dot(pair[t] - property.planepos, property.planedir);
                planeindex.push_back(distance/property.planedistance);
           }

           for(int t=0;t<planeindex.size();t++)
           { 
               if(planeindex[t]>=0 && planeindex[t]<n)
               {
                for(int tt=0;tt<pos[planeindex[t]].size();tt++)
                {
                      if(fabs(pos[planeindex[t]][tt][0] - pair[t][0])<1e-3
                     && fabs(pos[planeindex[t]][tt][1] - pair[t][1])<1e-3
                     && fabs(pos[planeindex[t]][tt][2] - pair[t][2])<1e-3
                     && fabs(dir[planeindex[t]][tt][0] - pairdir[t][0])<1e-3
                     && fabs(dir[planeindex[t]][tt][1] - pairdir[t][1])<1e-3
                     && fabs(dir[planeindex[t]][tt][2] - pairdir[t][2])<1e-3)
                    {
                          symmetryindex[0].push_back(planeindex[t]);
                          symmetryindex[1].push_back(tt);
                          break; 
                    }
                }
               }
           }
         //  cerr<<symmetryindex[0].size()<<endl;
           pair.clear();
           pairdir.clear();
           planeindex.clear();
 
             int value = -1;
             for(int t=0;t<symmetryindex[0].size();t++)
             {
                if(index[symmetryindex[0][t]][symmetryindex[1][t]]!=-1)
                {
                       value = index[symmetryindex[0][t]][symmetryindex[1][t]];
                }
             }
             index[i][j] = value;
             if(index[i][j] == -1)
             {
                  count++;
                  index[i][j] = count;
             }
             for(int t=0;t<symmetryindex[0].size();t++)
             {
                  bool flag = false;
                  index[symmetryindex[0][t]][symmetryindex[1][t]] = index[i][j];
                  for(int m=0;m<symmetry[0][index[i][j]].size();m++)
                  {
                        if(symmetryindex[0][t] == symmetry[0][index[i][j]][m]
                           && symmetryindex[1][t] == symmetry[1][index[i][j]][m])
                        {
                              flag = true;
                              break;
                        }
                  }                 
                  if(!flag)
                  {
                      symmetry[0][index[i][j]].push_back(symmetryindex[0][t]);
                      symmetry[1][index[i][j]].push_back(symmetryindex[1][t]);
                  } 
             }
                 bool flag = false;
                 for(int m=0;m<symmetry[0][index[i][j]].size();m++)
                  {
                        if(i == symmetry[0][index[i][j]][m]
                           && j == symmetry[1][index[i][j]][m])
                        {
                              flag = true;
                              break;
                        }
                  }
                  if(!flag)
                  {
                      symmetry[0][index[i][j]].push_back(i);
                      symmetry[1][index[i][j]].push_back(j);
                  }
        }
     //   for(int t=0;t<symmetryindex[0].size();t++)
     //       cerr<<symmetryindex[0][t]<<" "<<symmetryindex[1][t]<<" ";
     //   cerr<<endl;
     //  cerr<<symmetryindex[0].size()<<endl;
        symmetryindex[0].clear();
        symmetryindex[1].clear();
        ii++;
      }
   }
/*
  for(int i=0;i<n;i++)
  {
    for(int j=0;j<pos[i].size();j++)
    {
          if(index[i][j] == 1)
               cout<<"a"<<symmetry[0][index[i][j]].size()<<endl;
    }
  }
*/
cerr<<n<<endl;
//-----------savetofile-----------------
   ofstream outfile(property.outputfile);
   outfile<<count+1<<endl;
   count = 0;
   vector< vector< vector<int> > > list[2];
   list[0].resize(n);
   list[1].resize(n);
 /* for(int i=0;i<N;i++)
   { for(int j=0;j<symmetry[0][i].size();j++)
     cout<<symmetry[0][i][j]<<" "<<symmetry[1][i][j]<<" ";
     cout<<endl;
   }cout<<endl; 
*/
   for(int i=0;i<n;i++)
   {
       // cerr<<i<<" "<<n<<endl;
        list[0][i].resize(pos[i].size());
        list[1][i].resize(pos[i].size());

        for(int j=0;j<pos[i].size();j++)
        {
                list[0][i][j].push_back(i);
                list[1][i][j].push_back(j);
                for(int t=0;t<symmetry[0][index[i][j]].size();t++)
                {
                       if(i == symmetry[0][index[i][j]][t] 
                        && j == symmetry[1][index[i][j]][t])
                       {
                               
                       }
                       else
                       {
                            list[0][i][j].push_back(symmetry[0][index[i][j]][t]);
                            list[1][i][j].push_back(symmetry[1][index[i][j]][t]);
                       }
                       
                }
        }
   }
   for(int i=0;i<n;i++)
   {
          for(int j=0;j<pos[i].size();j++)
          {
                 outfile<<list[0][i][j].size()<<" ";
                 for(int ii=0;ii<list[0][i][j].size();ii++)
                         outfile<<list[0][i][j][ii]<<" "<<list[1][i][j][ii]<<" ";
                 outfile<<endl;
          }
   }

   for(int i=0;i<n;i++)
   {
          for(int j=0;j<pos[i].size();j++)
          {
                list[0][i][j].clear();
                list[1][i][j].clear();
          }
          list[0][i].clear();
          list[1][i].clear();
   }
   list[0].clear();
   list[1].clear();

   for(int i=0;i<N;i++)
   {
       symmetry[0][i].clear();
       symmetry[1][i].clear();
   }
   symmetry[0].clear();
   symmetry[1].clear();

   outfile.close();

  for(int i=0;i<n;i++)
 {  pos[i].free();
    dir[i].free();
   index[i].free();
  }
   delete [] dir;
   delete [] pos;
   delete [] index;
  }
  else
     test.close();
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
