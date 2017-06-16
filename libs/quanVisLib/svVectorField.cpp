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

/*Neighbors*/
svNeighbor::svNeighbor(svVectorField *inputfield)
{
    field = inputfield;
}

void svNeighbor::ComputeNeighbors(NeighborProperty &property)
{
   cout<<"Neighbor processing ..."<<endl;

  ifstream test(property.outputfile);
//cerr<<property.outputdir<<" "<<str<<endl;
  if(!test.is_open())
  {
   svVector3Array *pos;
   svIntArray *label;
   svIntArray *index;

   ifstream infile(property.inputfile);
   int n;
   infile>>n;
   pos = new svVector3Array[n];
   index=  new svIntArray[n];
   int N = 0;
   for(int i=0;i<n;i++)
   {
       int m;
       infile>>m;
       for(int j=0;j<m;j++)
       {
         svVector3 p,d;
         svScalar m;
         infile>>p[0]>>p[1]>>p[2]
             >>d[0]>>d[1]>>d[2]>>m;
         pos[i].add(p);
         index[i].add(N);
         N++;
       }
   }
   label = new svIntArray[N];

//cerr<<"========================================================="<<endl;
   infile.close();
  
   int size = property.svectors.size();
   int count = 0;

   for(int i=0;i<n;i++)
   {
 //     int percentage = i*100/n;
//      ProgressBar(percentage);

      for(int j=0;j<pos[i].size();j++)
      {
        vector<svVector3> pair;
        pair.resize(size);
        vector<int> planeindex;
        planeindex.resize(size);

        for(int t=0;t<size;t++)
        {
            pair[t] = pos[i][j] + property.svectors[t];
           
            svScalar distance = dot(pair[t] - property.planepos, 
                                    property.planedir);
            planeindex[t] = distance/property.planedistance;

            int layer = planeindex[t];
            
            if(layer >=0 && layer<n)
            {
                 for(int tt=0;tt<pos[layer].size();tt++)
                 {
                      if(fabs(pos[layer][tt][0] - pair[t][0])<1e-3
                      && fabs(pos[layer][tt][1] - pair[t][1])<1e-3
                      && fabs(pos[layer][tt][2] - pair[t][2])<1e-3
                      &&count>index[layer][tt])
                   {
                       label[count].add(index[layer][tt]);break;
                   }
                 }
            }
        }
        
        int ii;
        svScalar zz;
        zz = pos[i][j][2] - (int)pos[i][j][2];
        if(zz<0) zz = zz + 1.;

        for(int t=0;t<property.zvalues.size();t++)
        {
              if(fabs(zz - property.zvalues[t])<1e-3) 
              {
                 ii = t;break;
              } 
        }  
        size = property.dvectors[ii].size();

        pair.resize(size);
        planeindex.resize(size);

        for(int t=0;t<size;t++)
        {
            pair[t] = pos[i][j] + property.dvectors[ii][t];
            svScalar distance = dot(pair[t] - property.planepos,
                                    property.planedir);
            planeindex[t] = distance/property.planedistance;

            int layer = planeindex[t];

            if(layer >=0 && layer<n)
            {
                 for(int tt=0;tt<pos[layer].size();tt++)
                 {
                      if(fabs(pos[layer][tt][0] - pair[t][0])<1e-3
                      && fabs(pos[layer][tt][1] - pair[t][1])<1e-3
                      && fabs(pos[layer][tt][2] - pair[t][2])<1e-3
                     && count>index[layer][tt])
                   {
                       label[count].add(index[layer][tt]);break;
                   }
                 }
            }
        }

        pair.clear();
        planeindex.clear();
        count++;
      }
   }
//-----------savetofile-----------------

   ofstream outfile(property.outputfile);
   outfile<<N<<endl;
   for(int i=0;i<N;i++)
   {
      outfile<<label[i].size()<<" ";
      for(int j=0;j<label[i].size();j++)
      {
          outfile<<label[i][j]<<" ";
      } outfile<<endl;
   }
   outfile<<endl;

   outfile.close();

   for(int i=0;i<n;i++)
   {
       pos[i].free();
       index[i].free();
   }
   delete [] pos;
   delete [] index;
   for(int i=0;i<N;i++)
          label[i].free();
   delete [] label;
  }
  else 
    test.close();   
}
/*=============Neighbors End==================================*/

/*Symmetry*/
svSymmetry::svSymmetry(svVectorField *inputfield)
{
    field = inputfield;
}
int svSymmetry::GetType(svVector3 pos1, svVector3 end1, svScalar mag1,
                         svVector3 pos2, svVector3 end2, svScalar mag2,
                         svScalar angle_uncertain, svScalar mag_uncertain)
{
    bool f = false;

//    if(fabs(pos1[0]-7.77817)<1e-3
//     && fabs(pos1[1]-0)<1e-3
//     && fabs(pos1[2]+9.8995)<1e-3
//     && fabs(pos2[0]-7.77817)<1e-3
//     && fabs(pos2[1]-0)<1e-3
//     && fabs(pos2[2]-9.8995)<1e-3)
//      f = true;
//if(f)cerr<<mag1<<" "<<mag2<<" "<<mag_uncertain<<endl;
    if(fabs(mag1-mag2) > mag_uncertain)
           return -1;
//     if(f)cerr<<"i===test========"<<endl;

    if(fabs(mag1)<1e-10) return -1;
//     if(f)cerr<<"test"<<endl;

    if(fabs(mag2)<1e-10) return -1; 
//     if(f)cerr<<"test"<<endl;

  
    if(fabs(pos1[0] - pos2[0])<1e-3
     && fabs(pos1[1] - pos2[1])<1e-3
     && fabs(pos1[2] - pos2[2])<1e-3)
            return -1;
//     if(f)cerr<<"test"<<endl;

    if(fabs(pos1[0] - pos2[0])>1e-3)
           return -1;
//     if(f)cerr<<"test"<<endl;

    if(fabs(pos1[1] - pos2[1])>1e-3)
           return -1;
//     if(f)cerr<<"test"<<endl;

    if(fabs(pos1[2] + pos2[2])>1e-3)
           return -1;
//        if(f)cerr<<"======test======="<<endl;

    

    svVector3 end;
    double distance = (-pos1[2] + end1[2])*2.;

    int type = -1;
    double aa = 9e+9;

    end[0] = end1[0];
    end[1] = end1[1];
    end[2] = -end1[2];
    double d = dot(normalize(end-pos2), normalize(end2-pos2));
    if(d>1) d = 1.;
    double angle = fabs(acos(d));
    if(angle<angle_uncertain)
    {
         if(angle < aa)
         {
              type = _XSYSZS;
              aa = angle;
         }
    }

    end[0] = end1[0];
    end[1] = end1[1];
    end[2] = -end1[2]+distance;
    d = dot(normalize(end-pos2), normalize(end2-pos2));
    if(d>1) d = 1.;
    angle = fabs(acos(d));
    if(angle<angle_uncertain)
    {
         if(angle < aa)
         {  
              type = _XSYSZA;
              aa = angle;
         }
    }

    end[0] = end1[0];
    end[1] = -end1[1];
    end[2] = -end1[2];
    d = dot(normalize(end-pos2), normalize(end2-pos2));
    if(d>1) d = 1.;
    angle = fabs(acos(d));
    if(angle<angle_uncertain)
    {
         if(angle < aa)
         {  
              type = _XSYAZS;
              aa = angle;
         }
    }


    end[0] = end1[0];
    end[1] = -end1[1];
    end[2] = -end1[2]+distance;
    d = dot(normalize(end-pos2), normalize(end2-pos2));
    if(d>1) d = 1.;
    angle = fabs(acos(d));
    if(angle<angle_uncertain)
    {
         if(angle < aa)
         {  
              type = _XSYAZA;
              aa = angle;
         }
    }


    end[0] = -end1[0];
    end[1] = end1[1];
    end[2] = -end1[2];
    d = dot(normalize(end-pos2), normalize(end2-pos2));
    if(d>1) d = 1.;
    angle = fabs(acos(d));
    if(angle<angle_uncertain)
    {
         if(angle < aa)
         {  
              type = _XAYSZS;
              aa = angle;
         }
    }


    end[0] = -end1[0];
    end[1] = end1[1];
    end[2] = -end1[2]+distance;
    d = dot(normalize(end-pos2), normalize(end2-pos2));
    if(d>1) d = 1.;
    angle = fabs(acos(d));
    if(angle<angle_uncertain)
    {
         if(angle < aa)
         {  
              type = _XAYSZA;
              aa = angle;
         }
    }


    end[0] = -end1[0];
    end[1] = -end1[1];
    end[2] = -end1[2];
    d = dot(normalize(end-pos2), normalize(end2-pos2));
    if(d>1) d = 1.;
    angle = fabs(acos(d));
    if(angle<angle_uncertain)
    {
         if(angle < aa)
         {  
              type = _XAYAZS;
              aa = angle;
         }
    }


    end[0] = -end1[0];
    end[1] = -end1[1];
    end[2] = -end1[2]+distance;
    d = dot(normalize(end-pos2), normalize(end2-pos2));
    if(d>1) d = 1.;
    angle = fabs(acos(d));
//cerr<<d<<" "<<angle<<endl;
    if(angle<angle_uncertain)
    {
         if(angle < aa)
         {  
              type = _XAYAZA;
              aa = angle;
         }
    }
    
    return type;
}
void svSymmetry::SymmetryPair(svVector3 pos, svVector3 end, 
                    svVector3 &pair, svVector3 &pairend,
                    SYMMETRYTYPE type)
{
    double distance = -pos[2] + end[2];
    pair = pos;
    pair[2] = -pos[2];

    if(type == _XSYSZS)
    {
      pairend[0] = end[0];
      pairend[1] = end[1];
      pairend[2] = -end[2];
    }
    else if(type == _XSYSZA)
    {
       pairend[0] = end[0];
       pairend[1] = end[1];
       pairend[2] = -end[2]+distance*2.;
    }
    else if(type == _XSYAZS)
    {
       pairend[0] = end[0];
       pairend[1] = -end[1];
       pairend[2] = -end[2];
    }
    else if(type == _XSYAZA)
    {
      pairend[0] = end[0];
      pairend[1] = -end[1];
      pairend[2] = -end[2]+distance*2.;
    }
    else if(type == _XAYSZS)
    {
      pairend[0] = -end[0];
      pairend[1] = end[1];
      pairend[2] = -end[2];
    }
    else if(type == _XAYSZA)
    {
      pairend[0] = -end[0];
      pairend[1] = end[1];
      pairend[2] = -end[2] + distance*2.;
    }
    else if(type == _XAYAZS)
    {
      pairend[0] = -end[0];
      pairend[1] = -end[1];
      pairend[2] = -end[2];
    }
    else if(type == _XAYAZA)
    {
       pairend[0] = -end[0];
       pairend[1] = -end[1];
       pairend[2] = -end[2] + distance*2.;
    }
}
/*void svSymmetry::SymmetryPair(SymmetryProperty &property, svVector3 pos, svVector3 dir,
                    svVector3 &pair, svVector3 &pairdir,
                    SYMMETRYTYPE type)
{
            int count = 0;
            svVector3 d = property.pos - pos;
//cerr<<d[0]<<" "<<d[1]<<" "<<d[2]<<" ";
            svVector3 norm;
            if(dot(d, property.dir)<0)
            {
                  norm = - property.dir;
            }
            else norm = property.dir;
//cerr<<norm[0]<<" "<<norm[1]<<" "<<norm[2]<<" ";
            svScalar distance = dot(d, norm)*2;
//cerr<<dot(d, norm)<<" "<<distance<<endl;
            svVector3 p = pos + norm * distance;
                 
            svVector3 d2 = property.pos - (pos+dir);
            svScalar distance2 = dot(d2, norm) * 2;
            svVector3 p2 = (pos + dir) + norm * distance2;

            svVector3 dir2 = p2 - p;

               if(!(fabs(p[0]-pos[0])<0.001
                    && fabs(p[1]-pos[1])<0.001
                    && fabs(p[2]-pos[2])<0.001))
               {
                  if(type == _XSYM || type == _YSYM || type == _ZSYM)
                  { 
                      pairdir = dir2;
                      pair = p;
                  } 
                  else
                  {
                       pairdir = -dir2;
                       pair = p;
                  } 
               }
}
*/
/*:
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
*/
void svSymmetry::ComputeSymmetry(SymmetryProperty &property) //deposite in small programs!!!
{
//   property.angle_uncertain = 0.1;
//   property.mag_uncertain = 1e-10;

   cout<<"Symmetry processing ..."<<endl;
   char *str = new char[400];

   char *symmetrystr = new char[200];
   for(int j=0;j<200;j++) symmetrystr[j] = '\0';
   sprintf(symmetrystr, "%s(%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f)", symmetrystr,
                  property.pos[0], property.pos[1], property.pos[2],
                  property.dir[0], property.dir[1], property.dir[2]);
   sprintf(str,"%s", property.outputfile[0]);
///xsyszs%s.txt",
 //                property.outputdir, symmetrystr);

  ifstream test(str);
  if(!test.is_open())
  {
   svVector3Array *pos;
   svVector3Array *end;
   svScalarArray *mag;

   ifstream infile(property.inputfile);
   int n;
   infile>>n;
   pos = new svVector3Array[n];
   end = new svVector3Array[n];
   mag = new svScalarArray[n];
   vector< vector< vector<int> > > symmetry;   
   symmetry.resize(n);
//cerr<<"========================================================="<<endl;

   int N = 0;
   for(int i=0;i<n;i++)
   {
       int m;
       infile>>m;
       for(int j=0;j<m;j++)
       {
         svVector3 p,d;
         svScalar m;
         infile>>p[0]>>p[1]>>p[2]
             >>d[0]>>d[1]>>d[2]>>m;

        
         
         svVector3 newp = CoordinateMatrix(p,
                             property.pos,
                             property.x,
                             property.y,
                             property.dir);
         svVector3 p2 = p + (m * (svScalar)(1e+20))* d; 
//cerr<<p[0]<<" "<<p[1]<<"======= "<<p[2]<<" "<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<endl;
         svVector3 newp2 = CoordinateMatrix(p2,
                             property.pos,
                             property.x,
                             property.y,
                             property.dir);

//         if(fabs(p[0]-1.5)<1e-3
//     && fabs(p[1]-12.5)<1e-3
//    && fabs(p[2]-0)<1e-3)
//          cerr<<newp[0]<<" "<<newp[1]<<" "<<newp[2]<<" "
//            <<newp2[0]<<" "<<newp2[1]<<" "<<newp2[2]<<endl;
//         if(fabs(p[0]+12.5)<1e-3
//     && fabs(p[1]+1.5)<1e-3
//    && fabs(p[2]-0)<1e-3)
//          cerr<<newp[0]<<" "<<newp[1]<<" "<<newp[2]<<" "
//            <<newp2[0]<<" "<<newp2[1]<<" "<<newp2[2]<<endl;

         pos[i].add(newp);
         end[i].add(newp2);
         mag[i].add(m* (svScalar)(1e+20));

         N++;
       }
   }
//cerr<<"========================================================="<<endl;

   svVector3 newplanepos = CoordinateMatrix(property.planepos,
                             property.pos,
                             property.x,
                             property.y,
                             property.dir);
   svVector3 tmpdir = property.planepos + property.planedir;
   svVector3 newplanedir = CoordinateMatrix(property.planedir,
                             property.pos,
                             property.x,
                             property.y,
                             property.dir);
   newplanedir = normalize(newplanedir - newplanepos);
//cerr<<"========================================================="<<endl;

   infile.close();
   int count[SYMMETRY_TYPE];
   for(int i=0;i<SYMMETRY_TYPE;i++)
         count[i] = -1;
   for(int i=0;i<n;i++)
   {
      cerr<<i<<" ";
      symmetry[i].resize(pos[i].size()); 
      #pragma omp parallel for
      for(int j=0;j<pos[i].size();j++)
      {
        int symmetryindex[2*SYMMETRY_TYPE];

        svVector3 tmppos;
        tmppos[0]= pos[i][j][0];
        tmppos[1]= pos[i][j][1];
        tmppos[2]= -pos[i][j][2];
        svScalar distance = dot(tmppos-newplanepos, newplanedir);
        for(int t=0;t<SYMMETRY_TYPE;t++)
        {
             symmetryindex[t*2] = -1;
             symmetryindex[t*2+1]=-1;
        } 
        int whole_count = 0;
        int layer = distance/property.planedistance;

    if(fabs(pos[i][j][0]-7.77817)<1e-3
     && fabs(pos[i][j][1]-0)<1e-3
     && fabs(pos[i][j][2]-9.8995)<1e-3)
         cerr<<layer<<endl;
        if(layer >=0 && layer<n)
        {
           for(int tt=0;tt<pos[layer].size();tt++)
           {
         //      if(whole_count == SYMMETRY_TYPE)break;
               int type = GetType(pos[i][j], end[i][j], mag[i][j],
                       pos[layer][tt], end[layer][tt], mag[layer][tt],
                       property.angle_uncertain, property.mag_uncertain*1e+20);
               if(type<0)
               {
                     continue;
               }
               symmetryindex[type*2]=layer;
               symmetryindex[type*2+1]=tt;
               count[type]++;
               whole_count++;
           }
        }
        for(int t=0;t<SYMMETRY_TYPE;t++)
        {
            symmetry[i][j].push_back(symmetryindex[t*2]);
            symmetry[i][j].push_back(symmetryindex[t*2+1]);
        }
      }

   }cerr<<endl;
//-----------savetofile-----------------
   for(int tt=0;tt<SYMMETRY_TYPE;tt++)
   {
     char *str2 = new char[400];
//     switch(tt)
//     {
//        case 0:  sprintf(str2,"%s/xsyszs%s.txt",
//                     property.outputdir, symmetrystr);break;
//        case 1:  sprintf(str2,"%s/xsysza%s.txt",
//                     property.outputdir, symmetrystr);break;
//        case 2:  sprintf(str2,"%s/xsyazs%s.txt",
//                     property.outputdir, symmetrystr);break;
//        case 3:  sprintf(str2,"%s/xsyaza%s.txt",
//                     property.outputdir, symmetrystr);break;
//        case 4:  sprintf(str2,"%s/xayszs%s.txt",
//                     property.outputdir, symmetrystr);break;
//        case 5:  sprintf(str2,"%s/xaysza%s.txt",
//                     property.outputdir, symmetrystr);break;
//        case 6:  sprintf(str2,"%s/xayazs%s.txt",
//                     property.outputdir, symmetrystr);break;
//        case 7:  sprintf(str2,"%s/xayaza%s.txt",
//                     property.outputdir, symmetrystr);break;
//     }  
     sprintf(str2, "%s", property.outputfile[tt]);
    ofstream outfile(str2);
     outfile<<count[tt]+1<<endl;
     for(int i = 0;i<n;i++)
     {
         for(int j=0;j<pos[i].size();j++)
         {
             if(symmetry[i][j][tt*2]>=0)
                outfile<<2<<" ";
             else 
                outfile<<1<<" ";
             outfile<<i<<" "<<j;
             if(symmetry[i][j][tt*2]>=0)
                 outfile<<" "<<symmetry[i][j][tt*2]<<" "<<symmetry[i][j][tt*2+1];
             outfile<<endl;
         }
     }
     outfile.close();
     delete [] str2;
  }

  for(int i=0;i<n;i++)
  {
    for(int j=0;j<pos[i].size();j++)
    {
          symmetry[i][j].clear();
    }
    symmetry[i].clear();
  }   
  symmetry.clear();

  for(int i=0;i<n;i++)
  {  pos[i].free();
     end[i].free();
     mag[i].free();
   }

   delete [] end;
   delete [] pos;
   delete [] mag;
   delete [] str;

cerr<<"========================================================="<<endl;

  }
  else 
    test.close();

}
/*
void svSymmetry::ComputeSymmetry(SymmetryProperty &property) //deposite in small programs!!!
{
   cout<<"Symmetry processing ..."<<endl;
   char *str = new char[400];

   char *symmetrystr = new char[200];
   for(int j=0;j<200;j++) symmetrystr[j] = '\0';
   sprintf(symmetrystr, "%s(%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f)", symmetrystr,
                  property.pos[0], property.pos[1], property.pos[2],
                  property.dir[0], property.dir[1], property.dir[2]);
   sprintf(str,"%s/zanti%s.txt",
                 property.outputdir, symmetrystr);

  ifstream test(str);
  if(!test.is_open())
  {
   svIntArray * index[SYMMETRY_TYPE];
   svVector3Array *pos;
   svVector3Array *end;

   ifstream infile(property.inputfile);
   int n;
   infile>>n;
   pos = new svVector3Array[n];
   end = new svVector3Array[n];
   for(int i=0;i<SYMMETRY_TYPE;i++)
         index[i] = new svIntArray[n];

   int N = 0;
   for(int i=0;i<n;i++)
   {
       int m;
       infile>>m;
       for(int j=0;j<m;j++)
       {
         svVector3 p,d;
         svScalar m;
         infile>>p[0]>>p[1]>>p[2]
             >>d[0]>>d[1]>>d[2]>>m;
         
         svVector3 newp = CoordinateMatrix(p,
                             property.pos,
                             property.x,
                             property.y,
                             property.dir);
         svVector3 p2 = p + m * d;
         svVector3 newp2 = CoordinateMatrix(p2,
                             property.pos,
                             property.x,
                             property.y,
                             property.dir);

         pos[i].add(newp);
         end[i].add(newp2);

         for(int t=0;t<SYMMETRY_TYPE;t++)
               index[t][i].add(-1);
         N++;
       }
   }

   svVector3 newplanepos = CoordinateMatrix(property.planepos,
                             property.pos,
                             property.x,
                             property.y,
                             property.dir);
   svVector3 tmpdir = property.planepos + property.planedir;
   svVector3 newplanedir = CoordinateMatrix(property.planedir,
                             property.pos,
                             property.x,
                             property.y,
                             property.dir);
   newplanedir = normalize(newplanedir - newplanepos);
//cerr<<"========================================================="<<endl;
   vector< vector<int> > symmetry[2*SYMMETRY_TYPE];
   for(int i=0;i<2*SYMMETRY_TYPE;i++)
          symmetry[i].resize(N);

   infile.close();
   int count[SYMMETRY_TYPE];
   for(int i=0;i<SYMMETRY_TYPE;i++)
         count[i] = -1;
   for(int i=0;i<n;i++)
   {
      cerr<<i<<" "; 
      for(int j=0;j<pos[i].size();j++)
      {
        int symmetryindex[2*SYMMETRY_TYPE];
        vector<svVector3> pair;
        vector<svVector3> pairdir;
        vector<int> planeindex;

        pair.resize(SYMMETRY_TYPE);
        pairdir.resize(SYMMETRY_TYPE);

        SymmetryPair(pos[i][j], end[i][j],
                     pair[0], pairdir[0],
                     _XSYM);
        SymmetryPair(pos[i][j], end[i][j],
                     pair[1], pairdir[1],
                     _XANTI);
        SymmetryPair(pos[i][j], end[i][j],
                     pair[2], pairdir[2],
                     _YSYM);
        SymmetryPair(pos[i][j], end[i][j],
                     pair[3], pairdir[3],
                     _YANTI);
        SymmetryPair(pos[i][j], end[i][j],
                     pair[4], pairdir[4],
                     _ZSYM);
        SymmetryPair(pos[i][j], end[i][j],
                     pair[5], pairdir[5],
                     _ZANTI);

//plane pos must be the minimum one!!!!!!!!!!!!!!!!!!
        for(int t=0;t<SYMMETRY_TYPE;t++)
        {
           svScalar distance = dot(pair[t]-newplanepos, newplanedir);
           planeindex.push_back(distance/property.planedistance);
        }

        int layer = planeindex[0];
        for(int t=0;t<SYMMETRY_TYPE;t++)
        {
                symmetryindex[t*2]=-1;
                symmetryindex[t*2+1]=-1;
        }
        if(layer >=0 && layer<n)
        {
           for(int tt=0;tt<pos[layer].size();tt++)
           {

             bool flag = false;
             int tindex;

             
                if(fabs(pos[layer][tt][0]-pair[0][0])<1e-20
                && fabs(pos[layer][tt][1]-pair[0][1])<1e-20
                && fabs(pos[layer][tt][2]-pair[0][2])<1e-20
                && !(layer==i&&tt==j))
                {
                    //tindex= t;
                    flag = true;//break;
                }
             //}
             if(flag)
             {
            //    cerr<<end[layer][tt][2]<<" "<<pairdir[4][2]<<endl; 
             tindex=0;
             //   {
                   if(fabs(end[layer][tt][0]-pairdir[tindex][0])<1e-20)
                   {
                      symmetryindex[tindex*2] = layer;
                      symmetryindex[tindex*2+1]=tt;
                   }
            //    }
           tindex=1;
           //     {
                   if(fabs(pairdir[tindex][0])>0 
                      &&fabs(end[layer][tt][0]-pairdir[tindex][0])<1e-20)
                   {
                      symmetryindex[tindex*2] = layer;
                      symmetryindex[tindex*2+1]=tt;
//cerr<<endl;
//cerr<<end[i][j][0]<<" "<<end[i][j][1]<<" "<<end[i][j][2]<<" ";
//                      cerr<<i<<" "<<j<<" "<<layer<<" "<<tt<<" "<<pos[layer][tt][0]<<" "<<pos[layer][tt][1]<<" "<<pos[layer][tt][2]<<" "<<end[layer][tt][0]<<" "<<end[layer][tt][1]<<" "<<end[layer][tt][2]<<" "<<pairdir[tindex][0]<<endl;
                   }
          //      }
          tindex=2;
          //      {
                   if(fabs(end[layer][tt][1]-pairdir[tindex][1])<1e-20)
                   {
                      symmetryindex[tindex*2] = layer;
                      symmetryindex[tindex*2+1]=tt;
                   }
          //      }
          tindex=3;
          //      {
                   if(fabs(end[layer][tt][1]-pairdir[tindex][1])<1e-20
                    &&fabs(pairdir[tindex][1])>0)
                   {
                      symmetryindex[tindex*2] = layer;
                      symmetryindex[tindex*2+1]=tt;
                   }
          //      }
          tindex=4;
          //      {
                   if(fabs(end[layer][tt][2]-pairdir[tindex][2])<1e-20)
                   {
                      symmetryindex[tindex*2] = layer;
                      symmetryindex[tindex*2+1]=tt;
                   }
          //      }
          tindex=5;
          //     {
                   if(fabs(end[layer][tt][2]-pairdir[tindex][2])<1e-20
                    &&fabs(pairdir[tindex][2])>0)
                   {
                      symmetryindex[tindex*2] = layer;
                      symmetryindex[tindex*2+1]=tt;
                   }
          //      }

               break;
             } 
           }
        }
        pair.clear();
        pairdir.clear();
        planeindex.clear();
//cerr<<"========================================================="<<endl;

//======================================================================= 
        for(int t=0;t<SYMMETRY_TYPE;t++)
        {
            int value = -1;
            int index1=  symmetryindex[t*2];
            int index2=  symmetryindex[t*2+1];
       //     if(index1<0 || index2<0)continue;

            if(index1>=0 && index2>=0)
            {
              if(index[t][index1][index2]!=-1)
              {
                   value = index[t][index1][index2];
              }
              index[t][i][j] = value;
            }

            if(index[t][i][j] == -1)
            {
                 count[t]++;
                 index[t][i][j] = count[t];
            }
            bool flag = false;

            if(index1>=0&&index2>=0)
           {           
            index[t][index1][index2] = index[t][i][j];
            for(int m=0;m<symmetry[t*2][index[t][i][j]].size();m++)
            {
                  if(index1 == symmetry[t*2][index[t][i][j]][m]
                      && index2 == symmetry[2*t+1][index[t][i][j]][m])
                        {
                        //      cerr<<index1<<" "<<index2<<" "<< symmetry[t*2][index[t][i][j]][m]<<" "<<symmetry[2*t+1][index[t][i][j]][m]<<endl;
                              flag = true;
                              break;
                        }
             }                 
            if(!flag)
            {
               symmetry[t*2][index[t][i][j]].push_back(index1);
               symmetry[t*2+1][index[t][i][j]].push_back(index2);
           //    cerr<<index1<<" "<<index2<<endl;
            }
           } 
//cerr<<"flag1========================================================="<<endl;

            flag = false;
            for(int m=0;m<symmetry[t*2][index[t][i][j]].size();m++)
             {
                        if(i == symmetry[t*2][index[t][i][j]][m]
                           && j == symmetry[t*2+1][index[t][i][j]][m])
                        {
     //                         cerr<<i<<" "<<j<<" "<<symmetry[t*2][index[t][i][j]][m]<<" "<<symmetry[t*2+1][index[t][i][j]][m]<<" "<<symmetry[t*2][index[t][i][j]].size()<<endl;
                              flag = true;
                              break;
                        }
            }
//cerr<<"flag2========================================================="<<endl;
            if(!flag)
             {
//                      cerr<<t*2<<endl;
//                      cerr<<symmetry[t*2].size()<<" "<<symmetry[t*2+1].size()<<endl;
                      symmetry[t*2][index[t][i][j]].push_back(i);
                      symmetry[t*2+1][index[t][i][j]].push_back(j);
             }
//cerr<<"flag3========================================================="<<endl;

        }
      }

   }cerr<<endl;
//-----------savetofile-----------------

   for(int tt=0;tt<SYMMETRY_TYPE;tt++)
   {
     char *str2 = new char[400];
     switch(tt)
     {
        case 0:  sprintf(str2,"%s/xsym%s.txt",
                     property.outputdir, symmetrystr);break;
        case 1:  sprintf(str2,"%s/xanti%s.txt",
                     property.outputdir, symmetrystr);break;
        case 2:  sprintf(str2,"%s/ysym%s.txt",
                     property.outputdir, symmetrystr);break;
        case 3:  sprintf(str2,"%s/yanti%s.txt",
                     property.outputdir, symmetrystr);break;
        case 4:  sprintf(str2,"%s/zsym%s.txt",
                     property.outputdir, symmetrystr);break;
        case 5:  sprintf(str2,"%s/zanti%s.txt",
                     property.outputdir, symmetrystr);break;
     }  
     ofstream outfile(str2);
     outfile<<count[tt]+1<<endl;
        
     vector< vector< vector<int> > > list[2];
     list[0].resize(n);
     list[1].resize(n);

     for(int i=0;i<n;i++)
     {
          list[0][i].resize(pos[i].size());
          list[1][i].resize(pos[i].size());
//cerr<<"===================="<<pos[i].size()<<endl;
          for(int j=0;j<pos[i].size();j++)
          {//cerr<<j<<endl;
              list[0][i][j].push_back(i);
              list[1][i][j].push_back(j);
//cerr<<tt*2<<endl;
//cerr<<tt<<" "<<i<<" "<<j<<endl;
//cerr<<index[tt][i][j]<<endl;
//cerr<<symmetry[tt*2][index[tt][i][j]].size()<<endl;
              for(int t=0;t<symmetry[tt*2][index[tt][i][j]].size();t++)
              {//cerr<<t<<" ";
                     if(i == symmetry[tt*2][index[tt][i][j]][t]
                      && j == symmetry[tt*2+1][index[tt][i][j]][t])
                     {

                     }
                     else
                     {
                          list[0][i][j].push_back(symmetry[tt*2][index[tt][i][j]][t]);
                          list[1][i][j].push_back(symmetry[tt*2+1][index[tt][i][j]][t]);
                     }

              }

          }
     }
//cerr<<"================"<<endl;
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

    outfile.close();
    delete [] str2;
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

  }
   
  for(int t=0;t<SYMMETRY_TYPE;t++)
  {
   for(int i=0;i<N;i++)
   {
       symmetry[t*2][i].clear();
       symmetry[t*2+1][i].clear();
   }
   
   symmetry[t*2].clear();
   symmetry[t*2+1].clear();
  }

  for(int i=0;i<n;i++)
  {  pos[i].free();
     end[i].free();
     for(int j=0;j<SYMMETRY_TYPE;j++)
        index[j][i].free();
   }

   delete [] end;
   delete [] pos;
   for(int j=0;j<SYMMETRY_TYPE;j++)
         delete [] index[j];
   delete [] str;
  }
  else 
    test.close();

}*/
/*void svSymmetry::ComputeSymmetry(SymmetryProperty &property, SYMMETRYTYPE type)
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
cerr<<n<<endl;
//-----------savetofile-----------------
   ofstream outfile(property.outputfile);
   outfile<<count+1<<endl;
   count = 0;
   vector< vector< vector<int> > > list[2];
   list[0].resize(n);
   list[1].resize(n);
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
*/
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
svVector3 svSymmetry::CoordinateMatrix(svVector3 p,
                             svVector3 origin,
                             svVector3 x,
                             svVector3 y,
                             svVector3 z)
{
    svVector3 newp;
    newp = p - origin;
    
    svVector3Array M;
    M.add(x);
    M.add(y);
    M.add(z);

    svVector3 tmp = newp;
    newp[0] = tmp[0] * M[0][0]
            + tmp[1] * M[0][1]
            + tmp[2] * M[0][2];
    newp[1] = tmp[0] * M[1][0]
            + tmp[1] * M[1][1]
            + tmp[2] * M[1][2];
    newp[2] = tmp[0] * M[2][0]
            + tmp[1] * M[2][1]
            + tmp[2] * M[2][2];

    return newp;

}
}
