/*  
 * svIconGlyph
 */



#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include <iomanip>      // std::setprecision
#include "svArrowGlyph.h"
#include "svException.h"
#include "svUtil.h"


#ifndef PI
#define PI 3.1415926
#endif

using namespace std;


namespace __svl_lib {

svArrowGlyph::svArrowGlyph(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe, svInt *atom,
		svInt size, vector<vector<vector<int> > > f)
{
cerr<<"setdata_arrow"<<endl;
  dataSize = 0;
cerr<<"setdata_arrow"<<endl;
  SetData(pos,vel,den,exp,coe,atom,size);
  ReadDataFormat(f);

  font_color[0] = 1;
  font_color[1] = 1;
  font_color[2] = 1;

}
svArrowGlyph::svArrowGlyph(vector< vector<svVector3> > pos, vector< vector<svVector3> > vel,
		vector< vector<svScalar> > den, vector< vector<svScalar> > exp, vector< vector<svScalar> > coe,
		vector< vector<svInt> > atom, 
		svInt size, vector<vector<vector<int> > > f)
{
cerr<<"setdata_arrow_vector"<<endl;
  dataSize = 0;

  SetData(pos,vel,den,exp,coe,atom,size);
  ReadDataFormat(f);

  font_color[0] = 1;
  font_color[1] = 1;
  font_color[2] = 1;
}
svArrowGlyph::svArrowGlyph(char *data_file, char *format_file)
{
  dataSize = 0;

  SetData(data_file);
  ReadDataFormat(format_file);

  font_color[0] = 1;
  font_color[1] = 1;
  font_color[2] = 1;

}
void svArrowGlyph::ReadDataFormat(vector<vector<vector<int> > > f)
{
   for(int i=0;i<rawFormat.size();i++)
    {
	for(int j=0;j<rawFormat[i].size();j++)
	{
		rawFormat[i][j].clear();
	}
	rawFormat[i].clear();
    }
    rawFormat.clear();

 rawFormat.resize(f.size());
 for(int i=0;i<rawFormat.size();i++)
 {
	rawFormat[i].resize(f[i].size());
	for(int j=0;j<rawFormat[i].size();j++)
	{
		rawFormat[i][j].resize(f[i][j].size());
		for(int t=0;t<f[i][j].size();t++)
		{
			rawFormat[i][j][t] = f[i][j][t];
		}
	}
 }

}
void svArrowGlyph::ReadDataFormat(char *file)
{ 
    for(int i=0;i<rawFormat.size();i++)
    {
	for(int j=0;j<rawFormat[i].size();j++)
	{
		rawFormat[i][j].clear();
	}
	rawFormat[i].clear();
    }
    rawFormat.clear();

 ifstream infile(file);
 
 double temp;
 double zlayer,xdistance,ydistance,zdistance,scaling;
 svVector3 lb, rb;
 
 infile>>zlayer;
 infile>>xdistance;
 infile>>ydistance;
 infile>>zdistance;
 infile>>scaling;
 infile>>lb[0]>>lb[1]>>lb[2];
 infile>>rb[0]>>rb[1]>>rb[2];

// cerr<<lb[0]<<" "<<lb[1]<<" "<<lb[2]<<" "<<scaling<<endl;
 
 double xl = rb[0] - lb[0];
 double yl = rb[1] - lb[1];
 double zmin = lb[2];
  double xd,yd;
 
 infile>>temp>>temp>>temp>>temp>>temp;

 int n;
 infile>>n;
 //rawFormat.resize(n);

 int count = 0;

 for(int i=0;i<n;i++)
 {
        infile>>temp>>temp>>temp>>temp>>yd; 
	vector< vector<int> > tmpyformat;
	int tn;
	infile>>tn;	
	for(int j=0;j<tn;j++)
	{
		vector<int> tmpxformat;
		int s, e;

   		infile>>temp>>s>>temp>>e>>xd;

		for(int k=s;k<=e;k++)
		{
			tmpxformat.push_back(k);
			count ++;
		}
	
		tmpyformat.push_back(tmpxformat);

		tmpxformat.clear();
	}
	rawFormat.push_back(tmpyformat);

	for(int j=0;j<tmpyformat.size();j++)
	{
		tmpyformat[j].clear();
	}
	tmpyformat.clear();
 }

//cerr<<"arrow format"<<count<<endl;
 
 infile.close();

}
void svArrowGlyph::SetSample(int Frequency, bool raw_data)
{
	glyphFrequency = Frequency;

	if(raw_data)
	{
	  int num = rawFormat.size();//cerr<<num<<endl;
	  int count = 0;
	  //int count2 = 0;

	  for(int i=0;i<num;i++)
	  {
		  int n = rawFormat[i].size();//cerr<<n<<endl;
		  for(int j=0;j<n;j++)
		  {
			  int nn = rawFormat[i][j].size();
			  //cerr<<nn<<" ";
			  for(int k=0;k<nn;k++)
			  {
				  //if(in[count])
				  //{
				  //	cerr<<count2<<" "<<dataSize<<endl;
					  if(i%Frequency == 0 && j%Frequency == 0 && k%Frequency == 0)
						  //dataSample[count2] = true;
						  //sample.push_back(true);
						  dataSample[count] = true;
					  else
						  dataSample[count] = false;
						  //sample.push_back(false);

					  //count2++;
					  count++;
				  //}
				  //count ++;
				  //k = k + Frequency;
			  }

			  //j = j + Frequency;
		  }//cerr<<endl;

	  }	//end of for loop
	}
	else
	{
	  for(int i=0;i<dataSize;i++)
	  {
	    if(i%glyphFrequency == 0)
	    {
	      dataSample[i] = true;
	    }
	    else
	    {
	      dataSample[i] = false;
	    }
	  } //end of for loop
	}
}

void svArrowGlyph::SetData(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe,
		svInt *atom,
		svInt num)
{
cerr<<"setdata1"<<" "<<num<<endl;
  if(dataSize > 0)
  {
    cleanup();
  }
  maxmag = -1;
  
  dataSize = num;
cerr<<"setdata2"<<endl;
  // data attributes
  glyphPos = new svVector3Array[3]; // starting and ending positions
  glyphDir = new svVector3Array[1];
  glyphLegend = new svVector3Array[1];
  glyphMag = new svScalar[dataSize];
  glyphCoe = new svScalar[dataSize];
  glyphExp = new svScalar[dataSize];
  glyphradius = new svScalar[dataSize];
  dataSample = new bool[dataSize];
  glyphInbox = new svBool[dataSize];
  glyphExpand = new svBool[dataSize];
  markedData = new bool[dataSize];
  dataAtom = new svInt[dataSize];
cerr<<"setdata3"<<endl;
  //rendering attributes
  glyphArrowPos = new svVector3Array[NUM_SEG+1];
  glyphArrowNorm = new svVector3Array[NUM_SEG+1];
  glyphFrequency = 1;
  glyphColors = new svVector4Array[1];
  int stacks = SPHERE_STACK;
  int slices = SPHERE_SLICE;
  int size = stacks * (slices+1) * 2 * 3;
  sphereVertexArray = new float[dataSize *size];
  sphereNormalArray = new float[dataSize *size];
 // dataAtom = new svInt[dataSize];
cerr<<"setdata4"<<endl;  
  selectedIndex = -1;

  for(int j=0; j<dataSize; j++)
  {
	// set positions
	dataSample[j] = false;
   	glyphPos[0].add(pos[j]);
   	glyphPos[1].add(pos[j]);
   	glyphPos[2].add(pos[j]);
	glyphLegend[0].add(pos[j]);

	for(int i=0;i<NUM_SEG+1;i++)	
	{
		glyphArrowPos[i].add(pos[j]);	
		glyphArrowNorm[i].add(pos[j]);
	}
		
   	// set vector dir
	svVector3 nvel = normalize(vel[j]);
	glyphDir[0].add(nvel);

	//set magnitude
	glyphMag[j]=(den[j]);// * pow(10., (float)SCALE);
	//if(glyphMag[j] > maxmag)
	//	maxmag = glyphMag[j];

	//set exp and coeffecient
	glyphCoe[j]=coe[j];
	glyphExp[j]=exp[j];
	
	markedData[j] = false;
	dataAtom[j] = atom[j];
  }; // end for(j)
//cerr<<"setdata5"<<endl;  
}


void svArrowGlyph::SetData(vector< vector<svVector3> > pos, vector< vector<svVector3> > vel,
		vector< vector<svScalar> > den, vector< vector<svScalar> > exp, vector< vector<svScalar> > coe,
		vector< vector<svInt> > atom, 
		svInt num)
{
cerr<<"setdata11"<<endl;
  if(dataSize > 0)
  {
    cleanup();
  }
  maxmag = -1;
  
  
  dataSize = num;

  // data attributes
  glyphPos = new svVector3Array[3]; // starting and ending positions
  glyphDir = new svVector3Array[1];
  glyphLegend = new svVector3Array[1];
  glyphMag = new svScalar[dataSize];
  glyphCoe = new svScalar[dataSize];
  glyphExp = new svScalar[dataSize];
  glyphradius = new svScalar[dataSize];
  dataSample = new bool[dataSize];
  glyphInbox = new svBool[dataSize];
  glyphExpand = new svBool[dataSize];
  markedData = new bool[dataSize];
  dataAtom = new svInt[dataSize];

  //rendering attributes
  glyphArrowPos = new svVector3Array[NUM_SEG+1];
  glyphArrowNorm = new svVector3Array[NUM_SEG+1];
  glyphFrequency = 1;
  glyphColors = new svVector4Array[1];
  int stacks = SPHERE_STACK;
  int slices = SPHERE_SLICE;
  int size = stacks * (slices+1) * 2 * 3;
  sphereVertexArray = new float[dataSize *size];
  sphereNormalArray = new float[dataSize *size];
 // dataAtom = new svInt[dataSize];
  
  selectedIndex = -1;

  int count = 0;

  for(int i=0;i<pos.size();i++)
  {
	for(int j=0;j<pos[i].size();j++)
	{
		// set positions
		dataSample[count] = false;

	   	glyphPos[0].add(pos[i][j]);
	   	glyphPos[1].add(pos[i][j]);
	   	glyphPos[2].add(pos[i][j]);
		glyphLegend[0].add(pos[i][j]);

		for(int ii=0;ii<NUM_SEG+1;ii++)	
		{
			glyphArrowPos[ii].add(pos[i][j]);	
			glyphArrowNorm[ii].add(pos[i][j]);
		}
		
	   	// set vector dir
		svVector3 nvel = normalize(vel[i][j]);
		glyphDir[0].add(nvel);

		//set magnitude
		glyphMag[count]=(den[i][j]);// * pow(10., (float)SCALE);
		//if(glyphMag[count] > maxmag)
		//	maxmag = glyphMag[j];

		//set exp and coeffecient
		glyphCoe[count]=coe[i][j];
		glyphExp[count]=exp[i][j];
	
		markedData[count] = false;
		dataAtom[count] = atom[i][j];

		count++;
	}
   }
  
}

void svArrowGlyph::SetScalar(int s)
{
	SCALE = s;

#ifdef JIAN
	//maxmag = -1;
	for(int i=0;i<dataSize;i++)
	{
		glyphExp[i] = glyphExp[i] + SCALE;
		//cerr<<i<<" "<<glyphMag[i]<<" ";
		glyphMag[i] = glyphMag[i] * pow(10.,(double)SCALE);
		//if(maxmag < glyphMag[i]) maxmag = glyphMag[i];
		//cerr<<i<<" "<<glyphMag[i]<<" "<<" "<<SCALE<<endl;
	}
	maxmag = maxmag * pow(10.,(double)SCALE);
	cerr<<"maxmag "<<maxmag<<endl;
#endif 
}

void svArrowGlyph::setLength(double l)
{
	min_length = l;
}
void svArrowGlyph::SetData(char *file)
{
  if(dataSize > 0)
  {
    cleanup();
  }
  maxmag = -1;
  
  ifstream infile(file);
  
  infile>>dataSize;

  // data attributes
  glyphPos = new svVector3Array[3]; // starting and ending positions
  glyphDir = new svVector3Array[1];
  glyphLegend = new svVector3Array[1];
  glyphMag = new svScalar[dataSize];
  glyphCoe = new svScalar[dataSize];
  glyphExp = new svScalar[dataSize];
  glyphradius = new svScalar[dataSize];
  dataSample = new bool[dataSize];
  glyphInbox = new svBool[dataSize];
  glyphExpand = new svBool[dataSize];
  markedData = new bool[dataSize];

  //rendering attributes
  glyphArrowPos = new svVector3Array[NUM_SEG+1];
  glyphArrowNorm = new svVector3Array[NUM_SEG+1];
  glyphFrequency = 1;
  glyphColors = new svVector4Array[1];
  int stacks = SPHERE_STACK;
  int slices = SPHERE_SLICE;
  int size = stacks * (slices+1) * 2 * 3;
  sphereVertexArray = new float[dataSize *size];
  sphereNormalArray = new float[dataSize *size];
  
  selectedIndex = -1;

  for(int j=0; j<dataSize; j++)
  {
	svVector3 posIn, velIn;
	svScalar denIn, exp, coe;
	
	infile>>posIn[0]>>posIn[1]>>posIn[2]
	      >>velIn[0]>>velIn[1]>>velIn[2]
	      >>denIn>>exp>>coe;
	
	// set positions
	dataSample[j] = false;
   	glyphPos[0].add(posIn);
   	glyphPos[1].add(posIn);
   	glyphPos[2].add(posIn);
	glyphLegend[0].add(posIn);

	for(int i=0;i<NUM_SEG+1;i++)	
	{
		glyphArrowPos[i].add(posIn);	
		glyphArrowNorm[i].add(posIn);
	}
		
   	// set vector dir
	svVector3 nvel = normalize(velIn);
	glyphDir[0].add(nvel);

	//set magnitude
#ifdef JIAN
	glyphMag[j]=(denIn) * pow(10., (float)SCALE);
#else
	glyphMag[j]=(denIn);// * pow(10., (float)SCALE);
#endif

	if(glyphMag[j] > maxmag)
		maxmag = glyphMag[j];

	//set exp and coeffecient
	glyphCoe[j]=coe;
	glyphExp[j]=exp;
	
	markedData[j] = false;
  }; // end for(j)
  
  infile.close();

}

void svArrowGlyph::SetInbox(bool *in)
{
 if(in!=NULL)
 {
	int count = 0;
	 for(int i=0;i<dataSize;i++)
	 {
	  	glyphInbox[i] = in[i]; 
		//if(glyphInbox[i])count++;
	 }
	//cerr<<"count "<<count<<endl;
 } 
 else
 {

	 for(int i=0;i<dataSize;i++)
	 {
	  	glyphInbox[i] = true; 

	 }

 } 
}

void svArrowGlyph::SetExpand(bool *in)
{
 for(int i=0;i<dataSize;i++)
 {
  glyphExpand[i] = in[i]; 
	//if(in[i])cerr<<i<<" ";
 }//cerr<<endl;
}

void svArrowGlyph::SetFontColor(svVector3 c)
{
	if(c[0]<0.5)
	{
		font_color[0] = 1 - c[0];
		font_color[1] = 1 - c[1];
		font_color[2] = 1 - c[2];
	}
	else
	{
		font_color[0] = c[0] - 0.5;
		font_color[1] = c[1] - 0.5;
		font_color[2] = c[2] - 0.5;
	}
}

void svArrowGlyph::SetColors(svVector4 *color)
{

  glyphColors[0].free();

  for(int j=0; j<dataSize; j++)
  {
	//set colors	
	if(color)  
	{
		if(color[0][0]>1)
		    glyphColors[0].add(svVector4(color[j][0]/255., color[j][1]/255., color[j][2]/255., color[j][3]/255.));
		else
		    glyphColors[0].add(svVector4(color[j][0], color[j][1], color[j][2], color[j][3]));
	}
	else  
	{
	  glyphColors[0].add(svVector4(0.5,0.5,0.5,0)); // default gray
	}

  }// end for(j)

}

void svArrowGlyph::extendLength(double length)
{
/*	double distance = length - min_length;
	min_length = length;

        for(int ii=0;ii<dataSize;ii++)
        {
		int i = ii;

		if(encode_mode!=1)
        	{
			double scale;

			if(encode_mode == 0) //linear approach
			{
				scale = min_length * glyphMag[i];//(min_size * 5e-11)
			}
			glyphPos[1][i][0] = glyphPos[0][i][0] +  scale * glyphDir[0][i][0];
			glyphPos[1][i][1] = glyphPos[0][i][1] +  scale * glyphDir[0][i][1];
			glyphPos[1][i][2] = glyphPos[0][i][2] +  scale * glyphDir[0][i][2];
		}
		for(int j=0;j<NUM_SEG+1;j++)
		{
			glyphArrowPos[j][i][0] = glyphArrowPos[j][i][0] + distance * glyphMag[i]* glyphDir[0][i][0];
			glyphArrowPos[j][i][1] = glyphArrowPos[j][i][1] + distance * glyphMag[i]* glyphDir[0][i][1];
			glyphArrowPos[j][i][2] = glyphArrowPos[j][i][2] + distance * glyphMag[i]* glyphDir[0][i][2];
		}
        }
*/

	min_length = length;

        svVector3 head;
	svVector3 norm;
	svVector3 segment_dir;	
	svFloat radius;
	//float length;
	svVector3 point;

	int s, e;
	int i;
	svScalar min_size = 0.25; //hard code!!!!!!!!!!!!!

	//cerr<<encode_mode<<endl;

        for(int ii=0;ii<dataSize;ii++)
        {
		i = ii;

		radius = ARROW_BASE;//hard code !!!!!!!!!!!!

		if(encode_mode!=1)
        	{
			double scale;

			if(encode_mode == 0) //linear approach
			{
				scale = min_length * glyphMag[i];//(min_size * 5e-11)
				radius = radius * glyphMag[i]/maxmag;
			//	cerr<<ARROW_BASE<<" "<<radius<<" "<<scale<<" "<<min_size<<" "<<glyphMag[i]<<" "<<maxmag<<endl;
			}
			else //uniform
			{
				scale = min_size * 5.;
			}

			glyphPos[1][i][0] = glyphPos[0][i][0] +  scale * glyphDir[0][i][0];
			glyphPos[1][i][1] = glyphPos[0][i][1] +  scale * glyphDir[0][i][1];
			glyphPos[1][i][2] = glyphPos[0][i][2] +  scale * glyphDir[0][i][2];
			glyphLegend[0][i][0] = glyphPos[0][i][0] + min_size * 5. * glyphDir[0][i][0];
			glyphLegend[0][i][1] = glyphPos[0][i][1] + min_size * 5. * glyphDir[0][i][1];
			glyphLegend[0][i][2] = glyphPos[0][i][2] + min_size * 5. * glyphDir[0][i][2];
		}
        	else //splitvectors
        	{       
		    	glyphPos[1][i][0] = glyphPos[0][i][0] + min_size * glyphCoe[i] * glyphDir[0][i][0];//scale * glyphDir[0][i][0];
		    	glyphPos[1][i][1] = glyphPos[0][i][1] + min_size * glyphCoe[i] * glyphDir[0][i][1];
		    	glyphPos[1][i][2] = glyphPos[0][i][2] + min_size * glyphCoe[i] * glyphDir[0][i][2];   
			glyphPos[2][i][0] = glyphPos[0][i][0] + min_size * glyphExp[i] * glyphDir[0][i][0];
			glyphPos[2][i][1] = glyphPos[0][i][1] + min_size * glyphExp[i] * glyphDir[0][i][1];
			glyphPos[2][i][2] = glyphPos[0][i][2] + min_size * glyphExp[i] * glyphDir[0][i][2]; 
			//cerr<<min_size<<" "<<glyphCoe[i]<<endl;
  
			glyphLegend[0][i][0] = glyphPos[0][i][0] + min_size * 5. * glyphDir[0][i][0];
			glyphLegend[0][i][1] = glyphPos[0][i][1] + min_size * 5. * glyphDir[0][i][1];
			glyphLegend[0][i][2] = glyphPos[0][i][2] + min_size * 5. * glyphDir[0][i][2];        	
        	}
 
		glyphradius[i] = radius;
        }		
}

void svArrowGlyph::extendRadius(double r)
{
	ARROW_BASE = r;
        svVector3 head;
	svVector3 norm;
	svVector3 segment_dir;	
	svFloat radius;
	float length;
	svVector3 point;

	int s, e;
	int i;
	svScalar min_size = 0.25; //hard code!!!!!!!!!!!!!

	//cerr<<encode_mode<<endl;

        for(int ii=0;ii<dataSize;ii++)
        {
		i = ii;

		radius = ARROW_BASE;//hard code !!!!!!!!!!!!

		if(encode_mode!=1)
        	{
			double scale;

			if(encode_mode == 0) //linear approach
			{
				radius = radius * glyphMag[i]/maxmag;
			}

		}
 
 
			glyphradius[i] = radius;
			length = radius*4.;
        		point[0]=glyphPos[1][i][0]+length*glyphDir[0][i][0];
			point[1]=glyphPos[1][i][1]+length*glyphDir[0][i][1];
			point[2]=glyphPos[1][i][2]+length*glyphDir[0][i][2];

			norm = svGetPerpendicularVector(glyphDir[0][i]);
			head = glyphPos[1][i] + radius * norm;

	
			segment_dir = normalize(glyphDir[0][i]);	

			for(int j=0;j<NUM_SEG;j++)
			{
				glyphArrowPos[j][i] = svGetRotatePoint(head, glyphPos[1][i], segment_dir, (float)(j) * 360./(float)NUM_SEG);
			}
			for(int j=0;j<NUM_SEG;j++)
			{
				if(j!=NUM_SEG-1)
					glyphArrowNorm[j+1][i] = svGetNorm(glyphArrowPos[j][i],point,glyphArrowPos[j+1][i]);
				else
					glyphArrowNorm[j+1][i] = svGetNorm(glyphArrowPos[j][i],point,glyphArrowPos[0][i]);

			}
			for(int j=1;j<NUM_SEG+1;j++)
			{
				if(j!=1)
					glyphArrowNorm[j][i] = (glyphArrowNorm[j][i] + glyphArrowNorm[j-1][i]-segment_dir);
				else
					glyphArrowNorm[j][i] = (glyphArrowNorm[j][i] + glyphArrowNorm[NUM_SEG][i]-segment_dir);
				glyphArrowNorm[j][i] = normalize(glyphArrowNorm[j][i]);
			}
			glyphArrowNorm[0][i] = segment_dir;
			glyphArrowPos[NUM_SEG][i] = point; 

        }	
}

void svArrowGlyph::SetArrows()
{
        svVector3 head;
	svVector3 norm;
	svVector3 segment_dir;	
	svFloat radius;
	float length;
	svVector3 point;

	int s, e;
	int i;
	svScalar min_size = 0.25; //hard code!!!!!!!!!!!!!

	//cerr<<encode_mode<<endl;

        for(int ii=0;ii<dataSize;ii++)
        {
		i = ii;

		radius = ARROW_BASE;//hard code !!!!!!!!!!!!

		if(encode_mode!=1)
        	{
			double scale;

			if(encode_mode == 0) //linear approach
			{
				scale = min_length * glyphMag[i];//(min_size * 5e-11)
				radius = radius * glyphMag[i]/maxmag;
			//	cerr<<ARROW_BASE<<" "<<radius<<" "<<scale<<" "<<min_size<<" "<<glyphMag[i]<<" "<<maxmag<<endl;
			}
			else //uniform
			{
				scale = min_size * 5.;
			}

			glyphPos[1][i][0] = glyphPos[0][i][0] +  scale * glyphDir[0][i][0];
			glyphPos[1][i][1] = glyphPos[0][i][1] +  scale * glyphDir[0][i][1];
			glyphPos[1][i][2] = glyphPos[0][i][2] +  scale * glyphDir[0][i][2];
			glyphLegend[0][i][0] = glyphPos[0][i][0] + min_size * 5. * glyphDir[0][i][0];
			glyphLegend[0][i][1] = glyphPos[0][i][1] + min_size * 5. * glyphDir[0][i][1];
			glyphLegend[0][i][2] = glyphPos[0][i][2] + min_size * 5. * glyphDir[0][i][2];
		}
        	else //splitvectors
        	{       
		    	glyphPos[1][i][0] = glyphPos[0][i][0] + min_size * glyphCoe[i] * glyphDir[0][i][0];//scale * glyphDir[0][i][0];
		    	glyphPos[1][i][1] = glyphPos[0][i][1] + min_size * glyphCoe[i] * glyphDir[0][i][1];
		    	glyphPos[1][i][2] = glyphPos[0][i][2] + min_size * glyphCoe[i] * glyphDir[0][i][2];   
			glyphPos[2][i][0] = glyphPos[0][i][0] + min_size * glyphExp[i] * glyphDir[0][i][0];
			glyphPos[2][i][1] = glyphPos[0][i][1] + min_size * glyphExp[i] * glyphDir[0][i][1];
			glyphPos[2][i][2] = glyphPos[0][i][2] + min_size * glyphExp[i] * glyphDir[0][i][2]; 
			//cerr<<min_size<<" "<<glyphCoe[i]<<endl;
  
			glyphLegend[0][i][0] = glyphPos[0][i][0] + min_size * 5. * glyphDir[0][i][0];
			glyphLegend[0][i][1] = glyphPos[0][i][1] + min_size * 5. * glyphDir[0][i][1];
			glyphLegend[0][i][2] = glyphPos[0][i][2] + min_size * 5. * glyphDir[0][i][2];        	
        	}
 
			glyphradius[i] = radius;
			length = radius*4.;
        		point[0]=glyphPos[1][i][0]+length*glyphDir[0][i][0];
			point[1]=glyphPos[1][i][1]+length*glyphDir[0][i][1];
			point[2]=glyphPos[1][i][2]+length*glyphDir[0][i][2];

			norm = svGetPerpendicularVector(glyphDir[0][i]);
			head = glyphPos[1][i] + radius * norm;

	
			segment_dir = normalize(glyphDir[0][i]);	

			for(int j=0;j<NUM_SEG;j++)
			{
				glyphArrowPos[j][i] = svGetRotatePoint(head, glyphPos[1][i], segment_dir, (float)(j) * 360./(float)NUM_SEG);
			}
			for(int j=0;j<NUM_SEG;j++)
			{
				if(j!=NUM_SEG-1)
					glyphArrowNorm[j+1][i] = svGetNorm(glyphArrowPos[j][i],point,glyphArrowPos[j+1][i]);
				else
					glyphArrowNorm[j+1][i] = svGetNorm(glyphArrowPos[j][i],point,glyphArrowPos[0][i]);

			}
			for(int j=1;j<NUM_SEG+1;j++)
			{
				if(j!=1)
					glyphArrowNorm[j][i] = (glyphArrowNorm[j][i] + glyphArrowNorm[j-1][i]);//-segment_dir);
				else
					glyphArrowNorm[j][i] = (glyphArrowNorm[j][i] + glyphArrowNorm[NUM_SEG][i]);//-segment_dir);
				glyphArrowNorm[j][i] = normalize(glyphArrowNorm[j][i]);
			}
			glyphArrowNorm[0][i] = segment_dir;
			glyphArrowPos[NUM_SEG][i] = point; 

        }	
     
	 if(encode_mode == 1)   
		createSphereArraysAndVBOs();
}


void svArrowGlyph::createSphereArraysAndVBOs() //hard code!!!!!!!!!!!!!
{
    int i,j;
    double radius = 0.25;//0.25;
    int stacks = SPHERE_STACK;
    int slices = SPHERE_SLICE;
    int size = stacks * (slices+1) * 2 * 3;
    int k = 0;
    int ii;

        for(int iii=0;iii<dataSize;iii++)
        {
		ii = iii;
		for (j = 0; j < stacks; j++) 
		{
			double latitude1 = (PI/stacks) * j - PI/2;
			double latitude2 = (PI/stacks) * (j+1) - PI/2;
			double sinLat1 = sin(latitude1);
			double cosLat1 = cos(latitude1);
			double sinLat2 = sin(latitude2);
			double cosLat2 = cos(latitude2);
			for (i = 0; i <= slices; i++) 
			{
			    double longitude = (2*PI/slices) * i;
			    double sinLong = sin(longitude);
			    double cosLong = cos(longitude);
			    double x1 = cosLong * cosLat1;
			    double y1 = sinLong * cosLat1;
			    double z1 = sinLat1;
			    double x2 = cosLong * cosLat2;
			    double y2 = sinLong * cosLat2;
			    double z2 = sinLat2;

			    sphereNormalArray[k] =  (float)x2;
			    sphereNormalArray[k+1] =  (float)y2;
			    sphereNormalArray[k+2] =  (float)z2;
			    sphereVertexArray[k] =    (float)((glyphradius[ii]*x2)+glyphPos[2][ii][0]+glyphradius[ii]*glyphDir[0][ii][0]);
			    sphereVertexArray[k+1] =  (float)((glyphradius[ii]*y2)+glyphPos[2][ii][1]+glyphradius[ii]*glyphDir[0][ii][1]);
			    sphereVertexArray[k+2] =  (float)((glyphradius[ii]*z2)+glyphPos[2][ii][2]+glyphradius[ii]*glyphDir[0][ii][2]);

			    k += 3;
			    sphereNormalArray[k] =  (float)x1;
			    sphereNormalArray[k+1] =  (float)y1;
			    sphereNormalArray[k+2] =  (float)z1;
			    sphereVertexArray[k] =    (float)((glyphradius[ii]*x1)+glyphPos[2][ii][0]+glyphradius[ii]*glyphDir[0][ii][0]);
			    sphereVertexArray[k+1] =  (float)((glyphradius[ii]*y1)+glyphPos[2][ii][1]+glyphradius[ii]*glyphDir[0][ii][1]);
			    sphereVertexArray[k+2] =  (float)((glyphradius[ii]*z1)+glyphPos[2][ii][2]+glyphradius[ii]*glyphDir[0][ii][2]);

			    k += 3;
			}
		    }
	}
    /*GLuint bufferIDs[3];
    glGenBuffers(3, bufferIDs);
    vertexVboId = bufferIDs[0];
    normalVboId = bufferIDs[1];
    colorVboId = bufferIDs[2];
    glBindBuffer(GL_ARRAY_BUFFER, vertexVboId);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), sphereVertexArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, normalVboId);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), sphereNormalArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorVboId);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), sphereColorArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

void svArrowGlyph::drawSphereDirectWithDataFromArrays()  
{
    int i,j;
    int slices = SPHERE_SLICE;
    int stacks = SPHERE_STACK;
    int vertices;
	int end;
	int ii;
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3,GL_FLOAT,0,sphereVertexArray);
            glNormalPointer(GL_FLOAT,0,sphereNormalArray);

		end = glyphPos[0].size();


	for(int iii=0;iii<end;iii++)
	{

			ii = iii;

	 	vertices = (slices+1)*2;
		glColor3f(glyphColors[0][ii][0],glyphColors[0][ii][1],glyphColors[0][ii][2]);

		    for (i = 0; i < stacks; i++) {
			int pos = ii * stacks*(slices+1)*2 + i*(slices+1)*2;
			glDrawArrays(GL_QUAD_STRIP, pos, vertices);
		    }
	}
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
}

void svArrowGlyph::CallList()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glCallList(displayList);
    
	
	//cerr<<displayList<<endl;
}

void svArrowGlyph::geometryDraw(int atom, double base)
{
	//cone
	if(atom==1)
	{	
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		  glBegin(GL_POLYGON);
		 // glNormal3f(0,0,1);
		  //glNormal3f(0,0,-1);
		  for (int i=0; i<=360; i++)
		  {
		     float degInRad = (float)i/180. * PI;
		     glVertex3f(cos(degInRad)*base,sin(degInRad)*base,0);
		  }
		  glEnd();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	//cube
	else if(atom == 2)	
	{
		/*glPushMatrix();
		glTranslatef(0,0,base);
		glutSolidCube(base*2);
		glPopMatrix();*/
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glLineWidth(5.);
		glBegin(GL_LINES);
		glVertex3f(-base,0,0);
		glVertex3f(base,0,0);
		glEnd();	
		glBegin(GL_LINES);
		glVertex3f(-base/2.,base/2. * sqrt(3),0);
		glVertex3f(base/2.,-base/2. * sqrt(3),0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(base/2.,base/2. * sqrt(3),0);
		glVertex3f(-base/2.,-base/2. * sqrt(3),0);
		glEnd();
		glLineWidth(1.);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	//x
	else if(atom == 3)
	{
		glBegin(GL_QUADS);
		glVertex3f(base, base, 0);
		glVertex3f(-base, base, 0);
		glVertex3f(-base,-base,2*base);
		glVertex3f(base,-base,2*base);
		glEnd();

		glBegin(GL_QUADS);
		glVertex3f(-base, -base, 0);
		glVertex3f(base, -base, 0);
		glVertex3f(base,base,2*base);
		glVertex3f(-base,base,2*base);
		glEnd();
		//glutSolidTorus(base/2., base, 6, 6);
	}
	//sphere
	else if(atom == 4)
	{
		glutSolidSphere(base, 6, 6);
	}
}
//http://www.cnblogs.com/xpvincent/archive/2013/02/16/2913537.html
void svArrowGlyph::DrawSpring(svVector3 ptStart, svVector3 ptEnd, double Radius, double ringHeight, double Rings)

{
    double springHeight;
    double ringDelta = 2.0 * PI / Rings;
    //double ringHeight;
    double ringAngle;
    double z;

    svVector3 ptOldFrame, ptNewFrame;
    svVector3 rotationAxis;
    double rotationAngle;

    ptOldFrame[0] = 0;
    ptOldFrame[1] = 0;
    ptOldFrame[2] = 1;
    ptNewFrame[0] = ptEnd[0] - ptStart[0];
    ptNewFrame[1] = ptEnd[1] - ptStart[1];
    ptNewFrame[2] = ptEnd[2] - ptStart[2];
    springHeight = sqrt(ptNewFrame[0] * ptNewFrame[0]
        + ptNewFrame[1]* ptNewFrame[1]
        + ptNewFrame[2]* ptNewFrame[2]);
    //ringHeight = (springHeight / Coils) / Rings;
	double Coils = springHeight /( ringHeight * Rings);

	Coils = (double)((int)Coils);
	ringHeight = (springHeight / Coils) / Rings;

    glBegin(GL_LINE_STRIP);
    z = 0;
    for (int i = 0; i < Coils; i++)
    {
        ringAngle = 0;
        for (int j = 0; j <Rings; j++)
        {

            glVertex3f(cos(ringAngle) * Radius, sin(ringAngle) * Radius, z);

            ringAngle += ringDelta;

            z += ringHeight;

        }

    }
    glEnd();


}


void svArrowGlyph::svArrowGlyphRenderArrow(float a)
{
  //displayList = glGenLists(1);
    if(glIsList(displayList))
	glDeleteLists(displayList, 1); 
    glNewList(displayList, GL_COMPILE);

    float alpha = 0.1;
    int slices = SPHERE_SLICE;
    int stacks = SPHERE_STACK;
    int vertices;
    
   // glDisable(GL_CULL_FACE) ;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    /*glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,sphereVertexArray);
    glNormalPointer(GL_FLOAT,0,sphereNormalArray);*/

    int i;
    int count = 0;
	
        for(int ii =0;ii<dataSize;ii++)
        {		
		i = ii;

		if(dataSample[i] && glyphInbox[i] )// && glyphInbox[i] && glyphExpand[i])
		{
			//cerr<<ii<<" ";
			alpha = a;
			glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);			  
			
			double radius = ARROW_BASE;

			if(encode_mode == 0) //linear approach
			{
				radius = radius * glyphMag[i]/maxmag;
			}
			//cerr<<radius<<" ";
			  double angle_x = acos(glyphDir[0][i][2]);
			  if(glyphDir[0][i][1] > 0)
			  {
				angle_x = - angle_x;
			  }
			  double xy_project = glyphDir[0][i][0] * glyphDir[0][i][0] + glyphDir[0][i][1] * glyphDir[0][i][1];
			  xy_project = sqrt(xy_project);
			  double angle_z = acos(glyphDir[0][i][1]/xy_project);
			  if(angle_x < 0)
			  {	
				if (glyphDir[0][i][0] > 0)
				 {
					angle_z =  -angle_z;
				  }
			  }
			  else
			  {
				angle_z = 3.1415926 - angle_z;
				if(glyphDir[0][i][0] < 0)
					angle_z =  -angle_z;		
			  }

			glPushMatrix();

			glTranslatef(glyphPos[1][i][0],glyphPos[1][i][1],glyphPos[1][i][2]);
			glRotatef(angle_z/3.1415926 * 180.,0,0,1);
			glRotatef(angle_x/3.1415926 * 180.,1,0,0);

			geometryDraw(dataAtom[i], radius*1.5); 

			glPopMatrix();

			svVector3 end;
			end = glyphPos[1][i];
			
			if(encode_mode==1)
			{
				/*vertices = (slices+1)*2;

			    	for (int ij = 0; ij < stacks; ij++) 
				{
					int pos = i * stacks*(slices+1)*2 + ij*(slices+1)*2;
					glDrawArrays(GL_QUAD_STRIP, pos, vertices);
			    	}*/
				glPushMatrix();
				glTranslatef(glyphPos[2][i][0]+radius*1.5*glyphDir[0][i][0],
					glyphPos[2][i][1]+radius*1.5*glyphDir[0][i][1],
					glyphPos[2][i][2]+radius*1.5*glyphDir[0][i][2]);
				glutSolidSphere(radius*1.5, 6, 6);
				if(glyphExp[i]>glyphCoe[i])
				{
					end = glyphPos[2][i];
				}	
				glPopMatrix();
			}

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);

			if(dataAtom[i] == 1)
			{
				glLineWidth(3.);
				//glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);

				glBegin(GL_LINES);
				glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
				glVertex3f(end[0],end[1],end[2]);
				glEnd();
				
				glLineWidth(6.);
				glColor4f(0.,0.,0.,alpha);
				glBegin(GL_LINES);
				glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
				glVertex3f(end[0],end[1],end[2]);
				glEnd();
			}
			else if(dataAtom[i] == 2)
			{
				glPushMatrix();

				glTranslatef(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
				glRotatef(angle_z/3.1415926 * 180.,0,0,1);
				glRotatef(angle_x/3.1415926 * 180.,1,0,0);
				if(encode_mode == 0)
				{
					double coil;
					coil = glyphMag[i]/pow(10.,(double)(SCALE-6));
					if(coil<1) coil = 10;				
					glLineWidth(3.);
					DrawSpring(glyphPos[0][i], glyphPos[1][i], radius/2., radius/2. , 10);

					glLineWidth(5.);
					glColor4f(0.,0.,0.,alpha);
					DrawSpring(glyphPos[0][i], glyphPos[1][i], radius/2., radius/2., 10);
				}
				else
				{
					glLineWidth(3.);
					DrawSpring(glyphPos[0][i], end, radius/2., radius/2., 10);

					glLineWidth(5.);
					glColor4f(0.,0.,0.,alpha);
					DrawSpring(glyphPos[0][i], end, radius/2., radius/2., 10);
				}
				glPopMatrix();
			}
			if(encode_mode == 1)
			{					
				glLineWidth(0.75);  
				glDepthRange(0,0.9999999);
				glColor4f(0.,0.,0.,alpha);
				glBegin(GL_LINES);
				glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
				glVertex3f(glyphLegend[0][i][0],glyphLegend[0][i][1],glyphLegend[0][i][2]);
				glEnd();
				glDepthRange(0.0f, 1.0f);
			}

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}	

        }

       /* glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);*/
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

	glLineWidth(1.);
	
	glEndList();

}
void svArrowGlyph::RenderArrow(float a, bool layer, float zvalue)//, vector<bool> inroibox, bool global )
{
  //displayList = glGenLists(1);
 if(glIsList(displayList))
	glDeleteLists(displayList, 1); 
 glNewList(displayList, GL_COMPILE);

    float alpha = 0.1;
    int slices = SPHERE_SLICE;
    int stacks = SPHERE_STACK;
    int vertices;
    
   // glDisable(GL_CULL_FACE) ;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL);

    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,sphereVertexArray);
    glNormalPointer(GL_FLOAT,0,sphereNormalArray);


    svVector3 point;
    int i;
    int count = 0;
	
        for(int ii =0;ii<dataSize;ii++)
        {		

		i = ii;
		//if(markedData[i])count++;
		if(fabs(glyphPos[0][ii][2] - zvalue) < 0.001)//(!layer)|| (layer && 
		{
		if(dataSample[i] && glyphInbox[i])// && glyphExpand[i])
		{
			alpha = 0.5;

			point = glyphArrowPos[NUM_SEG][i];
			
			if(selectedIndex == i)
			    glColor4f(1,0,0,alpha);
			//else if(markedData[i])
			// {glColor4f(1, 1, 0,alpha);}
			else
			    glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);			  
			
			glBegin(GL_TRIANGLES);
			for(int j=0;j<NUM_SEG;j++)
			{
				glNormal3f(glyphArrowNorm[0][i][0],glyphArrowNorm[0][i][1],glyphArrowNorm[0][i][2]);
				glVertex3f(point[0],point[1],point[2]);

				glNormal3f(glyphArrowNorm[j+1][i][0],glyphArrowNorm[j+1][i][1],glyphArrowNorm[j+1][i][2]);
				glVertex3f(glyphArrowPos[j][i][0],glyphArrowPos[j][i][1],glyphArrowPos[j][i][2]);

				if(j!=NUM_SEG-1)
				{
					glNormal3f(glyphArrowNorm[j+2][i][0],glyphArrowNorm[j+2][i][1],glyphArrowNorm[j+2][i][2]);
				        glVertex3f(glyphArrowPos[j+1][i][0],glyphArrowPos[j+1][i][1],glyphArrowPos[j+1][i][2]);
				}
				else
				{
				        glNormal3f(glyphArrowNorm[1][i][0],glyphArrowNorm[1][i][1],glyphArrowNorm[1][i][2]);
				        glVertex3f(glyphArrowPos[0][i][0],glyphArrowPos[0][i][1],glyphArrowPos[0][i][2]);
				}
			}
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-glyphArrowNorm[0][i][0],-glyphArrowNorm[0][i][1],-glyphArrowNorm[0][i][2]);
			for(int j=0;j<NUM_SEG;j++)
			{
			//	glNormal3f(glyphArrowNorm[j+1][i][0],glyphArrowNorm[j+1][i][1],glyphArrowNorm[j+1][i][2]);
				glVertex3f(glyphArrowPos[j][i][0],glyphArrowPos[j][i][1],glyphArrowPos[j][i][2]);
			}			
			glEnd();
			
			svVector3 end;
			end = glyphPos[1][i];
			
			if(encode_mode==1)
			{
				vertices = (slices+1)*2;
				//glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);

			    	for (int ij = 0; ij < stacks; ij++) 
				{
					int pos = i * stacks*(slices+1)*2 + ij*(slices+1)*2;
					glDrawArrays(GL_QUAD_STRIP, pos, vertices);
			    	}
				if(glyphExp[i]>glyphCoe[i])
				{
					end = glyphPos[2][i];
				}	
			}

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);

			glLineWidth(3.);
			//glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);

			glBegin(GL_LINES);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
			glVertex3f(end[0],end[1],end[2]);
			glEnd();
				
			glLineWidth(6.);
			glColor4f(0.,0.,0.,alpha);
			glBegin(GL_LINES);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
			glVertex3f(end[0],end[1],end[2]);
			glEnd();
								
			/*glLineWidth(0.75);  
			glDepthRange(0,0.9999999);
			glColor4f(0.,0.,0.,alpha);
			glBegin(GL_LINES);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
			glVertex3f(glyphLegend[0][i][0],glyphLegend[0][i][1],glyphLegend[0][i][2]);
			glEnd();
			glDepthRange(0.0f, 1.0f);

			if(markedData[i])
			{
				char str[10];
				glColor3f(font_color[0],font_color[1],font_color[2]);
				sprintf(str,"%d", count);
				glRasterPos3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
				for(int j=0;j<strlen(str);j++)
			    		 glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[j]);
			}*/

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}	
		}
        }//cerr<<count<<endl;
        count = 0;
        for(int ii =0;ii<dataSize;ii++)
        {		
		i = ii;
		//if(markedData[i])count++;
		if(fabs(glyphPos[0][ii][2] - zvalue) < 0.001)//(!layer)|| (layer && 
		{
		if(dataSample[i] && (!glyphInbox[i] || !glyphExpand[i]))
		{
			alpha = a;

			point = glyphArrowPos[NUM_SEG][i];
			if(selectedIndex == i)
			    glColor4f(1,0,0,alpha);
			//else if(markedData[i])
			// {glColor4f(1, 1, 0,alpha);}
			else
			    glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);		
			
			glBegin(GL_TRIANGLES);
			for(int j=0;j<NUM_SEG;j++)
			{
				glNormal3f(glyphArrowNorm[0][i][0],glyphArrowNorm[0][i][1],glyphArrowNorm[0][i][2]);
				glVertex3f(point[0],point[1],point[2]);

				glNormal3f(glyphArrowNorm[j+1][i][0],glyphArrowNorm[j+1][i][1],glyphArrowNorm[j+1][i][2]);
				glVertex3f(glyphArrowPos[j][i][0],glyphArrowPos[j][i][1],glyphArrowPos[j][i][2]);

				if(j!=NUM_SEG-1)
				{
					glNormal3f(glyphArrowNorm[j+2][i][0],glyphArrowNorm[j+2][i][1],glyphArrowNorm[j+2][i][2]);
				        glVertex3f(glyphArrowPos[j+1][i][0],glyphArrowPos[j+1][i][1],glyphArrowPos[j+1][i][2]);
				}
				else
				{
				        glNormal3f(glyphArrowNorm[1][i][0],glyphArrowNorm[1][i][1],glyphArrowNorm[1][i][2]);
				        glVertex3f(glyphArrowPos[0][i][0],glyphArrowPos[0][i][1],glyphArrowPos[0][i][2]);
				}
			}
			glEnd();
			
			glBegin(GL_QUADS);
			glNormal3f(-glyphArrowNorm[0][i][0],-glyphArrowNorm[0][i][1],-glyphArrowNorm[0][i][2]);
			for(int j=0;j<NUM_SEG;j++)
			{
				glVertex3f(glyphArrowPos[j][i][0],glyphArrowPos[j][i][1],glyphArrowPos[j][i][2]);
			}			
			glEnd();
			
			svVector3 end;
			end = glyphPos[1][i];
			
			if(encode_mode==1)
			{
				vertices = (slices+1)*2;
				//glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);

			    	for (int ij = 0; ij < stacks; ij++) 
				{
					int pos = i * stacks*(slices+1)*2 + ij*(slices+1)*2;
					glDrawArrays(GL_QUAD_STRIP, pos, vertices);
			    	}
				if(glyphExp[i]>glyphCoe[i])
				{
					end = glyphPos[2][i];
				}	
			}

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);

			glLineWidth(3.);
			//glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);

			glBegin(GL_LINES);
			glVertex3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
			glVertex3f(end[0],end[1],end[2]);
			glEnd();

/*
			if(markedData[i])
			{
				char str[10];
				glColor3f(font_color[0],font_color[1],font_color[2]);
				sprintf(str,"%d", count);
				glRasterPos3f(glyphPos[0][i][0],glyphPos[0][i][1],glyphPos[0][i][2]);
				for(int j=0;j<strlen(str);j++)
			    		 glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[j]);
			}*/

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}	
		}
        }
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

	glLineWidth(1.);
	
	glEndList();

}








void svArrowGlyph::cleanup()
{
	glyphPos[0].free();//cerr<<"!"<<endl;
	glyphPos[1].free();//cerr<<"!"<<endl;
	glyphPos[2].free();//cerr<<"!"<<endl;
	glyphLegend[0].free();//cerr<<"!"<<endl;
	delete [] glyphPos;//cerr<<"!"<<endl;
	delete [] glyphLegend;//cerr<<"!"<<endl;
	glyphPos=NULL;//cerr<<"!"<<endl;
	glyphLegend = NULL;//cerr<<"!"<<endl;
	//}cerr<<"!"<<endl;

	glyphDir[0].free();
	delete [] glyphDir;
	glyphDir=NULL;

	glyphColors[0].free();
	delete [] glyphColors;
	glyphColors=NULL;

	for(int i=0;i<NUM_SEG+1;i++)
	{
		glyphArrowPos[i].free();
	}
	delete [] glyphArrowPos;
	glyphArrowPos=NULL;

	for(int i=0;i<NUM_SEG+1;i++)
	{
		glyphArrowNorm[i].free();
	}
	delete [] glyphArrowNorm;
	glyphArrowNorm=NULL;

	delete [] glyphMag;
	delete [] glyphExp;
	delete [] dataSample;
	delete [] glyphCoe;
	delete [] glyphradius;
	delete [] glyphInbox;
	delete [] glyphExpand;
	delete [] sphereVertexArray;
	delete [] sphereNormalArray;
	delete [] markedData;
	delete [] dataAtom;

    for(int i=0;i<rawFormat.size();i++)
    {
	for(int j=0;j<rawFormat[i].size();j++)
	{
		rawFormat[i][j].clear();
	}
	rawFormat[i].clear();
    }
    rawFormat.clear();
}

}
