/*  svGlyph
 */

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "svQDOTGlyph.h"
#include "svException.h"
#include "svUtil.h"
#include "svColors.h"
using namespace std;

namespace __svl_lib {
	
svQDOTGlyph::svQDOTGlyph(svVectorField* f, int numPlane)
	: svGlyph(f,numPlane)
{
	exp = new svScalarArray[numPlane];
	digit = new svScalarArray[numPlane];
	
	atom = new svIntArray[numPlane];
	contourValues = new svScalarArray[numPlane];
	
}


void svQDOTGlyph::New(int numPlane)
{
	clean();
	cleanQDOTGlyph();
	
	glyph = new svVector3Array[numPlane];
	dir = new svVector3Array[numPlane];
	mag = new svScalarArray[numPlane];
	exp = new svScalarArray[numPlane];
	digit = new svScalarArray[numPlane];
	
	inROI = new svIntArray[numPlane];
	atom = new svIntArray[numPlane];
	glyphColors = new svVector4Array[numPlane];
	glyphWidth = new svScalarArray[numPlane];
	contourValues = new svScalarArray[numPlane];
	
	seed_num = numPlane;
}
void svQDOTGlyph::New(svVectorField* f, int numPlane)
{
	New(numPlane);
	field = f;
}
void svQDOTGlyph::SetROI(double mag1, double mag2)
{
	for(int i=0;i<seed_num;i++)
	{
		inROI[i].free();
	}

//cerr<<"release"<<endl;

	for(int i=0;i<seed_num;i++)
	{//cerr<<i<<endl;
		for(int j=0;j<mag[i].size();j++)
		{
			if(mag[i][j] > mag1
			&& mag[i][j] < mag2)
			{
				inROI[i].add(1);
			}
			else
			{
				inROI[i].add(0);
			}
		}
	}
}

void svQDOTGlyph::SetROI()
{
        for(int i=0;i<seed_num;i++)
        {
                inROI[i].free();
        }

        for(int i=0;i<seed_num;i++)
        {
		
                for(int j=0;j<glyph[i].size();j++)
                {
                                inROI[i].add(1);
                }
//cerr<<inROI[i].size()<<endl;
        }
}

void svQDOTGlyph::SetColorsbyCluster(int isWhole, int color_model, svIntArray &layer, 
				svScalarArray &clusterParameters, int numCluster)
{
	if(numCluster > 0)
	{
	char *outfname = new char[200];
	sprintf(outfname, "%s/%s/clusterfile",
                                field->GetDataDir(), field->GetDataFile()); 
	cerr<<outfname<<endl;
	char *clustername = new char[200];
        sprintf(clustername, "%s/%s/clusterresult",
                                field->GetDataDir(), field->GetDataFile());
	cerr<<clustername<<endl;

	svQDOT *qdotfield = new svQDOT(field->GetDataDir(),
					field->GetDataFile()); 
	svKmeans *clusterField = new svKmeans(field);
	qdotfield->SetIsWhole(isWhole);
	qdotfield->SetClusterInput(isRaw, layer, inROI, outfname, clusterParameters);
	clusterField->ComputeClusters(outfname, clustername, numCluster);

	ifstream infile(clustername);

	svColors *color = new svColors();

	int num;
	infile>>num;
	for(int i=0;i<layer.size();i++)
	{		
		//cerr<<i<<endl;
		for(int j=0;j<glyph[layer[i]].size();j++)
		{
			if(inROI[layer[i]][j])
			{
				int c;
				infile>>c;
				glyphColors[layer[i]][j] = color->GetDiscreteColors(color_model,c);
			//	cerr<<c<<" "<<glyphColors[layer[i]].size()<<endl;
			}
		}
	}

	infile.close();

	delete qdotfield;
	delete clusterField;
	delete color;

	delete [] outfname;	
	delete [] clustername;
	}
}

void svQDOTGlyph::SetData(svScalarArray * cV)
{
	cerr<<isRaw<<endl;
	if(isRaw == 1)
	{
		SetRaw();
	}
	else
	{
		SetContours(cV);
	}
	
	SetROI();
	DisableColor();
}

void svQDOTGlyph::SetContours(svScalarArray * cV)
{
	for(int i=0;i<seed_num;i++)
	{
		bool flag = false;
		
		//cerr<<"/*-------------------detect if contours have been generated---*/"<<endl;
		if(cV[i].size()!=contourValues[i].size())
		{
			flag = true;
		}
		else
		{
			for(int j=0;j<contourValues[i].size();j++)
			{
				if(contourValues[i][j]!=cV[i][j])
				{
					flag = true;
					break;
				}
			}
		}
		//cerr<<i<<" "<<flag<<endl;
		//cerr<<"/*-----------------detect end--------------------------------*/"<<endl;
		
		if(flag)
		{
			if(contourValues[i].size()>0)
			{
				contourValues[i].free();
			}
			for(int j=0;j<cV[i].size();j++)
			{
				contourValues[i].add(cV[i][j]);//Update contourvalues
				//cerr<<contourValues[i][j]<<endl;
			}
			
			char *outfname = new char[50];
			sprintf(outfname,"contour%d.txt",i);
			//cerr<<"/*-----------------detect end--------------------------------*/"<<endl;
			svContour *contourField = new svContour(field);
//cerr<<"/*-----------------detect end--------------------------------*/"<<endl;
			contourField->ComputeContours(i, outfname, contourValues[i]);//generate contours for the current plane

			delete contourField;
			
			//cerr<<"generate contours"<<endl;
			
			char *contourname = new char[400];
			sprintf(contourname, "%s/%s/%s", 
				field->GetDataDir(), field->GetDataFile(), outfname); 
			ifstream infile(contourname);
			
			glyph[i].free();
			mag[i].free();
			dir[i].free();
			digit[i].free();
			exp[i].free();
			
			int num;
			infile>>num;
			
			/*--------------update rendered variables-------------*/
			for(int j=0;j<num;j++)
			{
				svVector3 p;
				svVector3 v;
				svScalar d;
				svScalar e;
				svScalar c;
				
				infile>>p[0]>>p[1]>>p[2]
					>>v[0]>>v[1]>>v[2]
					>>d>>e>>c;
					
				glyph[i].add(p);
				mag[i].add(d);
				dir[i].add(v);
				digit[i].add(c);
				exp[i].add(e);
			}
			/*-------update end-------------------------------------*/
			infile.close();
			
			//cerr<<"generate contours"<<endl;
			
			delete [] contourname;
			delete [] outfname;
		}
	}

	//DisableColor();
}

void svQDOTGlyph::SetRaw()
{
	char *rawfile = new char[200];
	
	for(int i=0;i<seed_num;i++)
	{
		sprintf(rawfile, "%s/%s/%d.txt",field->GetDataDir(),
				field->GetDataFile(), i);
		ifstream infile(rawfile);
		
		int num;
		infile>>num;
		
		glyph[i].free();
		mag[i].free();
		dir[i].free();
		digit[i].free();
		exp[i].free();
		atom[i].free();
			
		for(int j=0;j<num;j++)
		{
			svVector3 p;
			svVector3 v;
			svScalar d;
			svScalar e;
			svScalar c;
			svScalar r;
			svScalar a;
				
			infile>>p[0]>>p[1]>>p[2]
				>>v[0]>>v[1]>>v[2]
				>>d>>e>>c
				>>a>>r;
			glyph[i].add(p);
			dir[i].add(v);
			mag[i].add(d);
			digit[i].add(c);
			exp[i].add(e);
			atom[i].add(a);
		}
		
		infile.close();
	}
	
	delete [] rawfile;
	
	//DisableColor();
}

svQDOTGlyph::~svQDOTGlyph()
{
  cleanQDOTGlyph();
  cleanDisplayList(SV_IMMEDIATE);
}

void svQDOTGlyph::cleanQDOTGlyph()
{
  if (digit!=NULL) {
    for(int i=0; i<seed_num; i++)
      digit[i].free();
    delete [] digit;
    digit=NULL;
  };
  if (exp!=NULL) {
    for(int i=0; i<seed_num; i++)
      exp[i].free();
    delete [] exp;
    exp=NULL;
  };
  if (atom!=NULL) {
    for(int i=0; i<seed_num; i++)
      atom[i].free();
    delete [] atom;
    atom=NULL;
  };
  if (contourValues!=NULL) {
    for(int i=0; i<seed_num; i++)
      contourValues[i].free();
    delete [] contourValues;
    contourValues=NULL;
  };
}

}
