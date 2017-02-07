
#include "svMap.h"

#ifndef PI
#define PI 3.1415926
#endif

using namespace std;

namespace __svl_lib {

svMap::svMap(svVector3 *pos,
		     int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xn, int yn, float z)
{
	dataSize = 0;
	setData(pos, num, l, r, x1, y1, x2, y2, xn, yn,z);
}

void svMap::setData(svVector3 *pos, 
		     int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xn, int yn,float z)
{
	/**
	it happens only when data are changed.
	**/
	if(dataSize > 0)
	{
		cleanup();
	}

	dataSize = num;	
	xd = x1; //distance of two adjacent x values
	yd = y1;
	xl = x2; 
	yl = y2;
	xnum = xn;
	ynum = yn;

	dataPos = new svVector3[dataSize];
	dataROI = new svBool[dataSize];

	int zlayer = (pos[dataSize-1][2] - pos[0][2])/z+1;
	mapPos.resize(zlayer);
	splitPos.resize(zlayer);
	dataIndex.resize(zlayer);

	lbbox[0] = 9e+9;
	lbbox[1] = 9e+9;
	lbbox[2] = 9e+9;
	rbbox[0] = -9e+9;
	rbbox[1] = -9e+9;
	rbbox[2] = -9e+9;	
	//cerr<<dataSize<<" "<<xd<<" "<<yd<<" "<<xl<<" "<<yl<<" "<<xnum<<" "<<ynum<<endl;
	//cerr<<dataSize<<" "<<zlayer<<" "<<z<<" "<<pos[dataSize-1][2]<<endl;
	for(int i=0;i<dataSize;i++)
	{
		dataPos[i] = pos[i];

		/**
		**/
		if(dataPos[i][0] < lbbox[0]) lbbox[0] = dataPos[i][0];
		if(dataPos[i][1] < lbbox[1]) lbbox[1] = dataPos[i][1];
		if(dataPos[i][2] < lbbox[2]) lbbox[2] = dataPos[i][2];
		if(dataPos[i][0] > rbbox[0]) rbbox[0] = dataPos[i][0];
		if(dataPos[i][1] > rbbox[1]) rbbox[1] = dataPos[i][1];
		if(dataPos[i][2] > rbbox[2]) rbbox[2] = dataPos[i][2];		

		/**
		computer 2D coordinates
		and mapped index
		**/
		int tmp_zlayer = (dataPos[i][2] - dataPos[0][2])/(float)z;
		double move_y = ( yl + 2 * yd ) * (tmp_zlayer/xnum);
		double move_x = ( xl + 2 * xd ) * (tmp_zlayer%xnum);
		//cerr<<move_x<<" "<<move_y<<endl;
		svVector3 tmp_mapPos;
		tmp_mapPos[0] = dataPos[i][0] + move_x;
		tmp_mapPos[1] = dataPos[i][1] - move_y;
		tmp_mapPos[2] = 0;
		mapPos[tmp_zlayer].push_back(tmp_mapPos);

		dataIndex[tmp_zlayer].push_back(i);
		splitPos[tmp_zlayer].push_back(pos[i]);
	}
/*
	ofstream outfile("map_Test");

	for(int i=0;i<mapPos.size();i++)
	{
		for(int j=0;j<mapPos[i].size();j++)
			outfile<<splitPos[i][j][0]<<" "<<splitPos[i][j][1]<<" "<<splitPos[i][j][2]<<endl;
	}

	outfile.close();*/
	/**
	move the rendering into the center (0,0)
	**/
	svVector3 tmpl = lbbox;
	svVector3 tmpr;
	tmpr[0] = lbbox[0]+ ( xl + 2 * xd ) * xnum;
	tmpr[1] = lbbox[1]- ( yl + 2 * yd ) * ynum;
	tmpr[2] = lbbox[2];

	svVector3 center = (tmpl + tmpr)/2;
	for(int i=0;i<mapPos.size();i++)
	{
		for(int j=0;j<mapPos[i].size();j++)
		{
			mapPos[i][j][0] = mapPos[i][j][0] - center[0];
			mapPos[i][j][1] = mapPos[i][j][1] - center[1];
		}	
	}

}
void svMap::setROI(svBool *in)
{
	for(int i=0;i<dataSize;i++)
	{
		dataROI[i] = in[i];
	}
}
void svMap::cleanup()
{
	delete [] dataPos;
	delete [] dataROI;

	for(int i=0;i<dataIndex.size();i++)
	{
		dataIndex[i].clear();
		mapPos[i].clear();
		splitPos[i].clear();
	}
	dataIndex.clear();
	mapPos.clear();
	splitPos.clear();


	dataSize = 0;
}	

}
