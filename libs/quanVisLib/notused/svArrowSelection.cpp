/*  
 * svIconGlyph
 */


#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include <iomanip>      // std::setprecision
#include "svArrowSelection.h"

#ifndef PI
#define PI 3.1415926
#endif

using namespace std;

namespace __svl_lib {

svArrowSelection::svArrowSelection(char *data_file, char *format_file)
		 : svArrowGlyph(data_file, format_file)
{

  SELECTED = false;

  current_index = -1;
  selected_index.clear();

  SELECTED_CURRENT = GL_FALSE;
  SELECTED = GL_FALSE;
}
svArrowSelection::svArrowSelection(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe,svInt *atom,
		svInt size, vector<vector<vector<int> > > f):svArrowGlyph(pos,vel,den,exp,coe,atom,size,f)
{

  SELECTED = false;

  current_index = -1;
  selected_index.clear();

  SELECTED_CURRENT = GL_FALSE;
  SELECTED = GL_FALSE;
}

svArrowSelection::svArrowSelection(vector< vector<svVector3> > pos, vector< vector<svVector3> > vel,
		vector< vector<svScalar> > den, vector< vector<svScalar> > exp, vector< vector<svScalar> > coe,
		vector< vector<svInt> > atom, 
		svInt size, vector<vector<vector<int> > > f):svArrowGlyph(pos,vel,den,exp,coe,atom,size,f)
{

  SELECTED = false;

  current_index = -1;
  selected_index.clear();

  SELECTED_CURRENT = GL_FALSE;
  SELECTED = GL_FALSE;
}

void svArrowSelection::Init()
{
	selected_index.push_back(0);
	current_index = 0;
	current_selected_index = 0;

	selectedIndex = 0;
}

void svArrowSelection::SetSelectedMarker(svVector3 c)
{
	int in;
	double dd, td;
	dd = 9e+9;
	for(int i=0;i<selected_index.size();i++)
	{
			td = (glyphPos[0][selected_index[i]][0] - c[0]) * (glyphPos[0][selected_index[i]][0] - c[0])
				+ (glyphPos[0][selected_index[i]][1] - c[1]) * (glyphPos[0][selected_index[i]][1] - c[1])
				+ (glyphPos[0][selected_index[i]][2] - c[2]) * (glyphPos[0][selected_index[i]][2] - c[2]);
			if(td < dd)
			{
				dd = td;
				in = i;
			}
	}

	current_index = selected_index[in];
	current_selected_index = in;
	selectedIndex = in;
}

void svArrowSelection::AddMarker(svVector3 c)
{
#if 0
/*
		//cerr<<"start"<<endl;
		int	nPts;					// actual number of data points
		ANNpointArray	dataPts;				// data points
		ANNpoint	queryPt;				// query point
		ANNidxArray	nnIdx;					// near neighbor indices
		ANNdistArray    dists;					// near neighbor distances
		ANNkd_tree*	kdTree;					// search structure
		int dim = 3;
		//int maxPts = dataSize*dim;
		int k = 1;

		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists
		queryPt = annAllocPt(dim);					// allocate query point

		int count = 0;
		for(int i=0;i<dataSize;i++)//in_count;i++)
		{
			if(dataSample[i])
			{
				bool flag = true;
				for(int jj=0;jj<selected_index.size();jj++)
				{
					if(i == selected_index[jj])
						flag = false;
				}
				if(flag)
					count++;
			}
		}

		nPts = count;
		dataPts = annAllocPts(count*dim, dim);

		int j = 0;
		for(int i=0;i<dataSize;i++)//in_count;i++)
		{
			if(dataSample[i])
			{
				bool flag = true;
				for(int jj=0;jj<selected_index.size();jj++)
				{
					if(i == selected_index[jj])
						flag = false;
				}
				if(flag)
				{
					dataPts[j][0] = glyphPos[0][i][0];
					dataPts[j][1] = glyphPos[0][i][1];
					dataPts[j][2] = glyphPos[0][i][2];	
					j++;
				}
			}	
		}

		kdTree = new ANNkd_tree(							// build search structure
						dataPts,					// the data points
						nPts,						// number of points
						dim);						// dimension of space


			queryPt[0] = c[0];
			queryPt[1] = c[1];
			queryPt[2] = c[2];

			kdTree->annkSearch(							// search
					queryPt,						// query point
					1,							// number of near neighbors
					nnIdx,							// nearest neighbors (returned)
					dists,							// distance (returned)
					0.0);		
					// error bound

		j = 0;
		for(int i=0;i<dataSize;i++)//in_count;i++)
		{
			if(dataSample[i])
			{
				bool flag = true;
				for(int jj=0;jj<selected_index.size();jj++)
				{
					if(i == selected_index[jj])
						flag = false;
				}
				if(flag)
				{
					if(j == nnIdx[0])
					{	
						selected_index.push_back(i);
						markedData[i] = true;
						break;
					}
					j++;
				}
			}

		}

	   delete [] nnIdx;							// clean things up
	   delete [] dists;
	   delete kdTree;

	dataPts = annAllocPts(0,0);
	queryPt = annAllocPt(0);
	annClose();			// done with ANN
	
	if(selected_index.size()>0)
	{
		current_index = selected_index[selected_index.size()-1];
		current_selected_index = selected_index.size()-1;
		selectedIndex = 	current_index;
	}
	count = 0;
	//for(int i=0;i<dataSize;i++) if(markedData[i])count++;
	//cerr<<"count "<<count<<endl;
*/
#endif
}


void svArrowSelection::AddCurrentIndex(svVector3 c)
{
#if 0 // JC
/*
		//cerr<<"start"<<endl;
		int	nPts;					// actual number of data points
		ANNpointArray	dataPts;				// data points
		ANNpoint	queryPt;				// query point
		ANNidxArray	nnIdx;					// near neighbor indices
		ANNdistArray    dists;					// near neighbor distances
		ANNkd_tree*	kdTree;					// search structure
		int dim = 3;
		//int maxPts = dataSize*dim;
		int k = 1;

		nnIdx = new ANNidx[k];						// allocate near neigh indices
		dists = new ANNdist[k];						// allocate near neighbor dists
		queryPt = annAllocPt(dim);					// allocate query point

		int count = selected_index.size();

		nPts = count;
		dataPts = annAllocPts(count*dim, dim);

		int j = 0;
		for(int i=0;i<count;i++)//in_count;i++)
		{
				dataPts[j][0] = glyphPos[0][selected_index[i]][0];
				dataPts[j][1] = glyphPos[0][selected_index[i]][1];
				dataPts[j][2] = glyphPos[0][selected_index[i]][2];	
				j++;
		}
		kdTree = new ANNkd_tree(							// build search structure
						dataPts,					// the data points
						nPts,						// number of points
						dim);						// dimension of space


			queryPt[0] = c[0];
			queryPt[1] = c[1];
			queryPt[2] = c[2];

			//cerr<<c[0]<<" "<<c[1]<<" "<<c[2]<<endl;

			kdTree->annkSearch(							// search
					queryPt,						// query point
					1,							// number of near neighbors
					nnIdx,							// nearest neighbors (returned)
					dists,							// distance (returned)
					0.0);		
					// error bound

			//selected_index.push_back(nnIdx[0]);

		current_index = selected_index[nnIdx[0]];//selected_index[selected_index.size()-1];
		current_selected_index = nnIdx[0];
		selectedIndex = nnIdx[0];



	   delete [] nnIdx;							// clean things up
	   delete [] dists;
	   delete kdTree;

	dataPts = annAllocPts(0,0);
	queryPt = annAllocPt(0);
	annClose();			// done with ANN
	

	//current_selected_index = selected_index.size()-1;	
*/
#endif
}

void svArrowSelection::DeleteMarker(svVector3 c)
{
	int in;
	double dd, td;
	dd = 9e+9;
	//cerr<<"delete "<<selected_index.size()<<endl;
	for(int i=0;i<selected_index.size();i++)
	{
			td = (glyphPos[0][selected_index[i]][0] - c[0]) * (glyphPos[0][selected_index[i]][0] - c[0])
				+ (glyphPos[0][selected_index[i]][1] - c[1]) * (glyphPos[0][selected_index[i]][1] - c[1])
				+ (glyphPos[0][selected_index[i]][2] - c[2]) * (glyphPos[0][selected_index[i]][2] - c[2]);
			if(td < dd)
			{
				dd = td;
				in = i;
			}
	}
	markedData[selected_index[in]] = false;
	selected_index.erase(selected_index.begin()+in);
	current_selected_index = 0;
	current_index = -1;
	selectedIndex = -1;
}

void svArrowSelection::DeleteMarker()
{
	int in;
	bool flag = false;
	for(int i=0;i<selected_index.size();i++)
	{
		if(selected_index[i] == current_index)
		{
			in = i;
			flag = true;
			break;
		}
	}
	if(flag)
	{
		markedData[selected_index[in]] = false;
		selected_index.erase(selected_index.begin()+in);
	}
	current_index = -1;
	current_selected_index = 0;
}



void svArrowSelection::Reset()
{
	selected_index.clear();
	for(int i=0;i<dataSize;i++)
	{
		markedData[i] =false;
	}
	selectedIndex = false;
	current_index = -1;
	current_selected_index = -1;

	SELECTED=GL_FALSE;
	SELECTED_CURRENT=GL_FALSE;
}

void svArrowSelection::Find_Current_Nearest(svVector3 rayPos, svVector3 rayDir, GLfloat *tm)
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

    for(int i=0;i<dataSize;i++)
    {
	if(markedData[i])
	{
	svScalar tmpD;
	bool inside;
	svVector3 tmpP;
	//inside arrows
	for(int j=0;j<NUM_SEG-1;j++)
	{
		inside = rayTriangleIntersect(newRaypos, newRaydir, 
			glyphArrowPos[NUM_SEG][i],
			glyphArrowPos[j+1][i],
			glyphArrowPos[j][i],
			svGetNorm(glyphArrowPos[NUM_SEG][i],glyphArrowPos[j+1][i],glyphArrowPos[j][i]), 
			tmpD, tmpP);
		if(inside)
		{
			rayD.push_back(tmpD);
		}
	}
	inside = rayTriangleIntersect(newRaypos, newRaydir, 
			glyphArrowPos[NUM_SEG][i],
			glyphArrowPos[0][i],
			glyphArrowPos[NUM_SEG-1][i],
			svGetNorm(glyphArrowPos[NUM_SEG][i],glyphArrowPos[0][i],glyphArrowPos[NUM_SEG][i]), 
			tmpD, tmpP);
	if(inside)
	{
		rayD.push_back(tmpD);
	}
	//base of arrows
	inside = rayTriangleIntersect(newRaypos, newRaydir, 
			glyphArrowPos[0][i],
			glyphArrowPos[2][i],
			glyphArrowPos[1][i],
			glyphDir[0][i], 
			tmpD, tmpP);
	if(inside)
	{
		rayD.push_back(tmpD);
	}
	inside = rayTriangleIntersect(newRaypos, newRaydir, 
			glyphArrowPos[0][i],
			glyphArrowPos[3][i],
			glyphArrowPos[2][i],
			glyphDir[0][i], 
			tmpD, tmpP);
	if(inside)
	{
		rayD.push_back(tmpD);
	}
	//inside sphere
	if(encode_mode == 1)
	{
		inside = raySphereIntersect(newRaypos, newRaydir, 
				glyphPos[2][i], 0.25,
				tmpD);
		//cerr<<"sphere "<<tmpD<<" "<<inside<<endl;
		if(inside)
		{
			//cerr<<"sphere "<<tmpD<<" "<<inside<<" ";
			rayD.push_back(tmpD);
		}
	}
	//detect if intersection points exist
	if(rayD.size() > 0)
	{
		svScalar minD = *min_element(rayD.begin(), rayD.end());
		if(minD < dd)
		{
			dd = minD; 
			current_index = i;
			selectedIndex = i;
		}
		SELECTED = true;
	}
    }//cerr<<dd<<endl;

	if(SELECTED==true)
	{
		bool flag = false;
		for(int i=0;i<selected_index.size();i++)
		{
			if(current_index == selected_index[i])
			{
				current_selected_index = i;
				selectedIndex = selected_index[i];
				flag = true;
				SELECTED_CURRENT = true;
				break;
			}
		}
		/*if(!flag)
		{
			selectedIndex = current_index;
			selected_index.push_back(current_index);
			markedData[current_index] = true;
			current_selected_index = selected_index.size()-1;
			//for(int i=0;i<dataSize;i++){if(markedData[i])cerr<<i<<" selected"<<endl;}
		}*/
	}
	}
}

void svArrowSelection::Find_Nearest(svVector3 rayPos, svVector3 rayDir, GLfloat *tm)
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

    for(int i=0;i<dataSize;i++)
    {
	svScalar tmpD;
	bool inside;
	svVector3 tmpP;
	//inside arrows
	for(int j=0;j<NUM_SEG-1;j++)
	{
		inside = rayTriangleIntersect(newRaypos, newRaydir, 
			glyphArrowPos[NUM_SEG][i],
			glyphArrowPos[j+1][i],
			glyphArrowPos[j][i],
			svGetNorm(glyphArrowPos[NUM_SEG][i],glyphArrowPos[j+1][i],glyphArrowPos[j][i]), 
			tmpD, tmpP);
		if(inside)
		{
			rayD.push_back(tmpD);
		}
	}
	inside = rayTriangleIntersect(newRaypos, newRaydir, 
			glyphArrowPos[NUM_SEG][i],
			glyphArrowPos[0][i],
			glyphArrowPos[NUM_SEG-1][i],
			svGetNorm(glyphArrowPos[NUM_SEG][i],glyphArrowPos[0][i],glyphArrowPos[NUM_SEG][i]), 
			tmpD, tmpP);
	if(inside)
	{
		rayD.push_back(tmpD);
	}
	//base of arrows
	inside = rayTriangleIntersect(newRaypos, newRaydir, 
			glyphArrowPos[0][i],
			glyphArrowPos[2][i],
			glyphArrowPos[1][i],
			glyphDir[0][i], 
			tmpD, tmpP);
	if(inside)
	{
		rayD.push_back(tmpD);
	}
	inside = rayTriangleIntersect(newRaypos, newRaydir, 
			glyphArrowPos[0][i],
			glyphArrowPos[3][i],
			glyphArrowPos[2][i],
			glyphDir[0][i], 
			tmpD, tmpP);
	if(inside)
	{
		rayD.push_back(tmpD);
	}
	//inside sphere
	if(encode_mode == 1)
	{
		inside = raySphereIntersect(newRaypos, newRaydir, 
				glyphPos[2][i], 0.25,
				tmpD);
		//cerr<<"sphere "<<tmpD<<" "<<inside<<endl;
		if(inside)
		{
			//cerr<<"sphere "<<tmpD<<" "<<inside<<" ";
			rayD.push_back(tmpD);
		}
	}
	//detect if intersection points exist
	if(rayD.size() > 0)
	{
		svScalar minD = *min_element(rayD.begin(), rayD.end());
		if(minD < dd)
		{
			dd = minD; 
			current_index = i;
			selectedIndex = i;
		}
		SELECTED = true;
	}
    }cerr<<dd<<endl;

	if(SELECTED==true)
	{
		bool flag = false;
		for(int i=0;i<selected_index.size();i++)
		{
			if(current_index == selected_index[i])
			{
				current_selected_index = i;
				selectedIndex = selected_index[i];
				flag = true;
				SELECTED_CURRENT = true;
				break;
			}
		}cerr<<dd<<endl;

		if(!flag)
		{
			selectedIndex = current_index;
			selected_index.push_back(current_index);
			markedData[current_index] = true;
			current_selected_index = selected_index.size()-1;
			//for(int i=0;i<dataSize;i++){if(markedData[i])cerr<<i<<" selected"<<endl;}
		}cerr<<dd<<endl;

	}
}

void svArrowSelection::DecideSelection()
{
	selected_index.push_back(current_index);
	current_selected_index = selected_index.size()-1;
}

svVector3 *svArrowSelection::GetSelected2DAllPos(GLfloat *tm)
{
	svVector3 *xyz = new svVector3[selected_index.size()];

	    GLdouble wx;
	    GLdouble wy;
	    GLdouble wz;

	    GLint viewport[4];
	    GLdouble mvmatrix[16];
	    GLdouble projmatrix[16];


	    glGetIntegerv (GL_VIEWPORT, viewport);
	    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(int i=0;i<selected_index.size();i++)
	{
		svVector3 p;

		p[0]  = glyphPos[0][i][0] * tm[0] +
				    glyphPos[0][i][1] * tm[4] +
				    glyphPos[0][i][2] * tm[8] +
				    tm[12] + p[0];
		p[1]  = glyphPos[0][i][0] * tm[1] +
	 			    glyphPos[0][i][1] * tm[5] +
				    glyphPos[0][i][2] * tm[9]+
				    tm[13] + p[1];
		p[2]  = glyphPos[0][i][0] * tm[2]+
	 			    glyphPos[0][i][1] * tm[6] +
				    glyphPos[0][i][2] * tm[10] +
				    tm[14] + p[2];

		gluProject((GLdouble)p[0],
		                (GLdouble)p[1],
		                (GLdouble)p[2],
		                mvmatrix, projmatrix, viewport,
		                &(wx), &(wy), &(wz));

		if(fabs(wx - viewport[2])<viewport[2]/8.)
		{
			wx = wx-viewport[2]/8.;
		}	
		if(fabs(wy- viewport[3])<viewport[2]/8. *(3./4))
		{
			wy = wy-viewport[2]/8. *(3./4);
		}

		xyz[i][0]=wx;
		xyz[i][1]=wy;
	}
	return xyz;
	
}

svVector3 svArrowSelection::GetSelected2DPos(GLfloat *tm)
{
	svVector3 xyz;

	xyz[0] = 0;
	xyz[1] = 0;

	if(current_index>=0)
	{
	    GLdouble wx;
	    GLdouble wy;
	    GLdouble wz;

    GLint viewport[4];
    GLdouble mvmatrix[16];
    GLdouble projmatrix[16];


	    glGetIntegerv (GL_VIEWPORT, viewport);
	    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

		svVector3 p;

		p[0]  = glyphPos[0][current_index][0] * tm[0] +
				    glyphPos[0][current_index][1] * tm[4] +
				    glyphPos[0][current_index][2] * tm[8] +
				    tm[12] + p[0];
		p[1]  = glyphPos[0][current_index][0] * tm[1] +
	 			    glyphPos[0][current_index][1] * tm[5] +
				    glyphPos[0][current_index][2] * tm[9]+
				    tm[13] + p[1];
		p[2]  = glyphPos[0][current_index][0] * tm[2]+
	 			    glyphPos[0][current_index][1] * tm[6] +
				    glyphPos[0][current_index][2] * tm[10] +
				    tm[14] + p[2];

		gluProject((GLdouble)p[0],
		                (GLdouble)p[1],
		                (GLdouble)p[2],
		                mvmatrix, projmatrix, viewport,
		                &(wx), &(wy), &(wz));

		if(fabs(wx - viewport[2])<viewport[2]/8.)
		{
			wx = wx-viewport[2]/8.;
		}	
		if(fabs(wy- viewport[3])<viewport[2]/8. *(3./4))
		{
			wy = wy-viewport[2]/8. *(3./4);
		}

		xyz[0]=wx;
		xyz[1]=wy;
	}

	return xyz;
}

void svArrowSelection::cleanup()
{
  selected_index.clear();
}

}
