#include "svDistance.h"

namespace __svl_lib {
svDistance::svDistance(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe,
		svInt size)
{
  dataSize = 0;
  SetData(pos, vel, den, exp, coe, size);
}

void svDistance::SetData(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe,
		svInt size)
{
  if(dataSize > 0)
  {
	cleanup();
  }

  dataSize = size;  
 
  dataPos = new svVector3[dataSize];
  dataVel = new svVector3[dataSize];
  dataDen = new svScalar[dataSize];
  dataExp = new svScalar[dataSize];
  dataCoe = new svScalar[dataSize];
  dataDistance = new svScalar[dataSize];
 
  Z_format.push_back(0);
  for(int i=0;i<dataSize;i++)
  {     
    dataPos[i] = pos[i];
    dataVel[i] = normalize(vel[i]);
    dataDen[i] = den[i];
    dataExp[i] = exp[i];
    dataCoe[i] = coe[i];
    
    if(i>0)
    {
      if(dataPos[i][2]>dataPos[i-1][2])
      {
	Z_format.push_back(i);

      }
    }
  }
  Z_format.push_back(dataSize);
  
  
}

void svDistance::SetDirection(vector< svVector3> v, vector<svScalar> z)
{
  direction_vel.clear();
  direction_z.clear();
  select_index.clear();
  
  
  int num = v.size();
  
  for(int i=0;i<num;i++)
  {
    direction_vel.push_back(v[i]);
    direction_z.push_back(z[i]);
    //cerr<<z[i]<<" "<<v[i][0]<<" "<<v[i][1]<<" "<<v[i][2]<<endl;
  }

  ANN_Montecarlo();
  CosDistance();
}

void svDistance::ANN_Montecarlo()
{
	//cerr<<v[0]<<" "<<v[1]<<" "<<v[2]<<endl;
	int	nPts;					// actual number of data points
	ANNpointArray	dataPts;				// data points
	ANNpoint	queryPt;				// query point
	ANNidxArray	nnIdx;					// near neighbor indices
	ANNdistArray    dists;					// near neighbor distances
	ANNkd_tree*	kdTree;					// search structure
	nnIdx = new ANNidx[5];						// allocate near neigh indices
	dists = new ANNdist[5];						// allocate near neighbor dists
	int *p[5];
	vector<double> tmpv(3);

	int z = decide_layer;
	//for(int i=0;i<vnum;i++)
		//cerr<<vectorz[i]<<endl;

	nPts = Z_format[decide_layer+1] - Z_format[decide_layer];
	int dim = 3;
	int maxPts = nPts*dim;
	p[0] = new int[nPts];
	p[1] = new int[nPts];
	p[2] = new int[nPts];
	p[3] = new int[nPts];
	p[4] = new int[nPts];
	queryPt = annAllocPt(dim);					// allocate query point
	dataPts = annAllocPts(maxPts, dim);			// allocate data points

	for(int i=0;i<nPts;i++)
	{
		dataPts[i][0] = dataPos[i + Z_format[decide_layer]][0];
		dataPts[i][1] = dataPos[i + Z_format[decide_layer]][1];
		dataPts[i][2] = dataPos[i + Z_format[decide_layer]][2];
	}

	kdTree = new ANNkd_tree(	dataPts,					// the data points
					nPts,						// number of points
					dim);						// dimension of space	
	for(int i=0;i<nPts;i++)
	{
		queryPt[0] = dataPos[i + Z_format[decide_layer]][0];
		queryPt[1] = dataPos[i + Z_format[decide_layer]][1];
		queryPt[2] = dataPos[i + Z_format[decide_layer]][2];

		kdTree->annkSearch(									// search
				queryPt,						// query point
				5,								// number of near neighbors
				nnIdx,							// nearest neighbors (returned)
				dists,							// distance (returned)
				0.0);		// error bound

		p[0][i] = nnIdx[0];
		p[1][i] = nnIdx[1];
		p[2][i] = nnIdx[2];
		p[3][i] = nnIdx[3];
		p[4][i] = nnIdx[4];
	}

	delete [] nnIdx;
	delete [] dists;
	nnIdx = new ANNidx[1];
	dists = new ANNdist[1];

	int t = 0;
	int tt = 0;
	for(int i=0;i<Z_format.size()-1;i++)
	{//cerr<<i<<endl;
		if(i!=z)
		{

			double fractpart[2];
			double intpart[2];
			fractpart[0] = modf(dataPos[Z_format[i]][2], &intpart[0]);
			//fractpart[1] = modf(pos[z][0][2], &intpart[1]);
			if(fractpart[0] < 0)
			{
				fractpart[0] = fractpart[0] + 1.;
				fractpart[0] = modf(fractpart[0], &intpart[0]);
			}
			for(int iii=0;iii<direction_z.size();iii++)
			{
				if(fabs(fractpart[0] - direction_z[iii])<0.00001)
				{
					tmpv[0] = direction_vel[iii][0];
					tmpv[1] = direction_vel[iii][1];
					tmpv[2] = direction_vel[iii][2];
					break;
				}
			}
			//cerr<<tmpv[2]<<endl;


			//cerr<<pos[i].size()<<" "<<endl;
			for(int j=0;j<Z_format[i+1] - Z_format[i];j++)
			{
			  //cerr<<i<<" "<<j<<endl;
				queryPt[0] = dataPos[Z_format[i] + j][0];
				queryPt[1] = dataPos[Z_format[i] + j][1];
				queryPt[2] = dataPos[Z_format[i] + j][2];

				kdTree->annkSearch(									
						queryPt,						
						1,								
						nnIdx,							
						dists,							
						0.0);		// error bound
				int ii = z;
				int jj = nnIdx[0];
				int jj2 = -1;
				float dd[5];
				float td[5];
				float dd_min;
				float td_min;
				int index_min;
				float sum;
				vector<double> nv(3);
				int index_m;
				td[0] = sqrt((dataPos[Z_format[ii] + jj][0] - dataPos[Z_format[i]+j][0]) * 
						(dataPos[Z_format[ii] + jj][0] - dataPos[Z_format[i]+j][0]) +
						(dataPos[Z_format[ii] + jj][1] - dataPos[Z_format[i]+j][1]) * 
						(dataPos[Z_format[ii] + jj][1] - dataPos[Z_format[i]+j][1]));
				
				if(td[0] == 0. && tmpv[0] == 0. && tmpv[1] == 0. && tmpv[2] == 0.)
				{
					//n1[t] = z;
					//n2[t] = jj;
					select_index.push_back(Z_format[z] + jj);
				}
				else
				{
					
					if(tmpv[0] == 0. && tmpv[1] == 0. && tmpv[2] == 0.)
					{
					//	jj = -1;
					}//
					else
					{
					  nv[0] = (dataPos[Z_format[ii] + jj][0] - dataPos[Z_format[i]+j][0]) / td[0];
					  nv[1] = (dataPos[Z_format[ii] + jj][1] - dataPos[Z_format[i]+j][1]) / td[0];
					  dd[0] = nv[0] * tmpv[0] + nv[1] * tmpv[1];	
					  td_min = td[0];
					  dd_min = dd[0];
					  index_min = jj;
					  index_m = 0;
					  while(1)
					  {		
					   
						  for(int m=0;m<4;m++)
						  {//cerr<<"m: "<<m<<" "<<p[m+1][jj]<< endl;
							  td[m+1] = sqrt((dataPos[Z_format[ii] + p[m+1][jj]][0] - dataPos[Z_format[i] + j][0]) * 
								  (dataPos[Z_format[ii] + p[m+1][jj]][0] - dataPos[Z_format[i] + j][0]) +
								  (dataPos[Z_format[ii] + p[m+1][jj]][1] - dataPos[Z_format[i] + j][1]) * 
								  (dataPos[Z_format[ii] + p[m+1][jj]][1] - dataPos[Z_format[i] + j][1]));
							  nv[0] = (dataPos[Z_format[ii] + p[m+1][jj]][0] - dataPos[Z_format[i] + j][0]) / td[m+1];
							  nv[1] = (dataPos[Z_format[ii] + p[m+1][jj]][1] - dataPos[Z_format[i] + j][1]) / td[m+1];
							  dd[m+1] = nv[0] * tmpv[0] + nv[1] * tmpv[1];

							  //if(pos[i][j][0] == 14.25 && pos[i][j][1] == 16.75)
							  //	cerr<<m+1<<" "<<td[m+1]<<" "<<dd[m+1]<<" "<<pos[ii][p[m+1][jj]][0]<<" "<<pos[ii][p[m+1][jj]][1]<<" "<<pos[ii][p[0][jj]][0]<<" "<<pos[ii][p[0][jj]][1]<<endl;

							  if(fabs (dd[m+1] - dd_min) < 1e-10 )
							  {
								  if(td[m+1] <= td_min && p[m+1][jj]!=jj2)
								  {
									  index_min = p[m+1][jj];
									  index_m = m+1;
									  dd_min = dd[m+1];
									  td_min = td[m+1];
								  }
							  }
							  else if(dd[m+1] > dd_min)// && td[m+1]<=td_min )|| td_min ==0)
							  {
								  index_min = p[m+1][jj];
								  index_m = m+1;
								  dd_min = dd[m+1];
								  td_min = td[m+1];
							  }
						  }
						  
						 // if(i == 2 && j == 5012)
						  //  cerr<<dd_min<<" "<<td_min<<" "<<index_min<<" "<<jj<<endl;

						  if(fabs(dd_min - 1.) < 1e-10 && index_min == jj)
						  {
							  break;
						  }
						  else if(index_min == jj)
						  {
						  //cerr<<pos[i][j][0]<<" "<<pos[i][j][1]<<" "<<" "<<pos[ii][index_min][0]<<" "<<pos[ii][index_min][1]<<" "<<index_min<<" "<<index_m<<" "<<td_min<<" "<<jj<<" "<<dd_min<<endl;
							  //jj = -1;
							  break;
						  }
						  else
						  {
							  jj2 = jj;
							  jj = index_min;
							  td[0] = td[index_m];
							  dd[0] = dd[index_m];
						  }
					  }
					}
					tt++;

					//n1[t] = z;
					//n2[t] = jj;
					select_index.push_back(Z_format[z] + jj);
					//if(pos[i][j][0]==15.75&&pos[i][j][1]==16.75){
					//cerr<<jj<<" "<<dd_min<<" "<<td_min<<" "<<pos[i][j][0]<<" "<<pos[i][j][1]<<" "<<pos[z][index_min][0]<<" "<<pos[z][index_min][1]<<endl;}

				}
				t++;
			}
		}
		else
		{
			for(int j=0;j<Z_format[i+1] - Z_format[i];j++)
			{
				//n1[t] = -1;
				//n2[t] = -1;
				select_index.push_back(Z_format[i] + j);
				t++;
			}
		}
	}	
}

void svDistance::SetSelectedLayer(int n)
{
    if(n < Z_format.size()-1)
    {
      select_layer = n;
	decide_layer = n;
    }
    else
    {
     select_layer =  Z_format.size()-2;
	decide_layer=Z_format.size()-2;
    }
}

void svDistance::CosDistance()
{
  int count = 0;
  //cerr<<select_index.size()<<endl;
  for(int i=0;i<Z_format.size()-1;i++)
  {
    for(int j=Z_format[i];j< Z_format[i+1];j++)
    {
      double cos = dataVel[j][0] * dataVel[select_index[j]][0]
		+  dataVel[j][1] * dataVel[select_index[j]][1]
		+  dataVel[j][2] * dataVel[select_index[j]][2];
      dataDistance[count] = cos+1;
//	if(dataDistance[count] >2)cerr<<dataDistance[count]<<endl;
      count++;
    }
  }
/*
  ofstream outfile("distance_text");
  for(int i=0;i<dataSize;i++)
  {
	outfile<<dataDistance[i]<<endl;
  }
  outfile.close();
*/
}
void svDistance::getDistance(svScalar *dis)
{
	for(int i=0;i<dataSize;i++)
		dis[i] = dataDistance[i];
}
void svDistance::cleanup()
{
  delete [] dataPos;
  delete [] dataVel;
  delete [] dataDen;
  delete [] dataExp;
  delete [] dataCoe;
  delete [] dataDistance;
  Z_format.clear();
  
}
}
