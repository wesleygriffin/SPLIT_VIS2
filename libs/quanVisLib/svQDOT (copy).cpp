#include "svQDOT.h"

namespace __svl_lib {

/*
#ifdef __APPLE__
#define  LIB_DIR  "/Users/jian/Documents/MyProjects/SPLIT_VIS2"
#else
#define  LIB_DIR  "/home/davinci/Documents/SPLIT"
#endif
*/

int svQDOT:: getNumOfIntegerDigits(double num)
{
	  int multiplier = 0;

	  double numCopy = num;

	  if(numCopy<1&&numCopy!=0) 
	  {
	    while ((fabs(numCopy-1.)>0.000001)&&(numCopy-1.)<0.000001)
		{

		  multiplier--;//cerr<<num<<" "<<multiplier<<endl;
		  numCopy *= 10;
		}; // end while(1)
	  }
	  else if(numCopy==0)
		multiplier=0;
	  else if(numCopy>=1)
	  {
	    while(1)
		{
		  if(numCopy>=0 && numCopy <10)
		  {
		    break;
		  };
		  multiplier++;
		  numCopy /= 10.;
		}; // end while (1)
	  }; // end if(numCopy)

	  return multiplier;	
}


svQDOT::svQDOT(char *dir, char *file, char *outdir)
{
  dataSize = 0;
  contourSize = 0;
  // JC 
  // planeSize = 0;
  layerSize =0;
  dataDir = new char[200];
  dataFile = new char[200];

  // JC
  //svROI *qdotROI = new svROI();
  //svANNcluster *qdotANN;
  
  sortData(dir, file, outdir);
  setData();
  setCellRegion(1);
  setFormat();
}

void svQDOT::sortData(char *dir, char *file, char *outdir)
{
  //generate sorted file and vtk file	  
  char *folderstr = new char[200];
  sprintf(folderstr, "%s/%s", outdir, file);
  mkdir(folderstr, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  

  char *sortfile = new char[400];
  sprintf(sortfile, "%s/%s/%s_sorted", outdir, file, file);
	    
  ifstream infile(sortfile);
  if(!infile.is_open())
  {
    char *str = new char[1048];
    sprintf(str, "%s/exe/genVTK %s/%s %s/%s/sorted %s/%s/ %s/%s/format", 
		LIB_DIR, 
		  dir, file,
		  outdir, file, 
		  outdir, file, 
		  outdir, file);
cerr<<"======> " << str<<endl;
    system(str);
    delete [] str;
  }
  infile.close(); 
  
  sprintf(dataDir, "%s", outdir);
  sprintf(dataFile, "%s", file);
  
  delete [] folderstr;
  delete [] sortfile;
}

void svQDOT::setCellRegion(int index)
{
	char *out_fname = new char[400];
	sprintf(out_fname, "%s/%s/%s_region%d_surface", dataDir, dataFile, dataFile, index);
	ifstream  infile;

#ifdef _VTK
	//char *input = new char[100];
	//sprintf(input,"./dataProcessing/IsoSurface/input");
	char *in_fname = new char[400];
	sprintf(in_fname,"%s/%s/%s_region%d.vtk", dataDir, dataFile, dataFile, index);

	/*ofstream out(input);
	out<<regionIndex.size()<<endl;
	for(int i=0;i<regionIndex.size();i++)
	{
		out<<(float)regionIndex[i]+1e-8<<endl;
	}
	out.close();*/

        infile.open(out_fname);

	if(infile.is_open())
	{
		infile.close();
	}
	else
	{
//JC: obsolete TODO: need a new algo to generate the surface
		//char *exe = new char[1024];
		//sprintf(exe,"./dataProcessing/IsoSurface/Contour %s %s",
		//	in_fname, out_fname);

		//system(exe);
		//delete [] exe;
	}


	//delete [] input;
	delete [] in_fname;
	
#endif
//=========================================================================================================

	for(int i=0;i<cellPos.size();i++)
	{
		cellPos[i].clear();
		cellVel[i].clear();
		cellDen[i].clear();
		for(int ii=0;ii<cellIndex[i].size();ii++)
		{
			cellIndex[i][ii].clear();
		}
		cellIndex[i].clear();
	}
	cellPos.clear();
	cellVel.clear();
	cellDen.clear();
	cellIndex.clear();

	cellPos.resize(1);
	cellVel.resize(1);
	cellDen.resize(1);
	cellIndex.resize(1);

	infile.open(out_fname);

	if(infile.is_open())
	{
		svVector3 tmpp, tmpv;
		double tmpd;
		double t;
			int n;
			infile>>n;
			for(int j=0;j<n;j++)
			{
				infile>>tmpp[0]>>tmpp[1]>>tmpp[2]>>tmpv[0]>>tmpv[1]>>tmpv[2]>>tmpd>>t>>t;

				cellPos[0].push_back(tmpp);
				cellVel[0].push_back(tmpv);
				cellDen[0].push_back(tmpd);
				//cerr<<cellPos[0].size()<<" "<<cellVel[0].size()<<" "<<cellDen[0].size()<<endl;
			}

			infile>>n;
			for(int j=0;j<n;j++)
			{
				int m;
				infile>>m;
				vector<int> index;
				for(int ii=0;ii<m;ii++)
				{
					int in;
					infile>>in;
					index.push_back(in);
				}
				cellIndex[0].push_back(index);
				index.clear();
			}		
	}
	else
	{
		
	}

	delete [] out_fname;
}

void svQDOT::setData()
{
  if(dataSize > 0)
  {
    delete []dataCluster;
    delete [] dataPos;
    delete [] dataVel;
    delete [] dataDen;
    delete [] dataExp;
    delete [] dataCoe;
    delete [] dataDis;
    delete [] dataAtom;
    delete [] dataRegion;
    delete [] dataInbox;

    regionIndex.clear();
    
    for(int i=0;i<dataFormat.size();i++)
    {
	for(int j=0;j<dataFormat[i].size();j++)
	{
		dataFormat[i][j].clear();
	}
	dataFormat[i].clear();
    }
    dataFormat.clear();
	layer_density.clear();
  }
  //read sorted file
  
  char * str = new char[1024];
  sprintf(str, "%s/%s/sorted", dataDir, dataFile);
  
  ifstream infile(str);
  
  infile>>dataSize;
  
  dataPos = new svVector3[dataSize];
  dataVel = new svVector3[dataSize];
  dataDen = new svScalar[dataSize];
  dataExp = new svScalar[dataSize];
  dataCoe = new svScalar[dataSize];
  dataDis = new svScalar[dataSize];
  dataAtom = new svInt[dataSize];
  dataRegion = new svInt[dataSize];
  dataInbox = new bool[dataSize];
  dataCluster = new int[dataSize];
 /* dataMagcluster = new int[dataSize];
  dataVelcluster = new int[dataSize];
  layerMagcluster = new int[dataSize];
  layerVelcluster = new int[dataSize];
   //cerr<<"roiRawFlag"<<endl;
  planeRaw = new bool[dataSize];
  planeIndex = new int[dataSize];
  dataROI.resize(dataSize);*/

  min_density = 9e+20;
  min_density_nonzero = 9e+20;
  max_density = -9e+9;

  double maxe = -9e+9;
  double mine = 9e+9;
  
  for(int i=0;i<dataSize;i++)
  {
    infile>>dataPos[i][0]>>dataPos[i][1]>>dataPos[i][2]
      >>dataVel[i][0]>>dataVel[i][1]>>dataVel[i][2]
      >>dataDen[i]>>dataExp[i]>>dataCoe[i]>>dataAtom[i]>>dataRegion[i];

	bool flag = true;
	for(int ii=0;ii<regionIndex.size();ii++)
	{
		if(dataRegion[i] == regionIndex[ii])
		{
			flag = false;break;
		}
	}
	if(flag)
	{
		regionIndex.push_back(dataRegion[i]);
	}
      
      if(dataDen[i]>max_density)
      {
	max_density= dataDen[i];maxe = dataExp[i];
      }
      if(dataDen[i] < min_density)
      {
	min_density = dataDen[i];
      }
      if(dataDen[i] < min_density_nonzero && dataDen[i]>0)
      {
	min_density_nonzero = dataDen[i];mine = dataExp[i];
      }
    //  dataInbox [i] = true;
	//dataROI[i] = true;
  }
  
  infile.close();

//========================================================================
  int distance = maxe - mine+1;
  int min = (int)(log10(min_density_nonzero));
  /*for(int i=0;i<dataSize;i++)
  {
	int c;
	if(fabs(dataCoe[i])<0.001)
	{
		c = 0;
	}
	else
	{
		c = dataExp[i] - min;
	}
	//if(c == -1)
	//	cerr<<dataCoe[i]<<" "<<dataDen[i]<<" "<<mine<<" "<<min_density_nonzero<<" "<<maxe<<" "<<mine<<endl;
	//dataMagcluster[i] = c;
	//dataVelcluster[i] = 0;
  }*/
  magLevel = distance; //cerr<<magLevel<<" "<<maxe<<" "<<mine<<endl;

  delete [] str;
}

void svQDOT::setFormat()
{
 char *str = new char[1024];
 sprintf(str, "%s/%s/format", dataDir, dataFile);
  
 ifstream infile(str);
 
 double temp;
 
 infile>>zlayer;
 infile>>xdistance;
 infile>>ydistance;
 infile>>zdistance;
 infile>>scaling;
 infile>>lbbox[0]>>lbbox[1]>>lbbox[2];
 infile>>rbbox[0]>>rbbox[1]>>rbbox[2];

// cerr<<lb[0]<<" "<<lb[1]<<" "<<lb[2]<<" "<<scaling<<endl;
 
 xl = rbbox[0] - lbbox[0];
 yl = rbbox[1] - lbbox[1];
 zmin = lbbox[2];
 
 //hard coding!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 xnum = 11;//(int)sqrt(zlayer);
 ynum = zlayer/xnum+1;
 
 infile>>temp>>temp>>temp>>temp>>temp;

 int n;
 infile>>n;
 //rawFormat.resize(n);
 //cerr<<n<<endl;

 int count = 0;

 for(int i=0;i<n;i++)
 {
        infile>>temp>>temp>>temp>>temp>>yd; 
	vector< vector<int> > tmpyformat;
	int tn;
	infile>>tn;	
	//cerr<<i<<" "<<tn<<endl;
	//tmpyformat.resize(tn);
	for(int j=0;j<tn;j++)
	{
		vector<int> tmpxformat;
		int s, e;

   		infile>>temp>>s>>temp>>e>>xd;

			//if(i==0)
			//cerr<<s<<" "<<e<<endl;

		for(int k=s;k<=e;k++)
		{
			tmpxformat.push_back(k);
			count ++;
			//if(i==0&&j==0)
			//cerr<<k<<" ";
		}
	
		tmpyformat.push_back(tmpxformat);

		tmpxformat.clear();
	}
	dataFormat.push_back(tmpyformat);

	for(int j=0;j<tmpyformat.size();j++)
	{
		tmpyformat[j].clear();
	}
	tmpyformat.clear();
 }

 
 infile.close();
 
 layer_density.resize(zlayer);

 for(int i =0;i<zlayer;i++)
 {
	layer_density[i] = -9e+9;
  }

 for(int ii=0;ii<zlayer;ii++)
 {
	for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
	{
		if(dataDen[i] > layer_density[ii]) layer_density[ii] = dataDen[i];
	}
  }
 for(int i =0;i<zlayer;i++)
 {
	cerr<<layer_density[i]<<endl;
  }
 //contourLayerden = new double[zlayer];
 //range_layer_mag_cluster.resize(zlayer);
 
 delete [] str; 
}

void svQDOT::layerClustering(int *numCluster, double *mag1, double *mag2, 
				double *ws, double *wv, double *wd,
				vector<bool> selected_plane, double plane_d) //multiple layers
{
	int n = layerPos.size();
	double minmag, maxmag;

	for(int ii = 0; ii<n;ii++)
	{	
		for(int i=0;i<layerPos[ii].size();i++)
		{
			layerCluster[ii][i] = -1;
		}
	}

	  for(int t = 0; t<2; t ++) //two steps
	  {
		if(t == 0)
		{
			minmag = mag1[0];
			maxmag = mag1[1];
		}
		else
		{
			minmag = mag2[0];
			maxmag = mag2[1];
		}

			char *newfile = new char[400];
			char *outfile = new char[400];
			double mean[7];
			double std[7];
			for(int i=0;i<7;i++)
			{
				mean[i] = 0;
				std[i] = 0;
			}
			int count = 0;

		//cout<<minl<<" "<<maxl<<" "<<layerPos[maxl].size()<<endl;
	//cerr<<minmag<<" "<<maxmag<<endl;

		    for(int ii = 0;ii<n;ii++)
		    {
			if(selected_plane[ii])
			{
				for(int i=0;i<layerPos[ii].size();i++)
				{
					if((layerDen[ii][i] > minmag
					 && layerDen[ii][i] < maxmag)
					|| (fabs(layerDen[ii][i]-minmag)<1e-20)
					|| (fabs(layerDen[ii][i]-maxmag)<1e-20))
					{
						mean[0] = mean[0] + layerPos[ii][i][0];
						mean[1] = mean[1] + layerPos[ii][i][1];
						mean[2] = mean[2] + layerVel[ii][i][0];
						mean[3] = mean[3] + layerVel[ii][i][1];
						mean[4] = mean[4] + layerVel[ii][i][2];
						mean[5] = mean[5] + layerDen[ii][i];
						mean[6] = mean[6] + layerPos[ii][i][2];
						count++;		//cout<<layerPos[ii][i][0]<<" "<<layerPos[ii][i][1]<<" "<<layerPos[ii][i][2]<<endl;
					}
				}
			}
		     }
		//cout<<minl<<" "<<maxl<<" "<<mean[0]<<endl;
			if(count > 0)
			{
				for(int i=0;i<7;i++)
				{
					mean[i] = mean[i]/count;			
					std[i] = 0;
					//cerr<<mean[i]<<" ";
				}//cerr<<endl;

			    	for(int ii = 0;ii<n;ii++)
			    	{
					if(selected_plane[ii])
					{
						for(int i=0;i<layerPos[ii].size();i++)
						{
							if((layerDen[ii][i] > minmag
							 && layerDen[ii][i] < maxmag)
							|| (fabs(layerDen[ii][i]-minmag)<1e-20)
							|| (fabs(layerDen[ii][i]-maxmag)<1e-20))
							{
								std[0] = std[0] + (layerPos[ii][i][0] - mean[0]) * (layerPos[ii][i][0] - mean[0]);
								std[1] = std[1] + (layerPos[ii][i][1] - mean[1]) * (layerPos[ii][i][1] - mean[1]);
								std[2] = std[2] + (layerVel[ii][i][0] - mean[2]) * (layerVel[ii][i][0] - mean[2]);
								std[3] = std[3] + (layerVel[ii][i][1] - mean[3]) * (layerVel[ii][i][1] - mean[3]);
								std[4] = std[4] + (layerVel[ii][i][2] - mean[4]) * (layerVel[ii][i][2] - mean[4]);
								std[5] = std[5] + (layerDen[ii][i] - mean[5]) * (layerDen[ii][i] - mean[5]);
								std[6] = std[6] + (layerPos[ii][i][2] - mean[6]) * (layerPos[ii][i][2] - mean[6]);
							}
						}
					}
			     }

				for(int i=0;i<7;i++)
				{				//cout<<mean[i]<<" "<<std[i]<<" "<<ws[t]<<endl;
					std[i] = sqrt(std[i]/(count-1));
					//cerr<<std[i]<<" "<<count<<endl;
					if(std[i]==0)std[i] =1;
				}


				
				sprintf(newfile, "%s/%s/%s_layer_tmp.txt", 
							   dataDir, 
							   dataFile, 
							   dataFile);	
				//cout<<newfile<<endl;

				ofstream output(newfile);
			    	for(int ii = 0;ii<n;ii++)
			    	{
					if(selected_plane[ii])
					{
						for(int i=0;i<layerPos[ii].size();i++)
						{
							if((layerDen[ii][i] > minmag
							 && layerDen[ii][i] < maxmag)
							|| (fabs(layerDen[ii][i]-minmag)<1e-20)
							|| (fabs(layerDen[ii][i]-maxmag)<1e-20))
							{
						   		output<<  (layerPos[ii][i][0]-mean[0])/std[0]*ws[t]<<" "
									<<(layerPos[ii][i][1]-mean[1])/std[1]*ws[t]<<" "
									<<(layerPos[ii][i][2]-mean[6])/std[6]*ws[t]<<" "
							  		<<(layerVel[ii][i][0]-mean[2])/std[2]*wv[t]<<" "
									<<(layerVel[ii][i][1]-mean[3])/std[3]*wv[t]<<" "
									<<(layerVel[ii][i][2]-mean[4])/std[4]*wv[t]<<" "
							  		<<(layerDen[ii][i]-mean[5])/std[5]*wd[t]<<endl;
							}
						}
					}
			     	}

				output.close();

				if(count >= numCluster[t] && numCluster[t]>0)
				{
					sprintf(outfile, "%s/%s/%s_layer_kmeans_tmp_wohc.txt", 
								dataDir, 
								dataFile, 
								dataFile);

					char *exe = new char[900];
					sprintf(exe, "%s/libs/kmlocal-1.7.2/bin/kmlsample -d 7 -k %d -max %d -df %s > %s 2>&1",  LIB_DIR,
							numCluster[t], dataSize, newfile, outfile);
					system(exe);
					cerr<<exe<<endl;
					delete [] exe;

					cerr<<"==============================store======================================="<<endl;
					ifstream in(outfile);
				    	for(int ii = 0;ii<n;ii++)
				    	{
						if(selected_plane[ii])
						{
							for(int i=0;i<layerPos[ii].size();i++)
							{
								if((layerDen[ii][i] > minmag
								 && layerDen[ii][i] < maxmag)
								|| (fabs(layerDen[ii][i]-minmag)<1e-20)
								|| (fabs(layerDen[ii][i]-maxmag)<1e-20))
								{
									int iii;
									in>>iii;//layerVelcluster[i];//layerVelcluster[i]=layerVelcluster[i];
									if(t == 0)
										layerCluster[ii][i] = iii;
									else
										layerCluster[ii][i] = iii + numCluster[0];
								}
							}
						}
					}
					in.close();
				}
			}
		//cerr<<t<<endl;
		delete [] newfile;
		delete [] outfile;
       }

	for(int i=0;i<dataSize;i++)
	{	
		dataCluster[i] = -1;
	}
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			dataCluster[layerIndex[i][j]] = layerCluster[i][j];
		}
	}
	//delete [] c;
}
void svQDOT::layerContourClustering(double *mag1, double *mag2, 
				double *ws, double *wv, double *wd, vector<bool> selected_plane) //multiple layers
{
	//double mean[7];
	//double std[7];
	double mean2[7];
	double std2[7];
	for(int j=0;j<7;j++)
	{
		//mean[j] = 0;
		//std[j] = 0;
		mean2[j] = 0;
		std2[j] = 0;
	}
	int count1 = layerContourSize;
	int count2 = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		if(selected_plane[i])
		{
			for(int j=0;j<layerPos[i].size();j++)
			{
				mean2[0] = mean2[0]+layerPos[i][j][0];
				mean2[1] = mean2[1]+layerPos[i][j][1];
				mean2[2] = mean2[2]+layerPos[i][j][2];
				mean2[3] = mean2[3]+layerVel[i][j][0];
				mean2[4] = mean2[4]+layerVel[i][j][1];
				mean2[5] = mean2[5]+layerVel[i][j][2];
				mean2[6] = mean2[6]+layerDen[i][j];
				count2++;
			}
		}
	}
	for(int i=0;i<7;i++)
	{
		//if(count1 > 0)
		//	mean[i] = mean[i]/count1;
		if(count2 > 0)
			mean2[i] = mean2[i]/count2;
	}
	for(int i=0;i<layerPos.size();i++)
	{
		if(selected_plane[i])
		{
			for(int j=0;j<layerPos[i].size();j++)
			{
				std2[0] = std2[0]+ (layerPos[i][j][0]-mean2[0])*(layerPos[i][j][0]-mean2[0]);
				std2[1] = std2[1]+ (layerPos[i][j][1]-mean2[1])*(layerPos[i][j][1]-mean2[1]);
				std2[2] = std2[2]+ (layerPos[i][j][2]-mean2[2])*(layerPos[i][j][2]-mean2[2]);
				std2[3] = std2[3]+ (layerVel[i][j][0]-mean2[3])*(layerVel[i][j][0]-mean2[3]);
				std2[4] = std2[4]+ (layerVel[i][j][1]-mean2[4])*(layerVel[i][j][1]-mean2[4]);
				std2[5] = std2[5]+ (layerVel[i][j][2]-mean2[5])*(layerVel[i][j][2]-mean2[5]);
				std2[6] = std2[6]+ (layerDen[i][j]-mean2[6])*(layerDen[i][j]-mean2[6]);
			}

		}
	}
	for(int i=0;i<7;i++)
	{
		/*if(count1>0)
		{
			std[i] = sqrt(std[i]/(count1-1));
			if(std[i] == 0) std[i] = 1;
		}*/
		if(count2>0)
		{
			std2[i] = sqrt(std2[i]/(count2-1));
			if(std2[i] == 0) std2[i] = 1;
		}
		//cerr<<std2[i]<<" ";
	}
//cerr<<count1<<" "<<count2<<endl;
cerr<<"=========================================="<<endl;
	if(count2>0)
	{
		//raw data
		svVector3 * p = new svVector3[count2];
		svVector3 * v = new svVector3[count2];
		svScalar * d = new svScalar[count2];

		//contour data
		svVector3 * cp = new svVector3[count1];
		svVector3 * cv = new svVector3[count1];
		svScalar * cd = new svScalar[count1];

		int *c1 = new int[count1];
		int *c2 = new int[count2];

		count1=0;count2=0;
		for(int i=0;i<layerContourPos.size();i++)
		{
			for(int j=0;j<layerContourPos[i].size();j++)
			{
					cp[count1][0] = (layerContourPos[i][j][0]-mean2[0])/std2[0] * ws[0];
					cp[count1][1] = (layerContourPos[i][j][1]-mean2[1])/std2[1] * ws[0];
					cp[count1][2] = (layerContourPos[i][j][2]-mean2[2])/std2[2] * ws[0];
					cv[count1][0] = (layerContourVel[i][j][0]-mean2[3])/std2[3] * wv[0];
					cv[count1][1] = (layerContourVel[i][j][1]-mean2[4])/std2[4] * wv[0];
					cv[count1][2] = (layerContourVel[i][j][2]-mean2[5])/std2[5] * wv[0];
					cd[count1] = (layerContourDen[i][j]-mean2[6])/std2[6] * wd[0];
					count1++;
			}
		}
		for(int i=0;i<layerPos.size();i++)
		{
			if(selected_plane[i])
			{
				/*for(int j=0;j<layerContourPos[i].size();j++)
				{
					cp[count1][0] = (layerContourPos[i][j][0]-mean2[0])/std2[0] * ws[0];
					cp[count1][1] = (layerContourPos[i][j][1]-mean2[1])/std2[1] * ws[0];
					cp[count1][2] = (layerContourPos[i][j][2]-mean2[2])/std2[2] * ws[0];
					cv[count1][0] = (layerContourVel[i][j][0]-mean2[3])/std2[3] * wv[0];
					cv[count1][1] = (layerContourVel[i][j][1]-mean2[4])/std2[4] * wv[0];
					cv[count1][2] = (layerContourVel[i][j][2]-mean2[5])/std2[5] * wv[0];
					cd[count1] = (layerContourDen[i][j]-mean2[6])/std2[6] * wd[0];
					count1++;
				}*/
				for(int j=0;j<layerPos[i].size();j++)
				{
					p[count2][0] = (layerPos[i][j][0]-mean2[0])/std2[0] * ws[0];
					p[count2][1] = (layerPos[i][j][1]-mean2[1])/std2[1] * ws[0];
					p[count2][2] = (layerPos[i][j][2]-mean2[2])/std2[2] * ws[0];
					v[count2][0] = (layerVel[i][j][0]-mean2[3])/std2[3] * wv[0];
					v[count2][1] = (layerVel[i][j][1]-mean2[4])/std2[4] * wv[0];
					v[count2][2] = (layerVel[i][j][2]-mean2[5])/std2[5] * wv[0];
					d[count2] = (layerDen[i][j]-mean2[6])/std2[6] * wd[0];
					c2[count2] = layerCluster[i][j];
					//cerr<<p[count2][0] <<" "<<std2[0]<<" "<<ws[0]<<endl;
					count2++;
				}

			}
		}
cerr<<"=========================================="<<endl;
// JC
//		qdotANN->ANN_Cluster_IDX(p,v,d,count2,cp,cv,cd,count1,c2,c1);//raw, contour, raw, contour
cerr<<"=========================================="<<endl;
		count1 = 0;
		for(int i=0;i<layerPos.size();i++)
		{
			for(int j=0;j<layerContourPos[i].size();j++)
			{
				layerContourCluster[i][j] = -1;
			}
		}
cerr<<"=========================================="<<endl;
		for(int i=0;i<layerContourPos.size();i++)
		{
			//if(selected_plane[i])
			//{
				for(int j=0;j<layerContourPos[i].size();j++)
				{
					//layerContourCluster[i][j] = -1;
					if((layerContourDen[i][j] > mag1[0] && layerContourDen[i][j]<mag1[1])
					|| fabs(layerContourDen[i][j]-mag1[0])<1e-18
					|| fabs(layerContourDen[i][j]-mag1[1])<1e-18 )
					{
						layerContourCluster[i][j] = c1[count1];
					}
					count1++;
				}
			//}
		}
cerr<<"=========================================="<<endl;

		delete [] p;
		delete [] v;
		delete [] d;
		delete [] cp;
		delete [] cv;
		delete [] cd;
		delete [] c1;
		delete [] c2;
//=================================================================================================================================
		/*count1=0;count2=0;

		for(int i=0;i<layerPos.size();i++)
		{
			if(selected_plane[i])
			{
				for(int j=0;j<layerContourPos[i].size();j++)
				{
					cp[count1][0] = layerContourPos[i][j][0]/std2[0] * ws[1];
					cp[count1][1] = layerContourPos[i][j][1]/std2[1] * ws[1];
					cp[count1][2] = layerContourPos[i][j][2]/std2[2] * ws[1];
					cv[count1][0] = layerContourVel[i][j][0]/std2[3] * wv[1];
					cv[count1][1] = layerContourVel[i][j][1]/std2[4] * wv[1];
					cv[count1][2] = layerContourVel[i][j][2]/std2[5] * wv[1];
					cd[count1] = layerContourDen[i][j]/std2[6] * wd[1];
					count1++;
				}
				for(int j=0;j<layerPos[i].size();j++)
				{
					p[count2][0] = layerPos[i][j][0]/std2[0] * ws[1];
					p[count2][1] = layerPos[i][j][1]/std2[1] * ws[1];
					p[count2][2] = layerPos[i][j][2]/std2[2] * ws[1];
					v[count2][0] = layerVel[i][j][0]/std2[3] * wv[1];
					v[count2][1] = layerVel[i][j][1]/std2[4] * wv[1];
					v[count2][2] = layerVel[i][j][2]/std2[5] * wv[1];
					d[count2] = layerDen[i][j]/std2[6] * wd[1];
					c2[count2] = layerCluster[i][j];
					count2++;
				}

			}
		}
cerr<<"=========================================="<<endl;
// JC
		//qdotANN->ANN_Cluster_IDX(p,v,d,count2,cp,cv,cd,count1,c2,c1);//need to be implemented
cerr<<"=========================================="<<endl;
		count1 = 0;
		for(int i=0;i<layerPos.size();i++)
		{
			if(selected_plane[i])
			{
				for(int j=0;j<layerContourPos[i].size();j++)
				{
					if(layerContourDen[i][j] >= mag2[0] && layerContourDen[i][j]<=mag2[1])
					{
						layerContourCluster[i][j] = c1[count1];
					}
					count1++;
				}
			}
		}*/

	}

}
/*
void svQDOT::layerClustering(int *numCluster, 
				double *mag, 
				double *ws, double *wv, double *wd, bool hc)
{
//==============================weighted files=======================================
    for(int ii = 0; ii<zlayer;ii++)
    {
	char *file = new char[400];
	sprintf(file, "%s/%s/%s_layer%d_sorted.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii);

	char *newfile = new char[400];
	sprintf(newfile, "%s/%s/%s_layer%d_weight_s%0.2f_v%0.2f_d%0.2f.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii,
					ws[0],wv[0],wd[0]);

	ifstream inin(newfile);
	if(!inin.is_open())
	{
		ifstream input(file);
		ofstream output(newfile);
		double p[6];
		while((input>>p[0]>>p[1]>>p[2]>>p[3]>>p[4]>>p[5]))
		{
			p[0] = ws[0] * p[0];
			p[1] = ws[0] * p[1];
			p[2] = wv[0] * p[2];
			p[3] = wv[0] * p[3];
			p[4] = wv[0] * p[4];
			p[5] = wd[0] * p[5];
			output<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3]<<" "<<p[4]<<" "<<p[5]<<endl;
		}
		input.close();
		output.close();
	}
	else
	{
		inin.close();
	}

//==============================the first step=======================================
	char *outfile = new char[400];
	if(!hc)
		sprintf(outfile, "%s/%s/%s_layer%d_kmeans%d_s%0.2f_v%0.2f_d%0.2f_wohc.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii, numCluster[0],
				    ws[0],wv[0],wd[0]
			  	    );
	else
		sprintf(outfile, "%s/%s/%s_layer%d_kmeans%d_s%0.2f_v%0.2f_d%0.2f_whc.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii, numCluster[0],
				    ws[0],wv[0],wd[0]
			  	    );

	ifstream infile(outfile);
	if(!infile.is_open())
	{
		char *hcfile = new char[400];
		sprintf(hcfile, "%s/%s/%s_layer%d_HCcenter%d_l%0.2f_v%0.2f_d%0.2f.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii, numCluster[0],
				    ws[0],wv[0],wd[0]
			  	    );
		char *exe = new char[1024];
		if(!hc)
			sprintf(exe, "./dataProcessing/kmlocal-1.7.2/bin/kmlsample -d 6 -k %d -max %d -df %s > %s 2>&1",
				numCluster[0], dataSize, newfile, outfile);
		else
			sprintf(exe, "./dataProcessing/kmlocal-1.7.2/bin/kmlsample -d 6 -k %d -max %d -df %s -o %s> %s 2>&1",
				numCluster[0], dataSize, newfile, hcfile, outfile);
		system(exe);
		cerr<<exe<<endl;
		delete [] exe;
		delete [] hcfile;
	}
	else
	{
		infile.close();
	}

    	ifstream in(outfile);
	for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
	{
		in>>layerVelcluster[i];//layerVelcluster[i]=layerVelcluster[i];
	}
	in.close();	


//==============================the second step=======================================

	if(mag!=NULL && numCluster[1]>1)
	{
		double mean[6];
		double std[6];
		double sum[6];
		for(int i=0;i<6;i++)
		{
			mean[i] = 0;
			std[i] = 0;
			sum[i] = 0;
		}
		int count = 0;
		vector<int> cluster; //detect how many clusters are still there
	//==============================standarization=======================================
		for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
		{
			if(dataDen[i] >= mag[0]
			&& dataDen[i] <= mag[1])
			{
				mean[0] = mean[0] + dataPos[i][0];
				mean[1] = mean[1] + dataPos[i][1];
				mean[2] = mean[2] + dataVel[i][0];
				mean[3] = mean[3] + dataVel[i][1];
				mean[4] = mean[4] + dataVel[i][2];
				mean[5] = mean[5] + dataDen[i];
				count ++;
			}
			else
			{
				bool flag = true;
				for(int j=0;j<cluster.size();j++)
				{
					if(layerVelcluster[i] == cluster[j])
					{	
						flag =false;
						break;
					}
				}
				if(flag) cluster.push_back(layerVelcluster[i]);
			}
		}
		if(count > 0)
		{
			for(int i=0;i<6;i++)
			{
				mean[i] = mean[i]/count;
			}
		
			for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
			{
				if(dataDen[i] >= mag[0]
				&& dataDen[i] <= mag[1])
				{
					std[0] = std[0] + (dataPos[i][0] - mean[0]) * (dataPos[i][0] - mean[0]);
					std[1] = std[1] + (dataPos[i][1] - mean[1]) * (dataPos[i][1] - mean[1]);
					std[2] = std[2] + (dataVel[i][0] - mean[2]) * (dataVel[i][0] - mean[2]);
					std[3] = std[3] + (dataVel[i][1] - mean[3]) * (dataVel[i][1] - mean[3]);
					std[4] = std[4] + (dataVel[i][2] - mean[4]) * (dataVel[i][2] - mean[4]);
					std[5] = std[5] + (dataDen[i] - mean[5]) * (dataDen[i] - mean[5]);
				}
			}
			if(count > 0)
			{
				for(int i=0;i<6;i++)
				{
					std[i] = sqrt(std[i]/(count-1));
				}
			}	
			sprintf(newfile, "%s/%s/%s_layer%d_tmp.txt", 
					    dataDir, 
					    dataFile, 
					    dataFile,ii);	

			ofstream output(newfile);
			for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
			{
				if(dataDen[i] >= mag[0]
				&& dataDen[i] <= mag[1])
				{
		   			output<<(dataPos[i][0]-mean[0])/std[0]*ws[1]<<" "<<(dataPos[i][1]-mean[1])/std[1]*ws[1]<<" "
			  			<<(dataVel[i][0]-mean[2])/std[2]*wv[1]<<" "<<(dataVel[i][1]-mean[3])/std[3]*wv[1]<<" "
						<<(dataVel[i][2]-mean[4])/std[4]*wv[1]<<" "
			  			<<(dataDen[i]-mean[5])/std[5]*wd[1]<<endl;
				}
			}
			output.close();

		//==============================execution=======================================
			if(!hc)
				sprintf(outfile, "%s/%s/%s_layer%d_kmeans_tmp_wohc.txt", 
						    dataDir, 
						    dataFile, 
						    dataFile,ii);
			else
				sprintf(outfile, "%s/%s/%s_layer%d_kmeans_tmp_whc.txt", 
						    dataDir, 
						    dataFile, 
						    dataFile,ii);

			//ifstream infile(outfile);
			//if(!infile.is_open())
			//{
				char *exe = new char[900];
				sprintf(exe, "./dataProcessing/kmlocal-1.7.2/bin/kmlsample -d 6 -k %d -max %d -df %s > %s 2>&1",
					numCluster[1], dataSize, newfile, outfile);
				system(exe);
				cerr<<exe<<endl;
				delete [] exe;
			//}
			//else
			//{
			//	infile.close();
			//}

		//==============================store=======================================
			int *cluster_array = new int[numCluster[1]];
			int count2 = 0;
			for(int i=0;i<dataSize;i++)
			{
				bool flag = true;
				for(int j=0;j<cluster.size();j++)
				{
					if(i == cluster[j])
					{
						flag = false;
						break;
					}
				}
				if(flag)
				{
					cluster_array[count2] = i;
					count2 ++;
					if(count2 == numCluster[1])
						break;
				}
			}
		    	ifstream in(outfile);
			for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
			{
				if(dataDen[i] >= mag[0]
				&& dataDen[i] <= mag[1])
				{
					int iii;
					in>>iii;//layerVelcluster[i];//layerVelcluster[i]=layerVelcluster[i];
					layerVelcluster[i] = cluster_array[iii];
				}
			}
			in.close();
			delete [] cluster_array;
		}
	}

	delete [] newfile;
	delete [] file;
	delete [] outfile;
   }

}*/
/*
void svQDOT::layerClustering_stepone(int numCluster)//, int velCluster, vector<int> index)//hierarchical clustering
{
    for(int ii = 0; ii<zlayer;ii++)
    {
	range_layer_mag_cluster[ii].resize(numCluster);
	for(int i=0;i<numCluster;i++)
	{
		range_layer_mag_cluster[ii][i].push_back(9e+9);
		range_layer_mag_cluster[ii][i].push_back(-9e+9);
	}

	char *file = new char[400];
	sprintf(file, "%s/%s/%s_layer%d_sorted_ld.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii);

	char *outfile = new char[400];
	sprintf(outfile, "%s/%s/%s_layer%d_kmeans%d_ld.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii, numCluster);

//==============================the first step=======================================
	//ifstream infile(outfile);
	//if(!infile.is_open())
	//{
		char *exe = new char[900];
		sprintf(exe, "./dataProcessing/kmlocal-1.7.2/bin/kmlsample -d 3 -k %d -max %d -df %s > %s 2>&1",
			numCluster, dataSize, file, outfile);
		system(exe);
		//cerr<<exe<<endl;
		delete [] exe;
	//}
	//else
	//{
	//	infile.close();
	//}

	ifstream in(outfile);
	for(int j=dataFormat[ii][0][0];j<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];j++)
	{
		in>>layerMagcluster[j];

		if(dataDen[j] < range_layer_mag_cluster[ii][layerMagcluster[j]][0])
		{
			range_layer_mag_cluster[ii][layerMagcluster[j]][0] = dataDen[j];
		}
		if(dataDen[j] > range_layer_mag_cluster[ii][layerMagcluster[j]][1])
		{
			range_layer_mag_cluster[ii][layerMagcluster[j]][1] = dataDen[j];
		}

		layerVelcluster[j] = -1;
	}
	in.close();

	delete [] file;
	delete [] outfile;
   }
}
*/
/*
void svQDOT::layerClustering_steptwo(int numCluster, int velCluster, vector<int> index)//hierarchical clustering
{
//===========================the second step======================================
    for(int ii = 0; ii<zlayer;ii++)
    {
	char *outfile = new char[400];
	sprintf(outfile, "%s/%s/%s_layer%d_kmeans%d_ld.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii, numCluster);

	vector<int> tmpcluster;	
	ifstream infile(outfile);
	for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
	{
		int t;
		infile>>t;
		tmpcluster.push_back(t);
	}
	infile.close();

	for(int i=0;i<index.size();i++)
	{
		char *tmpout = new char[400];
		sprintf(tmpout, "%s/%s/%s_tmpclustering",
				    dataDir, 
				    dataFile, 
				    dataFile);
		ofstream outfile(tmpout);
		for(int j=dataFormat[ii][0][0];j<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];j++)
		{
			if(tmpcluster[j-dataFormat[ii][0][0]] == index[i])
			{
				outfile<<dataVel[j][0]<<" "<<dataVel[j][1]<<" "<<dataVel[j][2]<<endl;
			}
		}		
		outfile.close();

		char *tmpout2 = new char[400];
		sprintf(tmpout2, "%s/%s/%s_tmpclustering_vel",
				    dataDir, 
				    dataFile, 
				    dataFile);

		char *exe = new char[900];
		sprintf(exe, "./dataProcessing/kmlocal-1.7.2/bin/kmlsample -d 3 -k %d -max %d -df %s > %s 2>&1",
			velCluster, dataSize, tmpout, tmpout2);
		system(exe);

		cerr<<exe<<endl;

	    	ifstream in(tmpout2);
		for(int j=dataFormat[ii][0][0];j<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];j++)
		{
			if(tmpcluster[j-dataFormat[ii][0][0]] == index[i])
			{			
				in>>layerVelcluster[j];//layerVelcluster[i]=layerVelcluster[i];
			}
			else
			{
				layerVelcluster[j] = -1;
			}
		}
		in.close();	

		delete [] exe;
		delete [] tmpout;
		delete [] tmpout2;
	}

	tmpcluster.clear();
//================================================================================
	delete [] outfile;
   }
}
void svQDOT::layerMatlabClustering(int numCluster)
{
//======================================================================================
    for(int ii = 0; ii<zlayer;ii++)
    {
	char *outfile = new char[400];
	sprintf(outfile, "%s/%s/%s_layer%d_matlabkmeans%d.txt", 
				    dataDir, 
				    dataFile, 
				    dataFile,ii, numCluster);
	ifstream infile(outfile);
	if(infile.is_open())
	{
		    ifstream in(outfile);
		   // int nn;
		   // in>>nn;
		    for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
		    {
				in>>layerVelcluster[i];//layerVelcluster[i]=layerVelcluster[i];
		
		    }
		    in.close();
		infile.close();
	}
	else
	{
		    for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
		    {
				layerVelcluster[i]=0;
		
		    }
	}

	delete [] outfile;
   }
}*/
/*
int svQDOT::layerClustering(bool toggleROI, bool toggleAttribute, int numCluster, 
				char *file, bool *in, vector< vector< vector<int> > > layer,
				vector<svVector3> pv, vector<svVector3> pn,
				int *outcluster, bool clustering)
{

  if(layer.size() != zlayer) return 0;

//==============================================

  char *infile1 = new char[400];
  char *infile2 = new char[400];
  char *outfile1 = new char[400];
  char *outfile2 = new char[400];
  char *index_file1 = new char[400];
  char *index_file2 = new char[400];
  char *roifile1 = new char[400];
  char *roifile2 = new char[400];
  char *rangefile = new char[400];
  char *rangefile2 = new char[400];
  char *roirange1 = new char[400];
  char *roirange2 = new char[400];
  char *roiout1 = new char[400];
  char *roiout2 = new char[400];
  char *type = new char[10];
  char *folder = new char[50];
  
  if(!toggleAttribute)
  {
    sprintf(type, "mag");
	sprintf(folder,"");
  }
  else
  {
    sprintf(type, "vel");
	if(clustering == 0) sprintf(folder, "kmeans");
	else if(clustering ==1) sprintf(folder, "em");
  }
  
  bool sWholefield = false;	
			
  if(file != NULL)
  {
    for(int ii = 0; ii<zlayer;ii++)
    {
	    int count1 = 0;
	    int count2 = 0;
	    for(int i=layer[ii][0][0];i<=layer[ii][layer[ii].size()-1][layer[ii][layer[ii].size()-1].size()-1];i++)
	    {
	     if(in[i]) count1++; 
	    }
	    for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
	    {
	     if(dataInbox[i]) count2++; 
	    }
	    if(count1 == layer[ii][layer[ii].size()-1][layer[ii][layer[ii].size()-1].size()-1] - layer[ii][0][0] + 1
		&& count2 == dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1] - dataFormat[ii][0][0] + 1)
	    {
	      sWholefield = true;
	    }
	    
	    char *str = new char[100];
	    char *str2 = new char[100];
	    for(int i=0;i<100;i++)
	    {
	      str[i] = '\0';
	      str2[i] = '\0';
	    }
	    for(int i=0;i<symmetryV.size();i++)
	    {
	      sprintf(str, "%s[%0.2f%0.2f%0.2f,%0.2f%0.2f%0.2f]", 
				    str, 
				    symmetryV[i][0], symmetryV[i][1], symmetryV[i][2],
				    symmetryN[i][0], symmetryN[i][1], symmetryN[i][2]);
	    }
	    for(int i=0;i<pv.size();i++)
	    {
	      sprintf(str2, "%s[%0.2f%0.2f%0.2f,%0.2f%0.2f%0.2f]", 
				    str2, 
				    pv[i][0], pv[i][1], pv[i][2],
				    pn[i][0], pn[i][1], pn[i][2]);	      
	    }    
	    
	    sprintf(infile1, "%s/%s/%s_layer%d_sorted", dataDir, dataFile, dataFile, ii);
	    sprintf(infile2, "%s/%s/%s_layer%d_sorted", dataDir, file, file, ii);
	    sprintf(roifile1, "%s/%s/%s_layer%d_roiindex", dataDir, dataFile, dataFile,ii);
	    sprintf(roifile2, "%s/%s/%s_layer%d_roiindex", dataDir, file, file,ii);    
	    if(sWholefield || !toggleROI)
	    {
	     sprintf(index_file1, "%s/%s/%s_layer%d_symmetry_%s", 
				   dataDir, dataFile, dataFile, ii, str);
	     sprintf(index_file2, "%s/%s/%s_layer%d_symmetry_%s", 
				   dataDir, file, file, ii, str2);
	     if(strcmp(infile1, infile2))
	     {
		     sprintf(outfile1, "%s/%s/%s/%s_%s_%s_%slayer%d_%s%d.txt", 
				    dataDir, 
				    dataFile, 
					folder,
				    dataFile,str,
				    file, str2,
				    ii,
				    type,
				    numCluster);
		     sprintf(outfile2, "%s/%s/%s/%s_%s_%s_%slayer%d_%s%d.txt", 
				    dataDir, 
				    file, folder,
				    file, str2,
				    dataFile, str,ii,
				    type,
				    numCluster);
	     }
	     else 
	     {
		     sprintf(outfile1, "%s/%s/%s/%s_%slayer%d_%s%d.txt", 
				    dataDir, 
				    dataFile, folder,
				    dataFile,str,ii,
				    type,
				    numCluster);
		     sprintf(outfile2, "%s/%s/%s/%s_%slayer%d_%s%d.txt", 
				    dataDir, 
				    file, folder,
				    file, str2,ii,
				    type,
				    numCluster);
	     }
	     sprintf(roiout1, "%s/%s/%s_layer%d_roi_%s", 
			    dataDir, 
			    dataFile, 
			    dataFile,ii,
			    type);
	     sprintf(roiout2, "%s/%s/%s_layer%d_roi_%s", 
			    dataDir, 
			    file, 
			    file, ii,
			    type); 
	      sprintf(rangefile,  "%s/%s/%s_%s_%s_%s_layer%d_range%d", 
				dataDir, 
				dataFile, 
				dataFile, str,
				file, str2,ii,
				numCluster);
	      sprintf(rangefile2,  "%s/%s/%s_%s_%s_%s_layer%d_range%d", 
				dataDir, 
				file, 
		      		file, str2,
				dataFile, str,ii,
				numCluster);
	     sprintf(roirange1,  "%s/%s/%s_layer%d_roi_range", 
				dataDir, 
				dataFile, 
				dataFile,ii);
	     sprintf(roirange2,  "%s/%s/%s_layer%d_roi_range", 
				dataDir, 
				file, 
		      		file,ii);
	    }
	    else
	    {
	     sprintf(index_file1, "%s/%s/%s_layer%d_symmetry_%s", 
				   dataDir, dataFile, dataFile, ii,str);
	     sprintf(index_file2, "%s/%s/%s_layer%d_symmetry_%s", 
				   dataDir, file, file, ii,str2);
	     sprintf(outfile1, "%s/%s/%s_layer%d_roi_%s.txt", 
			    dataDir, 
			    dataFile, 
			    dataFile, ii,
			    type);
	     sprintf(outfile2, "%s/%s/%s_layer%d_roi_%s.txt", 
			    dataDir, 
			    file, 
			    file, ii,
			    type); 
	     sprintf(rangefile,  "%s/%s/%s_layer%d_roi_range", 
				dataDir, 
				dataFile, 
				dataFile,ii);
	     sprintf(rangefile2,  "%s/%s/%s_layer%d_roi_range", 
				dataDir, 
				file, 
		      		file,ii);
	    }
	    
	    delete [] str;
	    delete [] str2;


//========generating===========================================================
	  ifstream infile(outfile1);
	  if(infile.is_open() && sWholefield)
	  {
	    
	  }
	  else
	  {
	    char *clusterstr = new char[4000];
	    sprintf(clusterstr, "./dataProcessing/EMclustering/clustering %s %s %s %s %s %s %d %d %d %s %s %s", 
				infile1, infile2,
				index_file1, index_file2,
				outfile1, outfile2,
				numCluster,
				(int)(toggleAttribute),
				0,
				rangefile,
				roifile1, roifile2); //modify the program by adding ROI files
		cerr<<clusterstr<<endl;
	    system(clusterstr);
	    delete [] clusterstr;
	  }
	  infile.close();

	      if(sWholefield || !toggleROI)
	      {
		std::ifstream  src1(outfile1, std::ios::binary);		
		std::ofstream  roidst1(roiout1, std::ios::binary);
		roidst1 << src1.rdbuf();
		std::ifstream  src2(outfile2, std::ios::binary);		
		std::ofstream  roidst2(roiout2, std::ios::binary);
		roidst2 << src2.rdbuf();
	
		std::ifstream  srcrange(rangefile, std::ios::binary);		
		std::ofstream  roirange(rangefile2, std::ios::binary);
		roirange << srcrange.rdbuf(); 
		std::ifstream  srcrange1(rangefile, std::ios::binary);		
		std::ofstream  roi1(roirange1, std::ios::binary);
		roi1 << srcrange1.rdbuf(); 
		std::ifstream  srcrange2(rangefile2, std::ios::binary);		
		std::ofstream  roi2(roirange2, std::ios::binary);
		roi2 << srcrange2.rdbuf(); 
	      }

//================================================================================
	    //--------------read files--------------
	    if(file != NULL)
	    {
	     infile.open(roiout2);
	     int n;
	     infile >> n;
	     for(int i=0;i<n;i++) infile>>outcluster[i];
	     infile.close();
	    }
	    
	    infile.open(roiout1);
	    int nn;
	    infile>>nn;
	    for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
	    {
	      if(!toggleAttribute) infile>>layerMagcluster[i];
	      else infile>>layerVelcluster[i];
	    }
	    infile.close();
	    
	    if(!toggleAttribute)
	    {
	      for(int i=0;i<range_layer_mag_cluster[ii].size();i++)
		range_layer_mag_cluster[ii][i].clear();
	      range_layer_mag_cluster[ii].clear();
	      
	      range_layer_mag_cluster[ii].resize(numCluster);
	      
	      infile.open(roirange1);
	      infile>>nn;
	      for(int i=0;i<nn;i++)
	      {
		range_layer_mag_cluster[ii][i].resize(2);
		infile>>range_layer_mag_cluster[ii][i][0]
		  >> range_layer_mag_cluster[ii][i][1];
	      }
	      infile.close();
    	  }
    }
  }
  else
  {
    for(int ii = 0; ii<zlayer;ii++)
    {
	//	cerr<<ii<<" ";
	    int count2 = 0;
	    for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
	    {
	     if(dataInbox[i]) count2++; 
	    }
	    if(count2 == dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1] - dataFormat[ii][0][0] + 1)
	    {
	      sWholefield = true;
	    }
	  //  cerr<<ii<<endl;
	    char *str = new char[100];
	    for(int i=0;i<100;i++)
	    {
	      str[i] = '\0';
	    }
	    for(int i=0;i<symmetryV.size();i++)
	    {
	      sprintf(str, "%s[%0.2f%0.2f%0.2f,%0.2f%0.2f%0.2f]", 
				    str, 
				    symmetryV[i][0], symmetryV[i][1], symmetryV[i][2],
				    symmetryN[i][0], symmetryN[i][1], symmetryN[i][2]);
	    }    
	 //   cerr<<str<<endl;
	    
	    sprintf(infile1, "%s/%s/%s_layer%d_sorted", dataDir, dataFile, dataFile,ii);
	    sprintf(infile2, "%s/%s/%s_layer%d_sorted", dataDir, dataFile, dataFile,ii);
	    sprintf(roifile1, "%s/%s/%s_layer%d_roiindex", dataDir, dataFile, dataFile,ii);
	    sprintf(roifile2, "%s/%s/%s_layer%d_roiindex", dataDir, dataFile, dataFile,ii);
	    if(sWholefield || !toggleROI)
	    {
	     sprintf(index_file1, "%s/%s/%s_layer%d_symmetry_%s", 
				   dataDir, dataFile, dataFile, ii,str);
	     sprintf(index_file2, "%s/%s/%s_layer%d_symmetry_%s", 
				   dataDir, dataFile, dataFile, ii,str);
	     sprintf(outfile1, "%s/%s/%s/%s_%slayer%d_%s%d.txt", 
			    dataDir, 
			    dataFile, folder,
			    dataFile,str,ii,
			    type,
			    numCluster);
	     sprintf(outfile2, "%s/%s/%s/%s_%slayer%d_%s%d.txt", 
			    dataDir, 
			    dataFile, folder,
			    dataFile,str,ii,
			    type,
			    numCluster);
	     sprintf(roiout1, "%s/%s/%s_layer%d_roi_%s", 
			    dataDir, 
			    dataFile, 
			    dataFile,ii,
			    type);
	     sprintf(rangefile,  "%s/%s/%s_%s_layer%d_range%d", 
				dataDir, 
				dataFile, 
				dataFile, str,ii,
				numCluster);
	     sprintf(roirange1,  "%s/%s/%s_layer%d_roi_range", 
				dataDir, 
				dataFile, 
				dataFile,ii);
	    }
	    else
	    {
	     sprintf(index_file1, "%s/%s/%s_layer%d_symmetry_%s", 
				   dataDir, dataFile, dataFile, ii,str);
	     sprintf(index_file2, "%s/%s/%s_layer%d_symmetry_%s", 
				   dataDir, dataFile, dataFile, ii,str);
	     sprintf(outfile1, "%s/%s/%s/%s_layer%d_roi_%s.txt", 
			    dataDir, 
			    dataFile, folder,
			    dataFile,ii,
			    type);
	     sprintf(outfile2, "%s/%s/%s/%s_layer%d_roi_%s.txt", 
			    dataDir, 
			    dataFile, folder,
			    dataFile,ii,
			    type);  
	     sprintf(rangefile,  "%s/%s/%s_layer%d_roi_range", 
				dataDir, 
				dataFile, 
				dataFile,ii);		    
	    }
	    
	    delete [] str;

//cerr<<"========generating==========================================================="<<endl;
	  ifstream infile(outfile1);
	//	cerr<<outfile1<<endl;
	  if(infile.is_open() && sWholefield)
	  {
		//cerr<<"find the file"<<endl;	    
	  }
	  else
	  {
		//cerr<<outfile1<<endl;
	    char *clusterstr = new char[4000];
		//cerr<<outfile1<<endl;
	    sprintf(clusterstr, "./dataProcessing/EMclustering/clustering %s %s %s %s %s %s %d %d %d %s %s %s", 
				infile1, infile2,
				index_file1, index_file2,
				outfile1, outfile2,
				numCluster,
				(int)(toggleAttribute),
				0,
				rangefile,
				roifile1, roifile2); //modify the program by adding ROI files
		cerr<<clusterstr<<endl;
	    system(clusterstr);
	    delete [] clusterstr;
	  }
	  infile.close();

	      if(sWholefield || !toggleROI)
	      {
		std::ifstream  src1(outfile1, std::ios::binary);		
		std::ofstream  roidst1(roiout1, std::ios::binary);
		roidst1 << src1.rdbuf();
	
		std::ifstream  srcrange1(rangefile, std::ios::binary);		
		std::ofstream  roi1(roirange1, std::ios::binary);
		roi1 << srcrange1.rdbuf(); 
	      }      
//================================================================================ 
	    //--------------read files--------------
	    infile.open(roiout1);
	    int nn;
	    infile>>nn;
	    for(int i=dataFormat[ii][0][0];i<=dataFormat[ii][dataFormat[ii].size()-1][dataFormat[ii][dataFormat[ii].size()-1].size()-1];i++)
	    {
	      if(!toggleAttribute) infile>>layerMagcluster[i];
	      else
		{
			infile>>layerVelcluster[i];layerVelcluster[i]=layerVelcluster[i];
		}
	    }
	    infile.close();
	    
	    if(!toggleAttribute)
	    {
	      for(int i=0;i<range_layer_mag_cluster[ii].size();i++)
		range_layer_mag_cluster[ii][i].clear();
	      range_layer_mag_cluster[ii].clear();
	      
	      range_layer_mag_cluster[ii].resize(numCluster);
	      
	      infile.open(roirange1);
	      infile>>nn;
	      for(int i=0;i<nn;i++)
	      {
		range_layer_mag_cluster[ii][i].resize(2);
		infile>>range_layer_mag_cluster[ii][i][0]
		  >> range_layer_mag_cluster[ii][i][1];
	      }
	      infile.close();
    	  }
    }
  }

		   
    //------------release memory--------------
    delete [] infile1;
    delete [] infile2;
    delete [] outfile1;
    delete [] outfile2;
    delete [] index_file1;
    delete [] index_file2;
    delete [] roifile1;
    delete [] roifile2;
    delete [] rangefile;
    delete [] rangefile2;
    delete [] roirange1;
    delete [] roirange2;
    delete [] roiout1;
    delete [] roiout2;
    delete [] type;

}*/

//JC
/*
void svQDOT::setCluster(bool toggleAttribute, int *cluster, 
			vector< vector<double> > range)
{
    for(int i=0;i<dataSize;i++)
    {
      if(!toggleAttribute) 
	{
		dataMagcluster[i] = cluster[i];
	}
      else 
	{
		dataVelcluster[i] = cluster[i];

	}
    }
    
    if(!toggleAttribute)
    {
      for(int i=0;i<range_mag_cluster.size();i++)
	range_mag_cluster[i].clear();
      range_mag_cluster.clear();
      
      range_mag_cluster.resize(range.size());

      for(int i=0;i<range.size();i++)
      {
	range_mag_cluster[i].resize(2);
	range_mag_cluster[i][0] = range[i][0];
	range_mag_cluster[i][1] = range[i][1];
      }
    }	
}
*/

//JC
/*
void svQDOT::setLayerCluster(bool toggleAttribute, int *layercluster,
			vector< vector< vector<double> > > layerrange)
{
    for(int i=0;i<dataSize;i++)
    {
      if(!toggleAttribute) 
	{
		layerMagcluster[i] = layercluster[i];
	}
      else 
	{
		layerVelcluster[i] = layercluster[i];
	}
    }
    
    if(!toggleAttribute)
    {
	for(int ii=0;ii<zlayer;ii++)
	{
	      for(int i=0;i<range_layer_mag_cluster[ii].size();i++)
		range_layer_mag_cluster[ii][i].clear();
	      range_layer_mag_cluster[ii].clear();
	      
	      range_layer_mag_cluster[ii].resize(layerrange[ii].size());

	      for(int i=0;i<layerrange[ii].size();i++)
	      {
		range_layer_mag_cluster[ii][i].resize(2);
		range_layer_mag_cluster[ii][i][0] = layerrange[ii][i][0];
		range_layer_mag_cluster[ii][i][1] = layerrange[ii][i][1];
	      }
	}
    }	
}
*/

void svQDOT::layerContourGeneration(vector< vector<double> > locationR, vector<double> lr,
				vector< vector<double> > locationA, vector<double> la,
				vector<double> gr, vector<double> ga,vector<bool> selected_plane)
{
cerr<<planeNormal[0][0]<<" "<<planeNormal[0][1]<<" "<<planeNormal[0][2]<<endl;
	for(int i=0;i<layerContourPos.size();i++)
	{
		layerContourPos[i].clear();
		layerContourVel[i].clear();
		layerContourDen[i].clear();
		layerContourExp[i].clear();
		layerContourCoe[i].clear();  
		layerContourAtom[i].clear();
		layerContourCluster[i].clear();
		layerDenContourCluster[i].clear();
	}
	layerContourPos.clear();
	layerContourVel.clear();
	layerContourDen.clear();
	layerContourExp.clear();
	layerContourCoe.clear();
	layerContourAtom.clear();
	layerContourCluster.clear();  
	layerDenContourCluster.clear();

	int n = selected_plane.size();

	layerContourPos.resize(n);
	layerContourDen.resize(n);
	layerContourVel.resize(n);
	layerContourExp.resize(n);
	layerContourCoe.resize(n);
	layerContourAtom.resize(n);
	layerContourCluster.resize(n);
	layerDenContourCluster.resize(n);

	layerContourSize = 0;

	int min;
	float mind;
	mind = log10(min_density_nonzero);
	min = (int)log10(min_density_nonzero);
	if(fabs((float)min - mind)<1e-20) min = (int)mind;
	else min = (int)mind - 1;

cerr<<planeNormal[0][0]<<" "<<planeNormal[0][1]<<" "<<planeNormal[0][2]<<endl;

	if(!(fabs(planeNormal[0][0])<1e-9
		&& fabs(planeNormal[0][1])<1e-9
		&& (fabs(planeNormal[0][2]-1)<1e-9 || fabs(planeNormal[0][2]+1)<1e-9)))
	{
		for(int i=0;i<layerPos.size();i++)
		{
			if(selected_plane[i] && layerPos[i].size()>1)
			{
			for(int j=0;j<layerPos[i].size()-1;j++)
			{
				for(int ii=0;ii<locationR.size();ii++)
				{
					if(fabs(layerPos[i][j][2] - locationR[i][0])<1e-9
					|| fabs(layerPos[i][j][2] - locationR[i][1])<1e-9
					|| (layerPos[i][j][2] > locationR[i][0] && layerPos[i][j][2] < locationR[i][1]))
					{
						int zv = (int)((layerPos[i][j][2]-zmin)/zdistance+1);
						//int zi = (int)((layerPos[i][j][2]-locationR[i][0])/zdistance+1);
						double value = layer_density[zv] * lr[ii];
						double exp = getNumOfIntegerDigits(value);
						double coe = value/pow(10.,exp);
						if(fabs(layerPos[i][j][2] - layerPos[i][j+1][2])<1e-9
						&& 
						((layerDen[i][j]-value)*(layerDen[i][j+1]-value) <0 
						|| fabs(layerDen[i][j] - value)<1e-19 || fabs(layerDen[i][j+1] - value)<1e-19 ))
						{
							svVector3 tmp;
							double ratio = (value-layerDen[i][j])/(layerDen[i][j+1]-layerDen[i][j]);
							tmp[0] = ratio *  (layerPos[i][j+1][0] - layerPos[i][j][0]) + layerPos[i][j][0];
							tmp[1] = ratio *  (layerPos[i][j+1][1] - layerPos[i][j][1]) + layerPos[i][j][1];
							tmp[2] = ratio *  (layerPos[i][j+1][2] - layerPos[i][j][2]) + layerPos[i][j][2];
							layerContourPos[i].push_back(tmp);
							layerContourCluster[i].push_back(-1);
							tmp[0] = ratio *  (layerVel[i][j+1][0] - layerVel[i][j][0]) + layerVel[i][j][0];
							tmp[1] = ratio *  (layerVel[i][j+1][1] - layerVel[i][j][1]) + layerVel[i][j][1];
							tmp[2] = ratio *  (layerVel[i][j+1][2] - layerVel[i][j][2]) + layerVel[i][j][2];
							layerContourVel[i].push_back(tmp);
							layerContourDen[i].push_back(value);
							layerContourExp[i].push_back(exp);
							layerContourCoe[i].push_back(coe);
							if(ratio>0.5) layerContourAtom[i].push_back(layerAtom[i][j+1]);
							else layerContourAtom[i].push_back(layerAtom[i][j]);
							if(fabs(coe)<1e-9)
							{
								layerDenContourCluster[i].push_back(0);
							}
							else
							{
								layerDenContourCluster[i].push_back((int)exp - (int)(min));
							}
							layerContourSize++;
						}
					}
				}
				for(int ii=0;ii<locationA.size();ii++)
				{
					if(fabs(layerPos[i][j][2] - locationR[i][0])<1e-9
					|| fabs(layerPos[i][j][2] - locationR[i][1])<1e-9
					|| (layerPos[i][j][2] > locationR[i][0] && layerPos[i][j][2] < locationR[i][1]))
					{
						int zv = (int)((layerPos[i][j][2]-zmin)/zdistance+1);
						//int zi = (int)((layerPos[i][j][2]-locationR[i][0])/zdistance+1);
						double value = la[ii];

						double exp = getNumOfIntegerDigits(value);
						double coe = value/pow(10.,exp);
						if(fabs(layerPos[i][j][2] - layerPos[i][j+1][2])<1e-9
						&& 
						((layerDen[i][j]-value)*(layerDen[i][j+1]-value) <0 
						|| fabs(layerDen[i][j] - value)<1e-19 || fabs(layerDen[i][j+1] - value)<1e-19 ))
						{	
							//cerr<<value<<" ";
							svVector3 tmp;
							double ratio = (value-layerDen[i][j])/(layerDen[i][j+1]-layerDen[i][j]);
							tmp[0] = ratio *  (layerPos[i][j+1][0] - layerPos[i][j][0]) + layerPos[i][j][0];
							tmp[1] = ratio *  (layerPos[i][j+1][1] - layerPos[i][j][1]) + layerPos[i][j][1];
							tmp[2] = ratio *  (layerPos[i][j+1][2] - layerPos[i][j][2]) + layerPos[i][j][2];
							layerContourPos[i].push_back(tmp);
							layerContourCluster[i].push_back(-1);
							tmp[0] = ratio *  (layerVel[i][j+1][0] - layerVel[i][j][0]) + layerVel[i][j][0];
							tmp[1] = ratio *  (layerVel[i][j+1][1] - layerVel[i][j][1]) + layerVel[i][j][1];
							tmp[2] = ratio *  (layerVel[i][j+1][2] - layerVel[i][j][2]) + layerVel[i][j][2];
							layerContourVel[i].push_back(tmp);
							layerContourDen[i].push_back(value);
							layerContourExp[i].push_back(exp);
							layerContourCoe[i].push_back(coe);
							if(ratio>0.5) layerContourAtom[i].push_back(layerAtom[i][j+1]);
							else layerContourAtom[i].push_back(layerAtom[i][j]);
							if(fabs(coe)<1e-9)
							{
								layerDenContourCluster[i].push_back(0);
							}
							else
							{
								layerDenContourCluster[i].push_back((int)exp - (int)(min));
							}
							layerContourSize++;
						}
					}
				}
				for(int ii=0;ii<gr.size();ii++)
				{
						int zv = (int)((layerPos[i][j][2]-zmin)/zdistance+1);
						//int zi = (int)((layerPos[i][j][2]-locationR[i][0])/zdistance+1);
						double value = layer_density[zv] * gr[ii];
						double exp = (double)getNumOfIntegerDigits(value);
						double coe = value/pow(10.,exp);
						//cerr<<i<<" "<<j<<" "<<value<<" "<<layerDen[i][j]<<" "<<layerDen[i][j+1]<<endl;
						if(fabs(layerPos[i][j][2] - layerPos[i][j+1][2])<1e-9
						&& 
						((layerDen[i][j]-value)*(layerDen[i][j+1]-value) <0 
						||( fabs(layerDen[i][j] - value)<1e-19 || fabs(layerDen[i][j+1] - value)<1e-19 )))
						{
							svVector3 tmp;
							double ratio;
							ratio = (value-layerDen[i][j])/(layerDen[i][j+1]-layerDen[i][j]);
							tmp[0] = ratio *  (layerPos[i][j+1][0] - layerPos[i][j][0]) + layerPos[i][j][0];
							tmp[1] = ratio *  (layerPos[i][j+1][1] - layerPos[i][j][1]) + layerPos[i][j][1];
							tmp[2] = ratio *  (layerPos[i][j+1][2] - layerPos[i][j][2]) + layerPos[i][j][2];
							//-if(ratio > 1) cerr<<ratio<<" "<<value<<" "<<layerDen[i][j]<<" "<<layerDen[i][j+1] <<endl;
							layerContourPos[i].push_back(tmp);
							layerContourCluster[i].push_back(-1);
							tmp[0] = ratio *  (layerVel[i][j+1][0] - layerVel[i][j][0]) + layerVel[i][j][0];
							tmp[1] = ratio *  (layerVel[i][j+1][1] - layerVel[i][j][1]) + layerVel[i][j][1];
							tmp[2] = ratio *  (layerVel[i][j+1][2] - layerVel[i][j][2]) + layerVel[i][j][2];
							layerContourVel[i].push_back(tmp);
							layerContourDen[i].push_back(value);
							layerContourExp[i].push_back(exp);
							layerContourCoe[i].push_back(coe);
							if(ratio>0.5) layerContourAtom[i].push_back(layerAtom[i][j+1]);
							else layerContourAtom[i].push_back(layerAtom[i][j]);
							//if(layerAtom[i][j+1]==2)
							//cerr<<value<<" "<<layerPos[i][j][2]<<" "<<layerAtom[i][j+1]<<" "<<layerAtom[i][j]<<endl;;
							if(fabs(coe)<1e-9)
							{
								layerDenContourCluster[i].push_back(0);
							}
							else
							{
								layerDenContourCluster[i].push_back((int)exp - (int)(min));
							}
							layerContourSize++;
						}
				}
				for(int ii=0;ii<ga.size();ii++)
				{
						int zv = (int)((layerPos[i][j][2]-zmin)/zdistance+1);
						//int zi = (int)((layerPos[i][j][2]-locationR[i][0])/zdistance+1);
						double value = ga[ii];
						double exp = getNumOfIntegerDigits(value);
						double coe = value/pow(10.,exp);
						if(fabs(layerPos[i][j][2] - layerPos[i][j+1][2])<1e-9
						&& 
						((layerDen[i][j]-value)*(layerDen[i][j+1]-value) <0 
						|| fabs(layerDen[i][j] - value)<1e-19 || fabs(layerDen[i][j+1] - value)<1e-19 ))
						{
							svVector3 tmp;
							double ratio = (value-layerDen[i][j])/(layerDen[i][j+1]-layerDen[i][j]);
							tmp[0] = ratio *  (layerPos[i][j+1][0] - layerPos[i][j][0]) + layerPos[i][j][0];
							tmp[1] = ratio *  (layerPos[i][j+1][1] - layerPos[i][j][1]) + layerPos[i][j][1];
							tmp[2] = ratio *  (layerPos[i][j+1][2] - layerPos[i][j][2]) + layerPos[i][j][2];
							layerContourPos[i].push_back(tmp);
							layerContourCluster[i].push_back(-1);
							tmp[0] = ratio *  (layerVel[i][j+1][0] - layerVel[i][j][0]) + layerVel[i][j][0];
							tmp[1] = ratio *  (layerVel[i][j+1][1] - layerVel[i][j][1]) + layerVel[i][j][1];
							tmp[2] = ratio *  (layerVel[i][j+1][2] - layerVel[i][j][2]) + layerVel[i][j][2];
							layerContourVel[i].push_back(tmp);
							layerContourDen[i].push_back(value);
							layerContourExp[i].push_back(exp);
							layerContourCoe[i].push_back(coe);
							if(ratio>0.5) layerContourAtom[i].push_back(layerAtom[i][j+1]);
							else layerContourAtom[i].push_back(layerAtom[i][j]);
							if(fabs(coe)<1e-9)
							{
								layerDenContourCluster[i].push_back(0);
							}
							else
							{
								layerDenContourCluster[i].push_back((int)exp - (int)(min));
							}
							layerContourSize++;
						}
				}
			}
			}
		}
	}
	else
	{
		for(int i=0;i<contourSize;i++)
		{
			float index = (contourPos[i][2] - zmin)/zdistance; //cerr<<i<<" "<<index<<" "<<layerContourPos.size()<<endl;

			if(selected_plane[(int)index])
			{
				//cerr<<contourPos[i][0]<<endl;
				layerContourPos[(int)index].push_back(contourPos[i]);//cerr<<index<<" p"<<endl;
				layerContourCluster[(int)index].push_back(-1);
				layerContourVel[(int)index].push_back(contourVel[i]);//cerr<<index<<" v"<<endl;
				layerContourDen[(int)index].push_back(contourDen[i]);//cerr<<index<<" d"<<endl;
				layerContourExp[(int)index].push_back(contourExp[i]);//cerr<<index<<" e"<<endl;
				layerContourCoe[(int)index].push_back(contourCoe[i]);//cerr<<index<<" c"<<endl;
				layerContourAtom[(int)index].push_back(contourAtom[i]);//cerr<<index<<" a"<<endl;
				if(fabs(contourCoe[i])<1e-9)
				{
					layerDenContourCluster[(int)index].push_back(0);
				}
				else
				{
					layerDenContourCluster[(int)index].push_back((int)contourExp[i] - (int)(min));
				}//cerr<<index<<" dc"<<endl;
				layerContourSize++;
			}
		}
	}
	cerr<<"layercontoursize "<<layerContourSize<<endl;
}



void svQDOT::contourGeneration(vector< vector<double> > locationR, vector<double> lr,
				vector< vector<double> > locationA, vector<double> la,
				vector<double> gr, vector<double> ga)
{
  char *contourname = new char[400];
      sprintf(contourname, "%s/%s/%s_contour", dataDir, dataFile, dataFile); 

#ifdef _VTK
  char *outfilename = new char[1024];
  sprintf(outfilename, "%s/preprocessing/generateContours/input", LIB_DIR);
 ofstream outfile(outfilename);
delete outfilename;

 outfile<<zmin<<endl;;
 outfile<<zdistance<<endl;
 outfile<<lr.size() + la.size()
	+ gr.size() + ga.size()<<endl;

 for(int i=0;i<lr.size();i++)
 {
	outfile<<"0 "<<locationR[i][0]<<" "<<locationR[i][1]<<" ";
	outfile<<"1 "<<lr[i]<<endl;
 }	

 for(int i=0;i<la.size();i++)
 { 
	outfile<<"0 "<<locationA[i][0]<<" "<<locationA[i][1]<<" ";
		outfile<<"0 "<<la[i]<<endl;
 }

 for(int i=0;i<gr.size();i++)
 {
	outfile<<"1 "<<"1 "<<gr[i]<<endl;
 }
 for(int i=0;i<ga.size();i++)
 {
	outfile<<"1 "<<"0 "<<ga[i]<<endl;
 }
 outfile.close();

      char *exe = new char[2048];
      sprintf(exe, "%s/exe/Contour %d %s/%s/ %s /home/davinci/Documents/SPLIT/preprocessing/generateContours/input", LIB_DIR,
	      zlayer,
	      dataDir, dataFile, 
	      contourname);
     system(exe); 
#endif

  //---------------read contour------------------------
  if(contourSize > 0)
  {
   delete [] contourPos;
   delete [] contourCoe;
   delete [] contourVel;
   delete [] contourDen;
   delete [] contourExp;
//JC
//   delete [] contourInbox;
//   delete [] contourMagcluster;
//   delete [] contourVelcluster;
   delete [] contourAtom;
  
//JC
/*
  for(int i=0;i<contourSize;i++)
  {
    contourROI[i].clear();
  }
  contourROI.clear();
 */
  }
 
  ifstream infile(contourname);
  cerr<<contourname<<endl;

  if(infile.is_open())
  {
  
  infile>>contourSize;
  
  contourPos = new svVector3[contourSize];
  contourVel = new svVector3[contourSize];
  contourDen = new svScalar[contourSize];
  contourExp = new svScalar[contourSize];
  contourCoe = new svScalar[contourSize];

//JC
//  contourInbox = new svBool[contourSize];
//  contourMagcluster = new svInt[contourSize];
//  contourVelcluster = new svInt[contourSize];
  contourAtom = new int[contourSize];
//  contourROI.resize(contourSize);

  cerr<<contourSize<<endl;
  
  for(int i=0;i<contourSize;i++)
  {
    svVector3 p;
    svVector3 v;
    svScalar d;
    svScalar e;
    svScalar c;
    
    infile>>p[0]>>p[1]>>p[2]
	  >>v[0]>>v[1]>>v[2]
	  >>d>>e>>c;
	  
    contourPos[i] = p;
    contourVel[i] = v;
    contourDen[i] = d;
    contourExp[i] = e;
    contourCoe[i] = c;

// JC
 // qdotANN->ANN_Vel_IDX(dataPos, dataSize, contourPos, contourSize,
//			dataAtom, contourAtom);
  }

  
  infile.close();



  }
    delete [] contourname; 
#ifdef _VTK
  delete [] exe;

#endif
}

/*
void svQDOT::contourClustering(bool toggleAttribute, bool toggleROI, bool toggleLayer)
{
// JC
 // svANNcluster *qdotANN = new svANNcluster();
  
  if(!toggleAttribute)
  {
    if(!toggleLayer)
    	qdotANN->ANN_Mag_IDX(contourDen, contourSize,
			 range_mag_cluster,
			 contourMagcluster);
    else
    {//checkpoint
	for(int i=0;i<zlayer;i++)
	{
		
	}
    }
    if(!toggleROI)
    {
      for(int i=0;i<contourSize;i++)
      {
	if(contourInbox[i] == false)
	{
	  contourMagcluster[i] = -1;
	}
      }
    }
  }
  else
  {
    if(!toggleLayer)
    	qdotANN->ANN_Vel_IDX(dataVel, dataSize,
			 contourVel, contourSize,
			 dataVelcluster, contourVelcluster);
    else
    	qdotANN->ANN_Vel_IDX(dataVel, dataSize,
			 contourVel, contourSize,
			 layerVelcluster, contourVelcluster);
    if(!toggleROI)
    {
      for(int i=0;i<contourSize;i++)
      {
	if(contourInbox[i] == false)
	{
	  contourVelcluster[i] = -1;
	}
      }
    }
  }
 // delete qdotANN;
}
*/

// JC
void svQDOT::distanceGeneration(int select_layer, vector<svVector3> v, vector<svScalar> z)
{
  svDistance *qdotDistance = new svDistance(dataPos, dataVel,
					    dataDen, dataExp, dataCoe,
					    dataSize);
  qdotDistance->SetSelectedLayer(select_layer);
  qdotDistance->SetDirection(v, z);
  
  qdotDistance->getDistance(dataDis);
  
  delete qdotDistance;
}

double svQDOT::getMax(float z)
{
	double max = -9e+9;
	for(int i=0;i<dataSize;i++)
	{
		if(dataDen[i] > max && fabs(dataPos[i][2] - z) < 0.001)
		{
			max = dataDen[i];
		}
	}
	return max;
}
double svQDOT::getMin(float z)
{
	double min = 9e+9;
	for(int i=0;i<dataSize;i++)
	{
		if(dataDen[i] < min && fabs(dataPos[i][2] - z) < 0.001)
		{
			min = dataDen[i];
		}
	}
	return min;
}
// JC
/* 
void svQDOT::getROI(vector< vector<double> > mag,
			vector< vector<double> > zrange,
			vector< vector<double> > xyrange,
			vector< vector<svVector3> > space)
{
  svROI *qdotROI2 = new svROI();
  
  dataROI = qdotROI2->getROI(dataPos, dataVel, dataDen,
		  dataExp, dataCoe,
		  dataSize,
		  mag, zrange, xyrange, space, dataInbox);

  delete qdotROI2;
}
*/

//JC
/*
void svQDOT::getContourROI(vector< vector<double> > mag,
			vector< vector<double> > zrange,
			vector< vector<double> > xyrange,
			vector< vector<svVector3> > space)	
{
  svROI *qdotROI2 = new svROI();
  contourROI = qdotROI2->getROI(contourPos, contourVel, contourDen,
		  contourExp, contourCoe,
		  contourSize,
		  mag, zrange, xyrange, space, contourInbox);
  
  delete qdotROI2;
}
*/
// JC
/*
void svQDOT::getPlaneROI(vector< vector<double> > mag,
			vector< vector<double> > zrange,
			vector< vector<double> > xyrange,
			vector< vector<svVector3> > space)	
{
  svROI *qdotROI2 = new svROI();
  qdotROI2->getROI(planePos, planeVel, planeDen,
		  planeExp, planeCoe,
		  planeSize,
		  mag, zrange, xyrange, space, planeInbox);
  
  delete qdotROI2;
}
*/
// JC
/*
void svQDOT::getLayerROI(vector< vector<double> > mag,
			vector< vector<double> > zrange,
			vector< vector<double> > xyrange,
			vector< vector<svVector3> > space)	
{ svVector3 *pos = new svVector3[layerSize];
  svVector3 *vel = new svVector3[layerSize];
  svScalar *den = new svScalar[layerSize];
  svScalar *exp = new svScalar[layerSize];
  svScalar *coe = new svScalar[layerSize];
  svROI *qdotROI2 = new svROI();

  int count = 0;
  for(int i=0;i<layerPos.size();i++)
  {
	for(int j=0;j<layerPos[i].size();j++)
	{
		pos[count] = layerPos[i][j];
		vel[count] = layerVel[i][j];
		exp[count] = layerExp[i][j];
		den[count] = layerDen[i][j];
		coe[count] = layerCoe[i][j];
		layerInbox[count] = false;
		count++;
	}
  }

  qdotROI2->getROI(pos, vel, den,
		  exp, coe,
		  layerSize,
		  mag, zrange, xyrange, space, layerInbox);
  
  delete qdotROI2;
  delete [] pos;
  delete [] vel;
  delete [] den;
  delete [] exp;
  delete [] coe;
}
*/
// JC
/*
void svQDOT::getStepROI(vector< vector<double> > mag,
			vector< vector<double> > zrange,
			vector< vector<double> > xyrange,
			vector< vector<svVector3> > space,
			int index, bool toggleRaw)
{
  svROI *qdotROI2 = new svROI();
  if(toggleRaw)
    dataROI = qdotROI2->getSpecificROI(dataPos, dataVel, dataDen,
		  dataExp, dataCoe,
		  dataSize,
		  mag, zrange, xyrange, space, index, dataInbox);
  else
    contourROI = qdotROI2->getSpecificROI(contourPos, contourVel, contourDen,
		  contourExp, contourCoe,
		  contourSize,
		  mag, zrange, xyrange, space, index, contourInbox);
  //plane roi? modification
  
  delete qdotROI2;
}
*/

// JC
/*
void svQDOT::printROI()
{
  char *roi = new char[400];	  
  sprintf(roi, "%s%s/%s_roiindex", dataDir, dataFile, dataFile);  
  
  ofstream outfile(roi);
  outfile<<dataSize<<endl;
  for(int i=0;i<dataSize;i++)
  {
   outfile<<dataInbox[i]<<endl; 
  }
  outfile.close();
  

//=========================================================================================
  for(int i=0;i<dataFormat.size();i++)
  {
	sprintf(roi, "%s%s/%s_layer%d_roiindex", dataDir, dataFile, dataFile, i);

	outfile.open(roi);
	int n;
	n = dataFormat[i][dataFormat[i].size()-1][dataFormat[i][dataFormat[i].size()-1].size()-1]
			- dataFormat[i][0][0] + 1;
	//cerr<<dataFormat[i][0][0]<<" "<<dataFormat[i][dataFormat[i].size()-1][dataFormat[i][dataFormat[i].size()-1].size()-1] <<endl;
	outfile<<n<<endl;
	for(int j=dataFormat[i][0][0];j<=dataFormat[i][dataFormat[i].size()-1][dataFormat[i][dataFormat[i].size()-1].size()-1];j++)
	{
		outfile<<dataInbox[j]<<endl;
	}
	outfile.close();  
  }

  delete [] roi;
}
*/

//JC
/*
bool *svQDOT::returnROI(bool toggleRaw)
{
 if(toggleRaw)
 {
   return dataInbox;
 }
 else
 {
   return contourInbox;
 }
}
*/

// JC
/*
void svQDOT::getPlane(bool toggleData, bool toggleRaw, 
		      svVector3 plane_center, svVector3 plane_vector)
{
#if 0
  if(toggleData)
  {
    if(planeSize != dataSize)
    {
      delete [] planePos;
      delete [] planeVel;
      delete [] planeInbox;
      delete [] planeMagcluster;
      delete [] planeVelcluster;
      delete [] planeDen;
      delete [] planeExp;
      delete [] planeCoe;

      planePos = new svVector3[dataSize];
      planeVel = new svVector3[dataSize];
      planeDen = new svScalar[dataSize];
      planeExp = new svScalar[dataSize];
      planeCoe = new svScalar[dataSize];
      planeInbox = new svBool[dataSize];
      planeMagcluster = new svInt[dataSize];
      planeVelcluster = new svInt[dataSize];
    
      planeSize = dataSize;
      for(int i=0;i<dataSize;i++)
      {
	planePos[i] = dataPos[i];
	planeVel[i] = dataVel[i];
	planeInbox[i] = true;
      }
    }
  }
  else
  { 
    if(planeSize > 0)
    {
      delete [] planePos;
      delete [] planeVel;
      delete [] planeInbox;
      delete [] planeMagcluster;
      delete [] planeVelcluster;
      delete [] planeDen;
      delete [] planeExp;
      delete [] planeCoe;
    }      
	//cerr<<"plane"<<endl;
    svCuttingPlane *qdotCut = new svCuttingPlane();
    	//cerr<<"plane"<<endl;
    if(toggleRaw)
    {
      svVector3 min_distance;
      min_distance[0] = xdistance;
      min_distance[1] = ydistance;
      min_distance[2] = zdistance;
      planeRaw = qdotCut->GetSlicePos(dataPos, dataSize,
				lbbox, rbbox,
				plane_center, plane_vector,
				min_distance);
      int count = 0;
      for(int i=0;i<dataSize;i++)
      {
	if(planeRaw[i]){count++;}
      }
      planeSize = count;
      planePos = new svVector3[planeSize];
      planeVel = new svVector3[planeSize];
      planeInbox = new svBool[planeSize];
      planeMagcluster = new svInt[planeSize];
      planeVelcluster = new svInt[planeSize]; 
      planeDen = new svScalar[planeSize];
      planeExp = new svScalar[planeSize];
      planeCoe = new svScalar[planeSize];
      count = 0;
      for(int i = 0; i<dataSize;i++)
      {
	if(planeRaw[i])
	{
	  planePos[count] = dataPos[i];
	  planeVel[count] = dataVel[i];
	  count++;
	}
      }
    }
    else
    {
	//cerr<<"plane"<<contourSize<<endl;
      bool *contourPlane = new bool[contourSize];
	//cerr<<"plane"<<endl;
      contourPlane = qdotCut->GetContourSlicePos(contourPos, contourSize, zdistance,
			   plane_center, plane_vector);
	//cerr<<"plane"<<endl;
      int count = 0;
      for(int i=0;i<contourSize;i++)
      {
	if(contourPlane[i])count++;
      }
	//cerr<<"plane"<<endl;
      planeSize = count;
      planePos = new svVector3[planeSize];
      planeVel = new svVector3[planeSize];
      planeInbox = new svBool[planeSize];
      planeMagcluster = new svInt[planeSize];
      planeVelcluster = new svInt[planeSize]; 
      planeDen = new svScalar[planeSize];
      planeExp = new svScalar[planeSize];
      planeCoe = new svScalar[planeSize];
      count = 0;
	//cerr<<"plane"<<endl;
      for(int i = 0; i<contourSize;i++)
      {
	if(contourPlane[i])
	{
	  planePos[count] = contourPos[i];
	  planeVel[count] = contourVel[i];
	  count++;
	}
      }
	//cerr<<"plane"<<endl;
      delete [] contourPlane;
    }
    
    delete qdotCut;
  }
#endif
}
*/

void svQDOT::getPlane(svVector3 plane_center, svVector3 plane_vector,
			svScalar plane_d, vector<bool> selected_plane)
{ 

	/*vector< vector<svVector3> > layerContourPos;
	vector< vector<svVector3> >  layerContourVel;
	vector< vector<svScalar> >  layerContourDen;
	vector< vector<svScalar> >  layerContourExp;
	vector< vector<svScalar> >  layerContourCoe;	
	vector< vector<svInt> >  layerContourAtom;
	svInt layerContourSize;
	vector< vector<svInt> >layerContourCluster;
	vector< vector<svInt> > layerDenContourCluster;*/

	//plane_d = 0.5 * sqrt(2);

	double sum = plane_vector[0] * plane_vector[0]	
			+ plane_vector[1] * plane_vector[1]
			+ plane_vector[2] * plane_vector[2];
	sum = sqrt(sum);
	plane_vector[0] = plane_vector[0]/sum;
	plane_vector[1] = plane_vector[1]/sum;
	plane_vector[2] = plane_vector[2]/sum;

	cerr<<plane_vector[0]<<" "<<plane_vector[1]<<" "<<plane_vector[2]<<" "<<plane_center[0]<<" "<<plane_center[1]<<" "<<plane_center[2]<<endl;
	
	for(int i=0;i<layerPos.size();i++)
	{
		layerPos[i].clear();
		layerVel[i].clear();
		layerDen[i].clear();
		layerExp[i].clear();
		layerCoe[i].clear();  
		layerAtom[i].clear();
		layerCluster[i].clear();
		layerDenCluster[i].clear();
		layerInbox[i].clear();
		layerIndex[i].clear();
	}
	layerPos.clear();
	layerVel.clear();
	layerDen.clear();
	layerExp.clear();
	layerCoe.clear();
	layerAtom.clear();
	layerCluster.clear();  
	layerDenCluster.clear();
	layerInbox.clear();
	layerIndex.clear();


	planeCenter.clear();
	planeNormal.clear();

	layerSize = 0;
	
	svVector3 min_p = plane_center;
	svVector3 max_p = plane_center;	
	while(1)
	{
		min_p[0] = min_p[0] - plane_vector[0] * plane_d;
		min_p[1] = min_p[1] - plane_vector[1] * plane_d;
		min_p[2] = min_p[2] - plane_vector[2] * plane_d;

		svVector3 A,B, dd1, dd2;
		bool flag = planeBox(lbbox, rbbox, min_p ,plane_vector);
		
		//cout<<min_p[0]<<" "<<min_p[1]<<" "<<min_p[2]<<" "<<plane_d<<" "<<flag<<endl;

		if(flag == false)
		{
			min_p[0] = min_p[0] + plane_vector[0] * plane_d;
			min_p[1] = min_p[1] + plane_vector[1] * plane_d;
			min_p[2] = min_p[2] + plane_vector[2] * plane_d;
			break;
		}
	}	
	while(1)
	{
		max_p[0] = max_p[0] + plane_vector[0] * plane_d;
		max_p[1] = max_p[1] + plane_vector[1] * plane_d;
		max_p[2] = max_p[2] + plane_vector[2] * plane_d;

		svVector3 A,B, dd1, dd2;
		bool flag = planeBox(lbbox, rbbox, max_p ,plane_vector);
		
		if(flag == false)
		{ 
			max_p[0] = max_p[0] - plane_vector[0] * plane_d;
			max_p[1] = max_p[1] - plane_vector[1] * plane_d;
			max_p[2] = max_p[2] - plane_vector[2] * plane_d;
			break;
		}
	}
	int n = sqrt((max_p[0] - min_p[0]) * (max_p[0] - min_p[0])
		+ (max_p[1] - min_p[1]) * (max_p[1] - min_p[1])
		+ (max_p[2] - min_p[2]) * (max_p[2] - min_p[2]))/plane_d + 1;
	//cerr<<n<<endl;

	layerPos.resize(n);
	layerDen.resize(n);
	layerVel.resize(n);
	layerExp.resize(n);
	layerCoe.resize(n);
	layerAtom.resize(n);
	layerCluster.resize(n);
	layerDenCluster.resize(n);
	layerInbox.resize(n);
	layerIndex.resize(n);



	int min;
	float mind;
	mind = log10(min_density_nonzero);
	min = (int)log10(min_density_nonzero);
	if(fabs((float)min - mind)<1e-20) min = (int)mind;
	else min = (int)mind - 1;


	int count = 0;
	//int tttt = 0;
	cout<<min_p[0]<<" "<<min_p[1]<<" "<<min_p[2]<<" "<<plane_d<<endl;
	for(int i=0;i<dataSize;i++)
	{
		svVector3 tmp;
		tmp[0] = dataPos[i][0] - min_p[0];
		tmp[1] = dataPos[i][1] - min_p[1];
		tmp[2] = dataPos[i][2] - min_p[2];
		double dot = GetDot(tmp, plane_vector);

		if(fabs(dataPos[i][0] - 0.5)<1e-9
		&& fabs(dataPos[i][1] - 0.5)<1e-9
		&& fabs(dataPos[i][2]) < 1e-9)
		{
			cerr<<"dot " <<i<<" "<<dataPos[i][0]<<" "<<dataPos[i][1]<<" "<<dataPos[i][2]<<" "<<dot<<" "<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<fmod((double)dot, (double)plane_d)<<endl;
		}

		if((fmod(dot, (double)plane_d)<0.00001)) //detect if the points on the planes
		{
			int index = (int)(dot/plane_d);
			layerPos[index].push_back(dataPos[i]);//cerr<<index<<" p"<<endl;
			layerVel[index].push_back(dataVel[i]);//cerr<<index<<" v"<<endl;
			layerDen[index].push_back(dataDen[i]);//cerr<<index<<" d"<<endl;
			layerExp[index].push_back(dataExp[i]);//cerr<<index<<" e"<<endl;
			layerCoe[index].push_back(dataCoe[i]);//cerr<<index<<" c"<<endl;
			layerAtom[index].push_back(dataAtom[i]);//cerr<<index<<" a"<<endl;
			layerCluster[index].push_back(-1);//cerr<<index<<" cc"<<endl;
			if(fabs(dataCoe[i])<1e-9)
			{
				layerDenCluster[index].push_back(0);
			}
			else
			{
				layerDenCluster[index].push_back((int)dataExp[i] - (int)(min));
			}//cerr<<index<<" dc"<<endl;
			layerInbox[index].push_back(true);//cerr<<index<<" in"<<endl;
			layerIndex[index].push_back(i);
			if(selected_plane[index])
			{
				dataInbox[i] = true;
				count++;
			}
			else
			{
				dataInbox[i] = false;
			}
		}
		else if((plane_d-fmod(dot, (double)plane_d)<0.00001)) //detect if the points on the planes
		{
			int index = (int)(dot/plane_d)+1;
			layerPos[index].push_back(dataPos[i]);//cerr<<index<<" p"<<endl;
			layerVel[index].push_back(dataVel[i]);//cerr<<index<<" v"<<endl;
			layerDen[index].push_back(dataDen[i]);//cerr<<index<<" d"<<endl;
			layerExp[index].push_back(dataExp[i]);//cerr<<index<<" e"<<endl;
			layerCoe[index].push_back(dataCoe[i]);//cerr<<index<<" c"<<endl;
			layerAtom[index].push_back(dataAtom[i]);//cerr<<index<<" a"<<endl;
			layerCluster[index].push_back(-1);//cerr<<index<<" cc"<<endl;
			if(fabs(dataCoe[i])<1e-9)
			{
				layerDenCluster[index].push_back(0);
			}
			else
			{
				layerDenCluster[index].push_back((int)dataExp[i] - (int)(min));
			}//cerr<<index<<" dc"<<endl;
			layerInbox[index].push_back(true);//cerr<<index<<" in"<<endl;
			layerIndex[index].push_back(i);
			if(selected_plane[index])
			{
				dataInbox[i] = true;
				count++;
			}
			else
			{
				dataInbox[i] = false;
			}
		}
		else
		{
			dataInbox[i] = false;
		}
	}//cerr<<"count"<<count<<endl;

	for(int i=0;i<n;i++)
	{
		planeCenter.push_back(min_p);
		planeNormal.push_back(plane_vector);

		min_p[0] = min_p[0] + plane_vector[0] * plane_d;
		min_p[1] = min_p[1] + plane_vector[1] * plane_d;
		min_p[2] = min_p[2] + plane_vector[2] * plane_d;
	}
	
	/*for(int i=0;i<n;i++)
	{
		getPlane(min_p, plane_vector, i);
		min_p[0] = min_p[0] + plane_vector[0] * plane_d;
		min_p[1] = min_p[1] + plane_vector[1] * plane_d;
		min_p[2] = min_p[2] + plane_vector[2] * plane_d;		
	}*/
	//layerInbox = new svBool[layerSize];
}   

void svQDOT::getPlane(vector<svVector3> plane_center, svVector3 plane_vector)
{ 
/*	for(int i=0;i<layerPos.size();i++)
	{
		layerPos[i].clear();
		layerVel[i].clear();
		layerDen[i].clear();
		layerExp[i].clear();
		layerCoe[i].clear();  
		layerCluster[i].clear();
		layerDenCluster[i].clear();
	}
	layerPos.clear();
	layerVel.clear();
	layerDen.clear();
	layerExp.clear();
	layerCoe.clear();
	layerCluster.clear();  
	layerDenCluster.clear();

	if(layerSize == 0) delete [] layerInbox;
	layerSize = 0;
	
	
	int n = plane_center.size();

	layerPos.resize(n);
	layerDen.resize(n);
	layerVel.resize(n);
	layerExp.resize(n);
	layerCoe.resize(n);
	layerCluster.resize(n);
	layerDenCluster.resize(n);

	for(int i=0;i<n;i++)
	{
		svVector3 min_p = plane_center[i];
		getPlane(min_p, plane_vector, i);
	}

	layerInbox = new svBool[layerSize];*/
}   

void svQDOT::getPlane(svVector3 plane_center, svVector3 plane_vector, int index)
{
// JC
/*
      svCuttingPlane *qdotCut = new svCuttingPlane();

      svVector3 min_distance;
      min_distance[0] = xdistance;
      min_distance[1] = ydistance;
      min_distance[2] = zdistance;


      qdotCut->GetSlicePos(dataPos, dataSize, planeRaw, 
				plane_center, plane_vector);
      int count = 0;
      layerVel[index].resize(layerPos[index].size());
      layerDen[index].resize(layerPos[index].size());
      layerExp[index].resize(layerPos[index].size());
      layerCoe[index].resize(layerPos[index].size());
	layerCluster[index].resize(layerPos[index].size());
	layerDenCluster[index].resize(layerPos[index].size());

	int min;
	float mind;
	mind = log10(min_density_nonzero);
	min = (int)log10(min_density_nonzero);
	if(fabs((float)min - mind)<1e-20) min = (int)mind;
	else min = (int)mind - 1;

      for(int i=0;i<dataSize;i++)
      {
	if(planeRaw[i])	
	{
		layerVel[index][count]=dataVel[i];
		layerDen[index][count]=dataDen[i];
		layerExp[index][count]=dataExp[i];
		layerCoe[index][count]=dataCoe[i];

		if(fabs(layerCoe[index][count])<1e-9)
		{
			layerDenCluster[index][count] = 0;
		}
		else
		{
			layerDenCluster[index][count] = (int)dataExp[i] - (int)(min);
		}
		layerSize ++;
		count ++;
	}
      }

    delete qdotCut;
*/
}
void svQDOT::planeClustering(bool toggleAttribute, bool toggleROI)
{
#if 0
// JC
/*
  if(planeSize != dataSize)
  {
    //svANNcluster *qdotANN = new svANNcluster();
    
    if(!toggleAttribute)
    {
      qdotANN->ANN_Mag_IDX(planeDen, planeSize,
			  range_mag_cluster,
			  planeMagcluster);
      if(!toggleROI)
      {
	for(int i=0;i<planeSize;i++)
	{
	  if(planeInbox[i] == false)
	  {
	    planeMagcluster[i] = -1;
	  }
	}
      }
    }
    else
    {
      qdotANN->ANN_Vel_IDX(dataVel, dataSize,
			  planeVel, planeSize,
			  dataVelcluster, planeVelcluster);
      if(!toggleROI)
      {
	for(int i=0;i<planeSize;i++)
	{
	  if(planeInbox[i] == false)
	  {
	    planeVelcluster[i] = -1;
	  }
	}
      }
    }
    
    //delete qdotANN;
  }
  else
  {
    for(int i=0;i<dataSize;i++)
    {
      planeMagcluster[i] = dataMagcluster[i];
      planeVelcluster[i] = dataVelcluster[i];
    }
  }
*/
#endif
}

//JC
/*
vector< vector<double> > svQDOT::getLayermag(double z)
{
	double zz = (z -dataPos[0][2])/zdistance;

	int index = (int)zz;

	return range_layer_mag_cluster[index];
}
*/

svVector3 * svQDOT::getLayerRawVel(float z)
{	
	int count = 0;
	for(int i=0;i<dataSize;i++)
	{
		if(fabs(dataPos[i][2] - z)<0.0001)
			count++;
	}

	svVector3 *vel = new svVector3[count];
	int j =0;
	for(int i=0;i<dataSize;i++)
	{
		if(fabs(dataPos[i][2] - z)<0.0001)
		{
			vel[j] = dataVel[i];
			j++;
		}
	}
	return vel;
}


//JC
/*
svInt * svQDOT::getRawLayerVelcluster(float z)
{
	int count = 0;
	for(int i=0;i<dataSize;i++)
	{
		if(fabs(dataPos[i][2] - z)<0.0001)
			count++;
	}

	svInt *vel = new svInt[count];
	int j=0;
	for(int i=0;i<dataSize;i++)
	{
		if(fabs(dataPos[i][2] - z)<0.0001)
		{
			vel[j] = layerVelcluster[i];
			j++;
		}
	}
	return vel;
}
*/

int svQDOT::getRawLayerSize(float z)
{
	int count = 0;
	for(int i=0;i<dataSize;i++)
	{
		if(fabs(dataPos[i][2] - z)<0.0001)
			count++;
	}

	return count;
}

svVector3 *svQDOT::getLayerPos()
{
	int count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			count++;
		}
	}
	svVector3 *c = new svVector3[count];
	count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			c[count] = layerPos[i][j];
			count++;
		}
	}
	return c;
}
svVector3 *svQDOT::getLayerVel()
{
	int count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			count++;
		}
	}
	svVector3 *c = new svVector3[count];
	count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			c[count] = layerVel[i][j];
			count++;
		}
	}
	return c;
}
svScalar *svQDOT::getLayerDen()
{
	int count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			count++;
		}
	}
	svScalar *c = new svScalar[count];
	count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			c[count] = layerDen[i][j];
			count++;
		}
	}
	return c;
}
svScalar *svQDOT::getLayerExp()
{
	int count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			count++;
		}
	}
	svScalar *c = new svScalar[count];
	count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			c[count] = layerExp[i][j];
			count++;
		}
	}
	return c;
}
svScalar *svQDOT::getLayerCoe()
{
	int count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			count++;
		}
	}
	svScalar *c = new svScalar[count];
	count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			c[count] = layerCoe[i][j];
			count++;
		}
	}
	return c;
}
svInt svQDOT::getLayerSize()
{
	int count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			count++;
		}
	}
	return count;
}
int *svQDOT::getLayerCluster()
{
	int count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			count++;
		}
	}
	int *c = new int[count];
	count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			c[count] = layerCluster[i][j];
			count++;
		}
	}
	return c;
}
int *svQDOT::getLayerDenCluster()
{
	int count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			count++;
		}
	}
	int *c = new int[count];
	count = 0;
	for(int i=0;i<layerPos.size();i++)
	{
		for(int j=0;j<layerPos[i].size();j++)
		{
			c[count] = layerDenCluster[i][j];
			count++;
		}
	}
	return c;
}

void svQDOT::cleanup()
{
	delete [] dataDir;
	delete [] dataFile;

	//delete [] qdotROI;
	//delete [] qdotANN;


	delete [] dataPos;
	delete [] dataVel;
	delete [] dataDen;
	delete [] dataExp;
	delete [] dataCoe;
	delete [] dataDis;
	delete [] dataInbox;
	delete [] dataAtom;
	delete [] dataRegion;
	delete [] dataCluster;

	regionIndex.clear();
	for(int i=0;i<cellPos.size();i++)
	{
		cellPos[i].clear();
	}
	cellPos.clear();
	for(int i=0;i<cellVel.size();i++)
	{
		cellVel[i].clear();
	}
	cellVel.clear();
	for(int i=0;i<cellDen.size();i++)
	{
		cellDen[i].clear();
	}
	cellDen.clear();
	for(int i=0;i<cellIndex.size();i++)
	{
		for(int j=0;j<cellIndex[i].size();j++)
		{
			cellIndex[i][j].clear();
		}
		cellIndex[i].clear();
	}	
	cellIndex.clear();

	delete [] contourPos;
	delete [] contourVel;
	delete [] contourDen;
	delete [] contourExp;
	delete [] contourCoe;
	delete [] contourAtom;

	for(int i=0;i<dataFormat.size();i++)
	{	for(int j=0;j<dataFormat[i].size();j++)
			dataFormat[i][j].clear();
		dataFormat[i].clear();}
	dataFormat.clear();

	contourSize = 0;
	dataSize = 0;

	planeCenter.clear();
	planeNormal.clear();

	for(int i=0;i<layerPos.size();i++)
	{
		layerPos[i].clear();
		layerVel[i].clear();
		layerDen[i].clear();
		layerExp[i].clear();
		layerCoe[i].clear();  
		layerCluster[i].clear();
		layerDenCluster[i].clear();
		layerInbox[i].clear();
		layerIndex[i].clear();
	}
	layerPos.clear();
	layerCluster.clear();
	layerDenCluster.clear();
	layerVel.clear();
	layerDen.clear();
	layerExp.clear();
	layerCoe.clear();  
	layerInbox.clear();
	layerIndex.clear();

	layerSize = 0;
	layerContourSize = 0;
	
	for(int i=0;i<layerContourPos.size();i++)
	{
		layerContourPos[i].clear();
		layerContourVel[i].clear();
		layerContourDen[i].clear();
		layerContourExp[i].clear();
		layerContourCoe[i].clear();  	
		layerContourAtom[i].clear();
		layerContourCluster[i].clear();
		layerDenContourCluster[i].clear();
		//layerInbox[i].clear();
	}
	layerContourPos.clear();
	layerContourCluster.clear();
	layerDenContourCluster.clear();
	layerContourVel.clear();
	layerContourDen.clear();
	layerContourExp.clear();
	layerContourCoe.clear();  
	layerContourAtom.clear();

	layer_density.clear();
	
//JC
/*
	for(int i=0;i<dataROI.size();i++)
		dataROI[i].clear();
	dataROI.clear();
	delete [] dataMagcluster;
	delete [] dataVelcluster;
	delete [] layerMagcluster;
	delete [] layerVelcluster;	

	symmetryV.clear();
	symmetryN.clear();


	delete [] contourInbox;
	vector< vector<svInt> > contourROI;
	delete [] contourMagcluster;
	delete [] contourVelcluster;
      	delete [] contourLayerden; //not used

	delete [] planePos;
	delete [] planeVel;
	delete [] planeDen;
	delete [] planeExp;
	delete [] planeCoe;
	delete [] planeInbox;
	delete [] planeRaw;
	delete [] planeIndex;
	delete [] planeMagcluster;
	delete [] planeVelcluster;

	for(int i=0;i<range_mag_cluster.size();i++)
		range_mag_cluster[i].clear();
	range_mag_cluster.clear();
*/

// JC
	//delete [] qdotROI;
	//delete [] qdotANN;

}

}
