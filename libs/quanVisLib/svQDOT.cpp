#include "svQDOT.h"

namespace __svl_lib {

svQDOT::svQDOT(char*formatfName):svVectorField()
{
	New(formatfName);
}

void svQDOT::New(char *formatfName)
{
  ifstream infile(formatfName);

  infile>>plane_num;
  infile>>min_plane[0]>>min_plane[1]>>min_plane[2];
  infile>>plane_dir[0]>>plane_dir[1]>>plane_dir[2];
  infile>>plane_distance;
  infile>>xdistance;
  infile>>ydistance;
  infile>>zdistance;
  infile>>scaling;
  infile>>min_den_zero>>min_den>>max_den;
  infile>>numPower;
  infile>>lbbox[0]>>lbbox[1]>>lbbox[2];
  infile>>rbbox[0]>>rbbox[1]>>rbbox[2];
  
  infile.close();	
}


void svQDOT::SetVTK(svChar *rawdir, svChar *rawfile, svChar *dir,
                            svChar *sortFile, svChar *formatFile,
                            svChar *densityFile,
                            svVector3 ppos,
                                        svVector3 pdir, svScalar pdis)
{
  //generate sorted file and vtk file	  
  char *folderstr = new char[200];
  sprintf(folderstr, "%s/%s", dir, rawfile);
  mkdir(folderstr, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  

  char *sortfile = new char[200];
  sprintf(sortfile, "%s/%s/%s", dir, rawfile, sortFile);//sort.txt

  char *input = new char[50];
  sprintf(input, "../preprocessing/generateVTK/input");
  
  ofstream readinput(input);
  
  readinput<<ppos[0]<<" "<<ppos[1]<<" "<<ppos[2]<<endl;
  readinput<<pdir[0]<<" "<<pdir[1]<<" "<<pdir[2]<<endl;
  readinput<<pdis<<endl;
  
  readinput.close();
	    
  ifstream infile(sortfile);
  if(!infile.is_open())
  {
    char *str = new char[2048];
    sprintf(str, "./genVTK %s/%s %s/%s/%s %s/%s/ %s/%s/%s %s/%s/%s ../preprocessing/generateVTK/input", 
		  rawdir, rawfile, 
                  dir, rawfile, sortFile,
                  dir, rawfile, 
                  dir, rawfile, formatFile,
                  dir, rawfile, densityFile); 
	cerr<<"======> " << str<<endl;
    system(str);
    delete [] str;
  }
  infile.close(); 
  
  
  delete [] input;
  delete [] folderstr;
  delete [] sortfile;
}
/*
void svQDOT::SetClusterInput(svInt isRaw, svIntArray & layer, svIntArray *roi, 
				char *outfname, svScalarArray &parameters)
{
	if(Cluster_isWhole ==0)
	{
		SetClusterFilebyLayer(isRaw, layer, roi, outfname, parameters);
	}
	else
	{
		SetClusterFilebyWhole(isRaw, layer, roi, outfname, parameters);
	}
}

void svQDOT::SetClusterFilebyWhole(svInt isRaw, svIntArray & layer, svIntArray *roi, 
					char *outfname, svScalarArray &parameters)
{
        ofstream outfile(outfname);
        outfile<<1<<endl;
        outfile<<parameters[0]<<endl;   //dimension

        char * tempoutfname = new char[200];
        sprintf(tempoutfname, "%s/%s/precluster_outputtem",dataDir, dataFile);

        SetClusterFile(isRaw, layer, roi, tempoutfname, parameters); 

        ifstream infile(tempoutfname);
        int num;
        infile>>num;
        outfile<<num<<endl;
        for(int j=0;j<num;j++)
        {
              float value;
              for(int t=0;t<parameters[0];t++)
              {
                      infile>>value;
                      outfile<<value<<" ";
              }
              outfile<<endl;
        }
        infile.close();

        delete [] tempoutfname;
        

        outfile.close();
}

void svQDOT::SetClusterFilebyLayer(svInt isRaw, svIntArray & layer, svIntArray *roi, 
				char *outfname, svScalarArray &parameters)
{
	ofstream outfile(outfname);
	outfile<<layer.size()<<endl;
	outfile<<parameters[0]<<endl;	//dimension

	for(int i=0;i<layer.size();i++)
	{
		char * tempoutfname = new char[200];
		sprintf(tempoutfname, "%s/%s/precluster_outputtem",dataDir, dataFile);
		
		svIntArray tmpLayer;
		tmpLayer.add(layer[i]);

		SetClusterFile(isRaw, tmpLayer, roi, tempoutfname, parameters);		
	
		ifstream infile(tempoutfname);
		int num;
		infile>>num;
		outfile<<num<<endl;
		for(int j=0;j<num;j++)
		{
			float value;
			for(int t=0;t<parameters[0];t++)
			{
				infile>>value;
				outfile<<value<<" ";
			}
			outfile<<endl;
		}
		infile.close();

		delete [] tempoutfname;
	}

	outfile.close();
}

void svQDOT::SetClusterFile(svInt isRaw,
			svIntArray & layer, svIntArray *roi, char *outfname, svScalarArray &parameters)
{
	vector< vector<double> >  data;

	vector<double> tmp(parameters[0]);
	vector<double> mean(parameters[0]);
	vector<double> stdev(parameters[0]);

	for(int i=0;i<parameters[0];i++)
	{
		mean[i] = 0;	
		stdev[i] = 0;
	}

	for(int i=0;i<layer.size();i++)
	{
		char *infname = new char[200];
		if(isRaw)
			sprintf(infname, "%s/%s/%d.txt", dataDir, dataFile, layer[i]);
		else
			sprintf(infname, "%s/%s/contour%d.txt", dataDir, dataFile, layer[i]);

		ifstream infile(infname);
		
		int num;
		infile>>num;
		
		for(int j=0;j<num;j++)
		{		
			for(int t=0;t<parameters[0];t++)
			{			
				infile>>tmp[t];
				//cerr<<tmp[t]<<" ";
			}//cerr<<endl;
			double store;
			if(isRaw)
				infile>>store>>store>>store>>store;
			else
				infile>>store>>store;

			if(roi[layer[i]][j]==1)
			{	
				//cerr<<infname<<" "<<layer[i]<<endl;
				data.push_back(tmp);
				for(int t=0;t<parameters[0];t++)
				{
					mean[t] = mean[t] + tmp[t];
				}
			}
		}

		delete [] infname;
		infile.close();

		//cerr<<infname<<endl;
	}		

	ofstream outfile(outfname);
	outfile<<data.size()<<endl;

	if(data.size()>0)
	{
		for(int i=0;i<parameters[0];i++)
		{
			mean[i] = mean[i]/data.size();
		}		
		for(int i=0;i<data.size();i++)
		{
			for(int j=0;j<parameters[0];j++)
			{
				stdev[j] = stdev[j] + (data[i][j]-mean[j])*(data[i][j]-mean[j]);
			}
		}
		for(int i=0;i<parameters[0];i++)
		{
			stdev[i] = stdev[i]/(data.size()-1);
			if(stdev[i] == 0) stdev[i] = 1;
		}
	}
	for(int i=0;i<data.size();i++)
	{
		for(int j=0;j<parameters[0];j++)
		{
			outfile<<(data[i][j] - mean[j])/stdev[j] * parameters[j]<<" ";
		}
		outfile<<endl;
	}		
	outfile.close();

	for(int i=0;i<data.size();i++)
	{
		data[i].clear();
	}
	data.clear();
}

*/

svScalar svQDOT::GetMaxDensitybyLayer(char *densityfName, int layer)
{
	//char *densitybyLayer = new char[200];
	//sprintf(densitybyLayer, "%s/%s/density.txt",
	//		dataDir, dataFile);
	
	ifstream infile(densityfName);

	int num;
	infile>>num;

	svScalar den;

	for(int i=0;i<num;i++)
	{
		float tmp;
		infile>>tmp;
		if(i == layer)
		{
			den = (svScalar)tmp;
			break;
		}		
	}

	infile.close();
	//delete [] densitybyLayer;
	return den;
}


int svQDOT::GetPlaneIndex(svVector3 p)
{
	double dot = (p[0] - min_plane[0]) * plane_dir[0]
			+ (p[1] - min_plane[1]) * plane_dir[1]
			+ (p[2] - min_plane[2]) * plane_dir[2];
	return (int)(abs(dot)/plane_distance);
}

}
