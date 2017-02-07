#include "svDisc.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string.h>
#include <algorithm>
 #define SWAP(T, a, b)   { T t; t=a; a=b; b=t;  }

#define SV_PI     (3.14159265358979323846)
#define SV_2_PI   (3.14159265358979323846 * 2.0)
#define SV_SMALL  (1e-6)

#define svToDegree(x)             ((x)*(180.)/SV_PI)
#define svToRadian(x)              ((x)*SV_PI/(180.))
#define svMax(a,b)                (((a) > (b)) ? (a) : (b))
#define svMin(a,b)                (((a) < (b)) ? (a) : (b))
#define svClamp(v, min_v, max_v)  { v = svMax(svMin(v, max_v), min_v); }


namespace __svl_lib{

svDisc::svDisc()
{
	dataSize = 0;
	numCluster = 0;
	SELECTED =false;
}
void svDisc::SetData(vector< vector<svVector3> > pos, vector< vector<svVector3> > vel,
			vector< vector<svScalar> > den, vector< vector<svScalar> > exp, vector< vector<svScalar> > coe,
			int size)
{
	if(dataSize > 0)
	{
		delete [] dataPos;
		delete [] dataVel;
		delete [] dataDen;
		delete [] dataExp;
		delete [] dataCoe;
		delete [] dataCluster;
		delete [] dataIn;
	}	

	dataSize = size;

	dataPos = new svVector3[dataSize];
	dataVel = new svVector3[dataSize];
	dataDen = new svScalar[dataSize];
	dataExp = new svScalar[dataSize];
	dataCoe = new svScalar[dataSize];
	dataCluster = new svInt[dataSize];
	dataIn = new bool[dataSize];

	svScalar temp;

	int count = 0;
	for(int i=0;i<pos.size();i++)
	{
		for(int j=0;j<pos[i].size();j++)
		{
			dataPos[count] = pos[i][j];
			dataVel[count] = vel[i][j];
			dataDen[count] = den[i][j];
			dataExp[count] = exp[i][j];
			dataCoe[count] = coe[i][j];
			count++;
		}
	}
}
void svDisc::SetData(svVector3 *pos, svVector3 *vel, svScalar *den, 
			svScalar *exp, svScalar *coe, int size)
{
	if(dataSize > 0)
	{
		delete [] dataPos;
		delete [] dataVel;
		delete [] dataDen;
		delete [] dataExp;
		delete [] dataCoe;
		delete [] dataCluster;
		delete [] dataIn;
	}	

	dataSize = size;

	dataPos = new svVector3[dataSize];
	dataVel = new svVector3[dataSize];
	dataDen = new svScalar[dataSize];
	dataExp = new svScalar[dataSize];
	dataCoe = new svScalar[dataSize];
	dataCluster = new svInt[dataSize];
	dataIn = new bool[dataSize];

	svScalar temp;

	for(int i=0;i<dataSize;i++)
	{
		dataPos[i] = pos[i];
		dataVel[i] = vel[i];
		dataDen[i] = den[i];
		dataExp[i] = exp[i];
		dataCoe[i] = coe[i];
		//dataIn[i] = false;
	}
}
void svDisc::SetClusters(svInt *cluster, int num)
{
	//cerr<<"Setcluster"<<endl;
	if(numCluster > 0)
	{
		delete [] countCluster;//cerr<<"Setcluster"<<endl;
		delete [] splitCount;//cerr<<"Setcluster"<<endl;
		delete [] selected_disc;//cerr<<"Setcluster"<<endl;
	}
	//cerr<<"Setcluster"<<endl;
	numCluster = num;

	//int max = -1;
	for(int i=0;i<dataSize;i++)
	{
		dataCluster[i] = cluster[i];
		dataIn[i] = true;
		//if(max < cluster[i]) max = cluster[i];
		//if(dataCluster[i] == 2770) cout<<"haha"<<endl;
	}//cerr<<"---"<<endl;
	//numCluster = max+1;
	numSymmetry = 1;

}
void svDisc::SetClusters(svInt *cluster, int num, bool layer, int zlayer, double zmin, double zdistance)
{
	//cerr<<"Setcluster"<<endl;
	if(numCluster > 0)
	{
		delete [] countCluster;//cerr<<"Setcluster"<<endl;
		delete [] splitCount;//cerr<<"Setcluster"<<endl;
		delete [] selected_disc;//cerr<<"Setcluster"<<endl;
	}
	//cerr<<"Setcluster"<<endl;
	numCluster = num;

	if(layer)
	{
		for(int i=0;i<dataSize;i++)
		{
			int n;
			n = (dataPos[i][2] - zmin)/zdistance;
			dataCluster[i] = cluster[i] + n * num;//cerr<<cluster[i]<<endl;
			dataIn[i] = false;
		}//cerr<<"---"<<endl;
		numCluster = num  * zlayer;
	}
	else
	{
		int max = -1;
		for(int i=0;i<dataSize;i++)
		{
			dataCluster[i] = cluster[i];
			dataIn[i] = false;
			if(max < cluster[i]) max = cluster[i];
			if(dataCluster[i] == 2770) cout<<"haha"<<endl;
		}//cerr<<"---"<<endl;
		numCluster = max+1;
	}
	numSymmetry = 1;
}
void svDisc::SetClusters(vector< vector<int> > cluster, int num)
{
	//cerr<<"Setcluster"<<endl;
	if(numCluster > 0)
	{
		delete [] countCluster;//cerr<<"Setcluster"<<endl;
		delete [] splitCount;//cerr<<"Setcluster"<<endl;
		delete [] selected_disc;//cerr<<"Setcluster"<<endl;
	}
	//cerr<<"Setcluster"<<endl;
	numCluster = num;

	//int max = -1;
	int count = 0;
	for(int i=0;i<cluster.size();i++)
	{
		for(int j=0;j<cluster[i].size();j++)
		{
			dataCluster[count] = cluster[i][j];
			dataIn[count] = true;
			count++;
		}
		//if(max < cluster[i]) max = cluster[i];
		//if(dataCluster[i] == 2770) cout<<"haha"<<endl;
	}//cerr<<"---"<<endl;
	//numCluster = max+1;
	numSymmetry = 1;
}
void svDisc::SetSymmetry(vector<svVector3> v)
{
	/*cerr<<"s"<<v.size()<<endl;
	if(v.size()==1)
	{
		numSymmetry = 2;
		if(v[0][0] > 0)//y=-x
		{
			for(int i=0;i<dataSize;i++)
			{
				if(dataPos[i][0] < -dataPos[i][1])
				{
					dataCluster[i] = dataCluster[i] + numCluster;
				}	
			}
		}
		else
		if(v[0][0] > 0)//y=-x
		{
			for(int i=0;i<dataSize;i++)
			{
				if(dataPos[i][0] < dataPos[i][1])
				{
					dataCluster[i] = dataCluster[i] + numCluster;
				}	
			}
		}
	}
	else if(v.size()==2)	
	{
		numSymmetry = 4;
		for(int i=0;i<dataSize;i++)
		{
			if(dataPos[i][0] > -dataPos[i][1])
			{
				if(dataPos[i][0] > dataPos[i][1])
					dataCluster[i] = dataCluster[i];
				else
					dataCluster[i] = dataCluster[i] + numCluster;
			}
			else
			{
				if(dataPos[i][0] > dataPos[i][1])
					dataCluster[i] = dataCluster[i]+ numCluster*3;
				else
					dataCluster[i] = dataCluster[i] + numCluster*2;
			}
		}
	}
	else */
	{
		numSymmetry = 1;
	}
}


void svDisc::computeMean(vector< vector<int> > cluster2, int num)//num = # of magnitude cluster
{
	int cc = 0;
	for(int i=0;i<cluster2.size();i++)
	{
		for(int j=0;j<cluster2[i].size();j++)
		{	cc++;}
	}

	int *cluster = new int[cc];
	cc = 0;
	for(int i=0;i<cluster2.size();i++)
	{
		for(int j=0;j<cluster2[i].size();j++)
		{	cluster[cc] = cluster2[i][j];cc++;}
	}	

	//cerr<<numCluster<<" "<<numSymmetry<<endl;
	numDisc =  numCluster;// * numSymmetry;

	int count[numDisc];
	countCluster = new svInt[numDisc];
	selected_disc = new bool[numDisc];
	for(int i=0;i<numDisc;i++)
	{
		countCluster[i] = 0;
		selected_disc[i] =false;
	}
	distributeDen.resize(numDisc);
	cerr<<numDisc<<endl;
	int distanceDen = num;
	cerr<<numDisc<<" "<<num<<endl;
	for(int i=0;i<numDisc;i++)
	{//cerr<<distanceDen<<endl;
		distributeDen[i].resize(distanceDen);
		for(int j=0;j<distanceDen;j++)
		{
			distributeDen[i][j] = 0 ;
		}
		count[i] = 0;
	}
	//cerr<<numDisc<<endl;	
	splitCount = new svScalar[numDisc];
	//cerr<<numDisc<<endl;
	/**
	compute average postion/velocity and distribute of density
	based on each symmetric quads
	(not complete yet)
	**/

	centricPos.resize(numDisc);
	centricDen.resize(numDisc);
	centricVel.resize(numDisc);

	for(int j=0;j<numDisc;j++)
	{
		for(int i=0;i<3;i++)
		{
			centricPos[j][i] = 0;
			centricVel[j][i] = 0;
		}
		centricDen[j] = 0;
	}
	cerr<<numDisc<<endl;
	for(int i=0;i<dataSize;i++)
	{
		if(dataCluster[i]>-1)
		{
			//cerr<<cluster[i]<<" "<<dataCluster[i]<<" "<<numDisc<<" "<<distanceDen<<endl;;

			centricPos[dataCluster[i]][0] = centricPos[dataCluster[i]][0] + dataPos[i][0];
		        centricPos[dataCluster[i]][1] = centricPos[dataCluster[i]][1] + dataPos[i][1];
		        centricPos[dataCluster[i]][2] = centricPos[dataCluster[i]][2] + dataPos[i][2];
		        centricVel[dataCluster[i]][0] = centricVel[dataCluster[i]][0] + dataVel[i][0];
		        centricVel[dataCluster[i]][1] = centricVel[dataCluster[i]][1] + dataVel[i][1];
		        centricVel[dataCluster[i]][2] = centricVel[dataCluster[i]][2] + dataVel[i][2];
		        centricDen[dataCluster[i]] = centricDen[dataCluster[i]] + dataDen[i];	

			if(dataDen[i] > 0 )
			{
				distributeDen[dataCluster[i]][cluster[i]]++;//(int)dataExp[i]-((int)log10(denMin)-1)]++;		
			}


			count[dataCluster[i]] ++;
			countCluster[dataCluster[i]]++;
		}
	}
	cerr<<numDisc<<endl;
	for(int i=0;i<numDisc;i++)
	{
		if(count[i] > 0)
		{
			centricPos[i][0] = centricPos[i][0]/count[i];
		        centricPos[i][1] = centricPos[i][1]/count[i];
                        centricPos[i][2] = centricPos[i][2]/count[i];
                        centricVel[i][0] = centricVel[i][0]/count[i];
                        centricVel[i][1] = centricVel[i][1]/count[i];
                        centricVel[i][2] = centricVel[i][2]/count[i];
                        centricDen[i] = centricDen[i]/count[i];
		}//cerr<<i<<" ";
		for(int j=0;j<distanceDen;j++)
		{
			//if(i<numCluster)

			distributeDen[i][j] = distributeDen[i][j]/(float)count[i];
			//cerr<<distributeDen[i][j]<<" ";

		}//cerr<<endl;
		centricVel[i] = normalize(centricVel[i]);		
	}
	cerr<<numDisc<<endl;
	for(int i=0;i<numDisc;i++)
	{
		svScalar sum = 0;
		for(int j=0;j<numDisc;j++)
		{
			if(j%numCluster == i%numCluster)
			{
				sum = sum + countCluster[j];
			}
		}
		if(sum>0)
			splitCount[i] = countCluster[i]/sum+0.3;
		else
			splitCount[i] = 0;
		//cerr<<i<<" "<<splitCount[i]<<" "<<countCluster[i]<<" "<<sum<<endl;
	}
	cerr<<numDisc<<endl;

	delete [] cluster;
}


void svDisc::computeMean(int *cluster, int num)//num = # of magnitude cluster
{
	//cerr<<numCluster<<" "<<numSymmetry<<endl;
	numDisc =  numCluster;// * numSymmetry;

	int count[numDisc];
	countCluster = new svInt[numDisc];
	selected_disc = new bool[numDisc];
	for(int i=0;i<numDisc;i++)
	{
		countCluster[i] = 0;
		selected_disc[i] =false;
	}
	distributeDen.resize(numDisc);
	cerr<<numDisc<<endl;
	int distanceDen = num;
	cerr<<numDisc<<" "<<num<<endl;
	for(int i=0;i<numDisc;i++)
	{//cerr<<distanceDen<<endl;
		distributeDen[i].resize(distanceDen);
		for(int j=0;j<distanceDen;j++)
		{
			distributeDen[i][j] = 0 ;
		}
		count[i] = 0;
	}
	//cerr<<numDisc<<endl;	
	splitCount = new svScalar[numDisc];
	//cerr<<numDisc<<endl;
	/**
	compute average postion/velocity and distribute of density
	based on each symmetric quads
	(not complete yet)
	**/

	centricPos.resize(numDisc);
	centricDen.resize(numDisc);
	centricVel.resize(numDisc);

	for(int j=0;j<numDisc;j++)
	{
		for(int i=0;i<3;i++)
		{
			centricPos[j][i] = 0;
			centricVel[j][i] = 0;
		}
		centricDen[j] = 0;
	}
	cerr<<numDisc<<endl;
	for(int i=0;i<dataSize;i++)
	{
		if(dataCluster[i]>-1)
		{
			//cerr<<cluster[i]<<" "<<dataCluster[i]<<" "<<numDisc<<" "<<distanceDen<<endl;;

			centricPos[dataCluster[i]][0] = centricPos[dataCluster[i]][0] + dataPos[i][0];
		        centricPos[dataCluster[i]][1] = centricPos[dataCluster[i]][1] + dataPos[i][1];
		        centricPos[dataCluster[i]][2] = centricPos[dataCluster[i]][2] + dataPos[i][2];
		        centricVel[dataCluster[i]][0] = centricVel[dataCluster[i]][0] + dataVel[i][0];
		        centricVel[dataCluster[i]][1] = centricVel[dataCluster[i]][1] + dataVel[i][1];
		        centricVel[dataCluster[i]][2] = centricVel[dataCluster[i]][2] + dataVel[i][2];
		        centricDen[dataCluster[i]] = centricDen[dataCluster[i]] + dataDen[i];	

			if(dataDen[i] > 0 )
			{
				distributeDen[dataCluster[i]][cluster[i]]++;//(int)dataExp[i]-((int)log10(denMin)-1)]++;		
			}


			count[dataCluster[i]] ++;
			countCluster[dataCluster[i]]++;
		}
	}
	cerr<<numDisc<<endl;
	for(int i=0;i<numDisc;i++)
	{
		if(count[i] > 0)
		{
			centricPos[i][0] = centricPos[i][0]/count[i];
		        centricPos[i][1] = centricPos[i][1]/count[i];
                        centricPos[i][2] = centricPos[i][2]/count[i];
                        centricVel[i][0] = centricVel[i][0]/count[i];
                        centricVel[i][1] = centricVel[i][1]/count[i];
                        centricVel[i][2] = centricVel[i][2]/count[i];
                        centricDen[i] = centricDen[i]/count[i];
		}//cerr<<i<<" ";
		for(int j=0;j<distanceDen;j++)
		{
			//if(i<numCluster)

			distributeDen[i][j] = distributeDen[i][j]/(float)count[i];
			//cerr<<distributeDen[i][j]<<" ";

		}//cerr<<endl;
		centricVel[i] = normalize(centricVel[i]);		
	}
	cerr<<numDisc<<endl;
	for(int i=0;i<numDisc;i++)
	{
		svScalar sum = 0;
		for(int j=0;j<numDisc;j++)
		{
			if(j%numCluster == i%numCluster)
			{
				sum = sum + countCluster[j];
			}
		}
		if(sum>0)
			splitCount[i] = countCluster[i]/sum+0.3;
		else
			splitCount[i] = 0;
		//cerr<<i<<" "<<splitCount[i]<<" "<<countCluster[i]<<" "<<sum<<endl;
	}
	cerr<<numDisc<<endl;
}


void svDisc::setColors(svVector4 *color, svVector4 *ccolor, int num, bool layer,int zlayer)
{
	dataColor.resize(num);

	for(int i=0;i<num;i++)
	{
		dataColor[num-i-1] = color[i];
	}

	clusterColor.resize(numCluster);
	if(layer)
	{
		int n = numCluster/zlayer;
		for(int i=0;i<zlayer;i++)
		{
			for(int j=0;j<n;j++)
			{
				clusterColor[i*n + j] = ccolor[j];
				//cerr<<i*n + j<<" "<<j<<endl;
			//cerr<<i*n + j<<" "<<j<<" "<<numCluster<<" "<<clusterColor[i*n + j][0]<<" "<<clusterColor[i*n + j][1]<<" "<<clusterColor[i*n + j][2]<<endl;
			}
		}
	}
	else
	{
		for(int i=0;i<numCluster;i++)
		{
			clusterColor[i] = ccolor[i];

		}
	}
}
void svDisc::renderAnnulus(svVector3 pos, svVector3 vel,
				svScalar x, svScalar H, svScalar r)
{
	double degree =1;
	int times1 = x/degree;	
	int times2 = (360-x)/degree;

	svVector3 norm = svGetPerpendicularVector(vel);
	svVector3 head_out, head_in;
	head_out = pos + H * norm;
	head_in  = pos + r * norm;

	glDisable(GL_LIGHTING);
        glBegin(GL_QUAD_STRIP);
	glNormal3f(vel[0], vel[1], vel[2]);
	glNormal3f(-vel[0], -vel[1], -vel[2]);
        for(int ii = 0; ii <= times1; ii++)
        {
		float i = (float)ii * (x/(float)times1);
		
		svVector3 draw_out = svGetRotatePoint(head_out, pos, vel, i);
		svVector3 draw_in = svGetRotatePoint(head_in, pos, vel, i);  
	 
                glVertex3f(draw_out[0], draw_out[1], draw_out[2]);
                glVertex3f(draw_in[0], draw_in[1], draw_in[2]);
        }
        glEnd();
	

	glEnable(GL_LIGHTING);
	glColor4f(0.8,0.8,0.8,0.5);
        glBegin(GL_QUAD_STRIP);
	glNormal3f(vel[0], vel[1], vel[2]);
	glNormal3f(-vel[0], -vel[1], -vel[2]);
        for(int ii = 0; ii <= times2; ii++)
        {
                float i = x +  (float)ii * ((360-x)/(float)times2);
                
                svVector3 draw_out = svGetRotatePoint(head_out, pos, vel, i);
                svVector3 draw_in = svGetRotatePoint(head_in, pos, vel, i);
         
                glVertex3f(draw_out[0], draw_out[1], draw_out[2]);
                glVertex3f(draw_in[0], draw_in[1], draw_in[2]);
        }
        glEnd();


}

void svDisc::renderDisc(float LINE_LENGTH)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
   	glEnable(GL_COLOR_MATERIAL);
    	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	int numColor = dataColor.size();
	//cerr<<numColor<<endl;

	svScalar H, r;

	//int count = 0;
	for(int i=0;i<numDisc;i++)
	{
		if(countCluster[i]>0 )//!((i==14 || i==7)&& expand) && countCluster[i]>0)
		{//count++;
		H = radius*splitCount[i];
		r = 0;

		int ii = 0;
	
		/**
		draw cones (arrows)
		**/
		glColor3f(0,0,0);
		char str[20];
		sprintf(str, "%d", i);
		//glRasterPos3f(centricPos[i][0], centricPos[i][1], centricPos[i][2]);
		//for(int j=0;j<strlen(str);j++)
		   //  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);


		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
                double rotate_x = acos(centricVel[i][2]);
                if(centricVel[i][1] > 0)
                {
                        rotate_x = -rotate_x;
                }
                double rotate_z = acos(centricVel[i][1]/sqrt(centricVel[i][0]*centricVel[i][0] + centricVel[i][1]*centricVel[i][1]));
                if(rotate_x < 0)
                {
                        if(centricVel[i][0] > 0)
                        {
                                rotate_z = -rotate_z;
                        }
                }
                else
                {
                        rotate_z = M_PI - rotate_z;
                        if (centricVel[i][0] <0)
                        {
                                rotate_z = - rotate_z;
                        }
                }
		
		//if(i==7 ||i == 14)
			glColor4f(clusterColor[i%numCluster][0],
				clusterColor[i%numCluster][1],
				clusterColor[i%numCluster][2],
				1);
		//else
		//	glColor4f(0.5,0.5,0.5,0.5);
                glPushMatrix();
                glTranslatef(centricPos[i][0]+LINE_LENGTH * centricDen[i] * centricVel[i][0],
			centricPos[i][1]+LINE_LENGTH  * centricDen[i] * centricVel[i][1],
			centricPos[i][2]+LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
                glRotatef(rotate_z/M_PI*180,0,0,1);
                glRotatef(rotate_x/M_PI*180,1,0,0);
                glutSolidCone(radius*1.5*splitCount[i], radius*6*splitCount[i], 10, 1);
                glPopMatrix();

		/**
		draw the line with halos
		**/

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glLineWidth(3.);
                glColor4f(0.5,0.5,0.5,0.5);
		glBegin(GL_LINES);
		glVertex3f(centricPos[i][0] - LINE_LENGTH  * centricDen[i] * centricVel[i][0],
			centricPos[i][1] - LINE_LENGTH  * centricDen[i] * centricVel[i][1],
			centricPos[i][2] - LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
                glVertex3f(centricPos[i][0] + LINE_LENGTH  * centricDen[i] * centricVel[i][0],
                        centricPos[i][1] + LINE_LENGTH  * centricDen[i] * centricVel[i][1],
                        centricPos[i][2] + LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
		glEnd();

                glLineWidth(6.);
                glColor4f(0,0,0,0.5);
                glBegin(GL_LINES);
                glVertex3f(centricPos[i][0] - LINE_LENGTH  * centricDen[i] * centricVel[i][0],
                        centricPos[i][1] - LINE_LENGTH  * centricDen[i] * centricVel[i][1],
                        centricPos[i][2] - LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
                glVertex3f(centricPos[i][0] + LINE_LENGTH * centricDen[i] *centricVel[i][0],
                        centricPos[i][1] + LINE_LENGTH  * centricDen[i] * centricVel[i][1],
                        centricPos[i][2] + LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
                glEnd();


		/**
		darw annulus (discs)
		**/
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 2);

		for(int j=0;j<dataColor.size();j++)
		{
			glColor4f(dataColor[j][0],
					dataColor[j][1],
					dataColor[j][2],1);
			renderAnnulus(centricPos[i],centricVel[i],
					distributeDen[i][j]*360,
					H, r);
			H = H + radius*splitCount[i];
			r = r + radius*splitCount[i];
		}
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glLineWidth(0.5);


		glColor4f(0.5,0.5,0.5,0.5);
		svVector3 norm = svGetPerpendicularVector(centricVel[i]);
		svVector3 head_out;
		svVector3 pos;
		svScalar moveD = 0.0;
		pos = centricPos[i] + moveD * centricVel[i];
		head_out = pos + (svScalar)(H-radius) * norm;
		for(int iii = 0; iii < 360; )
		{
			glBegin(GL_LINES);
			svVector3 draw_out = svGetRotatePoint(head_out, pos, centricVel[i], (float)iii);
		        glVertex3f(draw_out[0], draw_out[1], draw_out[2]);
		        glVertex3f(pos[0], pos[1], pos[2]);
			glEnd();

			iii = iii + 45;
		}

		glLineWidth(1.);

		glDisable(GL_POLYGON_OFFSET_FILL);

		}
	}

	//cerr<<count<<endl;
}
void svDisc::renderDisc(bool layer, float zvalue, float LINE_LENGTH)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
   	glEnable(GL_COLOR_MATERIAL);
    	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//glEnable(GL_NORMALIZE);

	int numColor = dataColor.size();
	//cerr<<numColor<<endl;

	svScalar H, r;

	//int count = 0;
	for(int i=0;i<numDisc;i++)
	{
		if((!layer) || (layer && fabs(centricPos[i][2] - zvalue)<0.001))
		if(countCluster[i]>0 )//!((i==14 || i==7)&& expand) && countCluster[i]>0)
		{//count++;
		H = radius*splitCount[i];
		r = 0;

		int ii = 0;
	
		/**
		draw cones (arrows)
		**/
		glColor3f(0,0,0);
		char str[20];
		sprintf(str, "%d", i);
		//glRasterPos3f(centricPos[i][0], centricPos[i][1], centricPos[i][2]);
		//for(int j=0;j<strlen(str);j++)
		   //  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);


		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
                double rotate_x = acos(centricVel[i][2]);
                if(centricVel[i][1] > 0)
                {
                        rotate_x = -rotate_x;
                }
                double rotate_z = acos(centricVel[i][1]/sqrt(centricVel[i][0]*centricVel[i][0] + centricVel[i][1]*centricVel[i][1]));
                if(rotate_x < 0)
                {
                        if(centricVel[i][0] > 0)
                        {
                                rotate_z = -rotate_z;
                        }
                }
                else
                {
                        rotate_z = M_PI - rotate_z;
                        if (centricVel[i][0] <0)
                        {
                                rotate_z = - rotate_z;
                        }
                }
		
		//if(i==7 ||i == 14)
			glColor4f(clusterColor[i%numCluster][0],
				clusterColor[i%numCluster][1],
				clusterColor[i%numCluster][2],
				1);
		//else
		//	glColor4f(0.5,0.5,0.5,0.5);
                glPushMatrix();
                glTranslatef(centricPos[i][0]+LINE_LENGTH * centricDen[i] * centricVel[i][0],
			centricPos[i][1]+LINE_LENGTH  * centricDen[i] * centricVel[i][1],
			centricPos[i][2]+LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
                glRotatef(rotate_z/M_PI*180,0,0,1);
                glRotatef(rotate_x/M_PI*180,1,0,0);
                glutSolidCone(radius*1.5*splitCount[i], radius*6*splitCount[i], 10, 1);
                glPopMatrix();

		/**
		draw the line with halos
		**/

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glLineWidth(3.);
                glColor4f(0.5,0.5,0.5,0.5);
		glBegin(GL_LINES);
		glVertex3f(centricPos[i][0] - LINE_LENGTH  * centricDen[i] * centricVel[i][0],
			centricPos[i][1] - LINE_LENGTH  * centricDen[i] * centricVel[i][1],
			centricPos[i][2] - LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
                glVertex3f(centricPos[i][0] + LINE_LENGTH  * centricDen[i] * centricVel[i][0],
                        centricPos[i][1] + LINE_LENGTH  * centricDen[i] * centricVel[i][1],
                        centricPos[i][2] + LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
		glEnd();

                glLineWidth(6.);
                glColor4f(0,0,0,0.5);
                glBegin(GL_LINES);
                glVertex3f(centricPos[i][0] - LINE_LENGTH  * centricDen[i] * centricVel[i][0],
                        centricPos[i][1] - LINE_LENGTH  * centricDen[i] * centricVel[i][1],
                        centricPos[i][2] - LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
                glVertex3f(centricPos[i][0] + LINE_LENGTH * centricDen[i] *centricVel[i][0],
                        centricPos[i][1] + LINE_LENGTH  * centricDen[i] * centricVel[i][1],
                        centricPos[i][2] + LINE_LENGTH  * centricDen[i] * centricVel[i][2]);
                glEnd();


		/**
		darw annulus (discs)
		**/
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 2);

		for(int j=0;j<dataColor.size();j++)
		{
			glColor4f(dataColor[j][0],
					dataColor[j][1],
					dataColor[j][2],1);
			renderAnnulus(centricPos[i],centricVel[i],
					distributeDen[i][j]*360,
					H, r);
			H = H + radius*splitCount[i];
			r = r + radius*splitCount[i];
		}
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glLineWidth(0.5);


		glColor4f(0.5,0.5,0.5,0.5);
		svVector3 norm = svGetPerpendicularVector(centricVel[i]);
		svVector3 head_out;
		svVector3 pos;
		svScalar moveD = 0.0;
		pos = centricPos[i] + moveD * centricVel[i];
		head_out = pos + (svScalar)(H-radius) * norm;
		for(int iii = 0; iii < 360; )
		{
			glBegin(GL_LINES);
			svVector3 draw_out = svGetRotatePoint(head_out, pos, centricVel[i], (float)iii);
		        glVertex3f(draw_out[0], draw_out[1], draw_out[2]);
		        glVertex3f(pos[0], pos[1], pos[2]);
			glEnd();

			iii = iii + 45;
		}

		glLineWidth(1.);

		glDisable(GL_POLYGON_OFFSET_FILL);

		}
	}

	//cerr<<count<<endl;
}
void svDisc::Find_Nearest(svVector3 rayPos, svVector3 rayDir, GLfloat *tm, bool layer, float zvalue)
{
    svVector3 newRaypos;
    svVector3 newRaydir;
    GLfloat invert_tb[16];

    if(!invert_matrix(tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
        exit(0);
    }

    newRaypos[0] = rayPos[0] * invert_tb[0] +
            rayPos[1] * invert_tb[4] +
            rayPos[2] * invert_tb[8] +
            invert_tb[12];
    newRaypos[1] = rayPos[0] * invert_tb[1] +
            rayPos[1] * invert_tb[5] +
            rayPos[2] * invert_tb[9] +
            invert_tb[13];
    newRaypos[2] = rayPos[0] * invert_tb[2] +
            rayPos[1] * invert_tb[6] +
            rayPos[2] * invert_tb[10] +
            invert_tb[14];

    newRaydir[0] = (rayPos[0]+rayDir[0]) * invert_tb[0] +
            (rayPos[1]+rayDir[1]) * invert_tb[4] +
            (rayPos[2]+rayDir[2]) * invert_tb[8] +
            invert_tb[12];
    newRaydir[1] = (rayPos[0]+rayDir[0]) * invert_tb[1] +
            (rayPos[1]+rayDir[1]) * invert_tb[5] +
            (rayPos[2]+rayDir[2]) * invert_tb[9] +
            invert_tb[13];
    newRaydir[2] = (rayPos[0]+rayDir[0])  * invert_tb[2] +
            (rayPos[1]+rayDir[1])  * invert_tb[6] +
            (rayPos[2]+rayDir[2])  * invert_tb[10] +
            invert_tb[14];
    newRaydir[0] = newRaydir[0] - newRaypos[0];
    newRaydir[1] = newRaydir[1] - newRaypos[1];
    newRaydir[2] = newRaydir[2] - newRaypos[2];

    vector<svScalar> rayD;
    svScalar dd = 9e+9;
    int NUM_SEG = 10;
    int ni;
	cerr<<"==============="<<endl;
    for(int i=0;i<numDisc;i++)
    {
	if(!layer || (layer && fabs(centricPos[i][2] - zvalue)<0.001))
	{
	svScalar tmpD;
	bool inside;
	svVector3 tmpP;
	//inside arrows

	svVector3 norm = svGetPerpendicularVector(centricVel[i]);
	svVector3 org = centricPos[i];
	svVector3 head = org + (svScalar)((dataColor.size()+1) * radius) * norm;//cerr<<dataColor.size()*splitCount[i] * radius<<endl;
	svVector3 prepos = head;
	svVector3 pos;
	for(int j=1;j<=NUM_SEG;j++)
	{
		pos = svGetRotatePoint(head, org, 
                           centricVel[i], j * 360/NUM_SEG);
		inside = rayTriangleIntersect(newRaypos, newRaydir, 
			org,
			pos,
			prepos,
			centricVel[i], 
			tmpD, tmpP);
		prepos = pos;
		if(inside)
		{
			rayD.push_back(tmpD);
		}
	}
	cerr<<"==============="<<endl;
	//detect if intersection points exist
	if(rayD.size() > 0)
	{
		svScalar minD = *min_element(rayD.begin(), rayD.end());
		if(minD < dd)
		{
			dd = minD; 
			ni = i;
		}
		SELECTED = true;
	}
	}
    }

	if(SELECTED==true)
	{

		if(selected_disc[ni]) selected_disc[ni] = false;
		else selected_disc[ni] =true;
		//cerr<<selected_disc[ni]<<" "<<ni<<endl;
		for(int ii=0;ii<dataSize;ii++)
		{
			if(dataCluster[ii] > -1)
			{
				if(selected_disc[dataCluster[ii]])
				{	dataIn[ii] = true; }
				else
					dataIn[ii] = false;
			}
		}		
	}
}
void svDisc::Find_Nearest(int x, int y)
{
   int i, j;
    GLdouble td, dd;
    svVector3 *p = new svVector3[numDisc];
  GLdouble *wx = new  GLdouble[numDisc];
  GLdouble *wy = new  GLdouble[numDisc];
  GLdouble *wz = new  GLdouble[numDisc];
    //glGetIntegerv (GL_VIEWPORT, viewport);
    //glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    //glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(i=0;i<numDisc;i++)
	{
		p[i][0]=0;p[i][1]=0;p[i][2]=0;	
	}

	for(i=0;i<numDisc;i++)
	{
		p[i][0]  = centricPos[i][0] * tb_transform[0][0] +
			    centricPos[i][1] * tb_transform[1][0] +
			    centricPos[i][2] * tb_transform[2][0] +
			    tb_transform[3][0] + p[i][0];
                p[i][1]  = centricPos[i][0] * tb_transform[0][1] +
                            centricPos[i][1] * tb_transform[1][1] +
                            centricPos[i][2] * tb_transform[2][1] +
                            tb_transform[3][1] + p[i][1];
                p[i][2]  = centricPos[i][0] * tb_transform[0][2] +
                            centricPos[i][1] * tb_transform[1][2] +
                           centricPos[i][2] * tb_transform[2][2] +
                            tb_transform[3][2] + p[i][2];
	}
	
	for(i=0;i<numDisc;i++)
	{
		gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
		wy[i] = (GLdouble)width-wy[i];
		///cerr<<wx[i]<<" "<<wy[i]<<endl;
	}


    	dd=9e+9;

    	int ni = 0;
	for(i=0;i<numDisc;i++)
	{
		td = ((GLdouble)x-wx[i]) *  ((GLdouble)x-wx[i]) +
                 ((GLdouble)y-wy[i]) * ((GLdouble)y-wy[i]);
		if(td<dd)
		{
			dd = td;
			//SELECTED = true;
			ni = i;
		}
	}//cerr<<x<<" "<<y<<" "<<td<<" "<<SELECTED<<" "<<dd<<endl;
	if(dd<500) SELECTED = true;
	if(SELECTED)
	{ 
		if(selected_disc[ni]) selected_disc[ni] = false;
		else selected_disc[ni] =true;
		//cerr<<selected_disc[ni]<<" "<<ni<<endl;
		for(int ii=0;ii<dataSize;ii++)
		{
			if(dataCluster[ii] > -1)
			{
				if(selected_disc[dataCluster[ii]])
				{	dataIn[ii] = true; }
				else
					dataIn[ii] = false;
			}
		}
	}

	delete [] p;
	delete [] wx;
	delete [] wy;
	delete [] wz;
}

void svDisc::cleanup(){

	delete [] dataPos;
	delete [] dataVel;
	delete [] dataDen;
	delete [] dataExp;
	delete [] dataCoe;
	delete [] dataCluster;
	delete [] countCluster;
	delete [] dataIn;
	dataSize = 0;


	dataColor.clear();
	clusterColor.clear();

	centricPos.clear();
	centricVel.clear();
	centricDen.clear();
	for(int i=0;i<numDisc;i++)
	{
		distributeDen[i].clear();
	}
	distributeDen.clear();
	delete [] selected_disc;
	delete [] splitCount;
	numDisc = 0;
}

}

